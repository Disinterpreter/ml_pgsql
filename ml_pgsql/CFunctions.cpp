#include "ml_pgsql.h"
#include "CFunctions.h"
#include "CPostgresManager.h"
#include "CScriptArgStream.h"

int CFunctions::pg_conn(lua_State* luaVM)
{
    std::string sConnectionInfo;
    CScriptArgStream pArgs(luaVM);

    if (pArgs.ReadString(sConnectionInfo))
    {
        CPostgresConnection* pConn = CPostgresManager::NewConnection(luaVM, sConnectionInfo);
    
        if (pConn && pConn->IsConnected())
        {
            lua_pushlightuserdata(luaVM, pConn);
            return 1;
        }
        else if(pConn)
        {
            lua_pushboolean(luaVM, false);
            lua_pushstring(luaVM, pConn->GetLastErrorMessage());

            SAFE_DELETE(pConn);
            return 2;
        }
    }
    
    lua_pushboolean(luaVM, false);
    return 1;
}


int CFunctions::pg_query(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_query", lua_gettop(luaVM) > 1);

    CPostgresConnection* pConnection = static_cast<CPostgresConnection*>(lua_touserdata(luaVM, 1));
    
    if (pConnection && pConnection->IsConnected())
    {
        PGresult* pResult = pConnection->Query(luaVM);

        if (pResult)
        {
            lua_pushlightuserdata(luaVM, pResult);
            return 1;
        }
    }
        
    lua_pushboolean(luaVM, false);
    return 1;
}


int CFunctions::pg_exec(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_exec", lua_gettop(luaVM) > 1);

    CPostgresConnection* pConnection = static_cast<CPostgresConnection*>(lua_touserdata(luaVM, 1));
    
    if (pConnection && pConnection->IsConnected())
    {
        lua_pushboolean(luaVM, pConnection->Exec(luaVM));
    }
        
    lua_pushboolean(luaVM, false);
    return 1;
}


int CFunctions::pg_poll(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_poll", lua_gettop(luaVM) == 1);

    PGresult* pResult = static_cast<PGresult*>(lua_touserdata(luaVM, 1));
    
    if (pResult)
    {
        int ncols = PQnfields(pResult);
        int nrows = PQntuples(pResult);

        lua_newtable(luaVM);
        for (int i = 0; i < nrows; i++)
        {
            lua_newtable(luaVM);
            for (int coloumn = 0; coloumn < ncols; coloumn++)
            {
                char* colname = PQfname(pResult, coloumn);
                lua_pushstring(luaVM, colname);
                char* dataname = PQgetvalue(pResult, i, coloumn);
                lua_pushstring(luaVM, dataname);
                lua_settable(luaVM, -3);
            }
        }

        PQclear(pResult);
        return 1;
    }

    lua_pushboolean(luaVM, false);
    return 1;
}


int CFunctions::pg_free(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_free", lua_gettop(luaVM) == 1);

    PGresult* pResult = static_cast<PGresult*>(lua_touserdata(luaVM, 1));
    
    if (pResult)
    {
        PQclear(pResult);
        lua_pushboolean(luaVM, true);
        return 1;
    }

    lua_pushboolean(luaVM, false);
    return 1;
}


int CFunctions::pg_close(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_close", lua_gettop(luaVM) == 1);

    CPostgresConnection* pConnection = static_cast<CPostgresConnection*>(lua_touserdata(luaVM, 1));
    
    if (pConnection)
    {
        g_pPostgresManager->RemoveConnection(pConnection);

        lua_pushboolean(luaVM, true);
        return 1;
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

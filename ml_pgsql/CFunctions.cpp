#include "ml_pgsql.h"
#include "CFunctions.h"
#include "CPostgresManager.h"
#include "CScriptArgStream.h"

int CFunctions::pg_conn(lua_State* luaVM)
{
    std::string sConnectionInfo;

    CScriptArgStream argStream(luaVM);
    
    if (argStream.ReadString(sConnectionInfo))
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
    std::string sQuery;
    CPostgresConnection* pConnection;
    CScriptArgStream argStream(luaVM);
    
    if (argStream.ReadUserData(pConnection) && argStream.ReadString(sQuery))
    {
        if (pConnection->IsConnected())
        {
            PGresult* pResult = pConnection->Query(sQuery);

            if (pResult)
            {
                lua_pushlightuserdata(luaVM, pResult);
                return 1;
            }
        }
    }
        
    lua_pushboolean(luaVM, false);
    return 1;
}


int CFunctions::pg_exec(lua_State* luaVM)
{
    CPostgresConnection* pConnection;
    CScriptArgStream argStream(luaVM);

    if (argStream.ReadUserData(pConnection))
    {
        if (pConnection->IsConnected())
        {
            lua_pushboolean(luaVM, pConnection->Exec(luaVM));
        }
    }
        
    lua_pushboolean(luaVM, false);
    return 1;
}


int CFunctions::pg_poll(lua_State* luaVM)
{
    PGresult* pResult;
    CScriptArgStream argStream(luaVM);

    if (argStream.ReadUserData(pResult))
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
    PGresult* pResult;

    CScriptArgStream argStream(luaVM);
    
    if (argStream.ReadUserData(pResult))
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
    CPostgresConnection* pConnection;
    CScriptArgStream argStream(luaVM);

    if (argStream.ReadUserData(pConnection))
    {
        g_pPostgresManager->RemoveConnection(pConnection);

        lua_pushboolean(luaVM, true);
        return 1;
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright � 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is � 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/
#include "ml_pgsql.hpp"
#include "CFunctions.hpp"
#include "CPostgresManager.hpp"

int CFunctions::pg_conn(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_conn", lua_gettop(luaVM) == 1);

    CPostgresConnection* pConn = CPostgresManager::NewConnection(luaVM);
    
    if (pConn->IsConnected())
    {
        lua_pushlightuserdata(luaVM, pConn);
        return 1;
    }
    
    lua_pushboolean(luaVM, false);
    lua_pushstring(luaVM, pConn->GetLastErrorMessage());
    return 2;
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

/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright © 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is © 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

#include "CFunctions.h"

int CFunctions::pg_conn(lua_State* luaVM)
{
    const char* str = luaL_checkstring(luaVM, 1);
    PGconn* connection = PQconnectdb(str);
    lua_pushlightuserdata(luaVM, connection);
    if (PQstatus(connection) != CONNECTION_OK)
    {
        char* errmsg = PQerrorMessage(connection);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    else {
        lua_pushlightuserdata(luaVM, connection);
    }
    return 1;
}

int CFunctions::pg_query(lua_State* luaVM)
{
    void* conn = lua_touserdata(luaVM, 1);
    const char* str = luaL_checkstring(luaVM, 2);
    PGresult* query;
    query = PQexec((PGconn*)conn, str); 
    if ( (PQresultStatus(query) != PGRES_COMMAND_OK) && (PQresultStatus(query) != PGRES_TUPLES_OK))
    {
        lua_pushstring(luaVM, PQerrorMessage((PGconn*)conn));
        PQclear(query);
        return 1;
    }

    int ncols = PQnfields(query);
    int nrows = PQntuples(query);

    lua_newtable(luaVM);
    for (int i = 0; i < nrows; i++)
    {
        lua_newtable(luaVM);
        for (int coloumn = 0; coloumn < ncols; coloumn++) 
        {

            char* colname = PQfname(query, coloumn);
            lua_pushstring(luaVM, colname);
            char* dataname = PQgetvalue(query, i, coloumn);
            lua_pushstring(luaVM, dataname);
            lua_settable(luaVM, -3);
        }
    }
    lua_settable(luaVM, -3);
    PQclear(query);
    return 1;
}


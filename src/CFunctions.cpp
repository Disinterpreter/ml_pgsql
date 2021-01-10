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
    if ( (PQstatus(connection) != CONNECTION_OK))
    {
        char* errmsg = PQerrorMessage(connection);
        PQfinish(connection);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    else 
    {
        if (PQsetnonblocking(connection, 1) == 0) {
            lua_pushlightuserdata(luaVM, connection);
            return 1;
        }
        else
        {
            char* errmsg = PQerrorMessage(connection);
            PQfinish(connection);
            lua_pushboolean(luaVM, 0);
            lua_pushstring(luaVM, errmsg);
            return 2;
        }
        
        
    }
   
}


int CFunctions::pg_query(lua_State* luaVM)
{
    void* conn = lua_touserdata(luaVM, 1);
    if (PQstatus((PGconn*)conn) != CONNECTION_OK && PQsetnonblocking((PGconn*)conn, 1) == 0) {
        char* errmsg = PQerrorMessage((PGconn*)conn);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    const char* str = luaL_checkstring(luaVM, 2);
    PGresult* query;

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int totalArgs = lua_gettop(luaVM);



    std::vector<const char*> args{};
    for (int i = totalArgs; i > 0; --i) {
        args.push_back(luaL_checkstring(luaVM, i));
    }


    query = PQexecParams((PGconn*)conn,
        str,
        totalArgs,       /* one param */
        NULL,    /* let the backend deduce param type */
        (char**)args.data(),
        NULL,
        NULL,
        0);
    if ( (PQresultStatus(query) != PGRES_COMMAND_OK) && (PQresultStatus(query) != PGRES_TUPLES_OK))
    {
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, PQerrorMessage((PGconn*)conn));
        PQclear(query);
        return 2;
    }



    lua_pushlightuserdata(luaVM, query);
    return 1;
}

int CFunctions::pg_exec(lua_State* luaVM)
{
    void* conn = lua_touserdata(luaVM, 1);
    if (PQstatus((PGconn*)conn) != CONNECTION_OK && PQsetnonblocking((PGconn*)conn, 1) == 0) {
        char* errmsg = PQerrorMessage((PGconn*)conn);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    const char* str = luaL_checkstring(luaVM, 2);
    PGresult* query;

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int totalArgs = lua_gettop(luaVM);



    std::vector<const char*> args{};
    for (int i = totalArgs; i > 0; --i) {
        args.push_back(luaL_checkstring(luaVM, i));
    }


    query = PQexecParams((PGconn*)conn,
        str,
        totalArgs,       /* one param */
        NULL,    /* let the backend deduce param type */
        (char**)args.data(),
        NULL,
        NULL,
        0);
    if ((PQresultStatus(query) != PGRES_COMMAND_OK) && (PQresultStatus(query) != PGRES_TUPLES_OK))
    {
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, PQerrorMessage((PGconn*)conn));
        PQclear(query);
        return 2;
    }


    PQclear(query);
    lua_pushboolean(luaVM, 1);
    return 1;
}
int CFunctions::pg_poll(lua_State* luaVM)
{
    void* argquery = lua_touserdata(luaVM, 1);
    
    PGresult* query = (PGresult*)argquery;
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
    //lua_settable(luaVM, -3);
    PQclear(query);
    return 1;
}

int CFunctions::pg_free(lua_State* luaVM)
{
    void* argquery = lua_touserdata(luaVM, 1);
    PGresult* query = (PGresult*)argquery;
    PQclear(query);
    lua_pushboolean(luaVM, 1);
    return 1;
}

int CFunctions::pg_close(lua_State* luaVM)
{
    void* conn = lua_touserdata(luaVM, 1);
    if (PQstatus((PGconn*)conn) != CONNECTION_OK && PQsetnonblocking((PGconn*)conn, 1) == 0) {
        char* errmsg = PQerrorMessage((PGconn*)conn);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    PQfinish((PGconn*)conn);
    lua_pushboolean(luaVM, 1);
    return 1;
}

int CFunctions::async_call(lua_State* luaVM, void* conn)
{

    int totalArgs = lua_gettop(luaVM);
    lua_pop(luaVM, 1);
    lua_pushlightuserdata(luaVM, (PGconn*)conn);
    lua_call(luaVM, 1, 0);

    return 0;
}

int CFunctions::pg_aquery(lua_State* luaVM)
{
    void* conn = lua_touserdata(luaVM, 1);
    // lua_remove(luaVM, 1);
    int totalArgs = lua_gettop(luaVM);
    // lua_pushlightuserdata(luaVM, conn);

    // THERE WAS ASYNC QUERY
    auto f = std::async(std::launch::async, async_call, luaVM, conn);
    // lua_cpcall(luaVM, fnc, NULL);

    lua_pushboolean(luaVM, 1);
    return 1;
}

int CFunctions::pg_aresult(lua_State* luaVM)
{
    void* conn = lua_touserdata(luaVM, 1);
    if (PQstatus((PGconn*)conn) != CONNECTION_OK && PQsetnonblocking((PGconn*)conn, 1) == 0) {
        char* errmsg = PQerrorMessage((PGconn*)conn);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    PGresult* res = PQgetResult((PGconn*)conn);
    lua_pushstring(luaVM, PQresStatus(PQresultStatus(res)));
    lua_pushlightuserdata(luaVM, res);
    return 2;
}
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
#include "ml_pgsql.h"
#include "CFunctions.h"

LUA_FUNCTION_DEFINE(CFunctions::pg_conn)
{
    LUA_FUNCTION_ASSERT("pg_conn", luaVM);
    LUA_FUNCTION_ASSERT("pg_conn", lua_gettop(luaVM) == 1);

    PGconn* connection = PQconnectdb(luaL_checkstring(luaVM, 1));
    if ((PQstatus(connection) != CONNECTION_OK))
    {
        LIBPQ_FINISH_AND_RETURN_ERROR(luaVM, connection);
    }
    else 
    {
        if (PQsetnonblocking(connection, 1) == 0) {
            lua_pushlightuserdata(luaVM, connection);
            return LUA_FUNCTION_SUCCESS;
        }
        else
        {
            LIBPQ_FINISH_AND_RETURN_ERROR(luaVM, connection);
        }
    }
}


int CFunctions::pg_query(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_query", luaVM);
    LUA_FUNCTION_ASSERT("pg_query", lua_gettop(luaVM) > 1);

    PGconn* connection = reinterpret_cast<PGconn*>(lua_touserdata(luaVM, 1));
    LUA_FUNCTION_ASSERT("pg_query", connection);

    if (PQstatus((PGconn*)connection) != CONNECTION_OK && PQsetnonblocking((PGconn*)connection, 1) == 0) {
        LIBPQ_RETURN_ERROR(luaVM, connection);
    }

    libpq_query_t query_str = luaL_checkstring(luaVM, 2);

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int args_count = lua_gettop(luaVM);

    std::vector<const char*> args{};
    for (int i = args_count; i > 0; --i) {
        args.push_back(luaL_checkstring(luaVM, i));
    }

    PGresult* query_result = PQexecParams((PGconn*)connection, query_str.c_str(), args.size(), NULL, (char**)args.data(), NULL, NULL, 0);

    if ((PQresultStatus(query_result) != PGRES_COMMAND_OK) && (PQresultStatus(query_result) != PGRES_TUPLES_OK))
    {
        LIBPQ_CLEAR_AND_RETURN_ERROR(luaVM, connection, query_result);
    }

    lua_pushlightuserdata(luaVM, query_result);
    return LUA_FUNCTION_SUCCESS;
}

int CFunctions::pg_exec(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_exec", luaVM);
    LUA_FUNCTION_ASSERT("pg_exec", lua_gettop(luaVM) > 1);

    PGconn* connection = reinterpret_cast<PGconn*>(lua_touserdata(luaVM, 1));
    LUA_FUNCTION_ASSERT("pg_exec", connection);

    if (PQstatus((PGconn*)connection) != CONNECTION_OK && PQsetnonblocking((PGconn*)connection, 1) == 0) {
        LIBPQ_RETURN_ERROR(luaVM, connection);
    }

    libpq_query_t query_str = luaL_checkstring(luaVM, 2);

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int args_count = lua_gettop(luaVM);

    std::vector<const char*> args{};
    for (int i = args_count; i > 0; --i) {
        args.push_back(luaL_checkstring(luaVM, i));
    }

    PGresult* query_result = PQexecParams((PGconn*)connection, query_str.c_str(), args.size(), NULL, (char**)args.data(), NULL, NULL, 0);

    if ((PQresultStatus(query_result) != PGRES_COMMAND_OK) && (PQresultStatus(query_result) != PGRES_TUPLES_OK))
    {
        LIBPQ_CLEAR_AND_RETURN_ERROR(luaVM, connection, query_result);
    }

    PQclear(query_result);
    lua_pushboolean(luaVM, 1);
    return LUA_FUNCTION_SUCCESS;
}
int CFunctions::pg_poll(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_poll", luaVM);
    LUA_FUNCTION_ASSERT("pg_poll", lua_gettop(luaVM) == 1);

    PGresult *query_result = reinterpret_cast<PGresult*>(lua_touserdata(luaVM, 1));
    LUA_FUNCTION_ASSERT("pg_poll", query_result);
    
    int ncols = PQnfields(query_result);
    int nrows = PQntuples(query_result);

    lua_newtable(luaVM);
    for (int i = 0; i < nrows; i++)
    {
        lua_newtable(luaVM);
        for (int coloumn = 0; coloumn < ncols; coloumn++)
        {
            char* colname = PQfname(query_result, coloumn);
            lua_pushstring(luaVM, colname);
            char* dataname = PQgetvalue(query_result, i, coloumn);
            lua_pushstring(luaVM, dataname);
            lua_settable(luaVM, -3);
        }
    }

    PQclear(query_result);
    return LUA_FUNCTION_SUCCESS;
}

int CFunctions::pg_free(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_free", luaVM);
    LUA_FUNCTION_ASSERT("pg_free", lua_gettop(luaVM) == 1);

    PGresult* query_result = reinterpret_cast<PGresult*>(lua_touserdata(luaVM, 1));
    LUA_FUNCTION_ASSERT("pg_free", query_result);

    PQclear(query_result);
    lua_pushboolean(luaVM, 1);
    return LUA_FUNCTION_SUCCESS;
}

int CFunctions::pg_close(lua_State* luaVM)
{
    LUA_FUNCTION_ASSERT("pg_close", luaVM);
    LUA_FUNCTION_ASSERT("pg_close", lua_gettop(luaVM) == 1);

    PGconn* connection = reinterpret_cast<PGconn*>(lua_touserdata(luaVM, 1));
    LUA_FUNCTION_ASSERT("pg_close", connection);

    if (PQstatus((PGconn*)connection) != CONNECTION_OK && PQsetnonblocking((PGconn*)connection, 1) == 0) {
        LIBPQ_RETURN_ERROR(luaVM, connection);
    }

    PQfinish((PGconn*)connection);
    lua_pushboolean(luaVM, 1);
    return LUA_FUNCTION_SUCCESS;
}
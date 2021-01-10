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

#include "CFunctions.h"
#include <unordered_map>

#define _TX_CLOSED 1
#define _TX_OPENED 2
#define _TX_ROLLBACKED 3
#define _TX_COMMITED 4
/*
    1 - has no opened transacition
    2 - has opened transaction
    3 - has rollbacked transaction
    4 - has commited transaction
*/
std::unordered_map<PGconn*, int> _CURRENT_TX_STATES = {{}};

void _tx_safety_close(PGconn *connection)
{
    switch (_CURRENT_TX_STATES[connection])
    {
    case _TX_CLOSED:
        break;
    case _TX_OPENED:
        PGresult *res = PQexec(connection, "ROLLBACK");
        break;
    case _TX_ROLLBACKED:
    case _TX_COMMITED:
        _CURRENT_TX_STATES[connection] = _TX_CLOSED;
        break;
    default:
        break;
    }
}

/*
    Creates new transaction only if has no other yet
*/
int CFunctions::pg_tx_begin(lua_State *luaVM)
{
    PGconn *conn = (PGconn *)lua_touserdata(luaVM, 1);

    if (PQstatus(conn) != CONNECTION_OK && PQsetnonblocking(conn, 1) == 0)
    {
        char *errmsg = PQerrorMessage(conn);
        lua_pushboolean(luaVM, false);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }

    switch (_CURRENT_TX_STATES[conn])
    {
    case _TX_CLOSED:
    {
        PGresult *res = PQexec(conn, "BEGIN");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            char *errmsg = PQerrorMessage(conn);
            lua_pushboolean(luaVM, false);
            lua_pushstring(luaVM, errmsg);
            return 2;
        }

        lua_pushboolean(luaVM, true);
        _CURRENT_TX_STATES[conn] = _TX_OPENED;
        return 1;
        break;
    }
    case _TX_OPENED:
    {
        return 1;
        break;
    }
    case _TX_COMMITED:
    case _TX_ROLLBACKED:
    {
        _tx_safety_close(conn);
        return pg_tx_begin(luaVM);
    }
    default:
        break;
    }

    return 1;
}

int CFunctions::pg_tx_commit(lua_State *luaVM)
{
    PGconn *conn = (PGconn *)lua_touserdata(luaVM, 1);

    if (PQstatus(conn) != CONNECTION_OK && PQsetnonblocking(conn, 1) == 0)
    {
        char *errmsg = PQerrorMessage(conn);
        lua_pushboolean(luaVM, false);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }

    switch (_CURRENT_TX_STATES[conn])
    {
    case _TX_OPENED:
    {
        PGresult *res = PQexec(conn, "COMMIT");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            char *errmsg = PQerrorMessage(conn);
            lua_pushboolean(luaVM, false);
            lua_pushstring(luaVM, errmsg);
            return 2;
        }

        lua_pushboolean(luaVM, true);
        _CURRENT_TX_STATES[conn] = _TX_COMMITED;
        return 1;
        break;
    }
    case _TX_COMMITED:
    {
        return 1;
        break;
    }
    case _TX_CLOSED:
    case _TX_ROLLBACKED:
    {
        _tx_safety_close(conn);
        return 1;
    }
    default:
        break;
    }

    return 1;
}

int CFunctions::pg_tx_rollback(lua_State *luaVM)
{
    PGconn *conn = (PGconn *)lua_touserdata(luaVM, 1);

    if (PQstatus(conn) != CONNECTION_OK && PQsetnonblocking(conn, 1) == 0)
    {
        char *errmsg = PQerrorMessage(conn);
        lua_pushboolean(luaVM, false);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }

    switch (_CURRENT_TX_STATES[conn])
    {
    case _TX_OPENED:
    {
        PGresult *res = PQexec(conn, "ROLLBACK");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            char *errmsg = PQerrorMessage(conn);
            lua_pushboolean(luaVM, false);
            lua_pushstring(luaVM, errmsg);
            return 2;
        }

        lua_pushboolean(luaVM, true);
        _CURRENT_TX_STATES[conn] = _TX_ROLLBACKED;
        return 1;
        break;
    }
    case _TX_ROLLBACKED:
    {
        return 1;
        break;
    }
    case _TX_CLOSED:
    case _TX_COMMITED:
    {
        _tx_safety_close(conn);
        return 1;
    }
    default:
        break;
    }

    return 1;
}

int CFunctions::pg_conn(lua_State *luaVM)
{
    const char *str = luaL_checkstring(luaVM, 1);
    PGconn *connection = PQconnectdb(str);
    if ((PQstatus(connection) != CONNECTION_OK))
    {
        char *errmsg = PQerrorMessage(connection);
        PQfinish(connection);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    else
    {
        if (PQsetnonblocking(connection, 1) == 0)
        {
            lua_pushlightuserdata(luaVM, connection);
            _CURRENT_TX_STATES[connection] = _TX_CLOSED;
            return 1;
        }
        else
        {
            char *errmsg = PQerrorMessage(connection);
            PQfinish(connection);
            lua_pushboolean(luaVM, 0);
            lua_pushstring(luaVM, errmsg);
            return 2;
        }
    }
}

int CFunctions::pg_query(lua_State *luaVM)
{
    void *conn = lua_touserdata(luaVM, 1);
    if (PQstatus((PGconn *)conn) != CONNECTION_OK && PQsetnonblocking((PGconn *)conn, 1) == 0)
    {
        char *errmsg = PQerrorMessage((PGconn *)conn);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    const char *str = luaL_checkstring(luaVM, 2);
    PGresult *query;

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int totalArgs = lua_gettop(luaVM);

    char *paramValues[255];
    for (int i = totalArgs; i > 0; --i)
    {
        const char *args = luaL_checkstring(luaVM, i);
        char *t_param = new char[255];
        strcpy(t_param, args);
        paramValues[i - 1] = t_param;
    }

    query = PQexecParams((PGconn *)conn,
                         str,
                         totalArgs, /* one param */
                         NULL,      /* let the backend deduce param type */
                         paramValues,
                         NULL,
                         NULL,
                         0);
    if ((PQresultStatus(query) != PGRES_COMMAND_OK) && (PQresultStatus(query) != PGRES_TUPLES_OK))
    {
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, PQerrorMessage((PGconn *)conn));
        PQclear(query);
        return 2;
    }

    lua_pushlightuserdata(luaVM, query);
    return 1;
}

int CFunctions::pg_exec(lua_State *luaVM)
{
    void *conn = lua_touserdata(luaVM, 1);
    if (PQstatus((PGconn *)conn) != CONNECTION_OK && PQsetnonblocking((PGconn *)conn, 1) == 0)
    {
        char *errmsg = PQerrorMessage((PGconn *)conn);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    const char *str = luaL_checkstring(luaVM, 2);
    PGresult *query;

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int totalArgs = lua_gettop(luaVM);

    char *paramValues[255];
    for (int i = totalArgs; i > 0; --i)
    {
        const char *args = luaL_checkstring(luaVM, i);
        char *t_param = new char[255];
        strcpy(t_param, args);
        paramValues[i - 1] = t_param;
    }

    query = PQexecParams((PGconn *)conn,
                         str,
                         totalArgs, /* one param */
                         NULL,      /* let the backend deduce param type */
                         paramValues,
                         NULL,
                         NULL,
                         0);
    if ((PQresultStatus(query) != PGRES_COMMAND_OK) && (PQresultStatus(query) != PGRES_TUPLES_OK))
    {
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, PQerrorMessage((PGconn *)conn));
        PQclear(query);
        return 2;
    }

    PQclear(query);
    lua_pushboolean(luaVM, 1);
    return 1;
}
int CFunctions::pg_poll(lua_State *luaVM)
{
    void *argquery = lua_touserdata(luaVM, 1);

    PGresult *query = (PGresult *)argquery;
    int ncols = PQnfields(query);
    int nrows = PQntuples(query);

    lua_newtable(luaVM);
    for (int i = 0; i < nrows; i++)
    {
        lua_newtable(luaVM);
        for (int coloumn = 0; coloumn < ncols; coloumn++)
        {

            char *colname = PQfname(query, coloumn);
            lua_pushstring(luaVM, colname);
            char *dataname = PQgetvalue(query, i, coloumn);
            lua_pushstring(luaVM, dataname);
            lua_settable(luaVM, -3);
        }
    }
    //lua_settable(luaVM, -3);
    PQclear(query);
    return 1;
}

int CFunctions::pg_free(lua_State *luaVM)
{
    void *argquery = lua_touserdata(luaVM, 1);
    PGresult *query = (PGresult *)argquery;
    PQclear(query);
    lua_pushboolean(luaVM, 1);
    return 1;
}

int CFunctions::pg_close(lua_State *luaVM)
{
    void *conn = lua_touserdata(luaVM, 1);
    if (PQstatus((PGconn *)conn) != CONNECTION_OK && PQsetnonblocking((PGconn *)conn, 1) == 0)
    {
        char *errmsg = PQerrorMessage((PGconn *)conn);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    PQfinish((PGconn *)conn);
    lua_pushboolean(luaVM, 1);
    return 1;
}
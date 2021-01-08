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

CFunctions* CFunctions::m_instance = 0;

int CFunctions::pg_conn(lua_State* luaVM)
{
    const char* connectionSring = luaL_checkstring(luaVM, 1);
    const int poolSize = luaL_checkinteger(luaVM, 2);

    //m_instance = new CFunctions();

    GetInstance()->createConnctionPool(connectionSring, poolSize);

    printf("test SiZe %d\n", GetInstance()->m_databaseConnectionPool.size());

    //printf("test SiZe %d\n", m_instance->m_databaseConnectionPool.size());
    

    /*PGconn* connection = PQconnectdb(str);
    if (PQstatus(connection) != CONNECTION_OK)
    {
        char* errmsg = PQerrorMessage(connection);
        lua_pushboolean(luaVM, 0);
        lua_pushstring(luaVM, errmsg);
        return 2;
    }
    else 
    {
        lua_pushlightuserdata(luaVM, connection);
    }*/


    return 1;
}


int CFunctions::pg_query(lua_State* luaVM)
{
    const char* query = luaL_checkstring(luaVM, 1);
    lua_remove(luaVM, 1);


    printf("pg_query test size%d\n", GetInstance()->m_databaseConnectionPool.size());

    auto databaseConnection = GetInstance()->connection();

    PQsendQuery(databaseConnection->connection().get(), query);

    while (auto result = PQgetResult(databaseConnection->connection().get())) {
        if (PQresultStatus(result) == PGRES_TUPLES_OK && PQntuples(result)) {
            printf("KOSTYAN, PIZDYI V AMERIKU!\n");
        }

        if (PQresultStatus(result) == PGRES_FATAL_ERROR) {
            printf("pezda, error\n");
        }
    }

    printf("pg_query test size%d\n", GetInstance()->m_databaseConnectionPool.size());
    GetInstance()->freeConnection(databaseConnection);
    printf("pg_query test size%d\n", GetInstance()->m_databaseConnectionPool.size());
    /*void* conn = lua_touserdata(luaVM, 1);
    const char* str = luaL_checkstring(luaVM, 2);
    PGresult* query;

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int totalArgs = lua_gettop(luaVM);



    char* paramValues[255];
    for (int i = totalArgs; i > 0; --i) {
        const char* args = luaL_checkstring(luaVM, i);
        char* t_param = new char[255];
        strcpy(t_param, args);
        paramValues[i-1] = t_param;
    }


    query = PQexecParams((PGconn*)conn,
        str,
        totalArgs,      
        NULL,    
        paramValues,
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
     


    lua_pushlightuserdata(luaVM, query);*/
    return 1;
}

int CFunctions::pg_exec(lua_State* luaVM)
{
    void* conn = lua_touserdata(luaVM, 1);
    // error handlers
    if (PQstatus((PGconn*)conn) != CONNECTION_OK) 
    {
        return 0;
    }
    const char* str = luaL_checkstring(luaVM, 2);
    PGresult* query;

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int totalArgs = lua_gettop(luaVM);



    char* paramValues[255];
    for (int i = totalArgs; i > 0; --i) {
        const char* args = luaL_checkstring(luaVM, i);
        char* t_param = new char[255];
        strcpy(t_param, args);
        paramValues[i - 1] = t_param;
    }


    query = PQexecParams((PGconn*)conn,
        str,
        totalArgs,       /* one param */
        NULL,    /* let the backend deduce param type */
        paramValues,
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

void CFunctions::createConnctionPool(const char* connectionString, int poolSize) {
    std::lock_guard<std::mutex> lock(m_databaseMutex);
    unsigned short int counter = 0;
    for (auto i = 0; i < poolSize; i++) {
        m_databaseConnectionPool.emplace(std::make_shared<CPGConnection>(connectionString));
        counter++;
    }
    printf("Pool size: %d", counter);
}

std::shared_ptr<CPGConnection> CFunctions::connection() {
    std::unique_lock<std::mutex> lock(m_databaseMutex);
    while (m_databaseConnectionPool.empty()) {
        m_databaseCondition.wait(lock);
    }

    auto databaseConnection = m_databaseConnectionPool.front();
    m_databaseConnectionPool.pop();
    return databaseConnection;
}

void CFunctions::freeConnection(std::shared_ptr<CPGConnection> databaseConnection) {
    std::unique_lock<std::mutex> lock(m_databaseMutex);
    m_databaseConnectionPool.push(databaseConnection);
    lock.unlock();
    m_databaseCondition.notify_one();
}

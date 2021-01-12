/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*  
*  Copyright ? 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is (c) 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

#include "CPostgresConnection.hpp"

CPostgresConnection::CPostgresConnection(lua_State* pLuaVM, const char* szConnectionInfo) : m_pLuaVM{pLuaVM}
{
    m_pConnection = PQconnectdb(szConnectionInfo);
}


CPostgresConnection::~CPostgresConnection()
{
    if (m_pConnection)
        PQfinish(m_pConnection);
}

PGresult* CPostgresConnection::Query(lua_State* luaVM)
{
    libpq_query_t query_str = luaL_checkstring(luaVM, 2);

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int args_count = lua_gettop(luaVM);

    std::vector<const char*> args{};
    for (int i = args_count; i > 0; --i) {
        args.push_back(luaL_checkstring(luaVM, i));
    }

    PGresult* pResult = PQexecParams(m_pConnection, query_str.c_str(), args.size(), NULL, (char**)args.data(), NULL, NULL, 0);

    if (pResult && PQresultStatus(pResult) == PGRES_TUPLES_OK)
    {
        return pResult;
    }

    return nullptr;
}

bool CPostgresConnection::Exec(lua_State* luaVM)
{
    libpq_query_t query_str = luaL_checkstring(luaVM, 2);

    lua_remove(luaVM, 1);
    lua_remove(luaVM, 1);

    int args_count = lua_gettop(luaVM);

    std::vector<const char*> args{};
    for (int i = args_count; i > 0; --i) {
        args.push_back(luaL_checkstring(luaVM, i));
    }

    PGresult* pResult = PQexecParams(m_pConnection, query_str.c_str(), args.size(), NULL, (char**)args.data(), NULL, NULL, 0);

    if (PQresultStatus(pResult) == PGRES_TUPLES_OK)
    {
        return false;
    }

    PQclear(pResult);
    return true;
}

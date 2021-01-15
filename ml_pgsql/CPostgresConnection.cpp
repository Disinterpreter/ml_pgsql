#include "CPostgresConnection.h"


CPostgresConnection::CPostgresConnection(lua_State* pLuaVM, std::string sConnectionInfo) : m_pLuaVM{pLuaVM}
{
    m_pConnection = PQconnectdb(sConnectionInfo.c_str());
}


CPostgresConnection::~CPostgresConnection()
{
    PQfinish(m_pConnection);
}

PGresult* CPostgresConnection::Query(std::string sQuery)
{
    //int args_count = lua_gettop(luaVM);

    std::vector<const char*> args{};
    /*for (int i = args_count; i > 0; --i) {
        args.push_back(luaL_checkstring(luaVM, i));
    }*/

    PGresult* pResult = PQexecParams(m_pConnection, sQuery.c_str(), args.size(), NULL, (char**)args.data(), NULL, NULL, 0);

    if (pResult && PQresultStatus(pResult) == PGRES_TUPLES_OK)
    {
        return pResult;
    }

    return nullptr;
}

bool CPostgresConnection::Exec(lua_State* luaVM)
{
    libpq_query_t query_str = luaL_checkstring(luaVM, 2);

    lua_pop(luaVM, 1);

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

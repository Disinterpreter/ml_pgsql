#include "CPostgresConnection.h"


CPostgresConnection::CPostgresConnection(lua_State* pLuaVM, const char* szConnectionInfo) : m_pLuaVM{pLuaVM}
{
    m_pConnection = PQconnectdb(szConnectionInfo);
}


CPostgresConnection::~CPostgresConnection()
{
    PQfinish(m_pConnection);
}

PGresult* CPostgresConnection::Query(lua_State* luaVM)
{

    std::map<int, const char*> mmArgsAndTypes;
    int args_count = lua_gettop(luaVM);
    for (int i = 1; i <= args_count; i++) {
        const char* namearg = lua_typename(luaVM, lua_type(luaVM, i));
        mmArgsAndTypes.insert({ i, namearg });
    }

    lua_remove(luaVM, 1);
    if (strcmp(mmArgsAndTypes[2], "function") == 0) {
        lua_remove(luaVM, 1);
        if (strcmp(mmArgsAndTypes[3], "table") == 0) {
            lua_remove(luaVM, 1);
        }
    }

    libpq_query_t query_str = luaL_checkstring(luaVM, 1);
    lua_remove(luaVM, 1);

    std::vector<const char*> args{};
    for (int i = lua_gettop(luaVM); i > 0; --i) {
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

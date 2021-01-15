#pragma once

#include "ml_pgsql.h"

class CPostgresConnection
{
private:
    lua_State* m_pLuaVM = nullptr;
    PGconn* m_pConnection = nullptr;

public:
    CPostgresConnection(lua_State* pLuaVM, std::string sConnectionInfo);
    ~CPostgresConnection();

    lua_State* GetVM() { return m_pLuaVM; }
    PGconn* GetConnection() { return m_pConnection; }

    bool IsConnected() { return PQstatus(m_pConnection) == CONNECTION_OK; }
    const char* GetLastErrorMessage() { return PQerrorMessage(m_pConnection); }

    PGresult* Query(lua_State* luaVM);
    bool Exec(lua_State* luaVM);
};

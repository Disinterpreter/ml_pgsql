#pragma once

#include "ml_pgsql.h"
#include "CPostgresConnection.h"

#include <vector>

class CPostgresManager
{
private:
    std::vector<CPostgresConnection*> m_vecConnections;

public:
    CPostgresManager();
    ~CPostgresManager();

    void Add(CPostgresConnection* pConnection) { m_vecConnections.push_back(pConnection); }

    static CPostgresConnection* NewConnection(lua_State* pLuaVM);
    void CloseAllConnections(lua_State* pLuaVM = nullptr);
    void RemoveConnection(CPostgresConnection* pConnection);
};

extern std::unique_ptr<CPostgresManager> g_pPostgresManager;

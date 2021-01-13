#include "CPostgresManager.h"


CPostgresManager::CPostgresManager()
{

}


CPostgresManager::~CPostgresManager()
{
    CloseAllConnections();
}


CPostgresConnection* CPostgresManager::NewConnection(lua_State* pLuaVM)
{
    const char* szConnectionInfo = luaL_checkstring(pLuaVM, 1);
    CPostgresConnection* pConnection = new CPostgresConnection(pLuaVM, szConnectionInfo);
    
    if (pConnection && pConnection->IsConnected())
        g_pPostgresManager->Add(pConnection);

    return pConnection;
}

void CPostgresManager::CloseAllConnections(lua_State* pLuaVM)
{
    m_vecConnections.erase(
        std::remove_if(
            m_vecConnections.begin(),
            m_vecConnections.end(),
            [=](auto& pConnection) -> bool {
                if (!pLuaVM || (pConnection && pConnection->GetVM() == pLuaVM))
                {
                    SAFE_DELETE(pConnection);
                    return true;
                }

                return false;
            }
        ),
        m_vecConnections.end()
    );
}

void CPostgresManager::RemoveConnection(CPostgresConnection* pConnection)
{
    int i = 0;
    for (auto& connection : m_vecConnections)
    {
        if(pConnection == connection)
        {
            SAFE_DELETE(connection);
            m_vecConnections.erase(m_vecConnections.begin() + i);
        }

        i++;
    }
}

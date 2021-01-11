/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*  
*  Copyright � 2003-2008 MTA.  All Rights Reserved.
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

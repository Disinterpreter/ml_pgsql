/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*  
*  Copyright (c) 2003-2008 MTA.  All Rights Reserved.
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

#include "ml_pgsql.hpp"

class CPostgresConnection
{
private:
    lua_State* m_pLuaVM = nullptr;
    PGconn* m_pConnection = nullptr;

public:
    CPostgresConnection(lua_State* pLuaVM, const char* szConnectionInfo);
    ~CPostgresConnection();

    lua_State* GetVM() { return m_pLuaVM; }
    PGconn* GetConnection() { return m_pConnection; }

    bool IsConnected() { return PQstatus(m_pConnection) == CONNECTION_OK; }
    const char* GetLastErrorMessage() { return PQerrorMessage(m_pConnection); }

    PGresult* Query(lua_State* luaVM);
    bool Exec(lua_State* luaVM);
};

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

class CFunctions;

#ifndef __CFUNCTIONS_H
#define __CFUNCTIONS_H

#include <stdio.h>
#include <libpq-fe.h>
#include <mutex>
#include <queue>

#include <CPGConnection.h>

#include "ml_pgsql.h"
#include "include/ILuaModuleManager.h"
#pragma comment(lib, "./lib/x64/libpq.lib")

extern ILuaModuleManager10 *pModuleManager;

class CFunctions
{
private:
    static CFunctions instance;

    std::mutex m_databaseMutex;
    std::condition_variable m_databaseCondition;
    std::queue<std::shared_ptr<CPGConnection>> m_databaseConnectionPool;

    void createConnctionPool(const char* connectionString, int poolSize = 1);
    std::shared_ptr<CPGConnection> connection();
    void freeConnection(std::shared_ptr<CPGConnection> databaseConnection);

public:
    static int pg_conn(lua_State* luaVM);
    static int pg_query(lua_State* luaVM);
    static int pg_poll(lua_State* luaVM);
    static int pg_free(lua_State* luaVM);
    static int pg_exec(lua_State* luaVM);
};
#endif

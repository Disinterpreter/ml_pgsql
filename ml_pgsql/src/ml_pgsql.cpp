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
#include "ml_pgsql.h"
#include "CFunctions.h"
#include "CPostgresManager.h"

ILuaModuleManager10* g_pLuaModuleManager;
std::unique_ptr<CPostgresManager> g_pPostgresManager;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
    g_pLuaModuleManager = pManager;
    g_pPostgresManager.reset(new CPostgresManager());

	// Set the module info
	strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
	strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
	(*fVersion) = MODULE_VERSION;

	return ImportLua();
}


MTAEXPORT void RegisterFunctions(lua_State *luaVM)
{
	if (g_pLuaModuleManager && luaVM)
	{
        // Register functions
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_conn", CFunctions::pg_conn);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_query", CFunctions::pg_query);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_exec", CFunctions::pg_exec);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_poll", CFunctions::pg_poll);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_free", CFunctions::pg_free);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_close", CFunctions::pg_close);
	}
}


MTAEXPORT bool DoPulse(void)
{
	return true;
}


MTAEXPORT void ResourceStopped(lua_State *luaVM)
{
    g_pPostgresManager->CloseAllConnections(luaVM);
}


MTAEXPORT void ShutdownModule()
{

}

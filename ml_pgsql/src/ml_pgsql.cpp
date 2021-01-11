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

std::unique_ptr<ILuaModuleManager10> g_pLuaModuleManager;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
    g_pLuaModuleManager.reset(pManager);

	// Set the module info
	strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
	strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
	(*fVersion) = MODULE_VERSION;

    if (!ImportLua())
    {
        g_pLuaModuleManager.reset();
        return false;
    }

	return true;
}


MTAEXPORT void RegisterFunctions(lua_State *luaVM)
{
	if (g_pLuaModuleManager.get() && luaVM)
	{
        // Register functions
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_conn", CFunctions::pg_conn);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_query", CFunctions::pg_query);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_exec", CFunctions::pg_exec);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_poll", CFunctions::pg_poll);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_free", CFunctions::pg_free);
        g_pLuaModuleManager->RegisterFunction(luaVM, "pg_close", CFunctions::pg_free);
	}
}


MTAEXPORT bool DoPulse(void)
{
	return true;
}


MTAEXPORT void ResourceStopped(lua_State *luaVM)
{

}


MTAEXPORT bool ShutdownModule(void)
{
    g_pLuaModuleManager.reset();
	return true;
}

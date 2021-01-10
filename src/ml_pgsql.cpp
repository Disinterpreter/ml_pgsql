/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*  
*  Copyright © 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is © 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

#include "ml_pgsql.h"
#include "luaimports.h"
#include <signal.h>

ILuaModuleManager10 *pModuleManager = NULL;
bool ms_bInitWorked = false;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule ( ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion )
{
	pModuleManager = pManager;

	// Set the module info
	strncpy ( szModuleName, MODULE_NAME, MAX_INFO_LENGTH );
	strncpy ( szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH );
	(*fVersion) = MODULE_VERSION;

    if ( !ImportLua() )
    {
        return false;
    }

    ms_bInitWorked = true;
	return true;
}


MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
    if ( !ms_bInitWorked )
        return;

	if ( pModuleManager && luaVM )
	{
        // Register functions
        pModuleManager->RegisterFunction(luaVM, "pg_conn", CFunctions::pg_conn);
        pModuleManager->RegisterFunction(luaVM, "pg_query", CFunctions::pg_query);
        pModuleManager->RegisterFunction(luaVM, "pg_exec", CFunctions::pg_exec);
        pModuleManager->RegisterFunction(luaVM, "pg_poll", CFunctions::pg_poll);
        pModuleManager->RegisterFunction(luaVM, "pg_free", CFunctions::pg_free);
        pModuleManager->RegisterFunction(luaVM, "pg_close", CFunctions::pg_free);
        pModuleManager->RegisterFunction(luaVM, "pg_aquery", CFunctions::pg_aquery);
        pModuleManager->RegisterFunction(luaVM, "pg_aresult", CFunctions::pg_aresult);
	}
}


MTAEXPORT bool DoPulse ( void )
{

	return true;
}


MTAEXPORT void ResourceStopped ( lua_State * luaVM )
{

}


MTAEXPORT bool ShutdownModule ( void )
{



	return true;
}

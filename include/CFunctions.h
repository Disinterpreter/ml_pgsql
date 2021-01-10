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
#pragma once

class CFunctions
{
public:
    LUA_FUNCTION_DECLARE(pg_conn);
    LUA_FUNCTION_DECLARE(pg_query);
    LUA_FUNCTION_DECLARE(pg_poll);
    LUA_FUNCTION_DECLARE(pg_free);
    LUA_FUNCTION_DECLARE(pg_exec);
    LUA_FUNCTION_DECLARE(pg_close);
};
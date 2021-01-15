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
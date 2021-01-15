#pragma once

#ifdef __linux__
#include <cstring>
#endif

/* Module basic configuration */
#define MODULE_NAME		"PostgreSQL"
#define MODULE_AUTHOR	"Disi and xLuxy"
#define MODULE_VERSION	0.6f

/* MTA-SA Module SDK */
#include "Common.h"
#include "ILuaModuleManager.h"
#include "lua/lua.h"


/* Function-related defines for easier working with API */
#define LUA_FUNCTION_DECLARE(function)  static int function(lua_State* luaVM)
#define LUA_FUNCTION_ASSERT(function, expression) { if(!(expression)) { lua_pushboolean(luaVM, 0); lua_pushstring(luaVM, "Assertation failed in function " function ": " #expression); return 2; } }

#define SAFE_DELETE(ptr) { delete ptr; ptr = nullptr; }

/* LUA imports */
#include "lua/luaimports.h"

/* Standard Library useful includes */
#include <memory> // smart pointers
#include <string> // std::string

/* libpq SDK */
#include <libpq/libpq-fe.h>

/* Define types here in case we need to change them in the future */
typedef std::string libpq_query_t;

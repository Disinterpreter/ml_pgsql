#pragma once

#include <cmath>
#include <string>
// #include "lua/CLuaArgument.h"

class CScriptArgStream
{
private:
	lua_State* m_pLuaVM;
	int m_iIndex;

public:	
	explicit CScriptArgStream(lua_State* pLuaVM) : m_pLuaVM(pLuaVM), m_iIndex(1) { }

	template <class T>
	bool ReadNumber(T& outValue)
	{
		static_assert(std::is_arithmetic_v<T>, "T must be arithmetic");

		switch (lua_type(m_pLuaVM, m_iIndex))
		{
		case LUA_TNUMBER:
		case LUA_TSTRING:
		{
			lua_Number num = lua_tonumber(m_pLuaVM, m_iIndex);

			if (std::isnan(num) || std::isinf(num))
				outValue = static_cast<T>(0);
			else
				outValue = static_cast<T>(num);
			break;
		}
		case LUA_TBOOLEAN:
		{
			outValue = lua_toboolean(m_pLuaVM, m_iIndex);
			break;
		}
		default:
			return false;
		}

		m_iIndex++;
		return true;
	}


    template <class T>
    bool ReadUserData(T*& outValue)
    {
		if (!lua_isuserdata(m_pLuaVM, m_iIndex))
			return false;

		outValue = reinterpret_cast<T*>(lua_touserdata(m_pLuaVM, m_iIndex));
		m_iIndex++;
		return outValue != nullptr;
    }


	bool ReadBool(bool& outValue)
	{
		return ReadNumber(outValue) == 1;
	}


	bool ReadString(std::string& outValue)
	{
		if (!lua_isstring(m_pLuaVM, m_iIndex))
			return false;
		
		outValue = std::string(lua_tostring(m_pLuaVM, m_iIndex));
		m_iIndex++;
		return true;
	}


	/*bool ReadTable(CLuaArguments& outValue)
	{
		if (!lua_istable(m_pLuaVM, m_iIndex))
			return false;

		
		//CLuaArguments* pArguments;

		while (!lua_isnone(m_pLuaVM, m_iIndex))
		{
			// CLuaArguments* pArg = new CLuaArguments(m_pLuaVM);

			// pArg.Read();
			m_iIndex++;
		}
	}*/
};

/*
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8
*/
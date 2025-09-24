// Windows.h includes debugapi.h, and compile error will occur if just including debugapi.h for undefined macros in Windows.h
#pragma warning(disable: 5045) // Qspectre is not supported

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4244)
#pragma warning(disable: 4242)
#include <algorithm>
#pragma warning(pop)
#include <cassert>
#include <cctype>
#include <malloc.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <iostream>
#include <string>
#include <unordered_map>
#pragma warning(push)
#pragma warning(disable: 5039)
#pragma warning(disable: 4668)
#include <Windows.h>
#pragma warning(pop)
#include <consoleapi2.h>

#define EXPORT_COMMAND_EXECUTION

#include "CommandExecution.h"

///////////////////////////////////////////////////////////////////////////////
//                               CommandExecution                            //
///////////////////////////////////////////////////////////////////////////////
void CE_HandleCommandArgs(const int _Argc, const char* const _Argv[], CEKeylessOption _DefaultTarget, CECommandOption* _Options, const size_t _OptionCount)
{
	bool isDefaultTargetVisited = false;
	bool isVisited = false;
	std::unordered_map<CECommandOption*, bool> isArgumentVisited;
	isArgumentVisited.reserve(_OptionCount);

	if (_Argc > 1)
	{
		for (int i = 1; i < _Argc; ++i)
		{
			int len = (int)strlen( _Argv[i] );
			switch (len)
			{
			case 3: __fallthrough;
			case 2: __fallthrough;
			case 1:
				if ( isVisited ) { std::cout << " "; }
				std::cout << std::string( _Argv[i] );
				_DefaultTarget.FuncPtr( _Argv[i] );
				isDefaultTargetVisited = true;
				isVisited = true;
				continue;
			default:
				// 정규식: R("[^\-][^\-][^\-\=]*(?!\=)")과 일치하는 인자 찾으면 default target 입니다.
				if ( _Argv[i][0] != '-' && _Argv[i][1] != '-' )
				{
					assert(_Argv[i][2] != '='); // '='가 3번째 글자에 온다면 이상한 값("--=")입니다.
					bool hasEqual = false;
					for (int j = 2; j < len; ++j)
					{
						if (_Argv[i][j] == '=')
						{
							hasEqual = true;
							break;
						}
					}
					if (hasEqual == false)
					{
						if ( isVisited ) { std::cout << " "; }
						std::cout << std::string( _Argv[i] );
						_DefaultTarget.FuncPtr( _Argv[i] );
						isDefaultTargetVisited = true;
						isVisited = true;
						continue;
					}
				}
				break;
			}

			for (size_t j = 0; j < _OptionCount; ++j)
			{
				std::string keyPrefix = std::string("--") + _Options[j].Key + "=";
				bool isStartWith = keyPrefix.compare(0, keyPrefix.length(), _Argv[i], 0, keyPrefix.length()) == 0;
				if (isStartWith)
				{
					std::cout << " --" << _Options[j].Key << "=" << std::string( _Argv[i] + keyPrefix.length() );
					_Options[i].FuncPtr( _Argv[i] + keyPrefix.length() );
					isArgumentVisited[&_Options[j]] = true;
					isVisited = true;
					break;
				}
				else
				{
					std::string loArg = _Argv[i];
					std::transform(loArg.begin(), loArg.end(), loArg.begin(), [](unsigned char c) { return std::tolower(c); });
					std::string loAliasKeyPrefix = std::string("-") + _Options[j].AliasKey + "=";
					std::transform(loAliasKeyPrefix.begin(), loAliasKeyPrefix.end(), loAliasKeyPrefix.begin(), [](unsigned char c) { return std::tolower(c); });
					isStartWith = loAliasKeyPrefix.compare(0, loAliasKeyPrefix.length(), loArg, 0, loAliasKeyPrefix.length()) == 0;
					if (isStartWith)
					{
						std::cout << " -" << _Options[j].AliasKey << "=" << std::string( _Argv[i] + loAliasKeyPrefix.length() );
						_Options[i].FuncPtr( _Argv[i] + loAliasKeyPrefix.length() );
						isArgumentVisited[&_Options[j]] = true;
						isVisited = true;
						break;
					}
				}
			}
		}
	}

	if ( isDefaultTargetVisited == false )
	{
		_DefaultTarget.FuncPtr( _DefaultTarget.DefaultValue );
		if ( isVisited )
		{
			std::cout << " ";
		}
		std::cout << std::string( _DefaultTarget.DefaultValue );
	}

	for (size_t i = 0; i < _OptionCount; ++i)
	{
		if ( isArgumentVisited[&_Options[i]] == false )
		{
			std::cout << " --" << _Options[i].Key << "(" << _Options[i].AliasKey << ")=" << std::string(_Options[i].DefaultValue);
			_Options[i].FuncPtr( _Options[i].DefaultValue );
		}
	}
	printf("\n");
}

CECommandOption CEUtility_GetDefaultCommandOption(CEDefaultCommandOption _DefaultOption)
{
	switch (_DefaultOption)
	{
	case CE_DEFAULT_COMMAND_OPTION_CODE_PAGE:
		return { "CodePage", "cp", "65001", CEUtility_SetConsoleOutputCP };
	case CE_DEFAULT_COMMAND_OPTION_LOCALE:
		return { "Locale", "l", ".utf8", CEUtility_SetLocale };
	}

	assert(false); // MUST NOT REACH HERE!!!
	return { 0 };
}


void CEUtility_SetConsoleOutputCP(const char* _Str)
{
	SetConsoleOutputCP(std::stoul(_Str));
}

void CEUtility_SetLocale(const char* _Str)
{
	setlocale(LC_ALL, _Str);
}
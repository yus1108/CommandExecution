// Windows.h includes debugapi.h, and compile error will occur if just including debugapi.h for undefined macros in Windows.h
#pragma warning(disable: 5045) // Qspectre is not supported

#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4242)
#pragma warning(disable: 4365)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <clocale>
#include <iostream>
#include <string>
#include <unordered_map>
#include <Windows.h>
#include <consoleapi2.h>
#pragma warning(pop)

#define EXPORT_COMMAND_EXECUTION

#include "CommandExecution.h"

///////////////////////////////////////////////////////////////////////////////
//                               CommandExecution                            //
///////////////////////////////////////////////////////////////////////////////
void CE_HandleCommandArgsA(const int _Argc, const char* const _Argv[], CEKeylessOptionA _DefaultTarget, CECommandOptionA* _Options, const size_t _OptionCount)
{
	bool isDefaultTargetVisited = false;
	bool isVisited = false;
	std::unordered_map<CECommandOptionA*, bool> isArgumentVisited;
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
					_Options[j].FuncPtr( _Argv[i] + keyPrefix.length() );
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
						_Options[j].FuncPtr(_Argv[i] + loAliasKeyPrefix.length());
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
	std::cout << std::endl;
}

COMMAND_EXECUTION_API void CE_HandleCommandArgsW(const int _Argc, const wchar_t* const _Argv[], CEKeylessOptionW _DefaultTarget, CECommandOptionW* _Options, const size_t _OptionCount)
{
	bool isDefaultTargetVisited = false;
	bool isVisited = false;
	std::unordered_map<CECommandOptionW*, bool> isArgumentVisited;
	isArgumentVisited.reserve(_OptionCount);

	if (_Argc > 1)
	{
		for (int i = 1; i < _Argc; ++i)
		{
			int len = static_cast<int>(wcslen(_Argv[i]));
			switch (len)
			{
			case 3: __fallthrough;
			case 2: __fallthrough;
			case 1:
				if (isVisited) { std::cout << " "; }
				std::wcout << std::wstring(_Argv[i]);
				_DefaultTarget.FuncPtr(_Argv[i]);
				isDefaultTargetVisited = true;
				isVisited = true;
				continue;
			default:
				// 정규식: R("[^\-][^\-][^\-\=]*(?!\=)")과 일치하는 인자 찾으면 default target 입니다.
				if (_Argv[i][0] != '-' && _Argv[i][1] != '-')
				{
					assert(_Argv[i][2] != L'='); // '='가 3번째 글자에 온다면 이상한 값("--=")입니다.
					bool hasEqual = false;
					for (int j = 2; j < len; ++j)
					{
						if (_Argv[i][j] == L'=')
						{
							hasEqual = true;
							break;
						}
					}
					if (hasEqual == false)
					{
						if (isVisited) { std::cout << " "; }
						std::wcout << std::wstring(_Argv[i]);
						_DefaultTarget.FuncPtr(_Argv[i]);
						isDefaultTargetVisited = true;
						isVisited = true;
						continue;
					}
				}
				break;
			}

			for (size_t j = 0; j < _OptionCount; ++j)
			{
				std::wstring keyPrefix = std::wstring(L"--") + _Options[j].Key + L"=";
				bool isStartWith = keyPrefix.compare(0, keyPrefix.length(), _Argv[i], 0, keyPrefix.length()) == 0;
				if (isStartWith)
				{
					std::wcout << L" --" << _Options[j].Key << L"=" << std::wstring(_Argv[i] + keyPrefix.length());
					_Options[j].FuncPtr(_Argv[i] + keyPrefix.length());
					isArgumentVisited[&_Options[j]] = true;
					isVisited = true;
					break;
				}
				else
				{
					std::wstring loArg = _Argv[i];
					std::transform(loArg.begin(), loArg.end(), loArg.begin(), [](unsigned char c) { return std::tolower(c); });
					std::wstring loAliasKeyPrefix = std::wstring(L"-") + _Options[j].AliasKey + L"=";
					std::transform(loAliasKeyPrefix.begin(), loAliasKeyPrefix.end(), loAliasKeyPrefix.begin(), [](unsigned char c) { return std::tolower(c); });
					isStartWith = loAliasKeyPrefix.compare(0, loAliasKeyPrefix.length(), loArg, 0, loAliasKeyPrefix.length()) == 0;
					if (isStartWith)
					{
						std::wcout << L" -" << _Options[j].AliasKey << L"=" << std::wstring(_Argv[i] + loAliasKeyPrefix.length());
						_Options[j].FuncPtr(_Argv[i] + loAliasKeyPrefix.length());
						isArgumentVisited[&_Options[j]] = true;
						isVisited = true;
						break;
					}
				}
			}
		}
	}

	if (isDefaultTargetVisited == false)
	{
		_DefaultTarget.FuncPtr(_DefaultTarget.DefaultValue);
		if (isVisited)
		{
			std::cout << " ";
		}
		std::wcout << std::wstring(_DefaultTarget.DefaultValue);
	}

	for (size_t i = 0; i < _OptionCount; ++i)
	{
		if (isArgumentVisited[&_Options[i]] == false)
		{
			std::wcout << L" --" << _Options[i].Key << L"(" << _Options[i].AliasKey << L")=" << std::wstring(_Options[i].DefaultValue);
			_Options[i].FuncPtr(_Options[i].DefaultValue);
		}
	}
	std::wcout << std::endl;
}

#include <sstream>
static std::vector<std::wstring> SplitByWhitespace(const std::wstring& s)
{
	std::wstringstream wss(s);
	std::vector<std::wstring> out;
#pragma warning(suppress : 26800) // C26800: Use of 'std::basic_istream<...>::operator>>' may be unsafe: this function does not perform bounds checking.
	for (std::wstring token; wss >> token;) { out.push_back(std::move(token)); } // 연속 공백은 무시됨
	return out;
}

COMMAND_EXECUTION_API void CE_HandleWinAPICmdLine(const wchar_t* _CommandLine, CEKeylessOptionW _DefaultTarget, CECommandOptionW* _Options, const size_t _OptionCount)
{
	bool isDefaultTargetVisited = false;
	bool isVisited = false;
	std::unordered_map<CECommandOptionW*, bool> isArgumentVisited;
	isArgumentVisited.reserve(_OptionCount);

	if (_CommandLine != nullptr && lstrcmpW(_CommandLine, L"") != 0)
	{
		std::vector<std::wstring> args = SplitByWhitespace(std::wstring(_CommandLine));
		for (auto arg : args)
		{
			size_t len = arg.size();
			switch (len)
			{
			case 3: __fallthrough;
			case 2: __fallthrough;
			case 1:
				if (isVisited) { std::cout << " "; }
				std::wcout << arg;
				_DefaultTarget.FuncPtr(arg.data());
				isDefaultTargetVisited = true;
				isVisited = true;
				continue;
			default:
				// 정규식: R("[^\-][^\-][^\-\=]*(?!\=)")과 일치하는 인자 찾으면 default target 입니다.
				if (arg[0] != '-' && arg[1] != '-')
				{
					assert(arg[2] != L'='); // '='가 3번째 글자에 온다면 이상한 값("--=")입니다.
					bool hasEqual = false;
					for (size_t i = 2; i < len; ++i)
					{
						if (arg[i] == L'=')
						{
							hasEqual = true;
							break;
						}
					}
					if (hasEqual == false)
					{
						if (isVisited) { std::cout << " "; }
						std::wcout << arg;
						_DefaultTarget.FuncPtr(arg.data());
						isDefaultTargetVisited = true;
						isVisited = true;
						continue;
					}
				}
				break;
			}

			for (size_t i = 0; i < _OptionCount; ++i)
			{
				std::wstring keyPrefix = std::wstring(L"--") + _Options[i].Key + L"=";
				bool isStartWith = keyPrefix.compare(0, keyPrefix.length(), arg, 0, keyPrefix.length()) == 0;
				if (isStartWith)
				{
					std::wcout << L" --" << _Options[i].Key << L"=" << std::wstring(arg.data() + keyPrefix.length());
					_Options[i].FuncPtr(arg.data() + keyPrefix.length());
					isArgumentVisited[&_Options[i]] = true;
					isVisited = true;
					break;
				}
				else
				{
					std::wstring loArg = arg;
					std::transform(loArg.begin(), loArg.end(), loArg.begin(), [](unsigned char c) { return std::tolower(c); });
					std::wstring loAliasKeyPrefix = std::wstring(L"-") + _Options[i].AliasKey + L"=";
					std::transform(loAliasKeyPrefix.begin(), loAliasKeyPrefix.end(), loAliasKeyPrefix.begin(), [](unsigned char c) { return std::tolower(c); });
					isStartWith = loAliasKeyPrefix.compare(0, loAliasKeyPrefix.length(), loArg, 0, loAliasKeyPrefix.length()) == 0;
					if (isStartWith)
					{
						std::wcout << L" -" << _Options[i].AliasKey << L"=" << std::wstring(arg.data() + loAliasKeyPrefix.length());
						_Options[i].FuncPtr(arg.data() + loAliasKeyPrefix.length());
						isArgumentVisited[&_Options[i]] = true;
						isVisited = true;
						break;
					}
				}
			}
		}
	}

	if (isDefaultTargetVisited == false)
	{
		_DefaultTarget.FuncPtr(_DefaultTarget.DefaultValue);
		if (isVisited)
		{
			std::cout << " ";
		}
		std::wcout << std::wstring(_DefaultTarget.DefaultValue);
	}

	for (size_t i = 0; i < _OptionCount; ++i)
	{
		if (isArgumentVisited[&_Options[i]] == false)
		{
			std::wcout << L" --" << _Options[i].Key << L"(" << _Options[i].AliasKey << L")=" << std::wstring(_Options[i].DefaultValue);
			_Options[i].FuncPtr(_Options[i].DefaultValue);
		}
	}
	std::wcout << std::endl;
}

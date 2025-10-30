#pragma once

#define COMMAND_EXECUTION_VERSION_MAJOR 4
#define COMMAND_EXECUTION_VERSION_MINOR 0
#define COMMAND_EXECUTION_VERSION_PATCH 2

#if defined(EXPORT_COMMAND_EXECUTION_DLL)
#define COMMAND_EXECUTION_API __declspec(dllexport)
#elif defined(EXPORT_COMMAND_EXECUTION_DLL)
#define COMMAND_EXECUTION_API __declspec(dllimport)
#else
#define COMMAND_EXECUTION_API
#endif

typedef struct
{
	const char* DefaultValue;
	void(*FuncPtr)(const char* _Value);
} CEKeylessOptionA;

typedef struct
{
	const wchar_t* DefaultValue;
	void(*FuncPtr)(const wchar_t* _Value);
} CEKeylessOptionW;

typedef struct
{
	const char* Key;
	const char* AliasKey;
	const char* DefaultValue;
	void(*FuncPtr)(const char* _Value);
} CECommandOptionA;

typedef struct
{
	const wchar_t* Key;
	const wchar_t* AliasKey;
	const wchar_t* DefaultValue;
	void(*FuncPtr)(const wchar_t* _Value);
} CECommandOptionW;

enum CEDefaultCommandOption
{
	CE_DEFAULT_COMMAND_OPTION_CODE_PAGE,
	CE_DEFAULT_COMMAND_OPTION_LOCALE,
};

// _DefaultTarget WILL BE CALLED FIRST PRIOR TO _Options
// THE ORDER MATTERS IN _Options (THIS FUNCTION WILL CALL CommandOption::FuncPtr IN ORDER OF DECLARATION in _Options)
extern "C" COMMAND_EXECUTION_API void CE_HandleCommandArgsA(const int _Argc, const char* const _Argv[], CEKeylessOptionA _DefaultTarget, CECommandOptionA* _Options, const size_t _OptionCount);
extern "C" COMMAND_EXECUTION_API void CE_HandleCommandArgsW(const int _Argc, const wchar_t* const _Argv[], CEKeylessOptionW _DefaultTarget, CECommandOptionW* _Options, const size_t _OptionCount);
#if defined(_WIN32)
extern "C" COMMAND_EXECUTION_API void CE_HandleWinAPICmdLine(const wchar_t* _CommandLine, CEKeylessOptionW _DefaultTarget, CECommandOptionW* _Options, const size_t _OptionCount);
#endif


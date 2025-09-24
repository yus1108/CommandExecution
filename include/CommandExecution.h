#pragma once

#define COMMAND_EXECUTION_VERSION_MAJOR 3
#define COMMAND_EXECUTION_VERSION_MINOR 1
#define COMMAND_EXECUTION_VERSION_PATCH 3

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
} CEKeylessOption;

typedef struct
{
	const char* Key;
	const char* AliasKey;
	const char* DefaultValue;
	void(*FuncPtr)(const char* _Value);
} CECommandOption;

enum CEDefaultCommandOption
{
	CE_DEFAULT_COMMAND_OPTION_CODE_PAGE,
	CE_DEFAULT_COMMAND_OPTION_LOCALE,
};

// _DefaultTarget WILL BE CALLED FIRST PRIOR TO _Options
// THE ORDER MATTERS IN _Options (THIS FUNCTION WILL CALL CommandOption::FuncPtr IN ORDER OF DECLARATION in _Options)
extern "C" COMMAND_EXECUTION_API void CE_HandleCommandArgs(const int _Argc, const char* const _Argv[], CEKeylessOption _DefaultTarget, CECommandOption* _Options, const size_t _OptionCount);

extern "C" COMMAND_EXECUTION_API CECommandOption CEUtility_GetDefaultCommandOption(CEDefaultCommandOption _DefaultOption);

extern "C" COMMAND_EXECUTION_API void CEUtility_SetConsoleOutputCP(const char* _Str);
extern "C" COMMAND_EXECUTION_API void CEUtility_SetLocale(const char* _Str);



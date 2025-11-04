#pragma once

#define COMMAND_EXECUTION_VERSION_MAJOR 4
#define COMMAND_EXECUTION_VERSION_MINOR 1
#define COMMAND_EXECUTION_VERSION_PATCH 0

#if defined(__cplusplus)
#if defined(EXPORT_COMMAND_EXECUTION_DLL)
#define COMMAND_EXECUTION_API extern "C" __declspec(dllexport)
#elif defined(IMPORT_COMMAND_EXECUTION_DLL)
#define COMMAND_EXECUTION_API extern "C" __declspec(dllimport)
#else
#define COMMAND_EXECUTION_API extern "C" 
#endif
#else
#if defined(EXPORT_COMMAND_EXECUTION_DLL)
#define COMMAND_EXECUTION_API extern __declspec(dllexport)
#elif defined(IMPORT_COMMAND_EXECUTION_DLL)
#define COMMAND_EXECUTION_API extern __declspec(dllimport)
#else
#define COMMAND_EXECUTION_API extern 
#endif
#endif // __cplusplus

typedef short CEWChar;

typedef struct CEKeylessOptionA
{
	const char* DefaultValue;
	void(*FuncPtr)(const char* _Value);
} CEKeylessOptionA;

typedef struct CEKeylessOptionW
{
	const CEWChar* DefaultValue;
	void(*FuncPtr)(const CEWChar* _Value);
} CEKeylessOptionW;

typedef struct CECommandOptionA
{
	const char* Key;
	const char* AliasKey;
	const char* DefaultValue;
	void(*FuncPtr)(const char* _Value);
} CECommandOptionA;

typedef struct CECommandOptionW
{
	const CEWChar* Key;
	const CEWChar* AliasKey;
	const CEWChar* DefaultValue;
	void(*FuncPtr)(const CEWChar* _Value);
} CECommandOptionW;

COMMAND_EXECUTION_API void CE_EnablePrintCommandLine(void);
COMMAND_EXECUTION_API void CE_DisablePrintCommandLine(void);

// _DefaultTarget WILL BE CALLED FIRST PRIOR TO _Options
// THE ORDER MATTERS IN _Options (THIS FUNCTION WILL CALL CommandOption::FuncPtr IN ORDER OF DECLARATION in _Options)
COMMAND_EXECUTION_API void CE_HandleCommandArgsA(const int _Argc, const char* const _Argv[], CEKeylessOptionA _DefaultTarget, CECommandOptionA* _Options, const size_t _OptionCount);
COMMAND_EXECUTION_API void CE_HandleCommandArgsW(const int _Argc, const CEWChar* const _Argv[], CEKeylessOptionW _DefaultTarget, CECommandOptionW* _Options, const size_t _OptionCount);
#if defined(_WIN32)
COMMAND_EXECUTION_API void CE_HandleWinAPICmdLine(const CEWChar* _CommandLine, CEKeylessOptionW _DefaultTarget, CECommandOptionW* _Options, const size_t _OptionCount);
#endif



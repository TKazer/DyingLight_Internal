#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>

namespace Debug
{
    inline bool IsConsole = false;
    inline std::ofstream m_Console;
    inline HANDLE hHandler = 0;

    void Log(LPCSTR szFormat, ...)
    {
        if (!IsConsole)
        {
            if (!AttachConsole(GetCurrentProcessId())) IsConsole = AllocConsole();
            SetConsoleTitleW(L"Log");
            SetConsoleCP(CP_UTF8);
            SetConsoleOutputCP(CP_UTF8);
            m_Console.open("CONOUT$");
        }
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        char szBuffer[1024] = { 0 };
        va_list pArgList;
        va_start(pArgList, szFormat);
        _vsnprintf_s(szBuffer, sizeof(szBuffer) / sizeof(char), szFormat, pArgList);
        va_end(pArgList);
        char szOutBuffer[1024] = { 0 };
        m_Console << szBuffer << std::endl;
    }

    LONG NTAPI Exception(struct _EXCEPTION_POINTERS* ExceptionInfo)
    {
        static DWORD64 Dis = (DWORD64)ExceptionInfo->ExceptionRecord->ExceptionAddress - (DWORD64)IsBadReadPtr;
        static bool ColorSetFlag = false;

        if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && Dis > 0x2000)
        {
            if (!ColorSetFlag)
            {
                ColorSetFlag = !ColorSetFlag;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            }

            printf("[Exception] Address:%p\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
            getchar();
            return EXCEPTION_CONTINUE_EXECUTION;
        }

        return EXCEPTION_CONTINUE_SEARCH;
    }
}

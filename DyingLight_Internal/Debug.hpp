#pragma once
#include <windows.h>
#include <iostream>

LONG NTAPI Debug(struct _EXCEPTION_POINTERS* ExceptionInfo)
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
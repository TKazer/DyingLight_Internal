#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace VEH
{
	struct DrData
	{
		DWORD64 Dr0;
		DWORD64 Dr1;
		DWORD64 Dr2;
		DWORD64 Dr3;
	};

	// Ó²¼þ¶Ïµã
	class DrHook
	{
	public:
		DrHook() : hHandler(nullptr), Dr7(0) {}

		void Hook(DrData HookData, PVECTORED_EXCEPTION_HANDLER Handler)
		{
			this->Data = HookData;
			this->Dr7 = 0x55;
			this->hHandler = AddVectoredExceptionHandler(1, Handler);
			this->SetThreadDr();
		}

		void UnHook()
		{
			this->Data = DrData{ 0 };
			this->Dr7 = 0;
			this->SetThreadDr();
			RemoveVectoredExceptionHandler(this->hHandler);
		}

		void SetThreadDr()
		{
			THREADENTRY32 ThreadInfoPE;
			ThreadInfoPE.dwSize = sizeof(THREADENTRY32);
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			Thread32First(hSnapshot, &ThreadInfoPE);
			do {
				if (GetCurrentProcessId() != ThreadInfoPE.th32OwnerProcessID)
					continue;
				if (GetCurrentThreadId() == ThreadInfoPE.th32ThreadID)
					continue;

				HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, ThreadInfoPE.th32ThreadID);
				if (hThread)
				{
					SuspendThread(hThread);
					CONTEXT context;
					context.ContextFlags = CONTEXT_ALL;
					if (GetThreadContext(hThread, &context))
					{
						context.Dr0 = this->Data.Dr0;
						context.Dr1 = this->Data.Dr1;
						context.Dr2 = this->Data.Dr2;
						context.Dr3 = this->Data.Dr3;
						context.Dr7 = this->Dr7;
						SetThreadContext(hThread, &context);
					}
					ResumeThread(hThread);
					CloseHandle(hThread);
				}

			} while (Thread32Next(hSnapshot, &ThreadInfoPE));
			CloseHandle(hSnapshot);
			return;

		}

	private:
		DrData Data{};
		PVOID  hHandler;
		DWORD64 Dr7;
	};


	// int¶Ïµã
	class IntHook
	{
	public:

		IntHook() : hHandler(nullptr){}

		void Hook(std::vector<DWORD64> AddressList, PVECTORED_EXCEPTION_HANDLER Handler)
		{
			this->HookAddressList = AddressList;
			if (AddressList.size() == 0)
				return;
			DWORD OldProtect;
			for (int i = 0; i < HookAddressList.size(); i++)
			{
				DWORD64 Address = HookAddressList.at(i);
				DWORD OldProtect;
				VirtualProtect((LPVOID)Address, 2, PAGE_EXECUTE_READWRITE, &OldProtect);
				this->OldByteList.push_back(*(byte*)(Address));
				*(byte*)(Address) = 0xCC;
				VirtualProtect((LPVOID)Address, 2, OldProtect, &OldProtect);
			}
			this->hHandler = AddVectoredExceptionHandler(1, Handler);
		}

		void UnHook()
		{
			DWORD OldProtect;
			for (int i = 0; i < HookAddressList.size(); i++)
			{
				DWORD64 Address = HookAddressList.at(i);
				DWORD OldProtect;
				VirtualProtect((LPVOID)Address, 2, PAGE_EXECUTE_READWRITE, &OldProtect);
				*(byte*)(Address) = this->OldByteList.at(i);
				VirtualProtect((LPVOID)Address, 2, OldProtect, &OldProtect);
			}
			RemoveVectoredExceptionHandler(this->hHandler);
		}
		
	private:
		std::vector<DWORD64> HookAddressList;
		std::vector<byte> OldByteList;
		PVOID hHandler;
	};
}

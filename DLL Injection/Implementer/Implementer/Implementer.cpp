// Implementer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Windows.h"
#include "dllmain.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	DWORD pid;
	HANDLE targetProcess;
	const char* dllPath = "../../InjectorDll/Debug/InjectorDll.dll";
	const char* injectDecoratedName = "?inject@@YGXKV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@0@Z";
	std::cout << "Введите номер процесса для внедрения: ";
	std::cin >> pid;

	targetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (targetProcess) {
		LPVOID LoadLibraryAStartAddr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		LPVOID DllPathRemoteParam = (LPVOID)VirtualAllocEx(targetProcess, NULL, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(targetProcess, DllPathRemoteParam, dllPath, strlen(dllPath) + 1, NULL);
		HANDLE remoteThread = CreateRemoteThread(targetProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAStartAddr, DllPathRemoteParam, 0, NULL);
		WaitForSingleObject(remoteThread, INFINITE);
		std::cout << "DLL внедрена" << std::endl;
		CloseHandle(remoteThread);
		CloseHandle(targetProcess);
		Sleep(1000);
		inject(pid, "You were hacked", "static hack");
		Sleep(2000);
		HMODULE HModule = LoadLibrary(L"../../InjectorDll/Debug/InjectorDll.dll");
		typedef void __stdcall Tinject(DWORD pid, std::string target, std::string replacement);
		if (HModule != 0) {
			Tinject* injectFromDll = (Tinject*)GetProcAddress(HModule, injectDecoratedName);
			if (injectFromDll == 0) {
				int x = GetLastError();
				std::cout << x;
			}
			else {
				injectFromDll(pid, "static hack", "dynamic hack");
			}
		}
		FreeLibrary(HModule);
	}
	else {
		std::cout << "Ошибка при получении дискриптора процесса" << std::endl;
	}
	return 0;
}
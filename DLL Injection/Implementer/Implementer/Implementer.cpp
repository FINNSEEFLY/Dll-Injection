// Implementer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <utility>
#include "windows.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	DWORD pid;
	HANDLE targetProcess;
	const char* dllPath = "../../InjectorDll/Debug/InjectorDll.dll";
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
	}
	else {
		std::cout << "Ошибка при получении дискриптора процесса" << std::endl;
	}
	return 0;
}
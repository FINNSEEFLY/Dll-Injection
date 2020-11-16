// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <vector>
#include <iostream>
#include <algorithm>
#define COMPILE_DLL_XHTYE102474
#include "dllmain.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	setlocale(LC_ALL, "Russian");
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::cout << "DLL PROCESS ATTACH" << std::endl;
		inject(GetCurrentProcessId(), "Hello World!", "You were hacked");
	case DLL_THREAD_ATTACH:
		std::cout << "DLL THREAD ATTACH" << std::endl;
	case DLL_THREAD_DETACH:
		std::cout << "DLL THREAD DETACH" << std::endl;
	case DLL_PROCESS_DETACH:
		std::cout << "DLL PROCESS DETACH" << std::endl;
		break;
	}
	return TRUE;
}
_declspec(dllexport) void __stdcall inject(DWORD pid, std::string target, std::string replacement) {
	std::cout << "Меняем '" << target << "' На '" << replacement << "'" << std::endl;

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (processHandle != nullptr) {
		MEMORY_BASIC_INFORMATION memoryBasicInformation;

		unsigned char* addr = nullptr;
		while (VirtualQueryEx(processHandle, addr, &memoryBasicInformation, sizeof(memoryBasicInformation))) {
			if (memoryBasicInformation.State == MEM_COMMIT && (memoryBasicInformation.Type == MEM_MAPPED || memoryBasicInformation.Type == MEM_PRIVATE)) {
				SIZE_T bytesRead;
				std::vector<char> buffer;
				buffer.resize(memoryBasicInformation.RegionSize);
				ReadProcessMemory(processHandle, addr, buffer.data(), memoryBasicInformation.RegionSize, &bytesRead);
				buffer.resize(bytesRead);
				auto offset = std::search(buffer.begin(), buffer.end(), target.begin(), target.end());
				if (offset != buffer.end()) {
					int distance = std::distance(buffer.begin(), offset);
					LPVOID addrToWriteTo = addr + distance;
					std::cout << "Обнаружена строка по адресу 0x" << addrToWriteTo << std::endl;

					WriteProcessMemory(processHandle, addrToWriteTo, replacement.data(), replacement.length() + 1,
						nullptr);
					std::cout << "Строка '" << target << "' изменена на '" << replacement << "'" << std::endl;

				}
			}
			addr += memoryBasicInformation.RegionSize;
		}

		CloseHandle(processHandle);
	}
	else {
		std::cout << "Не верный номер процесса " << pid << std::endl;
	}
}

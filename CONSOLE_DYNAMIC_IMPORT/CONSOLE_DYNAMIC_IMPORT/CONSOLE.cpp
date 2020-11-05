// CONSOLE.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>

int main()
{
    typedef int __stdcall TMin(int, int);

    TMin* pMin;
    HMODULE HModule = LoadLibrary(L"DllNew.dll");
    if (HModule != 0) {
        pMin = (TMin*)GetProcAddress(HModule, "?Min@@YGHHH@Z");
        if (pMin == 0) {
            int x = GetLastError();
            std::cout << x;
        }
        else {
            std::cout << "Hello World!\n";
            std::cout << "4 and 8, the smaller number is: " << (pMin)(4, 8);
        }
    }
    FreeModule(HModule);
}
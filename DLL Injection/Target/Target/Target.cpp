// Target.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "windows.h"

int main()
{
	char* s = (char*)calloc(256, sizeof(char));
	s = strcpy(s, "Hello World!");
	while (true) {
		std::cout << s << std::endl;
		Sleep(500);
	}
}

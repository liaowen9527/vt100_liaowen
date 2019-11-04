// vt100_liaowen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "vt100/terminal.h"

int main()
{
	int i = 0;
	int j = 1;
	std::swap(i, j);

	std::string str = "啊\r\n啊\b\b  ";
	terminal term;
	term.get_parser()->parse(str.c_str(), str.length());

	std::wstring wstr = term.get_screen()->get_lines()->get_string();
    std::cout << "Hello World!\n"; 
}


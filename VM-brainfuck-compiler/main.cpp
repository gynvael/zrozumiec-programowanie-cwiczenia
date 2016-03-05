#define _CRT_SECURE_NO_WARNINGS

#include "brainfuck_compiler.h"

#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Wrong arguments!\nSyntax: brainfuck.exe <source> <target>" << std::endl;
		std::cin.get();
		return 1;
	}

	if (FILE *file = fopen(argv[1], "r"))
	{
		fclose(file);
	}
	else
	{
		std::cout << "File doesn't exist!" << std::endl;
		std::cin.get();
		return 2;
	}

	BrainfuckCompiler bfcomp(argv[1]);
	if (!bfcomp.compile(argv[2]))
	{
		for (std::string error : bfcomp.get_errors())
			std::cout << error << std::endl;
			std::cin.get();
	}
	else
		std::cout << "DONE";


}

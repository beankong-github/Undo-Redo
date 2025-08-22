#include <iostream>
#include <sstream>
#include <Windows.h>   

#include "Command.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CommandSystem CS;

	while (true)
	{
		std::string commandLine;
		std::cout << "\nPress the command [A , B , C , E , D , \"\\Undo\", \"\\Redo\",\"\\Show\", \"\\Exit\"] without space.\n";
		std::getline(std::cin, commandLine);

		for (int i = 0; i < commandLine.size(); ++i)
		{
			char c = commandLine[i];

			// 키 이벤트
			ExecuteFunc execute;
			ExecuteFunc undo;
			if (c == 'A' || c == 'a')
			{
				execute = []() {std::cout << "\nExecute A";  };
				undo = []() {std::cout << "\nUndo A"; };
				CS.InputCommand("A", execute, undo);
			}
			else if (c == 'B' || c == 'b')
			{
				execute = []() {std::cout << "\nExecute B";  };
				undo = []() {std::cout << "\nUndo B"; };
				CS.InputCommand("B", execute, undo);
			}
			else if (c == 'C' || c == 'c')
			{
				execute = []() {std::cout << "\nExecute C";  };
				undo = []() {std::cout << "\nUndo C"; };
				CS.InputCommand("C", execute, undo);
			}
			else if (c == 'D' || c == 'd')
			{
				execute = []() {std::cout << "\nExecute D";  };
				undo = []() {std::cout << "\nUndo D"; };
				CS.InputCommand("D", execute, undo);
			}
			else if (c == 'E' || c == 'e')
			{
				execute = []() {std::cout << "\nExecute E";  };
				undo = []() {std::cout << "\nUndo E"; };
				CS.InputCommand("E", execute, undo);
			}
			else
				break;


			// 이번 프레임 입력 종료
			CS.Flush();
		}

		// 명령어 입력처리
		std::istringstream ss(commandLine);
		std::string commad = commandLine;
		bool isExit = false;
		while (std::getline(ss, commad, '\\'))
		{
			// 여기서 commad 앞뒤 공백 제거
			commad.erase(std::remove_if(commad.begin(), commad.end(),
				[](unsigned char ch) { return std::isspace(ch); }),
				commad.end());

			// undo
			if (_stricmp(commad.c_str(),"Undo") == 0)
			{
				CS.Undo();
			}
			// redo
			else if (_stricmp(commad.c_str(), "Redo") == 0)
			{
				CS.Redo();
			}
			//show
			else if (_stricmp(commad.c_str(), "Show") == 0)
			{
				CS.Show();
			}
			else if (_stricmp(commad.c_str(), "Exit") == 0)
			{
				isExit = true;
				break;
			}
		}

		if (isExit)
			break;
	}
	return 0;
}
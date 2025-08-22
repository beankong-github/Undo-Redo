#include <iostream>
#include <sstream>
#include <Windows.h>   

//�Է� ť -> Ű���� �̺�Ʈ ����
// Undo �� ���� ����
// Redo �� ���� ����
// command pattern

// a, b, c, d, e Ű �Է�
// �Է� ť�� �ֱ�
// �Է� ť�� ���� ������� undo ���ÿ� ���
//  "undo" �Է�-> undo ���
// "redo" �Է� ->redo ���
// "show" �Է� -> undo ���ð� redo ���� ���� ���

#include "Command.h"

int main()
{

	CommandSystem CS;

	while (true)
	{
		std::string commandLine;
		std::cout << "\nPress the command [A , B , C , E , D , \"\\Undo\", \"\\Redo\",\"\\Show\", \"\\Exit\"] without space.\n";
		std::getline(std::cin, commandLine);

		for (int i = 0; i < commandLine.size(); ++i)
		{
			char c = commandLine[i];

			// Ű �̺�Ʈ
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
			else if (c == '\\')
				break;


			// �̹� ������ �Է� ����
			CS.Flush();
		}

		// ��ɾ� �Է�ó��
		std::istringstream ss(commandLine);
		std::string commad = commandLine;
		bool isExit = false;
		while (std::getline(ss, commad, '\\'))
		{
			// ���⼭ commad �յ� ���� ����
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
#include "Command.h"
#include <memory>
#include <cassert>
#include <iostream>

CommandSystem::~CommandSystem()
{
	for (auto c : commandQueue)
	{
		if (c != nullptr)
		{
			delete c;
			c = nullptr;
		}
	}
}

void CommandSystem::InputCommand(std::string commandName, ExecuteFunc execute, ExecuteFunc undo)
{
	if (curIndex >= MaxCommandsPerFrame)
	{
		std::cout << "동시 입력 수 초과";
		return;
	}

	Command* cmd = new Command(commandName, execute, undo);
	commandQueue[curIndex] = cmd;

	curIndex++;
}

void CommandSystem::Flush()
{
	for (int i = 0; i < curIndex; ++i)
	{
		// 실행
		commandQueue[i]->execute();

		undoStack.Push(commandQueue[i]);
		commandQueue[i] = nullptr;
	}

	curIndex = 0;
}

void CommandSystem::Undo()
{
	// Undo
	Command* cmd = undoStack.Top();
	if(cmd != nullptr)
	{
		undoStack.Pop();
		cmd->undo();

		// Redo에 넣기
		redoStack.Push(cmd);
	}
}

void CommandSystem::Redo()
{
	//Redo
	Command* cmd = redoStack.Top();
	if (cmd != nullptr)
	{
		redoStack.Pop();
		cmd->execute();

		// Undo에 넣기 
		undoStack.Push(cmd);
	}
}

void CommandSystem::Show()
{
	std::cout << "\n=========== Undo List ==============\n";
	undoStack.Show();


	std::cout << "\n=========== Redo List ==============\n";
	redoStack.Show();
}

CommandStack::~CommandStack()
{
	// 남은 명령들을 삭제한다
	for (auto c : commands)
	{
		if (c == nullptr)
			continue;

		delete c;
		c = nullptr;
	}
}

bool CommandStack::Push(Command* command)
{
	if (command == nullptr)
		return false;

	if (IsEmpty())
	{
		commands[recent] = command;
		return true;
	}

	
	// 저장 공간이 더 이상 없으면?
	if((recent + 1) % MaxCommandStackSize == old)
	{
		// 가장 오래된 명령을 삭제한다.
		delete commands[old];
		commands[old] = nullptr;

		// bottom은 다음으로 오래된 명령을 가리킨다.
		old = (old + 1) % MaxCommandStackSize;
	// 명령 저장
	}

	// recent은 새로 저장할 위치 가리킨다.
	recent = (recent + 1) % MaxCommandStackSize;
	commands[recent] = command;

	return true;
}

bool CommandStack::Pop()
{
	if (IsEmpty())
		return false;

	if (commands[recent] == nullptr)
		return false;

	// 가장 최신 명령을 삭제한다.
	commands[recent] = nullptr;
	
	recent = (recent - 1 + MaxCommandStackSize) % MaxCommandStackSize;
	return true;
}


Command* CommandStack::Top( ) const
{
	if (IsEmpty())
		return nullptr;
	return commands[recent];
}


void CommandStack::Show() const
{
	if (IsEmpty())
		return;

	int start = old;
	int end = recent;
	while (true)
	{
		std::cout << commands[start]->name << '\n';
		if (start == end)
			break;
		start = (start + 1) % MaxCommandStackSize;
	}
}

bool CommandStack::IsEmpty() const
{
	return recent == old && commands[recent] == nullptr;
}

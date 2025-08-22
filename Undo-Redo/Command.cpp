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
		std::cout << "���� �Է� �� �ʰ�";
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
		// ����
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

		// Redo�� �ֱ�
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

		// Undo�� �ֱ� 
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
	// ���� ��ɵ��� �����Ѵ�
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

	
	// ���� ������ �� �̻� ������?
	if((recent + 1) % MaxCommandStackSize == old)
	{
		// ���� ������ ����� �����Ѵ�.
		delete commands[old];
		commands[old] = nullptr;

		// bottom�� �������� ������ ����� ����Ų��.
		old = (old + 1) % MaxCommandStackSize;
	// ��� ����
	}

	// recent�� ���� ������ ��ġ ����Ų��.
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

	// ���� �ֽ� ����� �����Ѵ�.
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

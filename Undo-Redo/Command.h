#pragma once
#include <string>
// 한 프레임 당 최대 동시 입력 가능한 명령
#define MaxCommandsPerFrame  10
#define MaxCommandStackSize 15

using ExecuteFunc = void(*)();
struct Command
{
    Command(std::string name, ExecuteFunc execute = nullptr, ExecuteFunc undo = nullptr)
        :name(name), execute(execute), undo(undo)
    {
    }

    std::string name;
    ExecuteFunc execute;
    ExecuteFunc undo;
};


class CommandStack
{
public:
    CommandStack() = default;
    ~CommandStack();
    
    bool Push(Command* command);
    bool Pop();
   Command* Top() const;
    void Show() const;
    bool IsEmpty() const;

private:
    int recent = 0;
    int old = 0;
    Command* commands[MaxCommandStackSize] = {nullptr,};
};

class CommandSystem
{
public:
    CommandSystem() = default;
    ~CommandSystem();
    void InputCommand(std::string commandName, ExecuteFunc execute , ExecuteFunc undo);
    void Flush();
    void Undo();
    void Redo();
    void Show();
private:
    int curIndex = 0;
    Command* commandQueue[MaxCommandsPerFrame] = { nullptr, };
    CommandStack undoStack;
    CommandStack redoStack;

};

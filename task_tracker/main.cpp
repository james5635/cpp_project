#include <iostream>
#include "task_manager.h"
#include <string>
void usage()
{
    std::cout << "Task CLI - Simple Task Manager\n";
    std::cout << "---------------------------------\n";
    std::cout << "Usage:\n";
    std::cout << "  task-cli <command>\n\n";
    std::cout << "Commands:\n";
    std::cout << "  list                       List all tasks\n";
    std::cout << "  list todo                  List all todo tasks\n";
    std::cout << "  list in-progress           List all in-progress tasks\n";
    std::cout << "  list done                  List all done tasks\n";
    std::cout << "  add <description>          Add a new task\n";
    std::cout << "  delete <id>                Delete a task by ID\n";
    std::cout << "  update <id> <desc>         Update a task's description\n";
    std::cout << "  mark-in-progress <id>      Mark a task as in-progress\n";
    std::cout << "  mark-done <id>             Mark a task as done\n\n";
    std::cout << "Examples:\n";
    std::cout << "  task-cli add \"Buy milk\"\n";
    std::cout << "  task-cli list\n";
    std::cout << "  task-cli delete 3\n\n";
}
int main(int argc, char *argv[])
{
    TaskManager manager;
    if (argc == 1)
    {
        usage();
        return 0;
    }
    else if (argc == 2)
    {
        if (std::string(argv[1]) == "add")
        {
            std::cout << "need description" << std::endl;
        }
        else if (std::string(argv[1]) == "list")
        {

            manager.listTask();
        }
    }
    else if (argc == 3)
    {
        if (std::string(argv[1]) == "add")
        {
            if (std::string(argv[2]) != "\0")
            {
                manager.addTask(std::string(argv[2]));
            }
            {
            }
        }
    }
    return 0;
}
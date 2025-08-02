#include "task_manager.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
TaskManager::TaskManager()
{
    readFromFile();
}

void TaskManager::addTask(const string &description,
                          const string &status)
{
    using system_clock = std::chrono::system_clock;
    auto currentTime = system_clock::to_time_t(system_clock::now());
    std::string createdAt = std::string((std::ctime(&currentTime)));
    createdAt.pop_back();
    std::string updatedAt = createdAt;
    tasks.push_back(Task{tasks.size() + 1, description, status,
                         createdAt,
                         updatedAt});

    saveToFile();
}
void TaskManager::listTask() const
{
    for (const auto &t : tasks)
    {
        cout << t.toJson() << endl;
    }
}
void TaskManager::saveToFile()
{
    std::ofstream out("tasks.json");
    out << "[";
    for (size_t i = 0; i < tasks.size(); i++)
    {
        out << tasks[i].toJson();
        if (i < tasks.size() - 1)
            out << ",";
    }
    out << "]";
}
void TaskManager::readFromFile()
{
    std::ifstream in("tasks.json");
    if (!in)
        return;

    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    // Very naive parsing (split by '{' and '}')
    size_t pos = 0;
    while ((pos = data.find("{", pos)) != std::string::npos)
    {
        size_t end = data.find("}", pos);
        tasks.push_back(Task::fromJson(data.substr(pos, end - pos + 1)));
        pos = end + 1;
    }
}
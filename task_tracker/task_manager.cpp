#include "task_manager.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
TaskManager::TaskManager() { readFromFile(); }

void TaskManager::addTask(const string &description, const string &status) {
    using system_clock = std::chrono::system_clock;
    auto currentTime = system_clock::to_time_t(system_clock::now());
    std::string createdAt = std::string((std::ctime(&currentTime)));
    createdAt.pop_back();
    std::string updatedAt = createdAt;
    tasks.push_back(
        Task{tasks.size() + 1, description, status, createdAt, updatedAt});

    saveToFile();
}
void TaskManager::addTask(const Task &task) {
    tasks.push_back(task);
    saveToFile();
}
void TaskManager::listTask() const {
    for (const auto &t : tasks) {
        cout << t.toJson() << endl;
    }
}
void TaskManager::listTodoTask() const {
    for (const auto &t : tasks) {
        if (t.status == "todo") cout << t.toJson() << endl;
    }
}
void TaskManager::listInProgressTask() const {
    for (const auto &t : tasks) {
        if (t.status == "in-progress") cout << t.toJson() << endl;
    }
}
void TaskManager::listDoneTask() const {
    for (const auto &t : tasks) {
        if (t.status == "done") cout << t.toJson() << endl;
    }
}

void TaskManager::deleteTask(size_t id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [id](const Task &t) { return t.id == id; });
    if (it != tasks.end()) {
        tasks.erase(it);
        saveToFile();
    }
}
void TaskManager::updateTask(size_t id, const string &description) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [id](const Task &t) { return t.id == id; });

    if (it != tasks.end()) {
        it->description = description;

        using system_clock = std::chrono::system_clock;
        auto currentTime = system_clock::to_time_t(system_clock::now());
        std::string updatedAt = std::string((std::ctime(&currentTime)));
        updatedAt.pop_back();

        it->updatedAt = updatedAt;
        saveToFile();
    }
}

void TaskManager::markInProgress(size_t id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [id](const Task &t) { return t.id == id; });
    if (it != tasks.end()) {
        it->status = "in-progress";
        saveToFile();
    }
}
void TaskManager::markDone(size_t id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [id](const Task &t) { return t.id == id; });
    if (it != tasks.end()) {
        it->status = "done";
        saveToFile();
    }
}
void TaskManager::saveToFile() {
    std::ofstream out("tasks.json");
    out << "[";
    for (size_t i = 0; i < tasks.size(); i++) {
        out << tasks[i].toJson();
        if (i < tasks.size() - 1) out << ",";
    }
    out << "]";
}
void TaskManager::readFromFile() {
    std::ifstream in("tasks.json");
    if (!in) return;

    std::string data((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());
    // Very naive parsing (split by '{' and '}')
    size_t pos = 0;
    while ((pos = data.find("{", pos)) != std::string::npos) {
        size_t end = data.find("}", pos);
        tasks.push_back(Task::fromJson(data.substr(pos, end - pos + 1)));
        pos = end + 1;
    }
}
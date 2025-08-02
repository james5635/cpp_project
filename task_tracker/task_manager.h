#pragma once

#include "task.h"
#include <vector>
using namespace std;
class TaskManager
{
private:
    vector<Task> tasks;

public:
    TaskManager();
    void addTask(const string &description,
                 const string &status = "todo");
    void listTask() const;
    void listTodoTask() const;
    void listInProgressTask() const;
    void listDoneTask() const;
    void deleteTask(size_t id);
    void updateTask(size_t id, const string &description);
    void markInProgress(size_t id);
    void markDone(size_t id);
    void saveToFile();
    void readFromFile();
};
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
                 const string &status = "todo") ;
    void listTask() const;       
    void saveToFile() ;
    void readFromFile();
};
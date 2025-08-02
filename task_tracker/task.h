#ifndef TASK_H
#define TASK_H
#include <string>
using namespace std;
struct Task
{
    size_t id;
    string description;
    string status;
    string createdAt;
    string updatedAt;

    string toJson() const;
    static Task fromJson(const string &json);
};

#endif
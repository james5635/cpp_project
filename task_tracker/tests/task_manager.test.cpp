#include "../task_manager.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../task.h"

std::string readFile(const std::string& path) {
    std::ifstream in(path);
    return std::string((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
}

void cleanFile() { std::remove("tasks.json"); }

TEST_CASE("TaskManager::listDoneTask prints only done tasks",
          "[TaskManager][listDoneTask]") {
    Task doneTask;
    doneTask.id = 1;
    doneTask.description = "Finish report";
    doneTask.status = "done";
    doneTask.createdAt = "2025-08-01T10:00:00Z";
    doneTask.updatedAt = "2025-08-01T12:00:00Z";

    Task pendingTask;
    pendingTask.id = 2;
    pendingTask.description = "Buy milk";
    pendingTask.status = "todo";
    pendingTask.createdAt = "2025-08-02T08:00:00Z";
    pendingTask.updatedAt = "2025-08-02T08:00:00Z";

    TaskManager manager;
    manager.addTask(doneTask);
    manager.addTask(pendingTask);

    // Capture std::cout
    std::streambuf* oldCout = std::cout.rdbuf();
    std::ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());

    manager.listDoneTask();

    // Restore original std::cout
    std::cout.rdbuf(oldCout);

    std::string output = oss.str();

    // Check that only the "done" task appears
    REQUIRE(output.find("Finish report") != std::string::npos);
    REQUIRE(output.find("Buy milk") == std::string::npos);
}

TEST_CASE("addTask with description and status", "[addTask]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("Test Task", "todo");

    std::string content = readFile("tasks.json");
    REQUIRE(content.find("Test Task") != std::string::npos);
    REQUIRE(content.find("todo") != std::string::npos);
}

TEST_CASE("addTask with Task object", "[addTask]") {
    cleanFile();
    TaskManager tm;
    Task t{1, "Manual task", "done", "now", "now"};
    tm.addTask(t);

    std::string content = readFile("tasks.json");
    REQUIRE(content.find("Manual task") != std::string::npos);
    REQUIRE(content.find("done") != std::string::npos);
}

TEST_CASE("listTask outputs all tasks", "[listTask]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("List me", "todo");

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm.listTask();
    std::cout.rdbuf(old);

    REQUIRE(out.str().find("List me") != std::string::npos);
}

TEST_CASE("listTodoTask filters correctly", "[listTodoTask]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("To do 1", "todo");
    tm.addTask("Not to do", "done");

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm.listTodoTask();
    std::cout.rdbuf(old);

    std::string result = out.str();
    REQUIRE(result.find("To do 1") != std::string::npos);
    REQUIRE(result.find("Not to do") == std::string::npos);
}

TEST_CASE("listInProgressTask filters correctly", "[listInProgressTask]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("In progress", "in-progress");
    tm.addTask("Todo", "todo");

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm.listInProgressTask();
    std::cout.rdbuf(old);

    REQUIRE(out.str().find("In progress") != std::string::npos);
    REQUIRE(out.str().find("Todo") == std::string::npos);
}

TEST_CASE("deleteTask removes task", "[deleteTask]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("Delete me", "todo");
    tm.deleteTask(1);

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm.listTask();
    std::cout.rdbuf(old);

    REQUIRE(out.str().find("Delete me") == std::string::npos);
}

TEST_CASE("updateTask changes description", "[updateTask]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("Old description", "todo");
    tm.updateTask(1, "Updated description");

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm.listTask();
    std::cout.rdbuf(old);

    std::string output = out.str();
    REQUIRE(output.find("Updated description") != std::string::npos);
    REQUIRE(output.find("Old description") == std::string::npos);
}

TEST_CASE("markInProgress updates status", "[markInProgress]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("Do this", "todo");
    tm.markInProgress(1);

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm.listInProgressTask();
    std::cout.rdbuf(old);

    REQUIRE(out.str().find("Do this") != std::string::npos);
}

TEST_CASE("markDone updates status", "[markDone]") {
    cleanFile();
    TaskManager tm;
    tm.addTask("Finish this", "in-progress");
    tm.markDone(1);

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm.listTodoTask();  // Should not appear here
    std::cout.rdbuf(old);

    REQUIRE(out.str().find("Finish this") == std::string::npos);
}

TEST_CASE("readFromFile loads tasks", "[readFromFile]") {
    cleanFile();
    {
        TaskManager tm;
        tm.addTask("Persisted task", "todo");
    }
    // Now re-initialize and see if task is reloaded
    TaskManager tm2;

    std::ostringstream out;
    std::streambuf* old = std::cout.rdbuf(out.rdbuf());
    tm2.listTask();
    std::cout.rdbuf(old);

    REQUIRE(out.str().find("Persisted task") != std::string::npos);
}

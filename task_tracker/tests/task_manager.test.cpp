#include "../task_manager.h"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>

#include "../task.h"

TEST_CASE("TaskManager::listDoneTask prints only done tasks", "[TaskManager]") {
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

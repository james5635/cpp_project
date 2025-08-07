#include "../task.h"

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST_CASE("Task::toJson outputs correct JSON", "[Task][toJson]") {
    Task task;
    task.id = 1;
    task.description = "Buy groceries";
    task.status = "Pending";
    task.createdAt = "2025-08-07T10:00:00Z";
    task.updatedAt = "2025-08-07T10:05:00Z";

    // Parse the JSON string produced by your code
    json actual = json::parse(task.toJson());

    // Construct expected JSON object directly
    json expected = {{"id", 1},
                     {"description", "Buy groceries"},
                     {"status", "Pending"},
                     {"createAt", "2025-08-07T10:00:00Z"},
                     {"updatedAt", "2025-08-07T10:05:00Z"}};

    REQUIRE(actual == expected);
}

TEST_CASE("Task::fromJson parses JSON correctly", "[Task][fromJson]") {
    std::string jsonStr = R"({
        "id": 1,
        "description": "Buy groceries",
        "status": "Pending",
        "createAt": "2025-08-07T10:00:00Z",
        "updatedAt": "2025-08-07T10:05:00Z"
    })";

    Task task = Task::fromJson(jsonStr);

    REQUIRE(task.id == 1);
    REQUIRE(task.description == "Buy groceries");
    REQUIRE(task.status == "Pending");
    REQUIRE(task.createdAt == "2025-08-07T10:00:00Z");
    REQUIRE(task.updatedAt == "2025-08-07T10:05:00Z");
}

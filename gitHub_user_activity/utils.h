#pragma once
#define UTILS_H


#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
// Separation to fetch JSON events
json fetch_github_events(const std::string& username);

// Separate function to parse events into messages
std::vector<std::string> parse_events(const json& events);
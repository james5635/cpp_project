#include "utils.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <fmt/format.h>
#include <httplib.h>

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
using json = nlohmann::json;

// Separate function to fetch JSON events
json fetch_github_events(const std::string& username) {
  httplib::Client cli("https://api.github.com");
  auto path = fmt::format("/users/{}/events", username);
  if (auto res = cli.Get(path)) {
    return json::parse(res->body);
  } else {
    throw std::runtime_error("Failed to fetch events from GitHub");
  }
}

// Separate function to parse events into messages
std::vector<std::string> parse_events(const json& events) {
  std::vector<std::string> result;
  for (const auto& event : events) {
    std::string type = event["type"];
    std::string repo = event["repo"]["name"];

    if (type == "PushEvent") {
      auto commits =
          event["payload"]["commits"].get<std::vector<json>>().size();
      result.push_back(fmt::format("- Pushed {} commit{} to {}", commits,
                                   commits == 1 ? "" : "s", repo));
    } else if (type == "IssuesEvent") {
      std::string action = event["payload"]["action"];
      std::string issue_title = event["payload"]["issue"]["title"];
      action[0] = std::toupper(action[0]);
      result.push_back(
          fmt::format("- {} issue '{}' in {}", action, issue_title, repo));
    } else if (type == "WatchEvent") {
      result.push_back(fmt::format("- Starred {}", repo));
    } else if (type == "ForkEvent") {
      result.push_back(fmt::format(
          "- Forked {} to {}", repo,
          event["payload"]["forkee"]["full_name"].get<std::string>()));
    } else if (type == "PullRequestEvent") {
      std::string action = event["payload"]["action"];
      std::string pr_title = event["payload"]["pull_request"]["title"];
      action[0] = std::toupper(action[0]);
      result.push_back(
          fmt::format("- {} pull request '{}' in {}", action, pr_title, repo));
    }
  }
  return result;
}

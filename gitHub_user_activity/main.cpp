#include <fmt/format.h>

#include <iostream>
#include <nlohmann/json.hpp>

#include "utils.h"

using json = nlohmann::json;
void usage() {
  std::cout << "Fetch the recent activity of a GitHub user\n";
  std::cout << "---------------------------------\n";
  std::cout << "Usage:\n";
  std::cout << "  github-activity <username>\n";
  std::cout << "  github-activity -h | --help\n";
  std::cout << "Examples:\n";
  std::cout << "  github-activity james5635\n";
  std::cout << "  github-activity kamranahmedse\n";
}
int main(int argc, char* argv[]) {
  if (argc != 2 || std::string(argv[1]) == "-h" ||
      std::string(argv[1]) == "--help") {
    usage();
    return 1;
  }

  try {
    json events = fetch_github_events(argv[1]);
    auto messages = parse_events(events);
    for (const auto& msg : messages) {
      fmt::println("{}", msg);
    }
  } catch (const std::exception& e) {
    fmt::print("Error: {}\n", e.what());
    return 1;
  }

  return 0;
}

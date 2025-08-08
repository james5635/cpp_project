#include <fmt/base.h>

#include <cctype>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <fmt/format.h>
#include <httplib.h>

#include <nlohmann/json.hpp>

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
  httplib::Client cli("https://api.github.com");
  auto path = fmt::format("/users/{}/events", std::string(argv[1]));
  if (auto res = cli.Get(path)) {
    json events = json::parse(res->body);
    for (const auto& event : events) {
      std::string type = event["type"];
      std::string repo = event["repo"]["name"];
      if (type == "PushEvent") {
        auto commits =
            event["payload"]["commits"].get<std::vector<json>>().size();
        fmt::println("- Pushed {} commit{} to {}", commits,
                     commits == 1 ? "" : "s", repo);
      } else if (type == "IssuesEvent") {
        std::string action = event["payload"]["action"];
        std::string issue_title = event["payload"]["issue"]["title"];
        action[0] = std::toupper(action[0]);
        fmt::println("- {} issue '{}' in {}", action, issue_title, repo);
      } else if (type == "WatchEvent") {
        fmt::println("- Starred {}", repo);
      } else if (type == "ForkEvent") {
        fmt::println(
            "- Forked {} to {}", repo,
            event["payload"]["forkee"]["full_name"].get<std::string>());
      } else if (type == "PullRequestEvent") {
        std::string action = event["payload"]["action"];
        std::string pr_title = event["payload"]["pull_request"]["title"];
        action[0] = std::toupper(action[0]);
        fmt::println("- {} pull request '{}' in {}", action, pr_title, repo);
      }
    }
  } else {
    printf("Request failed\n");
    return 1;
  }
  return 0;
}

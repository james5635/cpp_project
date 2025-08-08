#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "../utils.h"

using json = nlohmann::json;

TEST(ParseEventsTest, PushEvent) {
  json event = {
      {{"type", "PushEvent"},
       {"repo", {{"name", "user/repo"}}},
       {"payload",
        {
            {"commits", {json::object(), json::object()}}  // 2 commits
        }}}};

  auto result = parse_events({event});
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "- Pushed 2 commits to user/repo");
}

TEST(ParseEventsTest, IssuesEvent) {
  json event = {
      {{"type", "IssuesEvent"},
       {"repo", {{"name", "my/repo"}}},
       {"payload",
        {{"action", "opened"}, {"issue", {{"title", "Bug report"}}}}}}};

  auto result = parse_events({event});
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "- Opened issue 'Bug report' in my/repo");
}

TEST(ParseEventsTest, WatchEvent) {
  json event = {{{"type", "WatchEvent"},
                 {"repo", {{"name", "cool/repo"}}},
                 {"payload", json::object()}}};

  auto result = parse_events({event});
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "- Starred cool/repo");
}

TEST(ParseEventsTest, ForkEvent) {
  json event = {{{"type", "ForkEvent"},
                 {"repo", {{"name", "original/repo"}}},
                 {"payload", {{"forkee", {{"full_name", "user/forked"}}}}}}};

  auto result = parse_events({event});
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "- Forked original/repo to user/forked");
}

TEST(ParseEventsTest, PullRequestEvent) {
  json event = {
      {{"type", "PullRequestEvent"},
       {"repo", {{"name", "pull/repo"}}},
       {"payload",
        {{"action", "closed"}, {"pull_request", {{"title", "Fix feature"}}}}}}};

  auto result = parse_events({event});
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "- Closed pull request 'Fix feature' in pull/repo");
}

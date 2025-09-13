#include <CLI/CLI.hpp>
#include <cstdlib>
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

struct Response {
  std::string contents;
  CURLcode ret;
};
enum class Type { Playing, Popular, Top, Upcoming, Unknown };
Type parse_type(const std::string &s) {
  if (s == "playing")
    return Type::Playing;
  else if (s == "popular")
    return Type::Popular;
  else if (s == "top")
    return Type::Popular;
  else if (s == "upcoming")
    return Type::Upcoming;
  else
    return Type::Unknown;
}
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  size_t total_size = size * nmemb;

  // userdata is a std::string*
  std::string *response = static_cast<std::string *>(userdata);

  response->append(ptr, total_size); // append the chunk to the string

  return total_size;
}
struct Response *request(const char *url, const char *token) {
  std::string contents;

  CURL *hnd = curl_easy_init();

  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &contents);
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(hnd, CURLOPT_URL, url);

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "accept: application/json");
  headers = curl_slist_append(
      headers, (std::string("Authorization: Bearer ") + token).c_str());
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

  CURLcode ret = curl_easy_perform(hnd);
  return new Response{contents, ret};
}

int main(int argc, char *argv[]) {
  const char *TOKEN = std::getenv("TOKEN");
  CLI::App app{"Show the popular, top-rated, upcoming and now playing "
               "movies from the TMDB API"};
  std::string type;
  app.add_option("--type", type, "playing | popular | top | upcoming");
  CLI11_PARSE(app, argc, argv);
  switch (parse_type(type)) {
  case Type::Playing: {
    auto *resp = request(
        "https://api.themoviedb.org/3/movie/now_playing?language=en-US&page=1",
        TOKEN);
    nlohmann::json json_ = nlohmann::json::parse(resp->contents);
    for (auto &elm : json_["results"]) {
      std::cout << "- " << elm["original_title"].get<std::string>() << "\n";
    }
    break;
  }
  case Type::Popular: {
    auto *resp = request(
        "https://api.themoviedb.org/3/movie/popular?language=en-US&page=1",
        TOKEN);
    nlohmann::json json_ = nlohmann::json::parse(resp->contents);
    for (auto &elm : json_["results"]) {
      std::cout << "- " << elm["original_title"].get<std::string>() << "\n";
    }
    break;
  }
  case Type::Top: {
    auto *resp = request(
        "https://api.themoviedb.org/3/movie/top_rated?language=en-US&page=1",
        TOKEN);
    nlohmann::json json_ = nlohmann::json::parse(resp->contents);
    for (auto &elm : json_["results"]) {
      std::cout << "- " << elm["original_title"].get<std::string>() << "\n";
    }
    break;
  }
  case Type::Upcoming: {
    auto *resp = request(
        "https://api.themoviedb.org/3/movie/upcoming?language=en-US&page=1",
        TOKEN);
    nlohmann::json json_ = nlohmann::json::parse(resp->contents);
    for (auto &elm : json_["results"]) {
      std::cout << "- " << elm["original_title"].get<std::string>() << "\n";
    }
    break;
  }
  case Type::Unknown: {
    std::cerr << "error: unkown type\n";
    exit(1);
    break;
  }
  }

  return 0;
}

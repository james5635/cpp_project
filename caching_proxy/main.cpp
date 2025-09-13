/*
 * Author: Sou Chanrojame
 */
#include <CLI/CLI.hpp>
#include <asio.hpp>
#include <cstddef>
#include <curl/curl.h>
#include <iostream>
#include <istream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

// #define URL "https://httpbin.org/get"
// #define URL "https://www.google.com"

using asio::ip::tcp;

// Thread-safe in-memory cache
class Cache {
public:
  void set(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(mu_);
    data_[key] = value;
  }

  bool get(const std::string &key, std::string &value) {
    std::lock_guard<std::mutex> lock(mu_);
    auto it = data_.find(key);
    if (it != data_.end()) {
      value = it->second;
      return true;
    }
    return false;
  }
  void clear() { data_.clear(); }

private:
  std::unordered_map<std::string, std::string> data_;
  std::mutex mu_;
};

struct Memory {
  char *data;
  size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
  size_t realsize = size * nmemb;
  struct Memory *mem = (struct Memory *)userp;

  char *ptr = (char *)realloc(mem->data, mem->size + realsize + 1);
  if (ptr == NULL)
    return 0; // out of memory

  mem->data = ptr;
  memcpy(&(mem->data[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->data[mem->size] = 0;
  return realsize;
}

Cache cache;

std::string handle_headers() {}

std::string handle_body() {}

// This function handles a single client connection.
void handle_client(tcp::socket socket, std::string origin) {
  try {
    std::string request;
    std::string response;

    // A streambuf is used to hold data that has been read from the socket.
    asio::streambuf buf;

    // Read from the socket until the end of the HTTP headers.
    asio::read_until(socket, buf, "\r\n\r\n");

    // Create an input stream from the streambuf for easy parsing.
    std::istream request_stream(&buf);

    // Parse the request line (e.g., "GET /hello HTTP/1.1").
    std::string method, path, version;
    request_stream >> method >> path >> version;
    request_stream.ignore(2); // Consume the trailing "\r\n"
    // Parse the headers.
    std::unordered_map<std::string, std::string> headers;
    std::string header_line;
    while (std::getline(request_stream, header_line) && !header_line.empty() &&
           header_line != "\r") {
      // Trim the trailing '\r' if it exists.
      if (header_line.back() == '\r') {
        header_line.pop_back();
      }
      size_t colon_pos = header_line.find(": ");
      if (colon_pos != std::string::npos) {
        std::string key = header_line.substr(0, colon_pos);
        std::string value = header_line.substr(colon_pos + 2);
        headers[key] = value;
      }
    }

    // Strip the leading '/' from the path.
    // if (!path.empty() && path[0] == '/') {
    //   path.erase(0, 1);
    // }

    {
      if (path == "/clear-cache") {
        cache.clear();
        std::string response_body = "Cache cleared!";
        response = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " +
                   std::to_string(response_body.size()) + "\r\n\r\n" +
                   response_body;

        asio::write(socket, asio::buffer(response));
        return;
      }
    }
    {
      std::string request_status_line = method + " " + path + " " + version;
      request_status_line += "\r\n";
      request = request_status_line;
      for (const auto &[key, value] : headers) {
        request += key + ": " + value + "\r\n";
      }
      request += "\r\n";
    }

    // if (method == "POST") {
    size_t content_length = 0;
    if (headers.count("Content-Length")) {
      try {
        content_length = std::stoul(headers.at("Content-Length"));
      } catch (const std::exception &e) {
        std::string response_body = "Invalid Content-Length: ";
        response = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\n"
                   "Content-Length: " +
                   std::to_string(response_body.size()) + "\r\n\r\n" +
                   response_body;

        std::cerr << response_body + std::string(e.what()) << std::endl;
        asio::write(socket, asio::buffer(response));
        return;
      }
    }
    //  else {
    //   std::string response_body = "Request have no Content-Length";
    //   response = "HTTP/1.1 200 OK\r\n"
    //              "Content-Type: text/plain\r\n"
    //              "Content-Length: " +
    //              std::to_string(response_body.size()) + "\r\n\r\n" +
    //              response_body;

    //   std::cerr << "Request have no Content-Length" << std::endl;
    //   asio::write(socket, asio::buffer(response));

    //   return;
    // }

    if (content_length > 0) {
      // Pre-allocate the string for the entire body.
      std::string body(content_length, '\0');

      // Use an istream to get the data that is already in the buffer.
      std::istream is(&buf);
      is.read(&body[0], buf.size());
      size_t bytes_in_buffer = is.gcount();

      // Read the rest of the body directly from the socket into the string.
      if (bytes_in_buffer < content_length) {
        size_t remaining = content_length - bytes_in_buffer;
        asio::read(socket, asio::buffer(&body[bytes_in_buffer], remaining),
                   asio::transfer_exactly(remaining));
      }
      request += body;
    }
    std::cerr << request;

    // }

    if (!cache.get(request, response)) {

      {

        CURL *curl;
        CURLcode res;
        std::istringstream ss(request);
        std::string line;

        struct Memory response_headers = {0};
        struct Memory response_body = {0};

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
          curl_easy_setopt(curl, CURLOPT_URL, origin.c_str());
          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
          std::getline(ss, line);
          struct curl_slist *request_headers = NULL;
          while (std::getline(ss, line) && line != "\r") {
            line.pop_back();
            request_headers = curl_slist_append(request_headers, line.c_str());
          }
          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request_headers);

          std::string request_body((std::istreambuf_iterator<char>(ss)),
                                   std::istreambuf_iterator<char>());
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.c_str());
          curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,
                           (long)strlen(request_body.c_str()));
          // std::cerr << "jame --- " << request_body;

          // capture response_body
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_body);

          // capture response_headers
          curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_callback);
          curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&response_headers);

          res = curl_easy_perform(curl);

          // if (res == CURLE_OK) {
          //   printf("=== Headers ===\n%s\n", response_headers.data);
          //   printf("=== Body ===\n%s\n", response_body.data);
          // } else {
          //   fprintf(stderr, "curl_easy_perform() failed: %s\n",
          //           curl_easy_strerror(res));
          // }

          response = std::string(response_headers.data) +
                     std::string(response_body.data); // Echo the POST data.
          // for (char c : std::string(response_body.data) ){
          //   switch (c) {
          //   case '\n':
          //     std::cerr << "\\n";
          //     break;
          //   case '\t':
          //     std::cerr << "\\t";
          //     break;
          //   case '\r':
          //     std::cerr << "\\r";
          //     break;
          //   case '\\':
          //     std::cerr << "\\\\";
          //     break;
          //   default:
          //     std::cerr << c;
          //     break;
          //   }
          // }
          free(response_headers.data);
          free(response_body.data);
          curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
      }
      // std::cout << request << "\n-------\n" << response;
      std::cout << "\n----- [saving to cache] -----\n";
      cache.set(request, response); // Cache the POST data.
      std::string cache_header = "X-Cache: MISS\r\n";
      size_t pos = response.find("\r\n\r\n");
      if (pos != std::string::npos) {
        response.insert(pos + 2, cache_header);
      }

    } else {
      std::cerr << "\n----- [retrived from cache] -----\n";
      std::string cache_header = "X-Cache: HIT\r\n";
      size_t pos = response.find("\r\n\r\n");
      if (pos != std::string::npos) {
        response.insert(pos + 2, cache_header);
      }
    }

    // for (auto &[key, value] : cache.data_) {
    //   std::cerr << key << "\n --- \n" << value;
    // }
    asio::write(socket, asio::buffer(response));
  } catch (std::exception &e) {
    std::cerr << "Client error: " << e.what() << "\n";
  }
}

int main(int argc, char **argv) {

  CLI::App app{"A caching proxy server"};
  int port = 0;
  std::string origin;
  app.add_option("--port", port, "port number that server listen on");
  app.add_option("--origin", origin, "forwarding url");
  bool clear_cache = false;
  app.add_flag("--clear-cache", clear_cache, "clear cache");
  CLI11_PARSE(app, argc, argv);
  if (clear_cache) {
    std::ifstream meta("/home/jame/.cache/caching-proxy.lock");
    std::string line;
    int port = 0;
    while (std::getline(meta, line)) {
      if (line.rfind("port=", 0) == 0) {
        port = std::stoi(line.substr(5));
      }
    }
    if (port > 0) {
      CURL *curl = curl_easy_init();
      if (curl) {
        struct Memory response_body = {0};

        std::string url =
            "http://localhost:" + std::to_string(port) + "/clear-cache";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_body);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::cout << std::string(response_body.data) + "\n";
      }
    }

    return 0;
  }
  if (port != 0 && !origin.empty()) {
    {
      // After binding server and knowing port
      std::ofstream meta("/home/jame/.cache/caching-proxy.lock");
      meta << "pid=" << getpid() << "\n";
      meta << "port=" << port << "\n";
      meta.close();
    }
    try {
      asio::io_context io_context;
      tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
      std::cout << "HTTP cache server running on http://localhost:" << port
                << "\n";

      while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        // For each new connection, spawn a thread to handle it.
        std::thread(handle_client, std::move(socket), origin).detach();
      }
    } catch (std::exception &e) {
      std::cerr << "Server error: " << e.what() << "\n";
      return 1;
    }
  } else {
    std::cerr << app.help() << std::endl;
    return 1;
  }

  return 0;
}

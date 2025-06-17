#pragma once

#include <Parser.h>
#include <drogon/HttpClient.h>
#include <drogon/drogon.h>
#include <drogon/utils/coroutine.h>

#include <chrono>
#include <sstream>
#include <string>
#include <vector>

class ParseClient {
 public:
  // Disable instantiation/copy/move
  template <typename T>
  ParseClient(T) = delete;
  ParseClient(const ParseClient&) = delete;
  ParseClient(ParseClient&&) = delete;
  ParseClient& operator=(const ParseClient&) = delete;

  // for parsing link into domain nad route
  struct ParsedLink {
    std::string domain, route;
    ParsedLink(const std::string& link);
  };

  // recursively finding the meta tags and links and searching in the links
  static drogon::Task<void> crawl_and_index_async_coro(const std::string& link,
                                           unsigned int depth = 5);
};

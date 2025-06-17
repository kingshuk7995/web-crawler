#include <ParseClient.h>

using namespace drogon;

ParseClient::ParsedLink::ParsedLink(const std::string& link) {
  int slash_pos = 0;
  // find third '/'
  for (int i = 0; i < 3; ++i) {
    slash_pos = link.find('/', slash_pos + 1);
    if (slash_pos == std::string::npos) {
      slash_pos = link.size();
      break;
    }
  }
  domain = link.substr(0, slash_pos);
  route = (slash_pos < (int)link.size()) ? link.substr(slash_pos) : "/";
}

Task<> ParseClient::crawl_and_index_async_coro(const std::string& link,
                                               unsigned int depth) {
  if (depth == 0) {  // limit depth
    co_return;
  }

  // 1. Parse URL
  ParsedLink url(link);
  auto client = HttpClient::newHttpClient(url.domain);

  // 2. Fetch page
  auto req = HttpRequest::newHttpRequest();
  req->setPath(url.route);
  auto resp = co_await client->sendRequestCoro(req);
  if (!resp) {
    co_return;
  }

  // 3. Convert body to std::string
  std::string html(resp->body().data(), resp->body().size());

  // 4. Parse HTML
  auto parser = Parser::create_from(html.c_str());
  if (!parser) co_return;
  auto parsed = parser->parse();
  if (!parsed) co_return;

  // 5. Extract chunks to index
  auto chunks = parsed->get_parsed_meta_data();
  
  // 7. Execute in one go using the vector overload
  auto db = app().getDbClient();
  for(const auto& chunk : chunks) {
    co_await db->execSqlCoro("INSERT INTO inverted_index (url, chunk) VALUES ($1, $2)", link, chunk);
  }

  // 8. Recurse with 1s non-blocking delay
  using namespace std::chrono_literals;
  for (auto& lnk : parsed->links) {
    co_await drogon::sleepCoro(app().getLoop(), 1s);
    co_await crawl_and_index_async_coro(lnk, depth - 1);
  }

  co_return;
}

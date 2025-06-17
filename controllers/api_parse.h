#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
class parse : public drogon::HttpController<parse>
{
 public:
  METHOD_LIST_BEGIN

  METHOD_ADD(parse::parse_html_and_store, "/", Post);
  METHOD_ADD(parse::test_working, "/test", Get);
  METHOD_LIST_END

  void parse_html_and_store(
      const HttpRequestPtr& req,
      std::function<void(const HttpResponsePtr&)>&& callback);
  void test_working(
      const HttpRequestPtr& req,
      std::function<void(const HttpResponsePtr&)>&& callback){
        Json::Value ret;
        ret["status"] = "ok";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        callback(resp);
      }
 private:
  std::unordered_map<std::string, std::string> storage;
};
}

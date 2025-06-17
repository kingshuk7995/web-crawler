#include "api_parse.h"
#include <Parser.h>
#include <ParseClient.h>

using namespace api;

void parse::parse_html_and_store(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {

  auto reqJson = req->getJsonObject();
  auto resp = HttpResponse::newHttpResponse();

  if (!reqJson || !reqJson->isMember("page") || !(*reqJson)["page"].isString()) {
    resp->setStatusCode(k400BadRequest);
    Json::Value err;
    err["status"] = "invalid or missing 'page'";
    resp->setBody(err.toStyledString());
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    return;
  }

  const char* page = ((*reqJson)["page"]).asCString();

  auto parser_obj_nullable = Parser::create_from(page);
  if (!parser_obj_nullable.has_value()) {
    resp->setStatusCode(k400BadRequest);
    Json::Value err;
    err["status"] = "page cannot be parsed";
    resp->setBody(err.toStyledString());
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    return;
  }

  auto parsed_content = parser_obj_nullable.value().parse();
  if (!parsed_content.has_value()) {
    resp->setStatusCode(k400BadRequest);
    Json::Value err;
    err["status"] = "page cannot be parsed";
    resp->setBody(err.toStyledString());
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    return;
  }

  Json::Value result;
  result["status"] = "success";

  resp->setStatusCode(k200OK);
  resp->setBody(result.toStyledString());
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
}

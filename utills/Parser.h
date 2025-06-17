#pragma once

#include <gumbo.h>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

class Parser {
 public:
  struct ParsedData {
    std::vector<std::string> links;
    std::vector<std::string> meta_data;
    
    std::vector<std::string> get_parsed_meta_data(const size_t chunk_length_diff = 1) const;
  };

  Parser() = delete;
  Parser(const Parser& other) = delete;
  Parser(Parser&& other) noexcept;

  Parser& operator=(const Parser& other) = delete;

  ~Parser();

  static std::optional<Parser> create_from(const char* html_content);

  std::optional<ParsedData> parse() const;
  
 private:
  GumboOutput* m_output;

  explicit Parser(GumboOutput* output) : m_output(output) {}

  static void extract_links(GumboNode* node, std::vector<std::string>& links);

  static void extract_meta(GumboNode* node,
                           std::vector<std::string>& meta_data);
};

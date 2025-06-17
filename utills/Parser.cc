#include <Parser.h>
#include <set>

static const std::set<std::string> VOCABS({
  "to", "for", "from", "while", "to", "not", "etc"    // add more vocabuleries
});


Parser::Parser(Parser&& other) noexcept {
  m_output = other.m_output;
  other.m_output = nullptr;
}

Parser::~Parser() {
  if (m_output) {
    gumbo_destroy_output(&kGumboDefaultOptions, m_output);
  }
}

std::optional<Parser> Parser::create_from(const char* html_content) {
  GumboOutput* output = gumbo_parse(html_content);
  if (!output) return std::nullopt;
  return Parser(output);
}

void Parser::extract_links(GumboNode* node, std::vector<std::string>& links) {
  if (node->type != GUMBO_NODE_ELEMENT) return;

  GumboElement& element = node->v.element;
  if (element.tag == GUMBO_TAG_A) {
    GumboAttribute* href = gumbo_get_attribute(&element.attributes, "href");
    if (href && href->value) {
      links.emplace_back(href->value);
    }
  }

  const GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    extract_links(static_cast<GumboNode*>(children->data[i]), links);
  }
}
void Parser::extract_meta(GumboNode* node,
                          std::vector<std::string>& meta_data) {
  if (node->type != GUMBO_NODE_ELEMENT) return;

  GumboElement& element = node->v.element;
  if (element.tag == GUMBO_TAG_META) {
    GumboAttribute* name_attr =
        gumbo_get_attribute(&element.attributes, "name");
    GumboAttribute* content_attr =
        gumbo_get_attribute(&element.attributes, "content");

    if (name_attr && content_attr) {
      meta_data.emplace_back(std::string(name_attr->value) + ": " +
                             content_attr->value);
    }
  }

  const GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    extract_meta(static_cast<GumboNode*>(children->data[i]), meta_data);
  }
}

std::optional<Parser::ParsedData> Parser::parse() const {
  // extracts the links and meta tags
  if (!m_output) return std::nullopt;
  ParsedData result;
  extract_links(m_output->root, result.links);
  extract_meta(m_output->root, result.meta_data);
  return result;
}


std::vector<std::string>
Parser::ParsedData::get_parsed_meta_data(const size_t chunk_length_diff) const {
  // here we need to parse the string into chunks seperated by spaces first
  // then remove those for vocabuleries
  // then purse them into chunks for search
  std::vector<std::string> words;
  std::vector<std::string> chunks;
  for(const std::string& meta : this->meta_data) {
    /*
    using space_pos tracking the position when the last space hit
    for xyz abc abc it would be 0,3,6,9
    */
    size_t space_pos = 0;
    size_t next_pos = meta.find(' ', space_pos);
    while(space_pos != meta.size()) {
      words.push_back(meta.substr(space_pos, next_pos - space_pos));
    }
  }
  for(const auto& word : words) {
    if(VOCABS.find(word) != VOCABS.end()) continue;     // skip vocabuleries like for, when etc
    for(size_t chunk_split_indx = 0; chunk_split_indx < word.size(); chunk_split_indx += chunk_length_diff) {
      chunks.push_back(word.substr(0, chunk_split_indx));
    }
  }
  return chunks;
}
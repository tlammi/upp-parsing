
#include <fstream>

#include "upp/parsing/parser.hpp"

namespace p = upp::parsing;

class StringTerminal {
 public:
  p::MatchResult<char> match(std::string_view view) const noexcept {
    if (view.empty() || view.front() != '"') return {false, "", view};
    size_t idx = 1;
    while (true) {
      if (idx >= view.size()) return {false, "", view};
      if (view.at(idx) == '\\') {
        idx += 2;
      } else if (view.at(idx) == '"') {
        return {true, view.substr(0, idx + 1), view.substr(idx + 1)};
      } else {
        ++idx;
      }
    }
  }

  std::string name() const noexcept { return "string"; }

 private:
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path-to-json>\n";
    return EXIT_FAILURE;
  }
  p::Grammar g;
  auto value = g.nonterminal("value");
  auto null = g.lit("null");
  auto boolean = g.nonterminal("bool");
  boolean += g.lit("true");
  boolean += g.lit("false");
  auto integer = g.regex(R"([1-9][0-9]*|0)");
  auto fpoint = g.regex(R"(([1-9][0-9]*)\.[0-9]+|0)");
  // auto string = g.regex(R"(""|".*?[^\\]")");
  auto string = g.custom_term<StringTerminal>(StringTerminal{});
  auto key_value = g.nonterminal("key_value");
  auto obj_rest = g.nonterminal("obj_rest");
  auto obj_rest2 = g.nonterminal("obj_rest2");
  auto obj = g.nonterminal("obj");
  auto arr = g.nonterminal("arr");
  auto arr_rest = g.nonterminal("arr_rest");
  auto arr_rest2 = g.nonterminal("arr_rest2");

  value += null;
  value += boolean;
  value += fpoint;
  value += integer;
  value += string;
  value += obj;
  value += arr;

  key_value += (string, g.lit(":"), value);
  obj += (g.lit("{"), obj_rest);
  obj_rest += g.lit("}");
  obj_rest += (key_value, obj_rest2);
  obj_rest2 += g.lit("}");
  obj_rest2 += (g.lit(","), key_value, obj_rest2);

  arr += (g.lit("["), arr_rest);
  arr_rest += g.lit("]");
  arr_rest += (value, arr_rest2);
  arr_rest2 += g.lit("]");
  arr_rest2 += (g.lit(","), value, arr_rest2);

  p::Parser p{std::move(g), value};

  std::ifstream fs{argv[1]};
  std::istream_iterator<char> begin{fs};
  std::istream_iterator<char> end{};
  std::string json{begin, end};

  if (p.parse(json)) {
    std::cout << "success\n";
    return EXIT_SUCCESS;
  }
  std::cout << "error\n";
  return EXIT_FAILURE;
}

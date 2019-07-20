#include <iostream>
#include <vector>

#include "graph.h"

int main() {
  std::vector<std::string> v{"hello", "how", "are", "you"};
  gdwg::Graph<std::string, double> g{v.begin(),v.end()};
  g.InsertEdge("hello", "how", 5);
  g.InsertEdge("hello", "are", 8);
  g.InsertEdge("hello", "are", 2);

  g.InsertEdge("how", "you", 1);
  g.InsertEdge("how", "hello", 4);

  g.InsertEdge("are", "you", 3);
  std::cout << g;
  return 0;
}

int main() {
  std::string s1{"hello"};
  std::string s2{"how"};
  std::string s3{"are"};
  std::string s4{"shit"};

  std::string s3_1{"abc"};

  auto e1 = std::make_tuple(s1, s2, 5.4);
  auto e2 = std::make_tuple(s2, s3, 7.6);
  auto e3 = std::make_tuple(s3, s4, 2.8);

  auto e4 = std::make_tuple(s3_1, s4, 29.9);
  auto e5 = std::make_tuple(s3_1, s1, 7.7);
  auto e6 = std::make_tuple(s3_1, s4, 7.7);

  auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2, e3, e4, e5, e6};
  gdwg::Graph<std::string, double> g{e.begin(), e.end()};
  std::cout << g;
  return 0;
}

int main() {

  gdwg::Graph<std::string, double> g{"a", "b", "x", "y"};
  g.InsertEdge("a", "b", 4);
  std::cout << g;
  return 0;
}
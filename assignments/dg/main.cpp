#include <iostream>
#include "graph.h"
#include <vector>

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
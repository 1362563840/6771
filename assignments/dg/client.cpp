#include <iostream>
#include <string>

#include "graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  gdwg::Graph<std::string, int> g;
  g.InsertNode("A");
  g.InsertNode("B");
  g.InsertNode("C");
  g.InsertNode("D");

  g.InsertEdge("A", "B", 3);
  g.InsertEdge("C", "B", 2);
  g.InsertEdge("D", "B", 4);

  g.MergeReplace("A", "B");

  std::cout << g << '\n';
}

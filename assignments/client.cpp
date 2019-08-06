#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  auto t1 = std::make_tuple("A", "B", 2);
  auto t2 = std::make_tuple("B", "C", 2.5);
  auto t3 = std::make_tuple("B", "A", 1);
  auto t4 = std::make_tuple("B", "D", 0.5);
  auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2, t3, t4};
  gdwg::Graph<std::string, double> g{it.begin(), it.end()};
  g.erase("A", "B", 2);
  bool c = g.IsConnected("A", "B");
  std::cout << c << std::endl;
}

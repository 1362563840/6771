#include <iostream>
#include <vector>

#include "graph.h"

/***
 * tuple with const
 */
int main() {

  std::string s1{"hello"};
  std::string s2{"how"};
  std::string s3{"are"};
  std::string s4{"fine"};
  auto e1 = std::make_tuple(s1, s2, 5.4);
  auto e1_3 = std::make_tuple(s1, s2, 2.4);
  auto e1_4 = std::make_tuple(s1, s2, 3.4);
  auto e1_1 = std::make_tuple(s1, s3, 8);
  auto e1_2 = std::make_tuple(s1, s4, 2);

  auto e2 = std::make_tuple(s2, s1, 5.4);
  auto e2_1 = std::make_tuple(s2, s3, 7.6);

  auto e3 = std::make_tuple(s3, s4, 3.3);

  auto e4 = std::make_tuple(s4, s3, 2.8);

  auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e1_1, e1_2, e1_3, e1_4 e2, e2_1, e3, e4};
  const gdwg::Graph<std::string, double> g{e.begin(), e.end()};

  std::cout << g;

  const std::vector<double> v1 = g.GetWeights("hello","how");
  for( auto& it : v1 ) {
    std::cout << it << std::endl;
  }

//  const std::vector<std::string> v1 = g.GetConnected("hello");
//
//  for( auto& it : v1 ) {
//    std::cout << it << std::endl;
//  }

//  const std::vector<std::string> v1 = g.GetNodes();
//
//  for( auto& it : v1 ) {
//    std::cout << it << std::endl;
//  }

//  std::cout << " connected >>> " << g.IsConnected("are", "fine") << std::endl;
  return 0;
}

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
std::set<std::weak_ptr<gdwg::Graph<std::__cxx11::basic_string<a>, int>::Edge>, b,a > >::const_iterator&
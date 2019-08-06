/*

  == Explanation and rational of testing ==

  For this assignment, the test is complicated as the test of a method may require the call of other
  methods. We need to firstly ensure some basic methods are right. For some boolean methods, we test
  whether it is called successfully. If it has the exception, we also test the throw of it. We will
  also test the iterator to ensure the basic operations working.

*/

#include "assignments/dg/graph.h"
#include "catch.h"

// TODO(students): Fill this in.
SCENARIO("Test the begin and end of an empty const iterator") {
  WHEN("You have an empty graph") {
    gdwg::Graph<std::string, double> g;
    THEN("Get the begin and the end of the iterator") {
      auto cbegin = g.cbegin();
      auto cend = g.cend();
      auto begin = g.begin();
      auto end = g.end();
      REQUIRE(cbegin == cend);
      REQUIRE(begin == end);
    }
  }
}

SCENARIO("Test the increase and decrease of an const iterator") {
  WHEN("You have a graph") {
    std::vector<std::string> v{"A", "B", "C", "D"};
    gdwg::Graph<std::string, double> g{v.begin(), v.end()};
    g.InsertEdge("A", "B", 2.5);
    g.InsertEdge("B", "B", 1.5);
    g.InsertEdge("C", "B", 2);
    g.InsertEdge("A", "C", 1.5);
    g.InsertEdge("A", "D", 1.5);
    THEN("Get the iterator to different position") {
      auto AB = g.find("A", "B", 2.5);
      auto BB = g.find("B", "B", 1.5);
      auto CB = g.find("C", "B", 2);
      auto AC = g.find("A", "C", 1.5);
      auto AD = g.find("A", "D", 1.5);
      auto curr = g.cbegin();
      REQUIRE(curr == AB);
      curr++;
      REQUIRE(curr == AC);
      curr--;
      REQUIRE(curr == AB);
      curr++;
      curr++;
      REQUIRE(curr == AD);
      auto curr2 = g.cend();
      curr2--;
      REQUIRE(curr2 == CB);
      curr2--;
      REQUIRE(curr2 == BB);
      curr2++;
      REQUIRE(curr2 == CB);
    }
  }
}

SCENARIO("Test the begin and end of an empty const reverse iterator") {
  WHEN("You have an empty graph") {
    gdwg::Graph<std::string, double> g;
    THEN("Get the begin and the end of the iterator") {
      auto crbegin = g.crbegin();
      auto crend = g.crend();
      auto rbegin = g.rbegin();
      auto rend = g.rend();
      REQUIRE(crbegin == crend);
      REQUIRE(rbegin == rend);
    }
  }
}

SCENARIO("Test the increase and decrease of an const reverse iterator") {
  WHEN("You have a graph") {
    std::vector<std::string> v{"A", "B", "C", "D"};
    gdwg::Graph<std::string, double> g{v.begin(), v.end()};
    g.InsertEdge("A", "B", 2.5);
    g.InsertEdge("B", "B", 1.5);
    g.InsertEdge("C", "B", 2);
    g.InsertEdge("A", "C", 1.5);
    g.InsertEdge("A", "D", 1.5);
    THEN("Get the iterator to different position") {
      auto curr1 = g.crbegin();
      auto curr2 = g.crend();
      curr2--;
      curr2--;
      curr2--;
      curr2--;
      curr2--;
      REQUIRE(curr1 == curr2);
      curr2++;
      curr1++;
      REQUIRE(curr1 == curr2);
      curr2++;
      curr1++;
      REQUIRE(curr1 == curr2);
      curr2++;
      curr1++;
      REQUIRE(curr1 == curr2);
      curr2++;
      curr1++;
      REQUIRE(curr1 == curr2);
      curr2++;
      curr1++;
      REQUIRE(curr1 == curr2);
    }
  }
}

SCENARIO("Test the const iterator order and find") {
  WHEN("You have a graph") {
    std::vector<std::string> v{"A", "B", "C", "D"};
    gdwg::Graph<std::string, double> g{v.begin(), v.end()};
    g.InsertEdge("A", "B", 2.5);
    g.InsertEdge("B", "B", 1.5);
    g.InsertEdge("C", "B", 2);
    g.InsertEdge("A", "C", 1.5);
    g.InsertEdge("A", "D", 1.5);
    THEN("Get the iterator to different position") {
      auto AB = g.find("A", "B", 2.5);
      auto BB = g.find("B", "B", 1.5);
      auto CB = g.find("C", "B", 2);
      auto AC = g.find("A", "C", 1.5);
      auto AD = g.find("A", "D", 1.5);
      auto curr = g.cbegin();
      REQUIRE(curr == AB);
      curr++;
      REQUIRE(curr == AC);
      curr++;
      REQUIRE(curr == AD);
      curr++;
      REQUIRE(curr == BB);
      curr++;
      REQUIRE(curr == CB);
      auto curr2 = g.cend();
      curr2--;
      REQUIRE(curr2 == CB);
    }
  }
}

SCENARIO("Test the remove of the iterator") {
  WHEN("You have a graph") {
    std::vector<std::string> v{"A", "B", "C", "D"};
    gdwg::Graph<std::string, double> g{v.begin(), v.end()};
    g.InsertEdge("A", "B", 2.5);
    g.InsertEdge("B", "B", 1.5);
    g.InsertEdge("C", "B", 2);
    g.InsertEdge("A", "C", 1.5);
    g.InsertEdge("A", "D", 1.5);
    THEN("Get the iterator to different position") {
      auto AB = g.find("A", "B", 2.5);
      auto BB = g.find("B", "B", 1.5);
      auto CB = g.find("C", "B", 2);
      auto AC = g.find("A", "C", 1.5);
      auto AD = g.find("A", "D", 1.5);
      auto curr = g.cbegin();
      REQUIRE(curr == AB);
      curr++;
      REQUIRE(curr == AC);
      curr++;
      REQUIRE(curr == AD);
      g.erase(curr);
      REQUIRE(curr == BB);
      g.erase(curr);
      REQUIRE(curr == CB);
      g.erase(curr);
      REQUIRE(curr == g.end());
    }
  }
}

SCENARIO("Insert a node into an empty graph") {
  WHEN("You have a node and an empty graph") {
    gdwg::Graph<std::string, double> g;
    std::string node = "hello";
    THEN("Insert the node") {
      REQUIRE(g.InsertNode(node) == true);
      REQUIRE(g.IsNode(node) == true);
    }
  }
}

SCENARIO("Insert a duplicate node into a graph") {
  WHEN("You have a graph having a node") {
    gdwg::Graph<std::string, double> g;
    std::string node = "hello";
    g.InsertNode(node);
    std::string duplicate = "hello";
    THEN("Insert the node") { REQUIRE(g.InsertNode(duplicate) == false); }
  }
}

SCENARIO("Insert an edge between two nodes") {
  WHEN("You have a graph having two nodes") {
    gdwg::Graph<std::string, double> g;
    std::string node1 = "hello";
    g.InsertNode(node1);
    std::string node2 = "world";
    g.InsertNode(node2);
    THEN("Insert the edge") {
      REQUIRE(g.InsertEdge(node1, node2, 1) == true);
      REQUIRE(g.IsConnected(node1, node2) == true);
    }
  }
}

SCENARIO("Insert an existed edge between two nodes") {
  WHEN("You have a graph having two nodes with an edge") {
    gdwg::Graph<std::string, double> g;
    std::string node1 = "hello";
    g.InsertNode(node1);
    std::string node2 = "world";
    g.InsertNode(node2);
    g.InsertEdge(node1, node2, 1);
    THEN("Insert the edge") { REQUIRE(g.InsertEdge(node1, node2, 1) == false); }
  }
}

SCENARIO("Insert a self edge") {
  WHEN("You have a graph having a node") {
    gdwg::Graph<std::string, double> g;
    std::string node = "hello";
    g.InsertNode(node);
    THEN("Insert the edge") {
      REQUIRE(g.InsertEdge(node, node, 1) == true);
      REQUIRE(g.IsConnected(node, node) == true);
    }
  }
}

SCENARIO("Insert an edge between two non-exist nodes") {
  WHEN("You have a graph") {
    gdwg::Graph<std::string, double> g;
    THEN("Insert the edge") {
      REQUIRE_THROWS_WITH(
          g.InsertEdge("hello", "world", 1),
          "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
  }
}

SCENARIO("Delete a node") {
  WHEN("You have a graph having a node") {
    gdwg::Graph<std::string, double> g;
    std::string node = "hello";
    g.InsertNode(node);
    THEN("Delete the node") {
      REQUIRE(g.DeleteNode(node) == true);
      REQUIRE(g.IsNode(node) == false);
    }
  }
}

SCENARIO("Delete a node which does not exist") {
  WHEN("You have a graph") {
    gdwg::Graph<std::string, double> g;
    THEN("Delete a node") { REQUIRE(g.DeleteNode("hello") == false); }
  }
}

SCENARIO("Replace a node") {
  WHEN("You have a graph having a node") {
    gdwg::Graph<std::string, double> g;
    std::string node = "hello";
    g.InsertNode(node);
    THEN("Replace the node") { REQUIRE(g.Replace(node, "world") == true); }
  }
}

SCENARIO("Replace a node with an existed node") {
  WHEN("You have a graph having a node") {
    gdwg::Graph<std::string, double> g;
    std::string node = "hello";
    g.InsertNode(node);
    THEN("Replace the node with the same one") { REQUIRE(g.Replace(node, "hello") == false); }
  }
}

SCENARIO("Replace a non-existed node") {
  WHEN("You have a graph") {
    gdwg::Graph<std::string, double> g;
    THEN("Replace the non-existed node") {
      REQUIRE_THROWS_WITH(g.Replace("hello", "world"),
                          "Cannot call Graph::Replace on a node that doesn't exist");
    }
  }
}

SCENARIO("MergeReplace a node") {
  WHEN("You have a graph having some nodes") {
    gdwg::Graph<std::string, double> g;
    g.InsertNode("A");
    g.InsertNode("B");
    g.InsertNode("C");
    g.InsertEdge("A", "B", 5);
    g.InsertEdge("B", "A", 3);
    g.InsertEdge("C", "A", 1);
    THEN("Replace the node A with the node B") {
      g.MergeReplace("A", "B");
      REQUIRE(g.IsNode("A") == false);
      REQUIRE_THROWS_WITH(
          g.IsConnected("A", "B"),
          "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      REQUIRE(g.GetNodes().front() == "B");
      REQUIRE(g.GetNodes().size() == 2);
      REQUIRE(g.GetConnected("B").front() == "B");
      REQUIRE(g.GetWeights("B", "B").front() == 3);
    }
  }
}

SCENARIO("MergeReplace a node, duplicate edges removed") {
  WHEN("You have a graph having some nodes") {
    gdwg::Graph<std::string, double> g;
    g.InsertNode("A");
    g.InsertNode("B");
    g.InsertNode("C");
    g.InsertNode("D");
    g.InsertEdge("A", "B", 1);
    g.InsertEdge("A", "C", 2);
    g.InsertEdge("A", "D", 3);
    g.InsertEdge("B", "B", 1);
    THEN("Replace the node A with the node B") {
      g.MergeReplace("A", "B");
      REQUIRE(g.IsNode("A") == false);
      REQUIRE(g.GetConnected("B").front() == "B");
      REQUIRE(g.GetConnected("B").size() == 3);
      REQUIRE(g.GetWeights("B", "B").size() == 1);
    }
  }
}

SCENARIO("MergeReplace a non-existed node") {
  WHEN("You have a graph having some nodes") {
    gdwg::Graph<std::string, double> g;
    g.InsertNode("A");
    g.InsertNode("B");
    g.InsertNode("C");
    g.InsertEdge("A", "B", 5);
    g.InsertEdge("B", "A", 3);
    g.InsertEdge("C", "A", 1);
    THEN("Replace the node") {
      REQUIRE_THROWS_WITH(
          g.MergeReplace("A", "D"),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      REQUIRE_THROWS_WITH(
          g.MergeReplace("E", "A"),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      REQUIRE_THROWS_WITH(
          g.MergeReplace("E", "D"),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }
  }
}

SCENARIO("Clear all nodes and edges from the graph") {
  WHEN("You have a graph having some nodes") {
    gdwg::Graph<std::string, double> g;
    g.InsertNode("A");
    g.InsertNode("B");
    g.InsertNode("C");
    g.InsertNode("D");
    g.InsertEdge("A", "B", 1);
    g.InsertEdge("A", "C", 2);
    g.InsertEdge("A", "D", 3);
    g.InsertEdge("B", "B", 1);
    THEN("Clear the graph") {
      g.Clear();
      REQUIRE(g.IsNode("A") == false);
      REQUIRE(g.IsNode("B") == false);
      REQUIRE(g.IsNode("C") == false);
      REQUIRE(g.IsNode("D") == false);
    }
  }
}

SCENARIO("Check if src -> des exists") {
  WHEN("You have a graph having some nodes") {
    gdwg::Graph<std::string, double> g;
    g.InsertNode("A");
    g.InsertNode("B");
    g.InsertEdge("A", "B", 1.5);
    THEN("Check src -> des") {
      REQUIRE(g.IsConnected("A", "B") == true);
      REQUIRE(g.IsConnected("B", "B") == false);
      REQUIRE_THROWS_WITH(
          g.IsConnected("C", "A"),
          "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      REQUIRE_THROWS_WITH(
          g.IsConnected("A", "D"),
          "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
  }
}

SCENARIO("Get nodes") {
  WHEN("You have a graph having some nodes") {
    gdwg::Graph<std::string, double> g;
    g.InsertNode("A");
    g.InsertNode("B");
    g.InsertNode("D");
    g.InsertNode("C");
    THEN("Get nodes and check order") { REQUIRE(g.GetNodes().front() == "A"); }
  }
}

SCENARIO("Get connected nodes of a node") {
  WHEN("You have a graph having some nodes") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto t3 = std::make_tuple("B", "A", 1);
    auto t4 = std::make_tuple("B", "D", 0.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2, t3, t4};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Get connected nodes of B") {
      REQUIRE(g.GetConnected("B").front() == "A");
      REQUIRE(g.GetConnected("B").size() == 3);
    }
    THEN("Get connected nodes of A") { REQUIRE(g.GetConnected("A").front() == "B"); }
    THEN("Get connected nodes of C") { REQUIRE(g.GetConnected("C").size() == 0); }
  }
}

SCENARIO("Get connected nodes of a non-existed node") {
  WHEN("You have a graph having some nodes") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto t3 = std::make_tuple("B", "A", 1);
    auto t4 = std::make_tuple("B", "D", 0.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2, t3, t4};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Get connected nodes of E") {
      REQUIRE_THROWS_WITH(g.GetConnected("E"),
                          "Cannot call Graph::GetConnected if src doesn't exist in the graph");
    }
  }
}

SCENARIO("Get weight of edge between two nodes") {
  WHEN("You have a graph having some nodes") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto t3 = std::make_tuple("B", "A", 1);
    auto t4 = std::make_tuple("B", "D", 0.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2, t3, t4};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Get weight of edge between two nodes") {
      REQUIRE(g.GetWeights("A", "B").size() == 1);
      REQUIRE(g.GetWeights("B", "B").size() == 0);
      REQUIRE(g.GetWeights("B", "D").size() == 1);
    }
  }
}

SCENARIO("Get weight of edge between non-existed nodes") {
  WHEN("You have a graph having some nodes") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto t3 = std::make_tuple("B", "A", 1);
    auto t4 = std::make_tuple("B", "D", 0.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2, t3, t4};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Get weight of edge between non-existed nodes") {
      REQUIRE_THROWS_WITH(
          g.GetWeights("E", "A"),
          "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      REQUIRE_THROWS_WITH(
          g.GetWeights("B", "F"),
          "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
  }
}

SCENARIO("Erase an edge") {
  WHEN("You have a graph having some nodes") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto t3 = std::make_tuple("B", "A", 1);
    auto t4 = std::make_tuple("B", "D", 0.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2, t3, t4};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Get weight of edge between two nodes") {
      REQUIRE(g.erase("A", "B", 2) == true);
      REQUIRE(g.erase("A", "B", 2) == false);
    }
  }
}

SCENARIO("Construct a graph by an iterator over vectors") {
  WHEN("You have the start and the end of an iterator") {
    std::vector<std::string> v{"A", "B", "C", "D"};
    THEN("Call the constructor") {
      gdwg::Graph<std::string, double> g{v.begin(), v.end()};
      REQUIRE(g.IsNode("A") == true);
      REQUIRE(g.IsNode("B") == true);
      REQUIRE(g.IsNode("C") == true);
      REQUIRE(g.IsNode("D") == true);
      REQUIRE(g.IsNode("E") == false);
    }
  }
}

SCENARIO("Construct a graph by an iterator over tuples") {
  WHEN("You have an iterator over tuples") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2};
    THEN("Call the constructor") {
      gdwg::Graph<std::string, double> g{it.begin(), it.end()};
      REQUIRE(g.IsNode("A") == true);
      REQUIRE(g.IsNode("B") == true);
      REQUIRE(g.IsNode("C") == true);
      REQUIRE(g.IsNode("D") == false);
      REQUIRE(g.IsConnected("A", "B") == true);
      REQUIRE(g.IsConnected("B", "C") == true);
      REQUIRE(g.IsConnected("A", "C") == false);
    }
  }
}

SCENARIO("Construct a graph by an initialised list") {
  WHEN("You have an initialised list") {
    THEN("Call the constructor") {
      gdwg::Graph<char, std::string> g{'A', 'B', 'C', 'D'};
      REQUIRE(g.IsNode('A') == true);
      REQUIRE(g.IsNode('B') == true);
      REQUIRE(g.IsNode('C') == true);
      REQUIRE(g.IsNode('D') == true);
    }
  }
}

SCENARIO("Copy constructor") {
  WHEN("You have a graph") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Copy the graph to another") {
      gdwg::Graph<std::string, double> gCopy{g};
      REQUIRE(gCopy.IsNode("A") == true);
      REQUIRE(gCopy.IsNode("B") == true);
      REQUIRE(gCopy.IsNode("C") == true);
      REQUIRE(gCopy.IsNode("D") == false);
      REQUIRE(gCopy.IsConnected("A", "B") == true);
      REQUIRE(gCopy.IsConnected("B", "C") == true);
      REQUIRE(gCopy.IsConnected("A", "C") == false);
    }
  }
}

SCENARIO("Move constructor") {
  WHEN("You have a graph") {
    gdwg::Graph<char, std::string> g{'A', 'B', 'C', 'D'};
    THEN("Move the graph to another") {
      gdwg::Graph<char, std::string> gMove{std::move(g)};
      REQUIRE(gMove.IsNode('A') == true);
      REQUIRE(gMove.IsNode('B') == true);
      REQUIRE(gMove.IsNode('C') == true);
      REQUIRE(gMove.IsNode('D') == true);
      REQUIRE(gMove.IsNode('E') == false);
      REQUIRE(g.IsNode('A') == false);
      REQUIRE(g.IsNode('B') == false);
      REQUIRE(g.IsNode('C') == false);
      REQUIRE(g.IsNode('D') == false);
    }
  }
}

SCENARIO("Copy assignment") {
  WHEN("You have a graph") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Move the graph to another") {
      gdwg::Graph<std::string, double> gCopy{};
      gCopy = g;
      REQUIRE(gCopy.IsNode("A") == true);
      REQUIRE(gCopy.IsNode("B") == true);
      REQUIRE(gCopy.IsNode("C") == true);
      REQUIRE(gCopy.IsNode("D") == false);
      REQUIRE(gCopy.IsConnected("A", "B") == true);
      REQUIRE(gCopy.IsConnected("B", "C") == true);
      REQUIRE(gCopy.IsConnected("A", "C") == false);
    }
  }
}

SCENARIO("Move assignment") {
  WHEN("You have a graph") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2};
    gdwg::Graph<std::string, double> g{it.begin(), it.end()};
    THEN("Move the graph to another") {
      gdwg::Graph<std::string, double> gMove{};
      gMove = std::move(g);
      REQUIRE(gMove.IsNode("A") == true);
      REQUIRE(gMove.IsNode("B") == true);
      REQUIRE(gMove.IsNode("C") == true);
      REQUIRE(gMove.IsNode("D") == false);
      REQUIRE(gMove.IsConnected("A", "B") == true);
      REQUIRE(gMove.IsConnected("B", "C") == true);
      REQUIRE(gMove.IsConnected("A", "C") == false);
    }
  }
}

SCENARIO("Two identical graphs") {
  WHEN("You have two identical graphs") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2};
    gdwg::Graph<std::string, double> g1{it.begin(), it.end()};
    gdwg::Graph<std::string, double> g2;
    g2.InsertNode("A");
    g2.InsertNode("B");
    g2.InsertNode("C");
    g2.InsertEdge("A", "B", 2);
    g2.InsertEdge("B", "C", 2.5);
    THEN("Compare two graphs") { REQUIRE(g1 == g2); }
  }
}

SCENARIO("Two different graphs") {
  WHEN("You have two different graphs") {
    auto t1 = std::make_tuple("A", "B", 2);
    auto t2 = std::make_tuple("B", "C", 2.5);
    auto it = std::vector<std::tuple<std::string, std::string, double>>{t1, t2};
    gdwg::Graph<std::string, double> g1{it.begin(), it.end()};
    gdwg::Graph<std::string, double> g2;
    g2.InsertNode("A");
    g2.InsertNode("B");
    g2.InsertNode("C");
    g2.InsertEdge("A", "B", 3);
    g2.InsertEdge("B", "C", 2.5);
    THEN("Compare two graphs") { REQUIRE(g1 != g2); }
  }
}

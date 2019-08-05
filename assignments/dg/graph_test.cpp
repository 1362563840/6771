/*

  == Explanation and rational of testing ==

  For this assignment, the test is complicated as the test of a method may require the call of other
  methods. We need to firstly ensure some basic methods are right. For some boolean methods, we test
  whether it is called successfully. If it has the exception, we also test the throw of it.

*/

#include "assignments/dg/graph.h"
#include "catch.h"

// TODO(students): Fill this in.
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

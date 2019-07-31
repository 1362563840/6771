/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

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

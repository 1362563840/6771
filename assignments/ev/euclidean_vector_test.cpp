/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"

#include <vector>

/**
 * get() returns the reference of the unique_ptr
 */
SCENARIO("check constructor") {
  GIVEN("test default constructor") {
    EuclideanVector test1(3);
    REQUIRE(test1.GetNumDimensions() == 3 );
    REQUIRE(test1.get()[0] == 0 );
    REQUIRE(test1.get()[1] == 0 );
    REQUIRE(test1.get()[2] == 0 );
  }

  GIVEN("test second constructor with int and double") {
    EuclideanVector test1(4, 2.5);
    REQUIRE(test1.GetNumDimensions() == 4 );
    REQUIRE(test1.get()[0] == 2.5 );
    REQUIRE(test1.get()[1] == 2.5 );
    REQUIRE(test1.get()[2] == 2.5 );
    REQUIRE(test1.get()[3] == 2.5 );
  }

  GIVEN("test third constructor with vector") {
    std::vector<double> temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    temp.push_back(4);
    EuclideanVector test1(temp.cbegin(), temp.cend());
    REQUIRE(test1.GetNumDimensions() == 4 );
    REQUIRE(test1.get()[0] == 1 );
    REQUIRE(test1.get()[1] == 2 );
    REQUIRE(test1.get()[2] == 3 );
    REQUIRE(test1.get()[3] == 4 );
  }
  /**
   * right now, it is safe assume that second constructor is good
   */
  GIVEN("test fourth construct, copy constructor") {
    EuclideanVector test(3,4);
    EuclideanVector test1(test);
    REQUIRE(test.GetNumDimensions() == 3 );
    REQUIRE(test.get()[0] == 4 );
    REQUIRE(test.get()[1] == 4 );
    REQUIRE(test.get()[2] == 4 );

    REQUIRE(test1.GetNumDimensions() == 3 );
    REQUIRE(test1.get()[0] == 4 );
    REQUIRE(test1.get()[1] == 4 );
    REQUIRE(test1.get()[2] == 4 );

  }

  GIVEN("test fifth construct, move construsctor") {
    EuclideanVector test(3,4);
    EuclideanVector test1(std::move(test));
    REQUIRE(test.GetNumDimensions() == 3 );
    REQUIRE(test.get()[0] == 4 );
    REQUIRE(test.get()[1] == 4 );
    REQUIRE(test.get()[2] == 4 );

    REQUIRE(test1.GetNumDimensions() == 3 );
    REQUIRE(test1.get()[0] == 4 );
    REQUIRE(test1.get()[1] == 4 );
    REQUIRE(test1.get()[2] == 4 );

  }
}
/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/
#include "assignments/ev/euclidean_vector.h"

#include <cmath>
#include <utility>
#include <vector>

#include "catch.h"

/**
 * scenario 1
 * get() returns the reference of the unique_ptr
 */
SCENARIO("check constructor") {

  GIVEN("check GetNumDimensions()") {
    int size = 3;
    EuclideanVector test1( size );

    REQUIRE(test1.GetNumDimensions() == size );
  }

  GIVEN("test default constructor") {
    int size = 3;
    EuclideanVector test1( size );

    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == 0 );
    REQUIRE(test1.get()[1] == 0 );
    REQUIRE(test1.get()[2] == 0 );
  }

  GIVEN("test second constructor with int and double") {
    int size = 4;
    double value = 2.5;
    EuclideanVector test1( size, value );
    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == value );
    REQUIRE(test1.get()[1] == value );
    REQUIRE(test1.get()[2] == value );
    REQUIRE(test1.get()[3] == value );
  }

  GIVEN("test third constructor with vector") {
    int size = 4;
    std::vector<double> temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    temp.push_back(4);
    EuclideanVector test1(temp.cbegin(), temp.cend());
    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == 1 );
    REQUIRE(test1.get()[1] == 2 );
    REQUIRE(test1.get()[2] == 3 );
    REQUIRE(test1.get()[3] == 4 );
  }
  /**
   * right now, it is safe assume that second constructor is good
   */
  GIVEN("test fourth construct, copy constructor") {
    int size = 3;
    double value = 4;
    EuclideanVector test( size, value );
    EuclideanVector test1(test);
    REQUIRE(test.GetNumDimensions() == size );
    REQUIRE(test.get()[0] == value );
    REQUIRE(test.get()[1] == value );
    REQUIRE(test.get()[2] == value );

    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == value );
    REQUIRE(test1.get()[1] == value );
    REQUIRE(test1.get()[2] == value );
  }

  GIVEN("test fifth construct, move construsctor") {
    int size = 4;
    double value = 5.5;
    EuclideanVector test( size, value );
    EuclideanVector test1(std::move(test));

    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == value );
    REQUIRE(test1.get()[1] == value );
    REQUIRE(test1.get()[2] == value );
    REQUIRE(test1.get()[3] == value );
  }
}

/**
 * scenario 2
 */
SCENARIO("check member operator") {
  /**
   * it just swap contents of two referenced objects
   */
  GIVEN("check if function DefinedSwap() works properly") {
    int size = 2;
    double value = 1.9;
    int size_1 = 3;
    double value_1 = 2.6;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    DefinedSwap( test1, test2 );

    REQUIRE(test2.GetNumDimensions() == size );
    REQUIRE(test2.get()[0] == value );
    REQUIRE(test2.get()[1] == value );

    REQUIRE(test1.GetNumDimensions() == size_1 );
    REQUIRE(test1.get()[0] == value_1 );
    REQUIRE(test1.get()[1] == value_1 );
    REQUIRE(test1.get()[2] == value_1 );
  }

  GIVEN("check operator = copy") {
    int size = 5;
    double value = 6.7;
    int size_1 = 3;
    double value_1 = 2.1;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    test1 = test2;

    REQUIRE(test1.GetNumDimensions() == size_1 );
    REQUIRE(test1.get()[0] == value_1 );
    REQUIRE(test1.get()[1] == value_1 );
    REQUIRE(test1.get()[2] == value_1 );

    REQUIRE(test2.GetNumDimensions() == size_1 );
    REQUIRE(test2.get()[0] == value_1 );
    REQUIRE(test2.get()[1] == value_1 );
    REQUIRE(test2.get()[2] == value_1 );
  }

  GIVEN("check operator = move") {
    int size = 5;
    double value = 2.8;
    int size_1 = 3;
    double value_1 = 6.1;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    test1 = std::move(test2);

    REQUIRE(test1.GetNumDimensions() == size_1 );
    REQUIRE(test1.get()[0] == value_1 );
    REQUIRE(test1.get()[1] == value_1 );
    REQUIRE(test1.get()[2] == value_1 );
  }
  
  /**
   * self assignment
   */
  GIVEN("check operator = self assignment") {
    int size = 5;
    double value = 6.7;
    EuclideanVector test1( size, value );

    test1 = test1;

    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == value );
    REQUIRE(test1.get()[1] == value );
    REQUIRE(test1.get()[2] == value );
    REQUIRE(test1.get()[3] == value );
    REQUIRE(test1.get()[4] == value );
  }

  GIVEN("check operator += successful") {
    int size = 4;
    double value = 2.8;
    int size_1 = 4;
    double value_1 = 3.7;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    test1 += test2;

    REQUIRE(test1.GetNumDimensions() == size_1 );
    REQUIRE(test1.get()[0] == ( value + value_1 ) );
    REQUIRE(test1.get()[1] == ( value + value_1 ) );
    REQUIRE(test1.get()[2] == ( value + value_1 ) );
    REQUIRE(test1.get()[3] == ( value + value_1 ) );
  }

  GIVEN("check operator -= successful") {
    int size = 4;
    double value = 2.8;
    int size_1 = 4;
    double value_1 = 3.7;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    test1 -= test2;

    REQUIRE(test1.GetNumDimensions() == size_1 );
    REQUIRE(test1.get()[0] == ( value - value_1 ) );
    REQUIRE(test1.get()[1] == ( value - value_1 ) );
    REQUIRE(test1.get()[2] == ( value - value_1 ) );
    REQUIRE(test1.get()[3] == ( value - value_1 ) );
  }

  GIVEN("check operator *= successful") {
    int size = 4;
    double value = 2.8;
    double value_1 = 8.2;
    EuclideanVector test1( size, value );

    test1 *= value_1;

    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == ( value * value_1 ) );
    REQUIRE(test1.get()[1] == ( value * value_1 ) );
    REQUIRE(test1.get()[2] == ( value * value_1 ) );
    REQUIRE(test1.get()[3] == ( value * value_1 ) );
  }

  GIVEN("check operator /= successful") {
    int size = 4;
    double value = 2.8;
    double value_1 = 8.2;
    EuclideanVector test1( size, value );

    test1 /= value_1;

    REQUIRE(test1.GetNumDimensions() == size );
    REQUIRE(test1.get()[0] == ( value / value_1 ) );
    REQUIRE(test1.get()[1] == ( value / value_1 ) );
    REQUIRE(test1.get()[2] == ( value / value_1 ) );
    REQUIRE(test1.get()[3] == ( value / value_1 ) );
  }

  GIVEN("check convert to vector") {
    unsigned int size = 4;
    std::vector<double> temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    temp.push_back(4);
    EuclideanVector test1(temp.cbegin(), temp.cend());

    std::vector<double> result = (std::vector<double>)test1;

    REQUIRE(result.size() == size );
    REQUIRE(result.at(0) == 1 );
    REQUIRE(result.at(1) == 2 );
    REQUIRE(result.at(2) == 3 );
    REQUIRE(result.at(3) == 4 );
  }

  GIVEN("check convert to list") {
    unsigned int size = 4;
    std::vector<double> temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    temp.push_back(4);
    EuclideanVector test1(temp.cbegin(), temp.cend());

    std::list<double> result = (std::list<double>)test1;

    REQUIRE(result.size() == size );

    double val = result.front();
    REQUIRE( val == 1 );
    result.pop_front();

    val = result.front();
    REQUIRE( val == 2 );
    result.pop_front();

    val = result.front();
    REQUIRE( val == 3 );
    result.pop_front();

    val = result.front();
    REQUIRE( val == 4 );
    
  }
}

/**
 * scenario 3
 */
SCENARIO("check member methods") {
  GIVEN("check at()") {
    int size = 4;
    std::vector<double> temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    temp.push_back(4);
    EuclideanVector test1(temp.cbegin(), temp.cend());

    REQUIRE(test1.GetNumDimensions() == size );

    REQUIRE(test1.at(0) == 1 );
    REQUIRE(test1.at(1) == 2 );
    REQUIRE(test1.at(2) == 3 );
    REQUIRE(test1.at(3) == 4 );
  }

  GIVEN("check GetEuclideanNorm()") {
    // unsigned int size = 4;
    std::vector<double> temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    temp.push_back(4);
    EuclideanVector test1(temp.cbegin(), temp.cend());

    REQUIRE(test1.GetEuclideanNorm() == ( std::sqrt( 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4 ) ) );
  }

  GIVEN("check CreateUnitVector()") {
    // unsigned int size = 4;
    std::vector<double> temp;
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(3);
    temp.push_back(4);
    EuclideanVector test1(temp.cbegin(), temp.cend());

    EuclideanVector test2 = test1.CreateUnitVector();

    double div = test1.GetEuclideanNorm();

    REQUIRE(test2.at(0) == 1 / div );
    REQUIRE(test2.at(1) == 2 / div );
    REQUIRE(test2.at(2) == 3 / div );
    REQUIRE(test2.at(3) == 4 / div );
  }
}

/**
 * scenario 4
 */
SCENARIO("check friend operator") {
  GIVEN("test comparison operator == ") {
    int size = 4;
    double value = 2.5;
    EuclideanVector test1( size, value );

    EuclideanVector test2( size, value );

    REQUIRE( ( test1 == test2 ) == true );
  }

  GIVEN("test comparison operator == for self ") {
    int size = 4;
    double value = 2.5;
    EuclideanVector test1( size, value );

    REQUIRE( ( test1 == test1 ) == true );
  }

  GIVEN("test comparison operator != ") {
    int size = 4;
    double value = 2.5;
    EuclideanVector test1( size, value );

    EuclideanVector test2( size, value );

    REQUIRE( ( test1 != test2 ) == false );
  }

  GIVEN("check operator + ") {
    int size = 4;
    double value = 2.8;
    int size_1 = 4;
    double value_1 = 3.7;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    EuclideanVector test3 = test1 + test2;

    REQUIRE(test3.GetNumDimensions() == size_1 );
    REQUIRE(test3.at(0) == ( value + value_1 ) );
    REQUIRE(test3.at(1) == ( value + value_1 ) );
    REQUIRE(test3.at(2) == ( value + value_1 ) );
    REQUIRE(test3.at(3) == ( value + value_1 ) );
  }

  GIVEN("check operator - ") {
    int size = 4;
    double value = 2.8;
    int size_1 = 4;
    double value_1 = 3.7;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    EuclideanVector test3 = test1 - test2;

    REQUIRE(test3.GetNumDimensions() == size_1 );
    REQUIRE(test3.at(0) == ( value - value_1 ) );
    REQUIRE(test3.at(1) == ( value - value_1 ) );
    REQUIRE(test3.at(2) == ( value - value_1 ) );
    REQUIRE(test3.at(3) == ( value - value_1 ) );
  }

  GIVEN("check operator * dot product ") {
    int size = 4;
    double value = 2.8;
    int size_1 = 4;
    double value_1 = 3.7;
    EuclideanVector test1( size, value );
    EuclideanVector test2( size_1, value_1 );

    double result = test1 * test2;

    REQUIRE( result == ( 2.8*3.7*4 ) );
  }

  GIVEN("check operator *, scalar in right hand side") {
    int size = 4;
    double value = 9.1;
    double value_1 = 1.9;
    EuclideanVector test1( size, value );

    EuclideanVector test3 = test1 * value_1;

    REQUIRE(test3.GetNumDimensions() == size );
    REQUIRE(test3.at(0) == ( value * value_1 ) );
    REQUIRE(test3.at(1) == ( value * value_1 ) );
    REQUIRE(test3.at(2) == ( value * value_1 ) );
    REQUIRE(test3.at(3) == ( value * value_1 ) );
  }

  GIVEN("check operator *, scalar in left hand side") {
    int size = 4;
    double value = 6.7;
    double value_1 = 7.6;
    EuclideanVector test1( size, value );

    EuclideanVector test3 = value_1 * test1;

    REQUIRE(test3.GetNumDimensions() == size );
    REQUIRE(test3.at(0) == ( value * value_1 ) );
    REQUIRE(test3.at(1) == ( value * value_1 ) );
    REQUIRE(test3.at(2) == ( value * value_1 ) );
    REQUIRE(test3.at(3) == ( value * value_1 ) );
  }

  GIVEN("check operator / ") {
    int size = 4;
    double value = 4.8;
    double value_1 = 8.4;
    EuclideanVector test1( size, value );

    EuclideanVector test3 = test1 / value_1;

    REQUIRE(test3.GetNumDimensions() == size );
    REQUIRE(test3.at(0) == ( value / value_1 ) );
    REQUIRE(test3.at(1) == ( value / value_1 ) );
    REQUIRE(test3.at(2) == ( value / value_1 ) );
    REQUIRE(test3.at(3) == ( value / value_1 ) );
  }
}

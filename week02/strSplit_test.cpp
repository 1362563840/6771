//
// Created by matt on 9/06/19.
//

#include "catch.h"
#include <vector>
#include "week01_copy/strSplit.h"

// define some variables used below for tests
using namespace std;
string pattern = "";
vector<string> result;
vector<string> answer = {""};
string delimiter = " ,|";

TEST_CASE("empty string split should be empty") {
  split4( pattern , result , delimiter );
  REQUIRE( result == answer );
}

TEST_CASE("non empty") {

}

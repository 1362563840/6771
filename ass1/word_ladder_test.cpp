/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/
#include <unordered_set>

#include "catch.h"
#include "lexicon.h"
#include "word_ladder.h"


TEST_CASE("Check if function CheckValid() can accurately tell whether words in dict or not") {
  GIVEN("when dict is small") {

    unordered_set<string> words_dict = {"a","b","c","aa","bb","cc","x"};
    REQUIRE(words_dict.size()==7);

    WHEN("abc is not in dic") {
      bool result = CheckValid("abc", words_dict);
      REQUIRE(result==false);
    }

    WHEN("abc is not in dic") {
      bool result = CheckValid("abc", words_dict);
      REQUIRE(result==false);
    }
  }

}
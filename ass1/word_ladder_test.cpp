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
  /**
   * test for function CheckValid()
   * i.e. whether a word is in dict
   */
  GIVEN("when dict is small") {
    unordered_set<string> words_dict = {"a","b","c","aa","bb","cc","x"};
    REQUIRE(words_dict.size()==7);

    WHEN("abc is not in dic") {
      bool result = CheckValid("abc", words_dict);
      REQUIRE(result==false);
    }

    WHEN("aa is in dic") {
      bool result = CheckValid("aa", words_dict);
      REQUIRE(result==true);
    }

    WHEN("special chararcter space ' ' is not in dic") {
      bool result = CheckValid("", words_dict);
      REQUIRE(result==false);
    }

  }

  /**
   * it will compare each string in separate vector, v1, v2
   * if v1.at(i) > v2(i) or <, then should return result
   * othewise, continue
   */
  GIVEN("compare two vector<strubg> not just string") {
    WHEN("first same, then compare second") {
      vector<string> v1 = {"test","more","less"};
      vector<string> v2 = {"test","less","more"};
      int result = Compare(v1,v2);
      REQUIRE(result==0);
    }

    WHEN("first diff") {
      vector<string> v1 = {"abc","more","less"};
      vector<string> v2 = {"test","less","more"};
      int result = Compare(v1,v2);
      REQUIRE(result==-1);
    }

    WHEN("all same") {
      vector<string> v1 = {"abc","more","less"};
      vector<string> v2 = {"abc","more","less"};
      int result = Compare(v1,v2);
      REQUIRE(result==0);
    }
    
  }

  /**
   * test whether 2d string sort works
   * function QuickSort() and Partition()
   * 
   * in random insert
   * because initially size is 0, so it will causes bug
   */
  GIVEN("2d string sort") {
    unsigned int seed = 1;
    WHEN("in reverse order") {
      vector<string> v1 = {"zoo","ok","less","zee"};
      vector<string> v2 = {"test","more","less","zee"};
      vector<string> v3 = {"study","which","less","zee"};
      vector<string> v4 = {"bob","more","less","zee"};
      vector<string> v5 = {"moon","should","less","zee"};
      vector<string> v6 = {"bob","sun","less","zee"};
      vector<string> v7 = {"test","more","ok","zee"};
      vector<string> v8 = {"test","more","ok","jack"};
      vector < vector<string> > v= {v1,v2,v3,v4,v5,v6,v7,v8};
      QuickSort(v, 0, v.size()-1);

      REQUIRE(v.at(0).at(0)=="bob");
      REQUIRE(v.at(0).at(1)=="more");

      REQUIRE(v.at(1).at(0)=="bob");
      REQUIRE(v.at(1).at(1)=="sun");

      REQUIRE(v.at(2).at(0)=="moon");
      REQUIRE(v.at(2).at(1)=="should");

      REQUIRE(v.at(7).at(0)=="zoo");
      REQUIRE(v.at(7).at(1)=="ok");
    }

    WHEN("random insert") {
      vector<string> v1 = {"zoo","ok","less","zee"};
      vector<string> v2 = {"test","more","less","zee"};
      vector<string> v3 = {"study","which","less","zee"};
      vector<string> v4 = {"bob","more","less","zee"};
      vector<string> v5 = {"moon","should","less","zee"};
      vector<string> v6 = {"bob","sun","less","zee"};
      vector<string> v7 = {"test","more","ok","zee"};
      vector<string> v8 = {"test","more","ok","jack"};
      vector < vector<string> > v;

      v.insert(v.begin(),v1);
      v.insert(v.begin(),v2);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v3);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v4);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v5);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v6);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v7);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v8);
      QuickSort(v, 0, v.size()-1 );

      REQUIRE(v.at(0).at(0)=="bob");
      REQUIRE(v.at(0).at(1)=="more");

      REQUIRE(v.at(1).at(0)=="bob");
      REQUIRE(v.at(1).at(1)=="sun");

      REQUIRE(v.at(2).at(0)=="moon");
      REQUIRE(v.at(2).at(1)=="should");

      REQUIRE(v.at(7).at(0)=="zoo");
      REQUIRE(v.at(7).at(1)=="ok");
    }

    WHEN("random insert") {
      vector<string> v1 = {"zoo","ok","less","zee"};
      vector<string> v2 = {"test","more","less","zee"};
      vector<string> v3 = {"study","which","less","zee"};
      vector<string> v4 = {"bob","more","less","zee"};
      vector<string> v5 = {"moon","should","less","zee"};
      vector<string> v6 = {"bob","sun","less","zee"};
      vector<string> v7 = {"test","more","ok","zee"};
      vector<string> v8 = {"test","more","ok","jack"};

      vector < vector<string> > v;
      v.insert(v.begin(),v1);
      v.insert(v.begin(),v2);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v3);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v4);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v5);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v6);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v7);
      v.insert(v.begin()+( rand_r(&seed)%(v.size()) ),v8);

      QuickSort(v,0,6);
      REQUIRE(v.at(0).at(0)=="bob");
      REQUIRE(v.at(0).at(1)=="more");

      REQUIRE(v.at(1).at(0)=="bob");
      REQUIRE(v.at(1).at(1)=="sun");

      REQUIRE(v.at(2).at(0)=="moon");
      REQUIRE(v.at(2).at(1)=="should");

      REQUIRE(v.at(7).at(0)=="zoo");
      REQUIRE(v.at(7).at(1)=="ok");
    }
  }

}
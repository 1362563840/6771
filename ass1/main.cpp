#include <iostream>
#include <string>
#include <map>
#include "lexicon.h"
#include "word_ladder.h"
#include <unordered_set>
using namespace std;

int main(int argc, char *argv[]) {
  auto lexicon = GetLexicon("words.txt");

  // open file, then store all words in map
  // Explain: unordered_set<string> search is constant search time complexity
  unordered_set<string> words_dict;
  for(const auto& word : lexicon) {
    words_dict.insert(  word );
  }
  // debug, need to delete---------------
  // for(auto& elem : words_dict)
  // {
  //   std::cout << elem.first << " " << elem.second << "\n";
  // }

  // should only contains two arguments incluing ./
  if(argc != 2) {
//    Error("wrong command\n");
  }

  string start_string;
  string end_string;
  cout << "Enter start word (RETURN to quit): ";
  cin >> start_string;
  cout << "Enter destination word: ";
  cin >> end_string;

}

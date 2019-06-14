#include <iostream>
#include <map>
#include <string>
#include <time.h>
#include <unordered_set>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"

using std::cout;
using std::exit;
using std::getline;
using std::string;
using std::unordered_set;


int main() {
    auto lexicon = GetLexicon("words.txt");

    // open file, then store all words in map
    // Explain: unordered_set<string> search is constant search time complexity
    unordered_set<string> words_dict;
    for(const auto& word : lexicon) {
      words_dict.insert( word );
    }

    string start_word;
    string end_word;
    cout << "Enter start word (RETURN to quit): ";
    getline (std::cin,start_word);
    if( start_word.length() == 0 ) {
      exit(1);
    }
    cout << "Enter destination word: ";
    getline (std::cin,end_word);
    if( end_word.length() == 0 ) {
      std::exit(1);
    }

    FindPath(start_word, end_word, words_dict);
} 

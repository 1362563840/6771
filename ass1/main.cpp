#include <iostream>
#include <map>
#include <string>
#include <time.h>
#include <unordered_set>

#include "lexicon.h"
#include "word_ladder.h"

using namespace std;

int main(int argc, char *argv[]) {
    clock_t start = clock();
    auto lexicon = GetLexicon("words.txt");

    // open file, then store all words in map
    // Explain: unordered_set<string> search is constant search time complexity
    unordered_set<string> words_dict;
    for(const auto& word : lexicon) {
      words_dict.insert( word );
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
      exit(1);
    }

    FindPath(start_word, end_word, words_dict);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
} 

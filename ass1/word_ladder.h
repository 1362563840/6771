#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;
// third variable
void FindPath(string start_word, string end_word, const unordered_set<string> & words_dict);

void setLevelForStartWord(string start_word
    , unordered_map<string, unordered_set<string> >& prev_nodes
    , unordered_map<string, int>& nodes_level);

void SetLevel(string word, string prev
    , unordered_map<string, unordered_set<string> >& prev_nodes
    , unordered_map<string, int>& nodes_level);

bool CheckValid(string word,const unordered_set<string>& words_dict);

void display_map( const unordered_map<string, unordered_set<string> >& prev_nodes);

void SortPath();
#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_

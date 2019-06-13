#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
// third variable

bool CheckValid(string word,const unordered_set<string>& words_dict);

int Compare( const vector<string>& a, const vector<string>& b );

void DisplayMap( const unordered_map<string, unordered_set<string> >& prev_nodes);

void FindPath(string start_word, string end_word,const unordered_set<string> & words_dict);

int Partition( vector < vector<string> >& all_paths, int low, int high );

void QuickSort( vector < vector<string> >& all_paths, int low, int high );

void SetLevel(string word, string prev
    , unordered_map<string, unordered_set<string> >& prev_nodes
    , unordered_map<string, int>& nodes_level);

void SetLevelForStartWord(string start_word
    , unordered_map<string, unordered_set<string> >& prev_nodes
    , unordered_map<string, int>& nodes_level);

void SortPath(string start_word, string end_word,
                const unordered_map<string, unordered_set<string> >& prev_nodes,
                const unordered_map<string, int>& nodes_level, 
                vector < vector<string> >& all_paths);

void TestDisplay( const vector<string>& one );

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_

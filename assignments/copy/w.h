#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cout;
using std::exit;
using std::getline;
using std::queue;
using std::set;
using std::stack;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

bool CheckValid(string word,const unordered_set<string>& words_dict);

int Compare(const vector<string>& a, const vector<string>& b);

void DisplayMap(const vector < vector<string> >& all_paths);

void FindPath(string start_word, string end_word,const unordered_set<string> & words_dict);

int Partition(vector < vector<string> >& all_paths, int low, int high);

void QuickSort(vector < vector<string> >& all_paths, int low, int high);

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

void TestDisplay(const vector<string>& one);

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
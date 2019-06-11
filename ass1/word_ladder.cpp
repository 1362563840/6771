#include "word_ladder.h"
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <vector>
#include <set>
#include <unordered_set>
#include <limits>
#include <iostream>
using namespace std;

/**
 * for start_word only
 */
void setLevelForStartWord(string start_word
    , unordered_map<string, unordered_set<string> >& prev_nodes
    , unordered_map<string, int>& nodes_level) {
  unordered_set<string> temp_set;
  prev_nodes.insert( {start_word, temp_set} );
  nodes_level.insert( {start_word, 1} );
}

/**
 * if it has no previous node, then it must be start_word, set it to 1 otherwise
 * pre_level + 1
 */
void SetLevel(string word, string prev
    , unordered_map<string, unordered_set<string> >& prev_nodes
    , unordered_map<string, int>& nodes_level) {
  // debug need to delete----------------------------
  if( prev.compare("") == 0 ) {
    cout << "error 1 setLevel\n";
    exit(1);
  }
  // debug need to delete----------------------------
  // it is new node
  if( prev_nodes.find(word) == prev_nodes.end() ) {
    unordered_set<string> temp_set = {prev};
    prev_nodes.insert( {word, temp_set} );
    nodes_level.insert( {word, nodes_level.at(prev) + 1 } );
  }
    // old node
  else if( prev_nodes.find(word) != prev_nodes.end() ) {
    // if in prev_nodes, "word" already exists, check existing level
    // if existing is larger , then error
    // if samller, then ignore,
    // if same, then record

    // larger
    // debug need to delete----------------------------
    if( nodes_level.at(word) > nodes_level.at(prev) + 1 ) {
      cout << "error impossible case debug\n";
      exit(1);
    }
    // debug need to delete----------------------------
    // same
    if( nodes_level.at(word) == nodes_level.at(prev) + 1  ) {
      // find existing set
      unordered_set<string>& temp_set = prev_nodes.at(word);
      // insert new previous node with same length path
      temp_set.insert(prev);
    }

  }
    // debug need to delete----------------------------
  else {
    cout << "error 2 setLevel\n";
    exit(1);
  }
  // debug need to delete----------------------------
}

/**
 * when pop up a word, find all possible transforming words. After that,
 * mark this word as finished.
 *
 * one optimization
 *
 */
void FindPath(string start_word, string end_word,const unordered_set<string> & words_dict) {
  // queue waiting to be popped up
  queue<string> waiting_q;
  // store previous nodes, i.e. who can reach it
  unordered_map<string, unordered_set<string> > prev_nodes;
  // in BFS, each node has its own level, start_word's level is 1
  unordered_map<string, int> nodes_level;
  // when pop up a word, immediately store this word inside
  // since when find possible transforming words, itself is also inside
  // for example, "back", for 'b', we go through 'a'-'z', but in second loop, (b is after a in Alphabet order)
  // the possible one is "back" which is exactly the origin one
  unordered_set<string> finished_words;

  int found_path = 0;
  int current_level = 1;
  int shortest_level = std::numeric_limits<int>::max();
  string current;

  // before go to loop, do somthing
  waiting_q.push(start_word);
  setLevelForStartWord(start_word, prev_nodes, nodes_level);

  // end loop conditions:
  // 1. no entry in queue  2. find the end_word and level is larger than shortest_level
  for( ; ( waiting_q.empty() != true ) &&
    ( !( found_path == 1 && current_level > shortest_level ) ); ) {
    current = waiting_q.front();
    waiting_q.pop();
    // before do anthing further, check if this word is in set : finished_words
    if( finished_words.find(current) != finished_words.end() ) {
      continue;
    }
    // update current_level if it can be higher
    if( nodes_level.at(current) > current_level ) {
      current_level = nodes_level.at(current);
    }
    // check if current is end_word
    // the first time it finds, store it then freeze it
    if( current.compare(end_word) == 0 && found_path == 0 )  {
      found_path = 1;
      shortest_level = current_level;
    }
    finished_words.insert(current);
    // from left to right
    for( int i = 0 ; i < current.length() ; i++ ) {
      // from a - z, check each possible
      for( int j = 97 ; j <= 122 ; j++ ) {
        char temp = j;
        string substitu(1, j);
        string child = current;
        child.replace(i, 1, substitu);

        /**
         * when go through loop, if child is current itself(possible)
         * then still push into queue. but when pop up, it will fail the if check
         * so everything is still ok
         */
        // then check if this "word" exists in dictionary
        if( CheckValid(child, words_dict) == true ) {
          SetLevel(child, current, prev_nodes, nodes_level);
          waiting_q.push(child);
        }
      }
    }

  }

  // debug ---------------------------------
  display_map(prev_nodes);

}

void display_map( const unordered_map<string, unordered_set<string> >& prev_nodes) {
  cout << "start\n";
  for( auto const& pair : prev_nodes ) {
    cout << pair.first << "->\n";
    int column = 10;
    int current_col = 0;
    for( auto const& str : pair.second ) {
      // display in matrix
      if( current_col == column ) {
        cout << "\n" << str;
      }
      else {
        cout << str << ", ";
      }
      current_col++;
    }
    cout << "\n";
  }
}

/**
 *
 */
bool CheckValid(string word,const unordered_set<string>& words_dict) {
  if( words_dict.find( word ) != words_dict.end() ) {
    return true;
  }
  return false;
}

/**
 * after path is found, we need trace back and sort them
 */
// void SortPath(string start_word, string end_word,
//                 const unordered_map<string, unordered_set<string> >& prev_nodes,
//                 const unordered_map<string, int> nodes_level) {
//     vector< vector<string> > paths;
//     int how_many_paths;
//     int shortest_hops = nodes_level.at(end_word);

//     // before go through map, do something
//     // insert end_word
//     vector<string> temp = {end_word};
//     paths.insert(paths.end(), temp);

//     for( int i = 0 ; i < prev_nodes.at(end_word).size() ; i++ ) {
//         vector<string> parent_copy = paths.at(0);
//         if( i == 0 ) {
//             te
//         }
//     }
//     /**
//      * use stack to find all path
//      * 
//      */
//     for( int i = 1 ; i < shortest_hops ; i++ ) {
//         vector<string> parent_copy = paths.at(0);

//         for ()
//     }

// }

void SortPath(string start_word, string end_word,
                const unordered_map<string, unordered_set<string> >& prev_nodes,
                const unordered_map<string, int> nodes_level) {
    stack <string> stack_words;

    int how_many_paths;
    int current_path_level = 0;
    vector <string> curr_path;
    vector < vector<string> > all_paths;

    stack_words.push(end_word);
    /**
     * use stack to find all path
     * 
     */
    for( ; stack_words.empty() == false ; ) {
        string curr = stack_words.top();
        stack_words.pop();
        // next hop, add it to current path
        if( current_path_level < nodes_level.at(curr) ) {
            // debug need to delete ----------------------
            if( current_path_level + 1 != nodes_level.at(curr) ) {
                cout << "error stack level is not correct\n";
            }
            // debug need to delete ----------------------
            curr_path.insert( curr_path.end(), curr);
            current_path_level++;
        }
        // in this situation, change the last entry in curr_path
        else if( current_path_level == nodes_level.at(curr) ) {
            //replace value
            curr_path.at( current_path_level - 1 ) = curr;
        }
        // debug delete --------------------------------
        else {
            cout << "error check \n";
            exit(1);
        }
        // debug delete --------------------------------
        
        // check if reaches the end_word
        if() {

        }

        const unordered_set<string>& temp = prev_nodes.at(curr);
        set<string> order;
        // convert unorder_set to set
        copy( temp.begin(), temp.end(), order.begin() );
        
        // add children to stack
        for( const auto& word : order ) {
            stack_words.push(word);
        }

        // 
    }

}
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
void SetLevelForStartWord(string start_word
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
  SetLevelForStartWord(start_word, prev_nodes, nodes_level);

  // end loop conditions:
  // 1. no entry in queue  2. find the end_word and level is larger than shortest_level
  for( ; ( waiting_q.empty() != true ) &&
    ( !( found_path == 1 && current_level > shortest_level ) ); ) {
    current = waiting_q.front();
    waiting_q.pop();

    // debug
    cout << "current is " << current << "\n";
    // debug
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
        // debug
        cout << "one possible child is " << child << "\n";
        // debug
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
  // display_map(prev_nodes);

  // debug ---------------------------------
  vector < vector<string> > all_paths;
  if( found_path == 0 ) {
    cout << "No ladder found." << "\n";
  }
  else {
    SortPath(start_word, end_word,
                prev_nodes,
                nodes_level, 
                all_paths);
    for( int i = 0 ; i < all_paths.size() ; i++ ) {
      for( int j = 0 ; j < all_paths.at( i ).size() ; j++ ) {
        if( j == 0 ) {
          cout << all_paths.at(i).at(j);
        }
        else {
          cout << "->" << all_paths.at(i).at(j);
        }
      }
      cout << "\n";
    }
  }

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
                const unordered_map<string, int>& nodes_level, 
                vector < vector<string> >& all_paths) {
    stack <string> stack_words;

    int how_many_paths;
    int current_path_level = nodes_level.at(end_word) + 1;
    int how_many_vec = 0;
    vector <string> curr_path;

    stack_words.push(end_word);
    
    cout << "end_word level is >>" << nodes_level.at(end_word) << "\n";

    /**
     * use stack to find all path
     * 
     */
    for( ; stack_words.empty() == false ; ) {
        string curr = stack_words.top();
        stack_words.pop();
        // next hop, add it to current path
        if( current_path_level > nodes_level.at(curr) ) {
            // debug need to delete ----------------------
            if( current_path_level - 1 != nodes_level.at(curr) ) {
                cout << "error stack level is not correct\n";
                exit(1);
            }
            // debug need to delete ----------------------
            // reason to insert begin is that from end_word to start_word
            curr_path.insert( curr_path.begin(), curr);
            how_many_vec++;
            current_path_level--;
        }
        // in this situation, change the last entry in curr_path
        else if( current_path_level == nodes_level.at(curr) ) {
            //replace value
            curr_path.at( 0 ) = curr;
        }
        // debug delete --------------------------------
        else {
            cout << "error check \n";
            exit(1);
        }
        // debug delete --------------------------------
        // bug ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // check if reaches the end_word
        // if it is end_word, record this path into all_paths
        // then adjust curr_paths, make it go back two steps;
        if( curr == start_word ) {
          //debug need to delete ------------------
          if( current_path_level != nodes_level.at(start_word) ) {
            cout << "error level\n";
            exit(1);
          }
          //debug need to delete ------------------
          // insert front, then the vector is sorted in order
          all_paths.insert( all_paths.begin(), curr_path );

          // if stack is empty, then do nothing
          // 
          // bug ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
          if( stack_words.empty() == true ) {
            // do nothing
          }
          else {
            string current_top = stack_words.top();
            int should_be_level = nodes_level.at(current_top);
            for( int i = 1 ; i <= ( should_be_level - current_path_level ) ; i++ ) {
              curr_path.erase( curr_path.begin() );
              how_many_vec--;
            }
            // after delete nodes in curr_path, need to reset current_path_level
            current_path_level = should_be_level;
          }

        }

        const unordered_set<string>& temp = prev_nodes.at(curr);
        set<string> order;
        // convert unorder_set to set
        copy( temp.begin(), temp.end(), inserter( order, order.begin() ) );
        
        // add children to stack
        for( const auto& word : order ) {
            stack_words.push(word);
        }

        // 
    }

}

void QuickSort( vector < vector<string> >& all_paths ) {

}

/**
 *  if left is less than right return -1
 *  else return 0
 */
int ArraySort( const vector<string>& a, const vector<string>& b ) {
  for( int i = 0 ; i < a.size() ; i++ ) {
    if( a.at(i).compare( b.at(i) ) < 0 ) {
      return -1;
    }
  }
  return 0;
} 
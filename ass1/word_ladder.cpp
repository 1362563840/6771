#include "word_ladder.h"
#include <map>
#include <unordered_map> 
#include <queue>
#include <vector>
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
void FindPath(string start_word, string end_word, const unordered_set<string> & words_dict) {
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
    // 1. no entry in queu  2. find the end_word and level is larger than shortest_level 
    for( ; ( waiting_q.empty() == true ) ||
           ( found_path == 1 && current_level > shortest_level  ); ) {
        current = waiting_q.front();
        waiting_q.pop();
        // before do anthing further, check if this word is in set : finished_words
        if ( finished_words.find(current) != finished_words.end() ) {
            continue;
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

                // then check if this "word" exists in dictionary
                if( CheckValid(child, words_dict) == true ) {
                    SetLevel(child, current, prev_nodes, nodes_level);
                    waiting_q.push(); 
                }
            }
        }
        
    }
}


/**
 * 
 */
bool CheckValid(const string word, const unordered_set<string> & const words_dict ) {
    if( words_dict.find( word ) != words_dict.end() ) {
        return true;
    }
    return false;
}
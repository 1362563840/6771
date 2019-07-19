// #include "graph.h"
#include "assignments/dg/graph.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout
using std::set;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::vector;
using std::weak_ptr;

typedef gdwg::Graph::Node
{
    N name;
    // std::set< std::shared_ptr<Edge>, []( const shared_ptr<Edge> lhs, const shared_ptr<Edge> rhs ) {
    //                                         return (
    //                                             ( lhs.dest < rhs.dest ) ||
    //                                             ( ( lhs.dest == rhs.dest ) && ( lhs.weight < rhs.weight ) )
    //                                         );
    //                                             }
    // > outcoming;
    std::set< std::shared_ptr<Edge>, EdgeComparator > outcoming;
    std::set< std::shared_ptr<Edge>, EdgeComparator > incoming;
}Node

template<typename N, typename E>
Graph( std::vector<N>::const_iterator start, std::vector<N>::const_iterator end )
{
    
}
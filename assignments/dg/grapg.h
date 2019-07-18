#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout;
using std::map;
using std::set;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::unordered_set;
using std::vector;
using std::weak_ptr;

namespace gdwg {

    template<typename N, typename E>
    class Graph {
        public:
            Graph() = default;
            // change ------------------------------- let clion thinks this one is good
            Graph( std::vector<N>::const_iterator start, std::vector<N>::const_iterator end ) 
            {
                for( auto it = start; it != end ; it++ ) {
                    shared_ptr<N> name = make_shared<N>(*it);
                    // possible bug ---------------------------
                    // make_shared should just call Node constructor
                    shared_ptr<Node> node = make_shared<Node>( name );
                    this->nodes.emplace( name, node );
                }
            }
            // change ------------------------------- let clion thinks this one is good
            Graph(std::vector<std::tuple<N, N, E>>::const_iterator start, std::vector<std::tuple<N, N, E>>::const_iterator end)
            {
                for( auto it = start ; it != end ; it++ ) {
                    
                }
            }

            class const_iterator {
                // const_iterator find(const N&, const N&, const E&);
            };

            bool InsertNode(const N& val)
            {
                if( IsNode( val ) == true ) {
                    return false;
                }
                shared_ptr<N> temp_N_ptr = make_shared<N>( val );
                shared_ptr<Node> temp_Node_ptr = make_shared<Node>( temp_N_ptr );
                /**
                 * possible bud, check how emplace() works, for name_ptr, should just exist one which is in map
                 */
                this->nodes.emplace( temp_N_ptr, temp_Node_ptr );
                return true;
            }
            
            bool InsertEdge(const N& src, const N& dest, const E& w)
            {
                // check if either one node does not exist
                if(  IsNode( src ) == true ||  IsNode( dest ) == true ) {
                    return false;
                }
                shared_ptr<N> temp_N_src_ptr = make_shared<N>( src );
                shared_ptr<N> temp_N_dst_ptr = make_shared<N>( dst );
                /**
                 * 
                 */
                // check if this edge exists;                                   second is outcoming
                shared_ptr<Node> temp_src_node = this->nodes_.find( temp_N_src_ptr )->second;
                // check set "outcoming"
                for( auto& it : temp_src_node.outcoming ) {
                    shared_ptr<Node> edge_dest = it.dest_.lock();
                    // after find Node "dest", need to access its name(N)
                    shared_ptr<Node> edge_dest_name = edge_dest.name_.lock();
                    if( *edge_dest_name == dest && it.weight_ == w ) {
                        // found a same edge
                        return false;
                    }
                }
                
                // if it is new edge, insert
                /**
                 * need to add two sets, one is node(src).outcoming, second is node(dest).incoming
                 * In order to achieve goal "one resource per edge", only one make_shared,
                 *                                                   but two pointer <---------------
                 */ 
                
                // still use the variable "temp_src_node"
                shared_ptr<>
                temp_src_node.outcoming.

            }

            bool IsNode(const N& val)
            {
                shared_ptr<N> temp_N_ptr = make_shared<N>( val );
                if( this->nodes_.find(temp_N_ptr) != this->nodes_.end() ) {
                    return false;
                }
                return true;
            }
        private:            
            struct Edge;
            struct Node;
            /**
             * need this one to let map implements correct order, instead of pointer self
             */
            struct MapNodeComparator
            {
                bool operator()(const shared_ptr<N>& _lhs, const shared_ptr<N>& _rhs) const {
                    return ( *_lhs < *_rhs );
                }
            };
            // struct NodeComparator
            // {
            //     bool operator()(const shared_ptr<Node>& _lhs, const shared_ptr<Node>& _rhs) const {
            //         return _lhs.name < _rhs.name;
            //     }
            // };
            struct EdgeComparator
            {
                bool operator()(const shared_ptr<Edge>& _lhs, const shared_ptr<Edge>& _rhs) const {
                    // debug --------------------------------------
                    if( _lhs.dest_.expired() == true || _rhs.dest_.expired() == true ) {
                        std::cout << "expire impossible\n";
                        std::exit(1);
                    }
                    // debug --------------------------------------
                    shared_ptr<Node> lhs = _lhs.dest_.lock();
                    shared_ptr<Node> rhs = _rhs.dest_.lock();
                    
                    shared_ptr<N> lhs_name = lhs.name_.lock();
                    shared_ptr<N> rhs_name = rhs.name_.lock();
                    return ( *lhs_name < *rhs_name ) ||
                            ( ( *lhs_name == *rhs_name ) && ( _lhs.weight_ < _rhs.weight_ ) )
                    ;
                }
            };
            typedef struct Node
            {
                Node( shared_ptr<N> name ) : name_{name} {}
                std::weak_ptr<N> name_;
                // std::set< std::shared_ptr<Edge>, []( const shared_ptr<Edge> lhs, const shared_ptr<Edge> rhs ) {
                //                                         return (
                //                                             ( lhs.dest < rhs.dest ) ||
                //                                             ( ( lhs.dest == rhs.dest ) && ( lhs.weight < rhs.weight ) )
                //                                         );
                //                                             }
                // > outcoming;
                std::set< std::shared_ptr<Edge>, EdgeComparator > outcoming;
                std::set< std::shared_ptr<Edge>, EdgeComparator > incoming;
            }Node;

            typedef struct Edge
            {
                Edge( shared_ptr<Node> src, shared_ptr<Node> dest, E weight ) : src_{src}, dest_{dest}, weight_{weight} {}
                std::weak_ptr<Node> src_;
                std::weak_ptr<Node> dest_;
                E weight_;
            }Edge;
            
            /**
             * one possible bug, key is shared_ptr<N>
             */
            std::map< shared_ptr<N>, shared_ptr<Node>, MapNodeComparator > nodes_;
            // std::map< int, weak_ptr<Edge> > edges;

    };

}

#endif  // ASSIGNMENTS_DG_GRAPH_H_

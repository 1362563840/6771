#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout;
using std::map;
using std::set;
using std::shared_ptr;
using std::make_shared;
using std::stringstream;
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
            Graph( typename std::vector<N>::const_iterator start, typename std::vector<N>::const_iterator end ) 
            {
                for( auto it = start; it != end ; it++ ) {
                    shared_ptr<N> name = make_shared<N>(*it);
                    // possible bug ---------------------------
                    // make_shared should just call Node constructor
                    shared_ptr<Node> node = make_shared<Node>( name );
                    this->nodes_.emplace( name, node );
                }
            }
            // change ------------------------------- let clion thinks this one is good
            Graph( typename  std::vector<std::tuple<N, N, E>>::const_iterator start, typename std::vector<std::tuple<N, N, E>>::const_iterator end)
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
                    stringstream ss;
                    ss << "Cannot call Graph::InsertEdge when either src or dst node does not exist";
                    throw std::runtime_error(ss.str());
                }
                /**
                 * There are two ways to check if such edge exists
                 * one is direct way, using variables set "edges_"
                 * second way is to find src node, then use src node to find outcoming edges, then check if 
                 * such one is satisfied
                 */
                //------------------------------------------------------- First way
                // shared_ptr<N> temp_N_src_ptr = getNode( src );
                // shared_ptr<N> temp_N_dest_ptr = getNode( dest );
                // // check if this edge exists;                                   second is outcoming
                // shared_ptr<Node> temp_src_node = this->nodes_.find( temp_N_src_ptr )->second;
                // // check set "outcoming"
                // for( auto& it : temp_src_node.outcoming ) {
                //     shared_ptr<Node> edge_dest = it.dest_.lock();
                //     // after find Node "dest", need to access its name(N)
                //     shared_ptr<Node> edge_dest_name = edge_dest.name_.lock();
                //     if( *edge_dest_name == dest && it.weight_ == w ) {
                //         // found a same edge
                //         return false;
                //     }
                // }
                // // if it is new edge, insert
                // /**
                //  * need to add two sets, one is node(src).outcoming, second is node(dest).incoming
                //  * In order to achieve goal "one resource per edge", only one make_shared,
                //  */
                
                // // still use the variable "temp_src_node"
                // shared_ptr<Node> temp_dest_node = this->nodes_.find( temp_N_dest_ptr )->second;
                // shared_ptr<Edge> temp_edge = make_shared<Edge>( temp_src_node, temp_dest_node, w );
                // temp_src_node.outcoming.insert(temp_edge);
                // temp_dest_node.incoming.insert(temp_edge);
                //------------------------------------------------------- First way
                
                //------------------------------------------------------- Second way

                shared_ptr<Edge> temp_edge = makeEdge( src, dest, w );

                if( this->edges_.find(temp_edge) != this->edges_.end() ) {
                    return false;
                }
                shared_ptr<Node> temp_src_node = getNode( src );
                shared_ptr<Node> temp_dest_node = getNode( dest );

                (*temp_src_node).outcoming.insert(temp_edge);
                (*temp_dest_node).incoming.insert(temp_edge);

                
                //------------------------------------------------------- Second way

                return true;
            }

            /**
             * Question, if node does not exist, return true or false?
             */
            bool DeleteNode(const N& val)
            {
                if( IsNode( val ) == false ) {
                    return false;
                }
                shared_ptr<N> temp_N_ptr = make_shared<N>( val );
                shared_ptr<Node> temp_Node_ptr = this->nodes_.find( temp_N_ptr )->second;
                // go through outcoming, delete each edge
                // use reference
                /**
                 * Question :
                 * if use reference here, temp_Node_ptr.outcoming.erase(self) will cause error or not???
                 */
                for( auto it : (*temp_Node_ptr).outcoming ) {
                    // access dest node, then delete this edge in tis incoming
                    std::shared_ptr<Node> temp_dest = (*it).dest_.lock();
                    (*temp_dest).incoming.erase(it);

                    (*temp_Node_ptr).outcoming.erase(it);
                    this->edges_.erase(it);
                }
                // same thing for incoming, but no need to delete in variable ""
                for( auto it : (*temp_Node_ptr).incoming ) {
                    std::shared_ptr<Node> temp_src = (*it).src_.lock();
                    (*temp_src).outcoming.erase(it);

                    (*temp_Node_ptr).incoming.erase(it);
                }
                // last, delete node it self
                this->nodes_.erase(temp_N_ptr);
                return true;
            }

            bool Replace(const N& oldData, const N& newData)
            {
                if( IsNode( oldData ) == false ) {
                    stringstream ss;
                    ss << "Cannot call Graph::Replace on a node that doesn't exist";
                    throw std::runtime_error(ss.str());
                }
                if( IsNode( newData ) == true ) {
                    return false;
                }
                shared_ptr<N> temp_N_ptr = make_shared<N>( newData );
                auto temp_node_handler = this->nodes_.extract(temp_N_ptr);
                shared_ptr<N> temp_value_name = ( *(temp_node_handler.value()) ).name_.lock();
                *temp_value_name = newData;
                shared_ptr<N> temp_key_name = temp_node_handler.key();
                *temp_key_name = newData;
                
                this->nodes_.insert( std::move(temp_node_handler) );
                return true;
            }

            bool IsNode(const N& val)
            {
                shared_ptr<N> temp_N_ptr = make_shared<N>( val );
                if( this->nodes_.find(temp_N_ptr) != this->nodes_.end() ) {
                    return true;
                }
                return false;
            }
            
            friend std::ostream& operator<<(std::ostream& out, const gdwg::Graph<N, E>& graph)
            {
                // can not initialize this one, since the initial value may be same with node.name_
                N last;
                int first = 0;
                // go through variable "edges_" since it is already sorted 
                for( auto& edge : graph.edges_ ) {
                    shared_ptr<Edge> temp_edge = edge.lock();
                    shared_ptr<Node> temp_src_node = (*temp_edge).src_.lock();
                    shared_ptr<N> temp_src_node_name = (*temp_src_node).name_.lock();

                    shared_ptr<Node> temp_dest_node = (*temp_edge).dest_.lock();
                    shared_ptr<N> temp_dest_node_name = (*temp_dest_node).name_.lock();

                    if( first == 0 ) {
                        out << *temp_src_node_name << " (" << "\n";
                        out << "  " << *temp_dest_node_name << " | " << (*temp_edge).weight_ << "\n";
                        last = *temp_src_node_name;
                        first++;
                    }else {
                        // new src node
                        if( last != *temp_src_node_name ) {
                            out << ")" << "\n";
                            out << *temp_src_node_name << " (" << "\n";
                            out << "  " << *temp_dest_node_name << " | " << (*temp_edge).weight_ << "\n";
                            last = *temp_src_node_name;
                        }else {
                            out << "  " << *temp_dest_node_name << " | " << (*temp_edge).weight_ << "\n";
                        }
                    }
                }
                out << "\n";
                return out;
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

            struct EdgeComparator_shared
            {
                bool operator()(const shared_ptr<Edge>& _lhs, const shared_ptr<Edge>& _rhs) const {
                    // debug --------------------------------------
                    if( (*_lhs).dest_.expired() == true || (*_rhs).dest_.expired() == true ){
                        std::cout << "expire impossible\n";
                        std::exit(1);
                    }
                    // debug --------------------------------------
                    shared_ptr<Node> lhs_src = (*_lhs).src_.lock();
                    shared_ptr<Node> rhs_src = (*_rhs).src_.lock();

                    shared_ptr<N> lhs_src_name = (*lhs_src).name_.lock();
                    shared_ptr<N> rhs_src_name = (*rhs_src).name_.lock();

                    shared_ptr<Node> lhs_dest = (*_lhs).dest_.lock();
                    shared_ptr<Node> rhs_dest = (*_rhs).dest_.lock();
                    
                    shared_ptr<N> lhs_dest_name = (*lhs_dest).name_.lock();
                    shared_ptr<N> rhs_dest_name = (*rhs_dest).name_.lock();

                    return ( *lhs_src_name < *lhs_src_name ) ||
                            ( ( *lhs_src_name == *lhs_src_name ) && ( *lhs_dest_name < *rhs_dest_name ) ) ||
                            ( ( *lhs_src_name == *lhs_src_name ) && ( *lhs_dest_name == *rhs_dest_name ) && ( (*_lhs).weight < (*_rhs).weight ) )
                    ;
                }
            };
            /**
             * Be careful, parameter starts with two underlying
             */
            struct EdgeComparator_weak
            {
                bool operator()(const weak_ptr<Edge>& __lhs, const weak_ptr<Edge>& __rhs) const {
                    shared_ptr<Edge> _lhs = __lhs.lock();
                    shared_ptr<Edge> _rhs = __rhs.lock();
                    // debug --------------------------------------
                    if( (*_lhs).dest_.expired() == true || (*_rhs).dest_.expired() == true ) {
                        std::cout << "expire impossible\n";
                        std::exit(1);
                    }
                    // debug --------------------------------------
                    shared_ptr<Node> lhs_src = (*_lhs).src_.lock();
                    shared_ptr<Node> rhs_src = (*_rhs).src_.lock();

                    shared_ptr<N> lhs_src_name = (*lhs_src).name_.lock();
                    shared_ptr<N> rhs_src_name = (*rhs_src).name_.lock();

                    shared_ptr<Node> lhs_dest = (*_lhs).dest_.lock();
                    shared_ptr<Node> rhs_dest = (*_rhs).dest_.lock();
                    
                    shared_ptr<N> lhs_dest_name = (*lhs_dest).name_.lock();
                    shared_ptr<N> rhs_dest_name = (*rhs_dest).name_.lock();

                    return ( *lhs_src_name < *lhs_src_name ) ||
                            ( ( *lhs_src_name == *lhs_src_name ) && ( *lhs_dest_name < *rhs_dest_name ) ) ||
                            ( ( *lhs_src_name == *lhs_src_name ) && ( *lhs_dest_name == *rhs_dest_name ) && ( (*_lhs).weight < (*_rhs).weight ) )
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
                std::set< std::shared_ptr<Edge>, EdgeComparator_shared > outcoming;
                std::set< std::shared_ptr<Edge>, EdgeComparator_shared > incoming;
            }Node;

            typedef struct Edge
            {
                Edge( shared_ptr<Node> src, shared_ptr<Node> dest, E weight ) : src_{src}, dest_{dest}, weight_{weight} {}
                std::weak_ptr<Node> src_;
                std::weak_ptr<Node> dest_;
                E weight_;
            }Edge;

            /**
             * assume node exists
             * 
             * you can not declare it as public, since struct Node is encapsulated
             * diff wtih getNodes()
             */
            shared_ptr<Node> getNode( const N& val )
            {
                shared_ptr<N> temp_N_ptr = make_shared<N>( val );
                shared_ptr<Node> temp_Node_ptr = this->node_.find(temp_N_ptr)->second;
                return temp_Node_ptr;
            }

            // shared_ptr<Node> makeNode( const N& val )
            // {
            //     shared_ptr<N> temp_N_ptr = make_shared<N>( val );
            //     shared_ptr<Node> new_Node = make_shared<Node>( temp_N_ptr );
            //     return new_Node;
            // }

            shared_ptr<Edge> makeEdge( const N& src, const N& dest, const E& w ) 
            {
                shared_ptr<Node> temp_src = getNode(src);
                shared_ptr<Node> temp_dest = getNode(dest);

                shared_ptr<Edge> temp_edge = make_shared<Edge>( temp_src, temp_dest, w );
                return temp_edge;
            }
            /**
             * one possible bug, key is shared_ptr<N>
             */
            std::map< shared_ptr<N>, shared_ptr<Node>, MapNodeComparator > nodes_;
            std::set< weak_ptr<Edge>, EdgeComparator_weak > edges_;

    };

}

#endif  // ASSIGNMENTS_DG_GRAPH_H_

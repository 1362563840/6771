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
#include <tuple>
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

            /**
             * Question, do we assume it is valid node, or possible duplicated node
             */
            Graph( typename std::vector<N>::const_iterator _start, typename std::vector<N>::const_iterator _end ) 
            {
                for( auto it = _start; it != _end ; it++ ) {
                    // Because InsertNode() already check if such node exists or not
                    InsertNode( *it );
                }
            }

            /**
             * if node does not exist, create
             * 
             * Question : Assume, edges are valid, but it should not matter, if smae edge, the set should remove duplicate based
             * on the custom comparator
             */
            Graph( typename  std::vector<std::tuple<N, N, E>>::const_iterator _start, typename std::vector<std::tuple<N, N, E>>::const_iterator _end)
            {
                for( auto it = _start ; it != _end ; it++ ) {
                    // Because InsertNode() already check if such node exists or not
                    N temp_src_name = std::get<0>(*it);
                    InsertNode( temp_src_name );

                    N temp_dest_name = std::get<1>(*it);
                    InsertNode( temp_dest_name );

                    InsertEdge( temp_src_name, temp_dest_name, std::get<2>(*it) );
                }
            }

            /**
             * 
             */
            Graph( typename std::initializer_list<N> _list ) 
            {
                for( auto it : _list ) {
                    // Because InsertNode() already check if such node exists or not
                    InsertNode( it );
                }
            }

            /**
             * in order to save time avoind adding duplicate edge to this->edges_(despite that set will automatically remove duplicate)
             * We only add outcoming_ edges to new Graph->edges_
             */
            Graph( typename gdwg::Graph<N,E>& _graph )
            {
                // first, copy all nodes ignoring edges into new graph(this)
                for( auto& it : _graph.nodes_ ) {
                                                                // "first" is key
                    shared_ptr<N> temp_N_ptr = make_shared<N>( *(it.first) );
                    shared_ptr<Node> temp_Node_ptr = make_shared<Node>( temp_N_ptr );
                    
                    this->nodes_.emplace( temp_N_ptr, temp_Node_ptr );
                }

                /**
                 * after coping node, iterator all edges, and add only one copy to corresponding nodes
                 * Reason : if iterating nodes, when you assign this node a edge as src, then for edge, the dest
                 * also need this copy, but you have to go through src again in order to make a pointer referencing it 
                 * */ 
                for( auto& it : _graph.edges_ ) {
                    shared_ptr<Edge> new_edge = copyEdge( it.lock() );
                    
                    shared_ptr<Node> temp_src_node = getNodePassedByNode( (*new_edge).src_.lock() );
                    shared_ptr<Node> temp_dest_node = getNodePassedByNode( (*new_edge).dest_.lock() );

                    (*temp_src_node).outcoming_.insert(new_edge);
                    (*temp_dest_node).incoming_.insert(new_edge);
                    this->edges_.insert(new_edge);
                }
            }
            
            Graph( typename gdwg::Graph<N,E>&& _another ) : nodes_{_another.nodes_}, 
                                    edges_{_another.edges_} {}


            class const_iterator {
                // const_iterator find(const N&, const N&, const E&);
            };

            /**
             * Question, Do we need to check if it is node
             */
            bool InsertNode(const N& _val)
            {
                if( IsNode( _val ) == true ) {
                    return false;
                }
                shared_ptr<N> temp_N_ptr = make_shared<N>( _val );
                shared_ptr<Node> temp_Node_ptr = make_shared<Node>( temp_N_ptr );
                /**
                 * possible bud, check how emplace() works, for name_ptr, should just exist one which is in map
                 */
                this->nodes_.emplace( temp_N_ptr, temp_Node_ptr );
                return true;
            }
            
            bool InsertEdge(const N& _src, const N& _dest, const E& _w)
            {
                // check if either one node does not exist
                if(  IsNode( _src ) == false ||  IsNode( _dest ) == false ) {
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
                
                //------------------------------------------------------- Second way

                shared_ptr<Edge> temp_edge = makeEdge( _src, _dest, _w );

                if( this->edges_.find(temp_edge) != this->edges_.end() ) {
                    return false;
                }
                shared_ptr<Node> temp_src_node = getNode( _src );
                shared_ptr<Node> temp_dest_node = getNode( _dest );

                (*temp_src_node).outcoming_.insert(temp_edge);
                (*temp_dest_node).incoming_.insert(temp_edge);

                this->edges_.insert(temp_edge);
                
                //------------------------------------------------------- Second way

                return true;
            }

            /**
             * Question, if node does not exist, return true or false?
             */
            bool DeleteNode(const N& _val)
            {
                if( IsNode( _val ) == false ) {
                    return false;
                }
                shared_ptr<N> temp_N_ptr = make_shared<N>( _val );
                shared_ptr<Node> temp_Node_ptr = this->nodes_.find( temp_N_ptr )->second;
                // go through outcoming, delete each edge
                // use reference
                /**
                 * Question :
                 * if use reference here, temp_Node_ptr.outcoming.erase(self) will cause error or not???
                 */
                for( auto it : (*temp_Node_ptr).outcoming_ ) {
                    // access dest node, then delete this edge in tis incoming
                    std::shared_ptr<Node> temp_dest = (*it).dest_.lock();
                    (*temp_dest).incoming_.erase(it);

                    (*temp_Node_ptr).outcoming_.erase(it);
                    this->edges_.erase(it);
                }
                // same thing for incoming_, but no need to delete in variable ""
                for( auto& it : (*temp_Node_ptr).incoming_ ) {
                    std::shared_ptr<Node> temp_src = (*it).src_.lock();
                    (*temp_src).outcoming_.erase(it);

                    (*temp_Node_ptr).incoming_.erase(it);
                }
                // last, delete node it self
                this->nodes_.erase(temp_N_ptr);
                return true;
            }

            bool Replace(const N& _oldData, const N& _newData)
            {
                if( IsNode( _oldData ) == false ) {
                    stringstream ss;
                    ss << "Cannot call Graph::Replace on a node that doesn't exist";
                    throw std::runtime_error(ss.str());
                }
                if( IsNode( _newData ) == true ) {
                    return false;
                }
                shared_ptr<N> temp_N_ptr = make_shared<N>( _newData );
                auto temp_node_handler = this->nodes_.extract(temp_N_ptr);
                shared_ptr<N> temp_value_name = ( *(temp_node_handler.value()) ).name_.lock();
                *temp_value_name = _newData;
                shared_ptr<N> temp_key_name = temp_node_handler.key();
                *temp_key_name = _newData;
                
                this->nodes_.insert( std::move(temp_node_handler) );
                return true;
            }

            bool IsNode(const N& _val)
            {
                shared_ptr<N> temp_N_ptr = make_shared<N>( _val );
                if( this->nodes_.find(temp_N_ptr) != this->nodes_.end() ) {
                    return true;
                }
                return false;
            }
            
            /**
             * go through each node and print out info
             */
            friend std::ostream& operator<<(std::ostream& _out, const gdwg::Graph<N, E>& _graph)
            {   
                // --------------------------------------------------
                for( auto& it : _graph.nodes_ ) {
                    shared_ptr<Node> temp_src_node = it.second;
                    shared_ptr<N> temp_src_node_name = (*temp_src_node).name_.lock();

                    _out << *temp_src_node_name << " (" << "\n";

                    for( auto& each_edge : (*temp_src_node).outcoming_ ) {
                        shared_ptr<Node> temp_dest_node = (*each_edge).dest_.lock();
                        shared_ptr<N> temp_dest_node_name = (*temp_dest_node).name_.lock();

                        _out << "  " << *temp_dest_node_name << " | " << (*each_edge).weight_ << "\n";                        

                    }
                    _out << ")" << "\n";

                }
                _out << "\n";
                return _out;
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

                    return ( *lhs_src_name < *rhs_src_name ) ||
                            ( ( *lhs_src_name == *rhs_src_name ) && ( *lhs_dest_name < *rhs_dest_name ) ) ||
                            ( ( *lhs_src_name == *rhs_src_name ) && ( *lhs_dest_name == *rhs_dest_name ) && ( (*_lhs).weight_ < (*_rhs).weight_ ) )
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

                    return ( *lhs_src_name < *rhs_src_name ) ||
                            ( ( *lhs_src_name == *rhs_src_name ) && ( *lhs_dest_name < *rhs_dest_name ) ) ||
                            ( ( *lhs_src_name == *rhs_src_name ) && ( *lhs_dest_name == *rhs_dest_name ) && ( (*_lhs).weight_ < (*_rhs).weight_ ) )
                    ;
                }
            };

            /**
             * Attention for Node constructor
             * The shared_ptr must be passed by reference, because this->nodes_.key() is created first before value
             * and key() is shared_ptr
             */
            typedef struct Node
            {
                Node( const shared_ptr<N>& _name ) : name_{_name} {}
                std::weak_ptr<N> name_;
                // std::set< std::shared_ptr<Edge>, []( const shared_ptr<Edge> lhs, const shared_ptr<Edge> rhs ) {
                //                                         return (
                //                                             ( lhs.dest < rhs.dest ) ||
                //                                             ( ( lhs.dest == rhs.dest ) && ( lhs.weight < rhs.weight ) )
                //                                         );
                //                                             }
                // > outcoming;
                std::set< std::shared_ptr<Edge>, EdgeComparator_shared > outcoming_;
                std::set< std::shared_ptr<Edge>, EdgeComparator_shared > incoming_;
            }Node;

            /**
             * same reason for Edge from Node
             */
            typedef struct Edge
            {
                Edge( const shared_ptr<Node>& _src, const shared_ptr<Node>& _dest, const E& _weight ) : src_{_src}, dest_{_dest}, weight_{_weight} {}
                std::weak_ptr<Node> src_;
                std::weak_ptr<Node> dest_;
                E weight_;
            }Edge;

            /**
             * has to be that node exists
             * 
             * you can not declare it as public, since struct Node is encapsulated
             * diff wtih getNodes()
             */
            shared_ptr<Node>& getNode( const N& _val )
            {
                shared_ptr<N> temp_N_ptr = make_shared<N>( _val );
                // debug test ---------------------------- delete
                if( this->nodes_.find(temp_N_ptr) == this->nodes_.end() ) {
                    throw std::runtime_error("impossible, src node does not exist");
                }
                // debug test ---------------------------- delete
                return this->nodes_.find(temp_N_ptr)->second;
            }   
            
            /**
             * 
             * Node must exist
             * 
             * when you pass a node, extract the variable "name_", 
             * use this info to find existing node in current graph
             */
            shared_ptr<Node>& getNodePassedByNode( const shared_ptr<Node>& _node )
            {
                shared_ptr<N> temp_node = (*_node).name_.lock();
                // debug test ---------------------------- delete
                if( this->nodes_.find( temp_node ) == this->nodes_.end() ) {
                    throw std::runtime_error("impossible, src node does not exist");
                }
                // debug test ---------------------------- delete
                return this->nodes_.find( temp_node )->second;
            }

            // shared_ptr<Node> makeNode( const N& val )
            // {
            //     shared_ptr<N> temp_N_ptr = make_shared<N>( val );
            //     shared_ptr<Node> new_Node = make_shared<Node>( temp_N_ptr );
            //     return new_Node;
            // }

            /**
             * here the reason to pass reference is to save time
             * in function body, the edge is created by make_shared<>
             */
            shared_ptr<Edge> makeEdge( const N& _src, const N& _dest, const E& _w ) 
            {
                shared_ptr<Node> temp_src = getNode(_src);
                shared_ptr<Node> temp_dest = getNode(_dest);

                shared_ptr<Edge> temp_edge = make_shared<Edge>( temp_src, temp_dest, _w );
                return temp_edge;
            }

            /**
             * Be careful, here, the node you find in new graph should be returned by reference instead of value
             * Reason : before coping edges, node already created, so you have to pass reference to edges,
             * to let the member in edges to pointing the same resource i.e. nodes
             * 
             * the nodes needed for this function should already exists
             * becuase we are just coping an existing edge, so just pass const refernece to save time
             */
            shared_ptr<Edge> copyEdge( const shared_ptr<Edge>& _edge )
            {
                
                shared_ptr<Node> existing_src_node = getNodePassedByNode( (*_edge).src_.lock() );

                shared_ptr<Node> existing_dest_node = getNodePassedByNode( (*_edge).dest_.lock() );

                shared_ptr<Edge> edge = make_shared<Edge>( existing_src_node,
                                                            existing_dest_node,
                                                            (*_edge).weight_
                                                        );
                
                return edge;
            }
            /**
             * one possible bug, key is shared_ptr<N>
             */
            std::map< shared_ptr<N>, shared_ptr<Node>, MapNodeComparator > nodes_;
            std::set< weak_ptr<Edge>, EdgeComparator_weak > edges_;

    };

}

#endif  // ASSIGNMENTS_DG_GRAPH_H_

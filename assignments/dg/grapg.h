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

            Graph(std::vector<std::tuple<N, N, E>>::const_iterator start, std::vector<std::tuple<N, N, E>>::const_iterator end)
            {
                for( auto it = start ; it != end ; it++ ) {
                    
                }
            }
            class const_iterator {};

            bool InsertNode(const N& val)
            {
                shared_ptr<N> temp_N_ptr = make_shared<N>( val );
                if( this->nodes_.find(temp_N_ptr) != this->nodes_.end() ) {
                    return false;
                }
                shared_ptr<Node> temp_Node_ptr = make_shared<Node>( temp_N_ptr );
                /**
                 * possible bud, check how emplace() works, for name_ptr, should just exist one which is in map
                 */
                this->nodes.emplace( temp_N_ptr, temp_Node_ptr );
                return true;
            }
            
            bool InsertEdge(const N& src, const N& dst, const E& w)
            {
                shared_ptr<N> temp_src_node = make_shared<N>( src );
                shared_ptr<N> temp_dest_node = make_shared<N>( dst );
                if( this->nodes_.find(temp_src_node) != this->nodes_.end() || this->nodes_.find(temp_dest_node) != this->nodes_.end() ) {
                    return false;
                }
                
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
                    if( _lhs.dest.expired() == true || _rhs.dest.expired() == true ) {
                        std::cout << "expire impossible\n";
                        std::exit(1);
                    }
                    // debug --------------------------------------
                    shared_ptr<Node> lhs = _lhs.dest.lock();
                    shared_ptr<Node> rhs = _rhs.dest.lock();
                    
                    shared_ptr<N> lhs_name = lhs.name_.lock();
                    shared_ptr<N> rhs_name = rhs.name_.lock();
                    return ( lhs_name < rhs_name ) ||
                            ( ( lhs_name == rhs_name ) && ( _lhs.weight_ < _rhs.weight_ ) )
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
                std::weak_ptr<Node> src;
                std::weak_ptr<Node> dest;
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

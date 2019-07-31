#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

using std::cout;
using std::make_shared;
using std::map;
using std::set;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unordered_set;
using std::vector;
using std::weak_ptr;

namespace gdwg {

template <typename N, typename E> class Graph {
private:
  struct EdgeComparator_shared;
  struct Edge;
  struct Node;

public:
  class const_iterator;
  class const_reverse_iterator;
  Graph() = default;

  /**
   * Question, do we assume it is valid node, or possible duplicated node
   */
  Graph(typename std::vector<N>::const_iterator _start,
        typename std::vector<N>::const_iterator _end) {
    for (auto it = _start; it != _end; it++) {
      // Because InsertNode() already check if such node exists or not
      this->InsertNode(*it);
    }
  }

  /**
   * if node does not exist, create
   *
   * Question : Assume, edges are valid, but it should not matter, if smae edge,
   * the set should remove duplicate based on the custom comparator
   */
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator _start,
        typename std::vector<std::tuple<N, N, E>>::const_iterator _end) {
    for (auto it = _start; it != _end; it++) {
      // Because InsertNode() already check if such node exists or not
      N temp_src_name = std::get<0>(*it);
      this->InsertNode(temp_src_name);

      N temp_dest_name = std::get<1>(*it);
      this->InsertNode(temp_dest_name);

      this->InsertEdge(temp_src_name, temp_dest_name, std::get<2>(*it));
    }
  }

  /**
   *
   */
  Graph(const typename std::initializer_list<N> _list) {
    for (auto it : _list) {
      // Because InsertNode() already check if such node exists or not
      this->InsertNode(it);
    }
  }

  /**
   * in order to save time avoind adding duplicate edge to this->edges_(despite
   * that set will automatically remove duplicate) We only add outcoming_ edges
   * to new Graph->edges_
   */
  Graph(const typename gdwg::Graph<N, E> &_graph) {
    // first, copy all nodes ignoring edges into new graph(this)
    for (auto &it : _graph.nodes_) {
      // "first" is key
      shared_ptr<N> temp_N_ptr = make_shared<N>(*(it.first));
      shared_ptr<Node> temp_Node_ptr = make_shared<Node>(temp_N_ptr);

      this->nodes_.emplace(temp_N_ptr, temp_Node_ptr);
    }

    /**
     * after coping node, iterator all edges, and add only one copy to
     * corresponding nodes Reason : if iterating nodes, when you assign this
     * node a edge as src, then for edge, the dest also need this copy, but you
     * have to go through src again in order to make a pointer referencing it
     * */
    for (auto &it : _graph.edges_) {
      shared_ptr<Edge> new_edge = copyEdge(it.lock());

      shared_ptr<Node> temp_src_node =
          getNodePassedByNode((*new_edge).src_.lock());
      shared_ptr<Node> temp_dest_node =
          getNodePassedByNode((*new_edge).dest_.lock());

      (*temp_src_node).outcoming_.insert(new_edge);
      (*temp_dest_node).incoming_.insert(new_edge);
      this->edges_.insert(new_edge);
    }
  }

  Graph(const typename gdwg::Graph<N, E> &&_another) noexcept
      : nodes_{_another.nodes_}, edges_{_another.edges_} {}

  ~Graph() = default;

  Graph &operator=(const typename gdwg::Graph<N, E> &_rhs) {
    // std::cout << "copy assignment\n";
    if (this == &_rhs) {
      return *this;
    }
    Graph<N, E> temp{_rhs};
    this->nodes_ = std::move(temp.nodes_);
    this->edges_ = std::move(temp.edges_);
    return *this;
  }

  Graph<N, E> &operator=(gdwg::Graph<N, E> &&_rhs) noexcept {
    // std::cout << "move assignment\n";
    this->nodes_ = std::move(_rhs.nodes_);
    this->edges_ = std::move(_rhs.edges_);
    _rhs.nodes_.clear();
    _rhs.edges.clear();
    return *this;
  }

  /**
   * Question, Do we need to check if it is node
   */
  bool InsertNode(const N &_val) {
    if (this->IsNode(_val) == true) {
      return false;
    }
    shared_ptr<N> temp_N_ptr = make_shared<N>(_val);
    shared_ptr<Node> temp_Node_ptr = make_shared<Node>(temp_N_ptr);
    /**
     * possible bud, check how emplace() works, for name_ptr, should just exist
     * one which is in map
     */
    this->nodes_.emplace(temp_N_ptr, temp_Node_ptr);
    return true;
  }

  bool InsertEdge(const N &_src, const N &_dest, const E &_w) {
    // check if either one node does not exist
    if (this->IsNode(_src) == false || this->IsNode(_dest) == false) {
      stringstream ss;
      ss << "Cannot call Graph::InsertEdge when either src or dst node does "
            "not exist";
      throw std::runtime_error(ss.str());
    }
    /**
     * There are two ways to check if such edge exists
     * one is direct way, using variables set "edges_"
     * second way is to find src node, then use src node to find outcoming
     * edges, then check if such one is satisfied
     */

    //------------------------------------------------------- Second way

    shared_ptr<Edge> temp_edge = makeEdge(_src, _dest, _w);

    if (this->edges_.find(temp_edge) != this->edges_.end()) {
      return false;
    }
    shared_ptr<Node> temp_src_node = this->getNode(_src);
    shared_ptr<Node> temp_dest_node = this->getNode(_dest);

    (*temp_src_node).outcoming_.insert(temp_edge);
    (*temp_dest_node).incoming_.insert(temp_edge);

    this->edges_.insert(temp_edge);

    //------------------------------------------------------- Second way

    return true;
  }

  /**
   * Question, if node does not exist, return true or false?
   */
  bool DeleteNode(const N &_val) {
    if (this->IsNode(_val) == false) {
      return false;
    }
    shared_ptr<N> temp_N_ptr = make_shared<N>(_val);
    shared_ptr<Node> temp_target_node = this->nodes_.find(temp_N_ptr)->second;
    // go through outcoming, delete each edge
    // use reference
    /**
     * Warning : when you iterator a set, you can not just delete element
     * pointed by iteator in this set Question : if use reference here,
     * temp_Node_ptr.outcoming.erase(self) will cause error or not???
     */
    std::set<shared_ptr<Edge>, EdgeComparator_shared> temp_back_up_outcoming;
    for (auto it : (*temp_target_node).outcoming_) {
      // access dest node, then delete this edge in tis incoming
      shared_ptr<Node> temp_dest_node = (*it).dest_.lock();
      // std::cout << "still have " <<
      // (*((*temp_dest_node).incoming_.find(it))).use_count() << std::endl;
      (*temp_dest_node).incoming_.erase(it);
      // std::cout << "still have " <<
      // (*((*temp_target_node).outcoming_.find(it))).use_count() << std::endl;
      // (*temp_target_node).outcoming_.erase(it);
      temp_back_up_outcoming.insert(it);
      // debug test -------------------------------- delete
      // if( (*(this->edges_.find(it))).use_count() != 1 ) {
      //     std::cout << "still have " <<
      //     (*(this->edges_.find(it))).use_count() << std::endl; throw
      //     std::runtime_error("impossible, delete fail");
      // }
      // debug test -------------------------------- delete
      this->edges_.erase(it);
    }
    /**
     * Possible bug, use reference or copy
     */
    for (auto &it : temp_back_up_outcoming) {
      (*temp_target_node).outcoming_.erase(it);
    }
    temp_back_up_outcoming.clear();
    // same thing for incoming_, but no need to delete in variable ""
    for (auto it : (*temp_target_node).incoming_) {
      shared_ptr<Node> temp_src_node = (*it).src_.lock();
      (*temp_src_node).outcoming_.erase(it);
      temp_back_up_outcoming.insert(it);
      // debug test -------------------------------- delete
      // because variable "it" is also shared_ptr
      // if( (*(this->edges_.find(it))).use_count() != 1 ) {
      //     std::cout << "still have " <<
      //     (*(this->edges_.find(it))).use_count() << std::endl; throw
      //     std::runtime_error("impossible, delete fail");
      // }
      // debug test -------------------------------- delete
      this->edges_.erase(it);
    }
    for (auto &it : temp_back_up_outcoming) {
      (*temp_target_node).incoming_.erase(it);
    }
    // last, delete node it self
    this->nodes_.erase(temp_N_ptr);
    return true;
  }

  /**
   * Because if I change the value, I need to resort, but cpp does not provide
   * such resort so i have to record all edge corresponding this node, and
   * reinsert again, no other way Question, replace?? just change the node
   * name???
   */
  bool Replace(const N &_oldData, const N &_newData) {
    if (this->IsNode(_oldData) == false) {
      stringstream ss;
      ss << "Cannot call Graph::Replace on a node that doesn't exist";
      throw std::runtime_error(ss.str());
    }
    if (this->IsNode(_newData) == true) {
      return false;
    }
    // shared_ptr<N> temp_old_N_ptr = make_shared<N>( _oldData );
    // shared_ptr<Node> temp_old_node = this->nodes_.find( temp_old_N_ptr
    // )->second;

    shared_ptr<Node> temp_old_node = this->getNode(_oldData);

    set<shared_ptr<Edge>> backup_outcoming;
    set<shared_ptr<Edge>> backup_incoming;
    /**
     * Question, &it or it , possible bug
     */
    for (auto &it : (*temp_old_node).outcoming_) {
      backup_outcoming.insert(it);
    }
    for (auto &it : (*temp_old_node).incoming_) {
      backup_incoming.insert(it);
    }

    this->DeleteNode(_oldData);
    this->InsertNode(_newData);
    for (auto &it : backup_outcoming) {
      this->InsertEdgeWithNewSrc(it, _newData);
    }

    for (auto &it : backup_incoming) {
      this->InsertEdgeWithNewDest(it, _newData);
    }

    return true;
  }

  void MergeReplace(const N &_oldData, const N &_newData) {
    if (this->IsNode(_oldData) == false) {
      stringstream ss;
      ss << "Cannot call Graph::MergeReplace on old or new data if they don't "
            "exist in the graph";
      throw std::runtime_error(ss.str());
    }
    if (this->IsNode(_newData) == false) {
      stringstream ss;
      ss << "Cannot call Graph::MergeReplace on old or new data if they don't "
            "exist in the graph";
      throw std::runtime_error(ss.str());
    }

    // shared_ptr<N> temp_old_N_ptr = make_shared<N>( _oldData );
    // shared_ptr<Node> temp_old_node = this->nodes_.find( temp_old_N_ptr
    // )->second;

    shared_ptr<Node> temp_old_node = this->getNode(_oldData);

    set<shared_ptr<Edge>> backup_outcoming;
    set<shared_ptr<Edge>> backup_incoming;
    /**
     * Question, &it or it , possible bug
     */
    for (auto &it : (*temp_old_node).outcoming_) {
      backup_outcoming.insert(it);
    }
    for (auto &it : (*temp_old_node).incoming_) {
      backup_incoming.insert(it);
    }

    this->DeleteNode(_oldData);
    for (auto &it : backup_outcoming) {
      this->InsertEdgeWithNewSrc(it, _newData);
    }

    for (auto &it : backup_incoming) {
      this->InsertEdgeWithNewDest(it, _newData);
    }
  }

  void Clear() {
    this->nodes_.clear();
    this->edges_.clear();
  }

  bool IsNode(const N &_val) const {
    shared_ptr<N> temp_N_ptr = make_shared<N>(_val);
    if (this->nodes_.find(temp_N_ptr) != this->nodes_.end()) {
      return true;
    }
    return false;
  }

  bool IsConnected(const N &_src, const N &_dest) const {
    if (this->IsNode(_src) == false) {
      stringstream ss;
      ss << "Cannot call Graph::IsConnected if src or dst node don't exist in "
            "the graph";
      throw std::runtime_error(ss.str());
    }
    if (this->IsNode(_dest) == false) {
      stringstream ss;
      ss << "If either node cannot be found in the graph";
      throw std::runtime_error(ss.str());
    }

    const shared_ptr<const Node> temp_src_node = this->getNode(_src);
    for (auto &it : (*temp_src_node).outcoming_) {
      const shared_ptr<const N> temp_dest_node_name =
          this->get_dest_N_ptr_from_edge(it);
      if (*temp_dest_node_name == _dest) {
        return true;
      }
    }
    return false;
  }

  std::vector<N> GetNodes() const {
    vector<N> result;
    for (auto &it : this->nodes_) {
      result.push_back(*(it.first));
    }
    return result;
  }

  std::vector<N> GetConnected(const N &_src) const {
    if (this->IsNode(_src) == false) {
      stringstream ss;
      ss << "Cannot call Graph::GetConnected if src doesn't exist in the graph";
      throw std::runtime_error(ss.str());
    }
    set<N> temp;
    const shared_ptr<const Node> temp_src_node = this->getNode(_src);
    for (auto &it : (*temp_src_node).outcoming_) {
      const shared_ptr<const N> temp_dest_node_name =
          this->get_dest_N_ptr_from_edge(it);
      temp.insert(*temp_dest_node_name);
    }
    vector<N> result;
    std::copy(temp.cbegin(), temp.cend(), std::back_inserter(result));
    return result;
  }

  std::vector<E> GetWeights(const N &_src, const N &_dest) const {
    if (this->IsNode(_src) == false) {
      stringstream ss;
      ss << "Cannot call Graph::GetWeights if src or dst node don't exist in "
            "the graph";
      throw std::runtime_error(ss.str());
    }
    vector<E> result;
    const shared_ptr<const Node> temp_src_node = this->getNode(_src);
    for (auto &it : (*temp_src_node).outcoming_) {
      const shared_ptr<const N> temp_dest_node_name =
          this->get_dest_N_ptr_from_edge(it);
      if (*temp_dest_node_name == _dest) {
        result.push_back((*it).weight_);
      }
    }
    return result;
  }
  /**
   * go through each node and print out info
   */
  friend std::ostream &operator<<(std::ostream &_out,
                                  const gdwg::Graph<N, E> &_graph) {
    // --------------------------------------------------
    for (auto &it : _graph.nodes_) {
      shared_ptr<Node> temp_src_node = it.second;
      shared_ptr<N> temp_src_node_name = (*temp_src_node).name_.lock();

      _out << *temp_src_node_name << " ("
           << "\n";

      for (auto &each_edge : (*temp_src_node).outcoming_) {
        const shared_ptr<const N> temp_dest_node_name =
            _graph.get_dest_N_ptr_from_edge(each_edge);

        _out << "  " << *temp_dest_node_name << " | " << (*each_edge).weight_
             << "\n";
      }
      _out << ")"
           << "\n";
    }
    return _out;
  }

  void check() const {
    for (auto &it : this->nodes_) {
      std::cout << "N size is " << it.first.use_count() << std::endl;
      std::cout << "Node size is " << it.second.use_count() << std::endl;
    }

    for (auto &it : this->edges_) {
      std::cout << "edge size is " << it.use_count() << std::endl;
    }
  }

  void display(gdwg::Graph<N, E>::const_iterator &_curr) const {
    shared_ptr<Node> temp_src = (*_curr).lock()->src_.lock();
    shared_ptr<N> temp_src_name = temp_src->name_.lock();
    shared_ptr<Node> temp_dest = (*_curr).lock()->dest_.lock();
    shared_ptr<N> temp_destc_name = temp_dest->name_.lock();

    std::cout << "src name " << *temp_src_name << " dest name "
              << *temp_destc_name << " weight_ " << (*_curr).lock()->weight_
              << "\n";
  }

  void display(gdwg::Graph<N, E>::const_reverse_iterator &_curr) const {
    shared_ptr<Node> temp_src = (*_curr).lock()->src_.lock();
    shared_ptr<N> temp_src_name = temp_src->name_.lock();
    shared_ptr<Node> temp_dest = (*_curr).lock()->dest_.lock();
    shared_ptr<N> temp_destc_name = temp_dest->name_.lock();

    std::cout << "src name " << *temp_src_name << " dest name "
              << *temp_destc_name << " weight_ " << (*_curr).lock()->weight_
              << "\n";
  }

  /**
   * Debug function
   * check self
   * check if edges in this->nodes_ is exactly same as edges_ in this->edges_
   */
  bool selfCheck() const {
    std::cout << "This graph has " << this->nodes_.size() << " nodes."
              << " also has " << this->edges_.size() << " edges\n";

    int total_edges_num{0};
    for (auto &it : this->nodes_) {
      shared_ptr<N> temp_node_name = it.first;
      shared_ptr<Node> temp_node = it.second;

      int how_many_outcoming_edges{0};
      for (auto &each_edge : temp_node->outcoming_) {
        total_edges_num++;
        how_many_outcoming_edges++;
        const shared_ptr<const N> temp_src_node_name =
            this->get_src_N_ptr_from_edge(each_edge);
        const shared_ptr<const N> temp_dest_node_name =
            this->get_dest_N_ptr_from_edge(each_edge);

        if (IsNode(*temp_dest_node_name) == false) {
          std::cout << "this edge's dest node does not exist\n";
          std::cout << "src is " << *temp_src_node_name << " dest is "
                    << temp_dest_node_name << " weight is "
                    << each_edge->weight_ << "\n";
          exit(1);
        }
        const shared_ptr<const Node> temp_dest_node =
            this->getNode(*temp_dest_node_name);

        auto result = temp_dest_node->incoming_.find(each_edge);

        if (result == temp_dest_node->incoming_.end()) {
          std::cout << "in src node, it has an edge, but in the corresponding "
                       "dest not, it does not have \n";
          std::cout << "src is " << *temp_src_node_name << " dest is "
                    << temp_dest_node_name << " weight is "
                    << each_edge->weight_ << "\n";
          exit(1);
        }
      }

      std::cout << "This node's name is " << *temp_node_name << " . It has "
                << how_many_outcoming_edges << " outcoming edges\n";
      how_many_outcoming_edges = 0;
    }

    if (total_edges_num != this->edges_.size()) {
      std::cout << "edges number is not correct\n";
      std::cout << "the set has " << this->edges_.size() << " while nodes has "
                << total_edges_num << "\n";
    }
    return true;
  }

  /**
   * In order to increase the speed of find, we use set::find() which is binary
   * search We first create a temp edge which is a shared_ptr<Edge>. It is find
   * as long as we do not insert
   * W
   */
  gdwg::Graph<N, E>::const_iterator find(const N &_src, const N &_dest,
                                         const E &_weight) const {

    shared_ptr<Edge> temp_edge = this->makeEdge(_src, _dest, _weight);

    auto result = this->edges_.find(temp_edge);
    if (result != this->edges_.end()) {
      return gdwg::Graph<N, E>::const_iterator(*this, result,
                                               this->edges_.end());
    }
    return this->end();
  }

  bool erase(const N &_src, const N &_dest, const E &_w) {
    gdwg::Graph<N, E>::const_iterator it = this->find(_src, _dest, _w);
    /**
     * Attention, it relies on operator == in class const_iterator
     */
    if (it == this->end()) {
      return false;
    }

    //
    shared_ptr<Node> temp_src_node = this->getNode(_src);
    shared_ptr<Node> temp_dest_node = this->getNode(_dest);
    /**
     * Attention, it relies on operator *
     * You also need to delete edges in this->edges
     */
    (*temp_src_node).outcoming_.erase((*it).lock());
    (*temp_dest_node).incoming_.erase((*it).lock());
    this->edges_.erase(*it);

    return true;
  }

  gdwg::Graph<N, E>::const_iterator
  erase(gdwg::Graph<N, E>::const_iterator &_it) {
    shared_ptr<N> temp_src_node_name =
        this->get_src_N_ptr_from_edge((*_it).lock());
    shared_ptr<N> temp_dest_node_name =
        this->get_dest_N_ptr_from_edge((*_it).lock());
    auto next = ++gdwg::Graph<N, E>::const_iterator(_it);
    this->display(next);
    this->erase(*temp_src_node_name, *temp_dest_node_name,
                (*_it).lock()->weight_);
    return next;
  }

  gdwg::Graph<N, E>::const_iterator cbegin() const {
    return gdwg::Graph<N, E>::const_iterator(*this, this->edges_.begin(),
                                             this->edges_.end());
  }

  gdwg::Graph<N, E>::const_iterator cend() const {
    return gdwg::Graph<N, E>::const_iterator(*this, this->edges_.end(),
                                             this->edges_.end());
  }

  gdwg::Graph<N, E>::const_reverse_iterator crbegin() const {
    return gdwg::Graph<N, E>::const_reverse_iterator(
        *this, this->edges_.rbegin(), this->edges_.rend());
  }

  gdwg::Graph<N, E>::const_reverse_iterator crend() const {
    return gdwg::Graph<N, E>::const_reverse_iterator(*this, this->edges_.rend(),
                                                     this->edges_.rend());
  }

  gdwg::Graph<N, E>::const_iterator begin() const { return cbegin(); }

  gdwg::Graph<N, E>::const_iterator end() const { return cend(); }

  gdwg::Graph<N, E>::const_reverse_iterator rbegin() const { return crbegin(); }

  gdwg::Graph<N, E>::const_reverse_iterator rend() const { return crend(); }

  /**
   * Attention, because iterator operator == need graph operator ==
   * So it is very likely that there is a bug;
   */
  friend bool operator==(const gdwg::Graph<N, E> &_lhs,
                         const gdwg::Graph<N, E> &_rhs) {
    if (_lhs.nodes_.size() != _rhs.nodes_.size() ||
        _lhs.edges_.size() != _rhs.edges_.size()) {
      return false;
    }

    auto lhs_nodes_it = _lhs.nodes_.cbegin();
    auto rhs_nodes_it = _rhs.nodes_.cbegin();
    for (; lhs_nodes_it != _lhs.nodes_.cend(); lhs_nodes_it++, rhs_nodes_it++) {
      shared_ptr<N> temp_lhs_node_name = (*lhs_nodes_it).first;
      shared_ptr<N> temp_rhs_node_name = (*rhs_nodes_it).first;
      if (*temp_lhs_node_name != *temp_rhs_node_name) {
        return false;
      }
    }

    auto lhs_edge_it = _lhs.edges_.cbegin();
    auto rhs_edge_it = _rhs.edges_.cbegin();
    for (; lhs_edge_it != _lhs.edges_.cend(); lhs_edge_it++, rhs_edge_it++) {
      shared_ptr<N> temp_lhs_src_node_name =
          _lhs.get_src_N_ptr_from_edge((*lhs_edge_it).lock());
      // shared_ptr<Node> temp_lhs_src_node =
      // (*lhs_edge_it).lock()->src_.lock(); shared_ptr<N>
      // temp_lhs_src_node_name =  temp_lhs_src_node->name_.lock();
      shared_ptr<N> temp_rhs_src_node_name =
          _rhs.get_src_N_ptr_from_edge((*rhs_edge_it).lock());
      // shared_ptr<Node> temp_rhs_src_node =
      // (*rhs_edge_it).lock()->src_.lock(); shared_ptr<N>
      // temp_rhs_src_node_name =  temp_rhs_src_node->name_.lock();
      if (*temp_lhs_src_node_name != *temp_rhs_src_node_name ||
          (*lhs_edge_it).lock()->weight_ != (*rhs_edge_it).lock()->weight_) {
        return false;
      }

      shared_ptr<N> temp_lhs_dest_node_name =
          _lhs.get_dest_N_ptr_from_edge((*lhs_edge_it).lock());
      shared_ptr<N> temp_rhs_dest_node_name =
          _rhs.get_dest_N_ptr_from_edge((*rhs_edge_it).lock());
      // shared_ptr<Node> temp_lhs_dest_node =
      // (*lhs_edge_it).lock()->dest_.lock(); shared_ptr<N>
      // temp_lhs_dest_node_name =  temp_lhs_dest_node->name_.lock();

      // shared_ptr<Node> temp_rhs_dest_node =
      // (*rhs_edge_it).lock()->dest_.lock(); shared_ptr<N>
      // temp_rhs_dest_node_name =  temp_rhs_dest_node->name_.lock();
      if (*temp_lhs_dest_node_name != *temp_rhs_dest_node_name) {
        return false;
      }
    }
    return true;
  }

  friend bool operator!=(const gdwg::Graph<N, E> &_lhs,
                         const gdwg::Graph<N, E> &_rhs) {
    return !(_lhs == _rhs);
  }

  class const_iterator {
  public:
    typedef typename gdwg::Graph<N, E>::Edge Edge;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N &, const N &, const E &>;
    /**
     *
     */
    using pointer =
        std::tuple<const N *, const N *,
                   const E *>;   // Not strictly required, but nice to have.
    using difference_type = int; // used to calculate distance, can be negative
    // std::ptrdiff_t

    /**
     * Question, Do i pass reference or value
     */
    const_iterator(
        const typename gdwg::Graph<N, E> &_container,
        const typename std::set<weak_ptr<Edge>>::const_iterator _curr,
        const typename std::set<weak_ptr<Edge>>::const_iterator _end)
        : container_{_container}, curr_{_curr}, end_{_end}, increment_{1} {}

    const_iterator(
        const typename gdwg::Graph<N, E> &_container,
        const typename std::set<weak_ptr<Edge>>::const_iterator _curr,
        const typename std::set<weak_ptr<Edge>>::const_iterator _end,
        const int &_increment)
        : container_{_container}, curr_{_curr}, end_{_end}, increment_{
                                                                _increment} {}

    const_iterator(const const_iterator &_another)
        : container_{_another.container_}, curr_{_another.curr_},
          end_{_another.end_}, increment_{_another.increment_} {}

    reference operator*() const {
      shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());
      N &temp_src = *temp_src_node_name;

      shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      N &temp_dest = *temp_dest_node_name;

      E &temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return {temp_src, temp_dest, temp_weight};
    }

    value_type operator*() {
      /**
       *          &
       */
      shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());

      // shared_ptr<Node> temp_src_node =
      // ((*(this->curr_)).lock())->src_.lock(); shared_ptr<N>
      // temp_src_node_name = temp_src_node->name_.lock();
      shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      E temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return std::make_tuple(*temp_src_node_name, *temp_dest_node_name,
                             temp_weight);
    }
    const_iterator &operator++() {
      ++this->curr_;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator temp{*this};
      ++(*this);
      return temp;
    }

    const_iterator &operator--() {
      --this->curr_;
      return *this;
    }
    const_iterator &operator--(int) {
      const_iterator temp{*this};
      --(*this);
      return temp;
    }

    /**
     * Attention : possinle bug
     */
    pointer operator->() const { return &(this->operator*()); }

    friend bool operator==(const const_iterator &_lhs,
                           const const_iterator &_rhs) {
      /**
       * Attention :
       * a class iterator vs a class iterator
       * if they do no point same instance of object, they are not equal
       * then need to compare values;
       * Attention : possible bug
       * rely on class graph operator ==
       */
      if (&(_lhs.container_) != &(_rhs.container_)) {
        return false;
      }
      /**
       * Question : when a set has shared_ptr<custom object>, set s1 == set s2,
       * the process, Does it call Object operator == ????
       *
       *
       * Attention : possible bug
       * rely on class graph operator ==
       */
      if (_lhs.curr_ != _rhs.curr_ || _lhs.end_ != _rhs.end_) {
        return false;
      }
      return true;
    }
    friend bool operator!=(const const_iterator &_lhs,
                           const const_iterator &_rhs) {
      return !(_lhs == _rhs);
    }

  private:
    friend class Graph;
    const typename gdwg::Graph<N, E> &container_;
    typename std::set<weak_ptr<Edge>>::const_iterator curr_;
    const typename std::set<weak_ptr<Edge>>::const_iterator end_;
    int increment_;
  };

  class const_reverse_iterator {
  public:
    typedef typename gdwg::Graph<N, E>::Edge Edge;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N &, const N &, const E &>;
    using pointer =
        std::tuple<const N *, const N *,
                   const E *>;   // Not strictly required, but nice to have.
    using difference_type = int; // used to calculate distance, can be negative
    // std::ptrdiff_t

    /**
     * Question, Do i pass reference or value
     */
    const_reverse_iterator(
        const typename gdwg::Graph<N, E> &_container,
        const typename std::set<value_type>::const_reverse_iterator _curr,
        const typename std::set<value_type>::const_reverse_iterator _end)
        : container_{_container}, curr_{_curr}, end_{_end}, increment_{1} {}

    const_reverse_iterator(
        const typename gdwg::Graph<N, E &> _container,
        const typename std::set<value_type>::const_reverse_iterator _curr,
        const typename std::set<value_type>::const_reverse_iterator _end,
        const int &_increment)
        : container_{_container}, curr_{_curr}, end_{_end}, increment_{
                                                                _increment} {}

    const_reverse_iterator(const const_reverse_iterator &_another)
        : container_{_another.container_}, curr_{_another.curr_},
          end_{_another.end_}, increment_{_another.increment_} {}

    reference operator*() const {
      shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());
      N &temp_src = *temp_src_node_name;

      shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      N &temp_dest = *temp_dest_node_name;

      E &temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return {temp_src, temp_dest, temp_weight};
    }

    value_type operator*() {
      /**
       *          &
       */
      shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());

      // shared_ptr<Node> temp_src_node =
      // ((*(this->curr_)).lock())->src_.lock(); shared_ptr<N>
      // temp_src_node_name = temp_src_node->name_.lock();
      shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      E temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return std::make_tuple(*temp_src_node_name, *temp_dest_node_name,
                             temp_weight);
    }
    const_reverse_iterator &operator++() {
      ++this->curr_;
      return *this;
    }
    const_reverse_iterator operator++(int) {
      const_reverse_iterator temp{*this};
      ++(*this);
      return temp;
    }

    const_reverse_iterator &operator--() {
      --this->curr_;
      return *this;
    }
    const_reverse_iterator &operator--(int) {
      const_reverse_iterator temp{*this};
      --(*this);
      return temp;
    }

    /**
     * Attention : possinle bug
     */
    pointer operator->() const { return &(this->operator*()); }

    friend bool operator==(const const_reverse_iterator &_lhs,
                           const const_reverse_iterator &_rhs) {
      /**
       * Attention : possible bug
       * rely on class graph operator ==
       */
      if (&(_lhs.container_) != &(_rhs.container_)) {
        return false;
      }
      if (_lhs.curr_ != _rhs.curr_ || _lhs.end_ != _rhs.end_) {
        return false;
      }
      return true;
    }
    friend bool operator!=(const const_reverse_iterator &_lhs,
                           const const_reverse_iterator &_rhs) {
      return !(_lhs == _rhs);
    }

  private:
    const typename gdwg::Graph<N, E> &container_;
    typename std::set<weak_ptr<Edge>>::const_reverse_iterator curr_;
    const typename std::set<weak_ptr<Edge>>::const_reverse_iterator end_;
    int increment_;
  };

  E &getWeight(shared_ptr<Edge> _edge) const { return _edge->weight_; }

  E getWeight(shared_ptr<Edge> _edge) { return _edge->weight_; }

private:
  /**
   * need this one to let map implements correct order, instead of pointer self
   */
  struct MapNodeComparator {
    bool operator()(const shared_ptr<N> &_lhs,
                    const shared_ptr<N> &_rhs) const {
      return (*_lhs < *_rhs);
    }
  };

  // struct NodeComparator
  // {
  //     bool operator()(const shared_ptr<Node>& _lhs, const shared_ptr<Node>&
  //     _rhs) const {
  //         return _lhs.name < _rhs.name;
  //     }
  // };

  struct EdgeComparator_shared {
    bool operator()(const shared_ptr<Edge> &_lhs,
                    const shared_ptr<Edge> &_rhs) const {
      // debug --------------------------------------
      if ((*_lhs).dest_.expired() == true || (*_rhs).dest_.expired() == true) {
        std::cout << "expire impossible\n";
        std::exit(1);
      }
      // debug --------------------------------------

      const shared_ptr<const N> lhs_src_name =
          get_src_N_ptr_from_edge_static(_lhs);
      const shared_ptr<const N> rhs_src_name =
          get_src_N_ptr_from_edge_static(_rhs);

      const shared_ptr<const N> lhs_dest_name =
          get_dest_N_ptr_from_edge_static(_lhs);
      const shared_ptr<const N> rhs_dest_name =
          get_dest_N_ptr_from_edge_static(_rhs);

      return (*lhs_src_name < *rhs_src_name) ||
             ((*lhs_src_name == *rhs_src_name) &&
              (*lhs_dest_name < *rhs_dest_name)) ||
             ((*lhs_src_name == *rhs_src_name) &&
              (*lhs_dest_name == *rhs_dest_name) &&
              ((*_lhs).weight_ < (*_rhs).weight_));
    }
  };
  /**
   * Be careful, parameter starts with two underlying
   */
  struct EdgeComparator_weak {
    bool operator()(const weak_ptr<Edge> &__lhs,
                    const weak_ptr<Edge> &__rhs) const {
      const shared_ptr<Edge> _lhs = __lhs.lock();
      const shared_ptr<Edge> _rhs = __rhs.lock();
      // debug --------------------------------------
      if ((*_lhs).dest_.expired() == true || (*_rhs).dest_.expired() == true) {
        std::cout << "expire impossible\n";
        std::exit(1);
      }
      // debug --------------------------------------

      const shared_ptr<const N> lhs_src_name =
          get_src_N_ptr_from_edge_static(_lhs);
      const shared_ptr<const N> rhs_src_name =
          get_src_N_ptr_from_edge_static(_rhs);

      const shared_ptr<const N> lhs_dest_name =
          get_dest_N_ptr_from_edge_static(_lhs);
      const shared_ptr<const N> rhs_dest_name =
          get_dest_N_ptr_from_edge_static(_rhs);

      return (*lhs_src_name < *rhs_src_name) ||
             ((*lhs_src_name == *rhs_src_name) &&
              (*lhs_dest_name < *rhs_dest_name)) ||
             ((*lhs_src_name == *rhs_src_name) &&
              (*lhs_dest_name == *rhs_dest_name) &&
              ((*_lhs).weight_ < (*_rhs).weight_));
    }
  };

  /**
   * Attention for Node constructor
   * The shared_ptr must be passed by reference, because this->nodes_.key() is
   * created first before value and key() is shared_ptr
   */
  typedef struct Node {
    Node(const shared_ptr<N> &_name) : name_{_name} {}

    std::weak_ptr<N> name_;
    // std::set< std::shared_ptr<Edge>, []( const shared_ptr<Edge> lhs, const
    // shared_ptr<Edge> rhs ) {
    //                                         return (
    //                                             ( lhs.dest < rhs.dest ) ||
    //                                             ( ( lhs.dest == rhs.dest ) &&
    //                                             ( lhs.weight < rhs.weight ) )
    //                                         );
    //                                             }
    // > outcoming;
    std::set<std::shared_ptr<Edge>, EdgeComparator_shared> outcoming_;
    std::set<std::shared_ptr<Edge>, EdgeComparator_shared> incoming_;
  } Node;

  /**
   * same reason for Edge from Node
   */
  typedef struct Edge {
    Edge(const shared_ptr<Node> &_src, const shared_ptr<Node> &_dest,
         const E &_weight)
        : src_{_src}, dest_{_dest}, weight_{_weight} {}

    std::weak_ptr<Node> src_;
    std::weak_ptr<Node> dest_;
    E weight_;
  } Edge;

  /**
   * const version
   * Question, do I declare return variable as const or not?
   * has to be that node exists
   *
   * you can not declare it as public, since struct Node is encapsulated
   * diff wtih getNodes()
   */
  shared_ptr<Node> getNode(const N &_val) const {
    shared_ptr<N> temp_N_ptr = make_shared<N>(_val);
    // debug test ---------------------------- delete
    if (this->nodes_.find(temp_N_ptr) == this->nodes_.end()) {
      throw std::runtime_error("impossible, src node does not exist");
    }
    // debug test ---------------------------- delete
    return this->nodes_.find(temp_N_ptr)->second;
  }

  /**
   * Attention : return value should be referenced or not
   * non const version
   */
  shared_ptr<Node> getNode(const N &_val) {
    shared_ptr<N> temp_N_ptr = make_shared<N>(_val);
    // debug test ---------------------------- delete
    if (this->nodes_.find(temp_N_ptr) == this->nodes_.end()) {
      throw std::runtime_error("impossible, src node does not exist");
    }
    // debug test ---------------------------- delete
    return this->nodes_.find(temp_N_ptr)->second;
  }

  /**
   * Attention : return value should be referenced or not
   * Question, do I declare return variable as const or not?
   * Node must exist
   *
   * when you pass a node, extract the variable "name_",
   * use this info to find existing node in current graph
   */
  shared_ptr<Node> getNodePassedByNode(const shared_ptr<Node> &_node) {
    shared_ptr<N> temp_node = (*_node).name_.lock();
    // debug test ---------------------------- delete
    if (this->nodes_.find(temp_node) == this->nodes_.end()) {
      throw std::runtime_error("impossible, src node does not exist");
    }
    // debug test ---------------------------- delete
    return this->nodes_.find(temp_node)->second;
  }

  // shared_ptr<Node> makeNode( const N& val )
  // {
  //     shared_ptr<N> temp_N_ptr = make_shared<N>( val );
  //     shared_ptr<Node> new_Node = make_shared<Node>( temp_N_ptr );
  //     return new_Node;
  // }

  /**
   * Assume, src, dest nodes exist
   *
   * here the reason to pass reference is to save time
   * in function body, the edge is created by make_shared<>
   */
  shared_ptr<Edge> makeEdge(const N &_src, const N &_dest, const E &_w) {
    shared_ptr<Node> temp_src = getNode(_src);
    shared_ptr<Node> temp_dest = getNode(_dest);

    shared_ptr<Edge> temp_edge = make_shared<Edge>(temp_src, temp_dest, _w);
    return temp_edge;
  }

  shared_ptr<Edge> makeEdge(const N &_src, const N &_dest, const E &_w) const {
    shared_ptr<Node> temp_src = getNode(_src);
    shared_ptr<Node> temp_dest = getNode(_dest);
    /**
     * Question const parsed
     */
    shared_ptr<Edge> temp_edge = make_shared<Edge>(temp_src, temp_dest, _w);
    return temp_edge;
  }

  /**
   * Be careful, here, the node you find in new graph should be returned by
   * reference instead of value Reason : before coping edges, node already
   * created, so you have to pass reference to edges, to let the member in edges
   * to pointing the same resource i.e. nodes
   *
   * the nodes needed for this function should already exists
   * becuase we are just coping an existing edge, so just pass const refernece
   * to save time
   */
  shared_ptr<Edge> copyEdge(const shared_ptr<Edge> &_edge) {

    shared_ptr<Node> existing_src_node =
        getNodePassedByNode((*_edge).src_.lock());

    shared_ptr<Node> existing_dest_node =
        getNodePassedByNode((*_edge).dest_.lock());

    shared_ptr<Edge> edge = make_shared<Edge>(
        existing_src_node, existing_dest_node, (*_edge).weight_);

    return edge;
  }

  bool InsertEdgeWithNewSrc(const shared_ptr<Edge> &_edge, const N &_new_src) {
    shared_ptr<N> temp_dest_node_name = get_dest_N_ptr_from_edge(_edge);

    this->InsertEdge(_new_src, *temp_dest_node_name, (*_edge).weight_);
    return true;
  }

  bool InsertEdgeWithNewDest(const shared_ptr<Edge> &_edge,
                             const N &_new_dest) {
    shared_ptr<N> temp_src_node_name = get_src_N_ptr_from_edge(_edge);

    this->InsertEdge(*temp_src_node_name, _new_dest, (*_edge).weight_);
    return true;
  }

  shared_ptr<N> get_src_N_ptr_from_edge(const shared_ptr<Edge> &_edge) {
    shared_ptr<Node> temp_src_node = (*_edge).src_.lock();
    return (*temp_src_node).name_.lock();
  }

  shared_ptr<N> get_src_N_ptr_from_edge(const shared_ptr<Edge> &_edge) const {
    shared_ptr<Node> temp_src_node = (*_edge).src_.lock();
    return (*temp_src_node).name_.lock();
  }

  static const shared_ptr<const N>
  get_src_N_ptr_from_edge_static(const shared_ptr<Edge> &_edge) {
    shared_ptr<Node> temp_src_node = (*_edge).src_.lock();
    return (*temp_src_node).name_.lock();
  }

  shared_ptr<N> get_dest_N_ptr_from_edge(const shared_ptr<Edge> &_edge) {
    shared_ptr<Node> temp_dest_node = (*_edge).dest_.lock();
    return (*temp_dest_node).name_.lock();
  }

  shared_ptr<N> get_dest_N_ptr_from_edge(const shared_ptr<Edge> &_edge) const {
    shared_ptr<Node> temp_dest_node = (*_edge).dest_.lock();
    return (*temp_dest_node).name_.lock();
  }

  static const shared_ptr<const N>
  get_dest_N_ptr_from_edge_static(const shared_ptr<Edge> &_edge) {
    shared_ptr<Node> temp_dest_node = (*_edge).dest_.lock();
    return (*temp_dest_node).name_.lock();
  }

  /**
   * one possible bug, key is shared_ptr<N>
   */
  std::map<shared_ptr<N>, shared_ptr<Node>, MapNodeComparator> nodes_;
  std::set<weak_ptr<Edge>, EdgeComparator_weak> edges_;
};

} // namespace gdwg

#endif // ASSIGNMENTS_DG_GRAPH_H_

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
#include <utility>
#include <vector>

namespace gdwg {

template <typename N, typename E>
class Graph {
 private:
  struct EdgeComparator_shared;
  struct Edge;
  struct Node;

 public:
  /*
   * A const iterator class
   */
  class const_iterator {
   public:
    typedef typename gdwg::Graph<N, E>::Edge Edge;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using difference_type = int;

    const_iterator(const typename gdwg::Graph<N, E>& container,
                   const typename std::set<std::weak_ptr<Edge>>::const_iterator curr,
                   const typename std::set<std::weak_ptr<Edge>>::const_iterator end)
      : container_{container}, curr_{curr}, end_{end}, increment_{1} {}

    const_iterator(const typename gdwg::Graph<N, E>& container,
                   const typename std::set<std::weak_ptr<Edge>>::const_iterator curr,
                   const typename std::set<std::weak_ptr<Edge>>::const_iterator end,
                   const int& increment)
      : container_{container}, curr_{curr}, end_{end}, increment_{increment} {}

    const_iterator(const const_iterator& another)
      : container_{another.container_}, curr_{another.curr_}, end_{another.end_},
        increment_{another.increment_} {}

    const_iterator(const const_iterator&& another)
      : container_{std::move(another.container_)}, curr_{std::move(another.curr_)},
        end_{std::move(another.end_)}, increment_{std::move(another.increment_)} {}

    reference operator*() const {
      std::shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());
      N& temp_src = *temp_src_node_name;

      std::shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      N& temp_dest = *temp_dest_node_name;

      E& temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return {temp_src, temp_dest, temp_weight};
    }

    value_type operator*() {
      std::shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());
      std::shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      E temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return std::make_tuple(*temp_src_node_name, *temp_dest_node_name, temp_weight);
    }

    const_iterator& operator++() {
      ++this->curr_;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator temp{*this};
      ++(*this);
      return temp;
    }

    const_iterator& operator--() {
      --this->curr_;
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator temp{*this};
      --(*this);
      return temp;
    }

    bool sameContainer(const typename gdwg::Graph<N, E>& another) {
      if (&(this->container_) != &another) {
        return false;
      } else {
        return true;
      }
    }

    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      if (&(lhs.container_) != &(rhs.container_)) {
        return false;
      }
      if (lhs.curr_ != rhs.curr_ || lhs.end_ != rhs.end_) {
        return false;
      }
      return true;
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    friend class Graph;
    const typename gdwg::Graph<N, E>& container_;
    typename std::set<std::weak_ptr<Edge>>::const_iterator curr_;
    const typename std::set<std::weak_ptr<Edge>>::const_iterator end_;
    int increment_;
  };

  /*
   * A const reverse iterator class
   */
  class const_reverse_iterator {
   public:
    typedef typename gdwg::Graph<N, E>::Edge Edge;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using difference_type = int;

    const_reverse_iterator(
        const typename gdwg::Graph<N, E>& container,
        const typename std::set<std::weak_ptr<Edge>>::const_reverse_iterator curr,
        const typename std::set<std::weak_ptr<Edge>>::const_reverse_iterator end)
      : container_{container}, curr_{curr}, end_{end}, increment_{1} {}

    const_reverse_iterator(
        const typename gdwg::Graph<N, E&> container,
        const typename std::set<std::weak_ptr<Edge>>::const_reverse_iterator curr,
        const typename std::set<std::weak_ptr<Edge>>::const_reverse_iterator end,
        const int& increment)
      : container_{container}, curr_{curr}, end_{end}, increment_{increment} {}

    const_reverse_iterator(const const_reverse_iterator& another)
      : container_{another.container_}, curr_{another.curr_}, end_{another.end_},
        increment_{another.increment_} {}

    const_reverse_iterator(const const_reverse_iterator&& another)
      : container_{std::move(another.container_)}, curr_{std::move(another.curr_)},
        end_{std::move(another.end_)}, increment_{std::move(another.increment_)} {}

    reference operator*() const {
      std::shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());
      N& temp_src = *temp_src_node_name;

      std::shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      N& temp_dest = *temp_dest_node_name;

      E& temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return {temp_src, temp_dest, temp_weight};
    }

    value_type operator*() {
      std::shared_ptr<N> temp_src_node_name =
          this->container_.get_src_N_ptr_from_edge((*(this->curr_)).lock());
      std::shared_ptr<N> temp_dest_node_name =
          this->container_.get_dest_N_ptr_from_edge((*(this->curr_)).lock());
      E temp_weight = this->container_.getWeight((*(this->curr_)).lock());
      return std::make_tuple(*temp_src_node_name, *temp_dest_node_name, temp_weight);
    }

    const_reverse_iterator& operator++() {
      ++this->curr_;
      return *this;
    }

    const_reverse_iterator operator++(int) {
      const_reverse_iterator temp{*this};
      ++(*this);
      return temp;
    }

    const_reverse_iterator& operator--() {
      --this->curr_;
      return *this;
    }

    const_reverse_iterator operator--(int) {
      const_reverse_iterator temp{*this};
      --(*this);
      return temp;
    }

    bool sameContainer(const typename gdwg::Graph<N, E>& another) {
      if (&(this->container_) != &another) {
        return false;
      } else {
        return true;
      }
    }

    friend bool operator==(const const_reverse_iterator& lhs, const const_reverse_iterator& rhs) {
      if (&(lhs.container_) != &(rhs.container_)) {
        return false;
      }
      if (lhs.curr_ != rhs.curr_ || lhs.end_ != rhs.end_) {
        return false;
      }
      return true;
    }

    friend bool operator!=(const const_reverse_iterator& _lhs, const const_reverse_iterator& _rhs) {
      return !(_lhs == _rhs);
    }

   private:
    friend class Graph;
    const typename gdwg::Graph<N, E>& container_;
    typename std::set<std::weak_ptr<Edge>>::const_reverse_iterator curr_;
    const typename std::set<std::weak_ptr<Edge>>::const_reverse_iterator end_;
    int increment_;
  };

  /*
   * Default constructor
   */
  Graph() = default;

  /*
   * Constructor which takes the start and end of a const_iterator
   */
  Graph(typename std::vector<N>::const_iterator start, typename std::vector<N>::const_iterator end);

  /*
   * Constructor which takes iterators over tuples
   */
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator start,
        typename std::vector<std::tuple<N, N, E>>::const_iterator end);

  /*
   * Constructor which takes an initialized list
   */
  Graph(typename std::initializer_list<N> list);

  /*
   * Copy Constructor
   */
  explicit Graph(const typename gdwg::Graph<N, E>& other);

  /*
   * Move Constructor
   */
  explicit Graph(typename gdwg::Graph<N, E>&& other);

  /*
   * Destructor
   */
  ~Graph() = default;

  /*
   * Copy Assignment
   */
  Graph<N, E>& operator=(const typename gdwg::Graph<N, E>& rhs);

  /*
   * Move Assignment
   */
  Graph<N, E>& operator=(gdwg::Graph<N, E>&& rhs) noexcept;

  /*
   * Adds a new node with value val to the graph.
   */
  bool InsertNode(const N& val);

  /*
   * Adds a new edge src → dst with weight w.
   */
  bool InsertEdge(const N& src, const N& dest, const E& w);

  /*
   * Deletes a given node and all its associated incoming and outgoing edges.
   */
  bool DeleteNode(const N& val);

  /*
   * Replaces the original data, oldData, stored at this particular node by the replacement data,
   * newData.
   */
  bool Replace(const N& oldData, const N& newData);

  /*
   * All instances of node oldData in the graph are replaced with instances of newData.
   */
  void MergeReplace(const N& oldData, const N& newData);

  /*
   * Remove all nodes and edges from the graph.
   */
  void Clear();

  /*
   * Returns true if a node with value val exists in the graph and false otherwise.
   */
  bool IsNode(const N& val) const;

  /*
   * Returns true if the edge src → dst exists in the graph and false otherwise.
   */
  bool IsConnected(const N& src, const N& dest) const;

  /*
   * Returns a vector of all nodes in the graph. Sorted by increasing order of node.
   */
  std::vector<N> GetNodes() const;

  /*
   * Returns a vector of the nodes (found from any immediate outgoing edge) connected to the src
   * node passed in.
   */
  std::vector<N> GetConnected(const N& src) const;

  /*
   * Returns a vector of the weights of edges between two nodes.
   */
  std::vector<E> GetWeights(const N& src, const N& dest) const;

  /*
   * Returns an iterator to the found edge in the graph.
   * If the edge is not found the equivalent value of gdwg::Graph<N, E>::cend() is returned.
   */
  gdwg::Graph<N, E>::const_iterator find(const N& src, const N& dest, const E& weight) const;

  /*
   * Deletes an edge from src to dst with weight w, only if the edge exists in the graph.
   * If the edge is not in the graph it returns false.
   */
  bool erase(const N& src, const N& dest, const E& w);

  /*
   * Removes the position at the location the iterator points to.
   */
  gdwg::Graph<N, E>::const_iterator erase(gdwg::Graph<N, E>::const_iterator& it);
  gdwg::Graph<N, E>::const_iterator erase(gdwg::Graph<N, E>::const_iterator&& it);

  /*
   * Returns a const_iterator pointing to the first in the container.
   */
  gdwg::Graph<N, E>::const_iterator cbegin() const;

  /*
   * Returns a const_iterator pointing to the past-the-end element in the container.
   */
  gdwg::Graph<N, E>::const_iterator cend() const;

  /*
   * Returns a const_reverse_iterator pointing to the last element in the container.
   */
  gdwg::Graph<N, E>::const_reverse_iterator crbegin() const;

  /*
   * Returns a const_reverse_iterator pointing to the before-the-first element in the container.
   */
  gdwg::Graph<N, E>::const_reverse_iterator crend() const;

  /*
   * Exactly the same as cbegin()
   */
  gdwg::Graph<N, E>::const_iterator begin() const;

  /*
   * Exactly the same as cend()
   */
  gdwg::Graph<N, E>::const_iterator end() const;

  /*
   * Exactly the same as crbegin()
   */
  gdwg::Graph<N, E>::const_reverse_iterator rbegin() const;

  /*
   * Exactly the same as crend()
   */
  gdwg::Graph<N, E>::const_reverse_iterator rend() const;

  /*
   * True if the two graphs contain the same nodes and edges.
   */
  friend bool operator==(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    if (lhs.nodes_.size() != rhs.nodes_.size() || lhs.edges_.size() != rhs.edges_.size()) {
      return false;
    }
    auto lhs_nodes_it = lhs.nodes_.cbegin();
    auto rhs_nodes_it = rhs.nodes_.cbegin();
    for (; lhs_nodes_it != lhs.nodes_.cend(); lhs_nodes_it++, rhs_nodes_it++) {
      std::shared_ptr<N> temp_lhs_node_name = (*lhs_nodes_it).first;
      std::shared_ptr<N> temp_rhs_node_name = (*rhs_nodes_it).first;
      if (*temp_lhs_node_name != *temp_rhs_node_name) {
        return false;
      }
    }
    auto lhs_edge_it = lhs.edges_.cbegin();
    auto rhs_edge_it = rhs.edges_.cbegin();
    for (; lhs_edge_it != lhs.edges_.cend(); lhs_edge_it++, rhs_edge_it++) {
      std::shared_ptr<N> temp_lhs_src_node_name =
          lhs.get_src_N_ptr_from_edge((*lhs_edge_it).lock());
      std::shared_ptr<N> temp_rhs_src_node_name =
          rhs.get_src_N_ptr_from_edge((*rhs_edge_it).lock());
      if (*temp_lhs_src_node_name != *temp_rhs_src_node_name ||
          (*lhs_edge_it).lock()->weight_ != (*rhs_edge_it).lock()->weight_) {
        return false;
      }
      std::shared_ptr<N> temp_lhs_dest_node_name =
          lhs.get_dest_N_ptr_from_edge((*lhs_edge_it).lock());
      std::shared_ptr<N> temp_rhs_dest_node_name =
          rhs.get_dest_N_ptr_from_edge((*rhs_edge_it).lock());
      if (*temp_lhs_dest_node_name != *temp_rhs_dest_node_name) {
        return false;
      }
    }
    return true;
  }

  /*
   * True if the two graphs do not contain the same nodes or edges.
   */
  friend bool operator!=(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    return !(lhs == rhs);
  }

  /*
   * Prints out the graph
   */
  friend std::ostream& operator<<(std::ostream& out, const gdwg::Graph<N, E>& graph) {
    for (auto& it : graph.nodes_) {
      std::shared_ptr<Node> temp_src_node = it.second;
      std::shared_ptr<N> temp_src_node_name = (*temp_src_node).name_.lock();
      out << *temp_src_node_name << " ("
          << "\n";
      for (auto& each_edge : (*temp_src_node).outgoing_) {
        const std::shared_ptr<const N> temp_dest_node_name =
            graph.get_dest_N_ptr_from_edge(each_edge);
        out << "  " << *temp_dest_node_name << " | " << (*each_edge).weight_ << "\n";
      }
      out << ")"
          << "\n";
    }
    return out;
  }

  E& getWeight(std::shared_ptr<Edge> edge) const { return edge->weight_; }
  E getWeight(std::shared_ptr<Edge> edge) { return edge->weight_; }

 private:
  struct MapNodeComparator {
    bool operator()(const std::shared_ptr<N>& lhs, const std::shared_ptr<N>& rhs) const {
      return (*lhs < *rhs);
    }
  };

  struct EdgeComparator_shared {
    bool operator()(const std::shared_ptr<Edge>& lhs, const std::shared_ptr<Edge>& rhs) const {
      const std::shared_ptr<const N> lhs_src_name = get_src_N_ptr_from_edge_static(lhs);
      const std::shared_ptr<const N> rhs_src_name = get_src_N_ptr_from_edge_static(rhs);
      const std::shared_ptr<const N> lhs_dest_name = get_dest_N_ptr_from_edge_static(lhs);
      const std::shared_ptr<const N> rhs_dest_name = get_dest_N_ptr_from_edge_static(rhs);
      return (*lhs_src_name < *rhs_src_name) ||
             ((*lhs_src_name == *rhs_src_name) && (*lhs_dest_name < *rhs_dest_name)) ||
             ((*lhs_src_name == *rhs_src_name) && (*lhs_dest_name == *rhs_dest_name) &&
              ((*lhs).weight_ < (*rhs).weight_));
    }
  };

  struct EdgeComparator_weak {
    bool operator()(const std::weak_ptr<Edge>& lhs, const std::weak_ptr<Edge>& rhs) const {
      const std::shared_ptr<Edge> _lhs = lhs.lock();
      const std::shared_ptr<Edge> _rhs = rhs.lock();
      const std::shared_ptr<const N> lhs_src_name = get_src_N_ptr_from_edge_static(_lhs);
      const std::shared_ptr<const N> rhs_src_name = get_src_N_ptr_from_edge_static(_rhs);
      const std::shared_ptr<const N> lhs_dest_name = get_dest_N_ptr_from_edge_static(_lhs);
      const std::shared_ptr<const N> rhs_dest_name = get_dest_N_ptr_from_edge_static(_rhs);
      return (*lhs_src_name < *rhs_src_name) ||
             ((*lhs_src_name == *rhs_src_name) && (*lhs_dest_name < *rhs_dest_name)) ||
             ((*lhs_src_name == *rhs_src_name) && (*lhs_dest_name == *rhs_dest_name) &&
              ((*_lhs).weight_ < (*_rhs).weight_));
    }
  };

  typedef struct Node {
    Node(const std::shared_ptr<N>& _name) : name_{_name} {}
    std::weak_ptr<N> name_;
    std::set<std::shared_ptr<Edge>, EdgeComparator_shared> outgoing_;
    std::set<std::shared_ptr<Edge>, EdgeComparator_shared> incoming_;
  } Node;

  typedef struct Edge {
    Edge(const std::shared_ptr<Node>& src, const std::shared_ptr<Node>& dest, const E& weight)
      : src_{src}, dest_{dest}, weight_{weight} {}
    std::weak_ptr<Node> src_;
    std::weak_ptr<Node> dest_;
    E weight_;
  } Edge;

  std::shared_ptr<Node> getNode(const N& val) const {
    std::shared_ptr<N> temp_N_ptr = std::make_shared<N>(val);
    return this->nodes_.find(temp_N_ptr)->second;
  }

  std::shared_ptr<Node> getNode(const N& val) {
    std::shared_ptr<N> temp_N_ptr = std::make_shared<N>(val);
    return this->nodes_.find(temp_N_ptr)->second;
  }

  std::shared_ptr<Node> getNodePassedByNode(const std::shared_ptr<Node>& node) {
    std::shared_ptr<N> temp_node = (*node).name_.lock();
    return this->nodes_.find(temp_node)->second;
  }

  std::shared_ptr<Edge> makeEdge(const N& src, const N& dest, const E& w) {
    std::shared_ptr<Node> temp_src = getNode(src);
    std::shared_ptr<Node> temp_dest = getNode(dest);
    std::shared_ptr<Edge> temp_edge = std::make_shared<Edge>(temp_src, temp_dest, w);
    return temp_edge;
  }

  std::shared_ptr<Edge> makeEdge(const N& src, const N& dest, const E& w) const {
    std::shared_ptr<Node> temp_src = getNode(src);
    std::shared_ptr<Node> temp_dest = getNode(dest);
    std::shared_ptr<Edge> temp_edge = std::make_shared<Edge>(temp_src, temp_dest, w);
    return temp_edge;
  }

  std::shared_ptr<Edge> copyEdge(const std::shared_ptr<Edge>& edge) {
    std::shared_ptr<Node> existing_src_node = getNodePassedByNode((*edge).src_.lock());
    std::shared_ptr<Node> existing_dest_node = getNodePassedByNode((*edge).dest_.lock());
    std::shared_ptr<Edge> copy_edge =
        std::make_shared<Edge>(existing_src_node, existing_dest_node, (*edge).weight_);
    return copy_edge;
  }

  bool InsertEdgeWithNewSrc(const std::shared_ptr<Edge>& edge, const N& new_src) {
    std::shared_ptr<N> temp_dest_node_name = get_dest_N_ptr_from_edge(edge);
    this->InsertEdge(new_src, *temp_dest_node_name, (*edge).weight_);
    return true;
  }

  bool InsertEdgeWithNewDest(const std::shared_ptr<Edge>& edge, const N& new_dest) {
    std::shared_ptr<N> temp_src_node_name = get_src_N_ptr_from_edge(edge);
    this->InsertEdge(*temp_src_node_name, new_dest, (*edge).weight_);
    return true;
  }

  std::shared_ptr<N> get_src_N_ptr_from_edge(const std::shared_ptr<Edge>& edge) {
    std::shared_ptr<Node> temp_src_node = (*edge).src_.lock();
    return (*temp_src_node).name_.lock();
  }

  std::shared_ptr<N> get_src_N_ptr_from_edge(const std::shared_ptr<Edge>& edge) const {
    std::shared_ptr<Node> temp_src_node = (*edge).src_.lock();
    return (*temp_src_node).name_.lock();
  }

  static const std::shared_ptr<const N>
  get_src_N_ptr_from_edge_static(const std::shared_ptr<Edge>& edge) {
    std::shared_ptr<Node> temp_src_node = (*edge).src_.lock();
    return (*temp_src_node).name_.lock();
  }

  std::shared_ptr<N> get_dest_N_ptr_from_edge(const std::shared_ptr<Edge>& edge) {
    std::shared_ptr<Node> temp_dest_node = (*edge).dest_.lock();
    return (*temp_dest_node).name_.lock();
  }

  std::shared_ptr<N> get_dest_N_ptr_from_edge(const std::shared_ptr<Edge>& edge) const {
    std::shared_ptr<Node> temp_dest_node = (*edge).dest_.lock();
    return (*temp_dest_node).name_.lock();
  }

  static const std::shared_ptr<const N>
  get_dest_N_ptr_from_edge_static(const std::shared_ptr<Edge>& edge) {
    std::shared_ptr<Node> temp_dest_node = (*edge).dest_.lock();
    return (*temp_dest_node).name_.lock();
  }

  std::map<std::shared_ptr<N>, std::shared_ptr<Node>, MapNodeComparator> nodes_;
  std::set<std::weak_ptr<Edge>, EdgeComparator_weak> edges_;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_

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
  class const_iterator {
   public:
    typedef typename gdwg::Graph<N, E>::Edge Edge;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<const N*,
                               const N*,
                               const E*>;  // Not strictly required, but nice to have.
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

    pointer operator->() const { return &(this->operator*()); }

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

  class const_reverse_iterator {
   public:
    typedef typename gdwg::Graph<N, E>::Edge Edge;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<const N*,
                               const N*,
                               const E*>;  // Not strictly required, but nice to have.
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

    pointer operator->() const { return &(this->operator*()); }

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

  Graph() = default;
  Graph(typename std::vector<N>::const_iterator start, typename std::vector<N>::const_iterator end);
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator start,
        typename std::vector<std::tuple<N, N, E>>::const_iterator end);
  Graph(typename std::initializer_list<N> list);
  explicit Graph(const typename gdwg::Graph<N, E>& graph);
  explicit Graph(typename gdwg::Graph<N, E>&& other);

  ~Graph() = default;

  Graph<N, E>& operator=(const typename gdwg::Graph<N, E>& rhs);
  Graph<N, E>& operator=(gdwg::Graph<N, E>&& rhs) noexcept;

  bool InsertNode(const N& val);
  bool InsertEdge(const N& src, const N& dest, const E& w);
  bool DeleteNode(const N& val);
  bool Replace(const N& oldData, const N& newData);
  void MergeReplace(const N& oldData, const N& newData);
  void Clear();
  bool IsNode(const N& val) const;
  bool IsConnected(const N& src, const N& dest) const;
  std::vector<N> GetNodes() const;
  std::vector<N> GetConnected(const N& src) const;
  std::vector<E> GetWeights(const N& src, const N& dest) const;

  gdwg::Graph<N, E>::const_iterator find(const N& src, const N& dest, const E& weight) const;
  bool erase(const N& src, const N& dest, const E& w);
  gdwg::Graph<N, E>::const_iterator erase(gdwg::Graph<N, E>::const_iterator& it);
  gdwg::Graph<N, E>::const_iterator cbegin() const;
  gdwg::Graph<N, E>::const_iterator cend() const;
  gdwg::Graph<N, E>::const_reverse_iterator crbegin() const;
  gdwg::Graph<N, E>::const_reverse_iterator crend() const;
  gdwg::Graph<N, E>::const_iterator begin() const;
  gdwg::Graph<N, E>::const_iterator end() const;
  gdwg::Graph<N, E>::const_reverse_iterator rbegin() const;
  gdwg::Graph<N, E>::const_reverse_iterator rend() const;

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

  friend bool operator!=(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) {
    return !(lhs == rhs);
  }

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

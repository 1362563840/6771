#include <algorithm>
#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator start,
                         typename std::vector<N>::const_iterator end) {
  for (auto it = start; it != end; it++) {
    this->InsertNode(*it);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator start,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  for (auto it = start; it != end; it++) {
    N temp_src_name = std::get<0>(*it);
    this->InsertNode(temp_src_name);
    N temp_dest_name = std::get<1>(*it);
    this->InsertNode(temp_dest_name);
    this->InsertEdge(temp_src_name, temp_dest_name, std::get<2>(*it));
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::initializer_list<N> list) {
  for (auto it : list) {
    this->InsertNode(it);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const typename gdwg::Graph<N, E>& other) {
  for (auto& it : other.nodes_) {
    std::shared_ptr<N> temp_N_ptr = std::make_shared<N>(*(it.first));
    std::shared_ptr<Node> temp_Node_ptr = std::make_shared<Node>(temp_N_ptr);
    this->nodes_.emplace(temp_N_ptr, temp_Node_ptr);
  }

  for (auto& it : other.edges_) {
    std::shared_ptr<Edge> new_edge = copyEdge(it.lock());
    std::shared_ptr<Node> temp_src_node = getNodePassedByNode((*new_edge).src_.lock());
    std::shared_ptr<Node> temp_dest_node = getNodePassedByNode((*new_edge).dest_.lock());
    (*temp_src_node).outgoing_.insert(new_edge);
    (*temp_dest_node).incoming_.insert(new_edge);
    this->edges_.insert(new_edge);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename gdwg::Graph<N, E>&& other) noexcept
  : nodes_{other.nodes_}, edges_{other.edges_} {
  other.nodes_.clear();
  other.edges_.clear();
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const typename gdwg::Graph<N, E>& rhs) {
  if (this == &rhs) {
    return *this;
  }
  Graph<N, E> temp{rhs};
  this->nodes_ = std::move(temp.nodes_);
  this->edges_ = std::move(temp.edges_);
  return *this;
}

template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& rhs) noexcept {
  this->nodes_ = std::move(rhs.nodes_);
  this->edges_ = std::move(rhs.edges_);
  rhs.nodes_.clear();
  rhs.edges_.clear();
  return *this;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) {
  if (this->IsNode(val) == true) {
    return false;
  }
  std::shared_ptr<N> temp_N_ptr = std::make_shared<N>(val);
  std::shared_ptr<Node> temp_Node_ptr = std::make_shared<Node>(temp_N_ptr);
  this->nodes_.emplace(temp_N_ptr, temp_Node_ptr);
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dest, const E& w) {
  if (this->IsNode(src) == false || this->IsNode(dest) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::InsertEdge when either src or dst node does "
          "not exist";
    throw std::runtime_error(ss.str());
  }
  std::shared_ptr<Edge> temp_edge = makeEdge(src, dest, w);
  if (this->edges_.find(temp_edge) != this->edges_.end()) {
    return false;
  }
  std::shared_ptr<Node> temp_src_node = this->getNode(src);
  std::shared_ptr<Node> temp_dest_node = this->getNode(dest);
  (*temp_src_node).outgoing_.insert(temp_edge);
  (*temp_dest_node).incoming_.insert(temp_edge);
  this->edges_.insert(temp_edge);
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& val) {
  if (this->IsNode(val) == false) {
    return false;
  }
  std::shared_ptr<N> temp_N_ptr = std::make_shared<N>(val);
  std::shared_ptr<Node> temp_target_node = this->nodes_.find(temp_N_ptr)->second;
  std::set<std::shared_ptr<Edge>, EdgeComparator_shared> temp_back_up_outgoing;
  for (auto it : (*temp_target_node).outgoing_) {
    std::shared_ptr<Node> temp_dest_node = (*it).dest_.lock();
    (*temp_dest_node).incoming_.erase(it);
    temp_back_up_outgoing.insert(it);
    this->edges_.erase(it);
  }
  for (auto& it : temp_back_up_outgoing) {
    (*temp_target_node).outgoing_.erase(it);
  }
  temp_back_up_outgoing.clear();
  for (auto it : (*temp_target_node).incoming_) {
    std::shared_ptr<Node> temp_src_node = (*it).src_.lock();
    (*temp_src_node).outgoing_.erase(it);
    temp_back_up_outgoing.insert(it);
    this->edges_.erase(it);
  }
  for (auto& it : temp_back_up_outgoing) {
    (*temp_target_node).incoming_.erase(it);
  }
  this->nodes_.erase(temp_N_ptr);
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
  if (this->IsNode(oldData) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::Replace on a node that doesn't exist";
    throw std::runtime_error(ss.str());
  }
  if (this->IsNode(newData) == true) {
    return false;
  }
  std::shared_ptr<Node> temp_old_node = this->getNode(oldData);
  std::set<std::shared_ptr<Edge>> backup_outgoing;
  std::set<std::shared_ptr<Edge>> backup_incoming;
  for (auto& it : (*temp_old_node).outgoing_) {
    backup_outgoing.insert(it);
  }
  for (auto& it : (*temp_old_node).incoming_) {
    backup_incoming.insert(it);
  }
  this->DeleteNode(oldData);
  this->InsertNode(newData);
  for (auto& it : backup_outgoing) {
    this->InsertEdgeWithNewSrc(it, newData);
  }
  for (auto& it : backup_incoming) {
    this->InsertEdgeWithNewDest(it, newData);
  }
  return true;
}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  if (this->IsNode(oldData) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::MergeReplace on old or new data if they don't "
          "exist in the graph";
    throw std::runtime_error(ss.str());
  }
  if (this->IsNode(newData) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::MergeReplace on old or new data if they don't "
          "exist in the graph";
    throw std::runtime_error(ss.str());
  }
  std::shared_ptr<Node> temp_old_node = this->getNode(oldData);
  std::set<std::shared_ptr<Edge>> backup_outgoing;
  std::set<std::shared_ptr<Edge>> backup_incoming;
  for (auto& it : (*temp_old_node).outgoing_) {
    backup_outgoing.insert(it);
  }
  for (auto& it : (*temp_old_node).incoming_) {
    backup_incoming.insert(it);
  }
  this->DeleteNode(oldData);
  for (auto& it : backup_outgoing) {
    this->InsertEdgeWithNewSrc(it, newData);
  }
  for (auto& it : backup_incoming) {
    this->InsertEdgeWithNewDest(it, newData);
  }
}

template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  this->nodes_.clear();
  this->edges_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const {
  std::shared_ptr<N> temp_N_ptr = std::make_shared<N>(val);
  if (this->nodes_.find(temp_N_ptr) != this->nodes_.end()) {
    return true;
  }
  return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dest) const {
  if (this->IsNode(src) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::IsConnected if src or dst node don't exist in "
          "the graph";
    throw std::runtime_error(ss.str());
  }
  if (this->IsNode(dest) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::IsConnected if src or dst node don't exist in "
          "the graph";
    throw std::runtime_error(ss.str());
  }
  const std::shared_ptr<const Node> temp_src_node = this->getNode(src);
  for (auto& it : (*temp_src_node).outgoing_) {
    const std::shared_ptr<const N> temp_dest_node_name = this->get_dest_N_ptr_from_edge(it);
    if (*temp_dest_node_name == dest) {
      return true;
    }
  }
  return false;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const {
  std::vector<N> result;
  for (auto& it : this->nodes_) {
    result.push_back(*(it.first));
  }
  return result;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {
  if (this->IsNode(src) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::GetConnected if src doesn't exist in the graph";
    throw std::runtime_error(ss.str());
  }
  std::set<N> temp;
  const std::shared_ptr<const Node> temp_src_node = this->getNode(src);
  for (auto& it : (*temp_src_node).outgoing_) {
    const std::shared_ptr<const N> temp_dest_node_name = this->get_dest_N_ptr_from_edge(it);
    temp.insert(*temp_dest_node_name);
  }
  std::vector<N> result;
  std::copy(temp.cbegin(), temp.cend(), std::back_inserter(result));
  return result;
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dest) const {
  if (this->IsNode(src) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::GetWeights if src or dst node don't exist in "
          "the graph";
    throw std::runtime_error(ss.str());
  }
  if (this->IsNode(dest) == false) {
    std::stringstream ss;
    ss << "Cannot call Graph::GetWeights if src or dst node don't exist in "
          "the graph";
    throw std::runtime_error(ss.str());
  }
  std::vector<E> result;
  const std::shared_ptr<const Node> temp_src_node = this->getNode(src);
  for (auto& it : (*temp_src_node).outgoing_) {
    const std::shared_ptr<const N> temp_dest_node_name = this->get_dest_N_ptr_from_edge(it);
    if (*temp_dest_node_name == dest) {
      result.push_back((*it).weight_);
    }
  }
  return result;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::find(const N& src, const N& dest, const E& weight) const {
  if (this->IsNode(src) == false || this->IsNode(dest) == false) {
    return this->end();
  }
  std::shared_ptr<Edge> temp_edge = this->makeEdge(src, dest, weight);
  auto result = this->edges_.find(temp_edge);
  if (result != this->edges_.end()) {
    return gdwg::Graph<N, E>::const_iterator(*this, result, this->edges_.end());
  }
  return this->end();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dest, const E& w) {
  gdwg::Graph<N, E>::const_iterator it = this->find(src, dest, w);
  if (it == this->end()) {
    return false;
  }
  std::shared_ptr<Node> temp_src_node = this->getNode(src);
  std::shared_ptr<Node> temp_dest_node = this->getNode(dest);
  std::shared_ptr<Edge> temp_edge = this->makeEdge(src, dest, w);
  this->edges_.erase(temp_edge);
  (*temp_src_node).outgoing_.erase(temp_edge);
  (*temp_dest_node).incoming_.erase(temp_edge);
  return true;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::erase(gdwg::Graph<N, E>::const_iterator& it) {
  if (it.sameContainer(*this) == false) {
    return this->end();
  }
  if (it == this->end()) {
    return this->end();
  }
  std::tuple<N, N, E> tuple_edge = *it;
  auto next = it++;
  this->erase(std::get<0>(tuple_edge), std::get<1>(tuple_edge), std::get<2>(tuple_edge));
  return next;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::erase(gdwg::Graph<N, E>::const_iterator&& it) noexcept {
  if (it.sameContainer(*this) == false) {
    return this->end();
  }
  if (it == this->end()) {
    return this->end();
  }
  std::tuple<N, N, E> tuple_edge = *it;
  auto next = it++;
  this->erase(std::get<0>(tuple_edge), std::get<1>(tuple_edge), std::get<2>(tuple_edge));
  return next;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const {
  return gdwg::Graph<N, E>::const_iterator(*this, this->edges_.begin(), this->edges_.end());
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const {
  return gdwg::Graph<N, E>::const_iterator(*this, this->edges_.end(), this->edges_.end());
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crbegin() const {
  return gdwg::Graph<N, E>::const_reverse_iterator(*this, this->edges_.rbegin(),
                                                   this->edges_.rend());
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crend() const {
  return gdwg::Graph<N, E>::const_reverse_iterator(*this, this->edges_.rend(), this->edges_.rend());
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() const {
  return cbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::end() const {
  return cend();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rbegin() const {
  return crbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rend() const {
  return crend();
}

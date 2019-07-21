class Iterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using reference = T&;
  using pointer = T*; // Not strictly required, but nice to have.
  using difference_type = int;

  reference operator*() const;
  Iterator& operator++();
  Iterator operator++(int) {
    auto copy{*this};
    ++(*this);
    return copy;
  }
  // This one isn't strictly required, but it's nice to have.
  pointer operator->() const { return &(operator*()); }

  friend bool operator==(const Iterator& lhs, const Iterator& rhs) { ... };
  friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return !(lhs == rhs); }
};

class Container {
  // Make the iterator
  class reverse_iterator {...};
  // or
  using reverse_iterator = ...;
 
  // Need to define these.
  reverse_iterator rbegin();
  reverse_iterator rend();
 
  // If you want const reverse iterators (hint: you do), define these.
  const_reverse_iterator rbegin() const { return crbegin(); }
  const_reverse_iterator crbegin();
  const_reverse_iterator rend() const { return crend(); }
  const_reverse_iterator crend() const;
};
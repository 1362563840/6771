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












/**
 * Name        : RingQueue.cpp
 * Author      : Some Guy
 * Version     :
 * Copyright   : Copyleft 2018 Some Guy. No rights reserved.
 * Description : A ring buffer with STL iterators in C++.
 */

#include <iostream>
#include <sstream>
#include <iterator>
#include <cassert>

template <typename T, int capacity> class RingIter;
template <typename T, int capacity> class RingIterConst;

template <typename T, int capacity> class RingQueue {
    friend class RingIter<T, capacity>;
    friend class RingIterConst<T, capacity>;
    typedef RingIter<T, capacity> iterator;
    typedef RingIterConst<T, capacity> const_iterator;
private:
    T buf[capacity];
    int begin_idx;
    int siz;

    int end_idx() { return (begin_idx + siz) % capacity; }
public:
    RingQueue() : begin_idx(0), siz(0) {}
    ~RingQueue() {}
    int size() const { return siz; }
    T &front() {
        if (siz) {
            return buf[begin_idx];
        } else {
            throw std::invalid_argument("RingQueue is empty");
        }
    }
    const T &front() const {
        if (siz) {
            return buf[begin_idx];
        } else {
            throw std::invalid_argument("RingQueue is empty");
        }
    }
    T &back() {
        if (siz) {
            return buf[end_idx()];
        } else {
            throw std::invalid_argument("RingQueue is empty");
        }
    }
    const T &back() const {
        if (siz) {
            return buf[end_idx()];
        } else {
            throw std::invalid_argument("RingQueue is empty");
        }
    }
    T &pop_front() {
        if (!siz) {
            throw std::invalid_argument("RingQueue is empty");
        }
        T &ret = buf[begin_idx];
        begin_idx++;
        begin_idx %= capacity;
        siz--;
        return ret;
    }
    void push_back(T val) {
        buf[end_idx()] = val;
        if (siz < capacity) {
            siz++;
        } else {
            begin_idx++;
            begin_idx %= capacity;
        }
    }
    iterator begin() { return iterator(*this, 0); }
    iterator end() { return iterator(*this, siz); }
    const_iterator cbegin() const { return const_iterator(*this, 0); }
    const_iterator cend() const { return const_iterator(*this, siz); }
    iterator rbegin() { return iterator(*this, siz - 1, -1); }
    iterator rend() { return iterator(*this, -1, -1); }
    const_iterator crbegin() const { return const_iterator(*this, siz - 1, -1); }
    const_iterator crend() const { return const_iterator(*this, -1, -1); }
};

template <typename T, int capacity> class RingIterConst {
    typedef RingIterConst<T, capacity> thisclass;
protected:
    const RingQueue<T, capacity> &rq;
    int off;
    int inc;
    inline const T& deref() { return rq.buf[(rq.begin_idx + off) % capacity]; }
public:
    RingIterConst(const RingQueue<T, capacity> &iterateOver, int offset, int increment = 1) : rq(iterateOver), off(offset), inc(increment) {}
    ~RingIterConst() {}
    bool operator==(const RingIterConst &i) {
        return &i.rq == &rq && i.off == off;
    }
    bool operator!=(const RingIterConst &i) {
        return !(*this == i);
    }
    thisclass & operator++()    { off += inc; return *this; }
    thisclass & operator++(int) { off += inc; return *this; }
    thisclass & operator--()    { off -= inc; return *this; }
    thisclass & operator--(int) { off -= inc; return *this; }
    typename std::iterator_traits<thisclass>::difference_type operator-(thisclass &sibling) const { return (off - sibling.off) / inc; }
    thisclass & operator+=(int amount) { off += (amount * inc); return *this; }
    thisclass & operator-=(int amount) { off -= (amount * inc); return *this; }
    thisclass & operator-() { return thisclass(rq, off, -inc); }
    bool operator<(thisclass &sibling) const { return (inc < 0) != (off < sibling.off);}
    bool operator<=(thisclass &sibling) const { return (inc < 0) != (off <= sibling.off); }
    bool operator>(thisclass &sibling) const { return (inc < 0) != (off > sibling.off); }
    bool operator>=(thisclass &sibling) const { return (inc < 0) != (off >= sibling.off); }
    const T& operator[](int index) {
        assert(index >= 0);
        assert(index < rq.siz);
        return rq.buf[(rq.begin_idx + off + (index * inc)) % capacity];
    }
    const T& operator*() { return deref(); }
};

template <typename T, int capacity> class RingIter : public RingIterConst<T, capacity> {
public:
    RingIter(RingQueue<T, capacity> &iterateOver, int offset) : RingIterConst<T, capacity>(iterateOver, offset) {}
    ~RingIter() {}
    T& operator[](int index) { return this->rq.buf[(this->rq.begin_idx + this->off + (index * this->inc)) % this->capacity]; }
    T &operator*() { return this->deref(); }
};

// FIXME: Do not pollute namespace 'std'.
namespace std {
template<typename T, int capacity> class iterator_traits<RingIterConst<T, capacity> > {
public:
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;
};
template<typename T, int capacity> class iterator_traits<RingIter<T, capacity> > : public iterator_traits<RingIterConst<T, capacity> > {
    typedef const T value_type;
    typedef const T* pointer;
    typedef const T& reference;
};
}

int main(void) {
    RingQueue<int, 4> rq;
    for (int i = 0; i < 10; i++) {
        rq.push_back(i * i);
    }
    assert(rq.size() == 4);
    std::ostringstream s;
    std::copy(rq.cbegin(), rq.cend(), std::ostream_iterator<const int>(s, " "));
    assert(s.str() == "36 49 64 81 ");
    s.str("");
    std::copy(rq.crbegin(), rq.crend(), std::ostream_iterator<const int>(s, " "));
    assert(s.str() == "81 64 49 36 ");
    return 0;
}
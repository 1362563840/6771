#ifndef ASS2_EUCLIDEAN_VECTOR_H
#define ASS2_EUCLIDEAN_VECTOR_H

// TODO(you): Include header guards
#include <iostream>
#include <exception>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::make_unique;
using std::move;
using std::vector;

class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept{ return what_.c_str(); }
 private:
  std::string what_;
};

class EuclideanVector {
 public:
  explicit EuclideanVector(int i);

  explicit EuclideanVector(int i, double j);

  explicit 	EuclideanVector(std::vector<double>::const_iterator start, std::vector<double>::const_iterator end);

  // explicit  EuclideanVector(EuclideanVector & another);
  EuclideanVector(EuclideanVector & another);

  explicit  EuclideanVector(const EuclideanVector & another);

  explicit  EuclideanVector(EuclideanVector && another);

  ~EuclideanVector();

  friend void DefinedSwap(EuclideanVector & first, EuclideanVector & second); // nothrow

  EuclideanVector & operator =(const EuclideanVector & rhs);
  EuclideanVector & operator = (EuclideanVector& rhs);
  EuclideanVector & operator =(EuclideanVector rhs);

  // EuclideanVector operator =( EuclideanVector&& ev );

  // bool operator ==( EuclideanVector& ev );

  void display() {
    cout << "size is " << this -> size_ << "\n";
    cout << "dimension is\n";
    for( int i = 0 ; i < this -> size_ ; i++ ) {
      cout << this -> magnitudes_.get()[ i ] << "\n";
    }
  }

  // TODO(you): add more
 private:
  int size_;
  std::unique_ptr<double[]> magnitudes_;


  // TODO(you): add more
};

#endif //ASS2_EUCLIDEAN_VECTOR_H
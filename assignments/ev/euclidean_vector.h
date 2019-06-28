#ifndef ASS2_EUCLIDEAN_VECTOR_H
#define ASS2_EUCLIDEAN_VECTOR_H

#include <exception>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::list;
using std::make_unique;
using std::move;
using std::ostream;
using std::unique_ptr;
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

  explicit  EuclideanVector(EuclideanVector && another) noexcept;

  ~EuclideanVector();

  // nothrow
  friend void DefinedSwap(EuclideanVector & first, EuclideanVector & second); 

  EuclideanVector & operator =(const EuclideanVector & rhs);
  // EuclideanVector & operator =(EuclideanVector rhs);

  EuclideanVector operator =(EuclideanVector&& ev) noexcept;

  double& operator [](const int index);

  EuclideanVector& operator +=(EuclideanVector& rhs);

  EuclideanVector& operator -=(EuclideanVector& rhs);

  EuclideanVector& operator *=(const int scalar);

  EuclideanVector& operator /=(const int scalar);

  explicit operator std::vector<double>();

  explicit operator std::list<double>();

  double at(int index);

  int GetNumDimensions();

  double GetEuclideanNorm();

  EuclideanVector CreateUnitVector();

  friend bool operator ==(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend bool operator !=(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend EuclideanVector operator +(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend EuclideanVector operator -(const EuclideanVector& lhs, const EuclideanVector& rhs);

  friend double operator *(const EuclideanVector& lhs, const EuclideanVector& rhs);

  // EuclideanVector operator *(EuclideanVector& rhs);

  friend EuclideanVector operator *(const EuclideanVector& lhs, const int scalar);

  friend EuclideanVector operator *(const int scalar, const EuclideanVector& rhs);

  friend EuclideanVector operator /(const EuclideanVector& lhs, const int divison);

  friend ostream& operator <<(ostream& os, const EuclideanVector& obj);

  void display() {
    cout << "size is " << this -> size_ << "\n";
    cout << "dimension is\n";
    for( int i = 0 ; i < this -> size_ ; i++ ) {
      cout << this -> magnitudes_.get()[ i ] << "\n";
    }
  }

 private:
  int size_;
  std::unique_ptr<double[]> magnitudes_;

};

#endif //ASS2_EUCLIDEAN_VECTOR_H


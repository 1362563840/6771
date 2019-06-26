// #include "assignments/ev/euclidean_vector.h"
#include "euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm

using std::cout;
using std::swap;

EuclideanVector::EuclideanVector(int i) : EuclideanVector( i, 0 ) {}

EuclideanVector::EuclideanVector(int i, double j) : size_{i}, magnitudes_{ make_unique<double[]>( i ) }
{
    for( int k = 0 ; k < i ; k++ ) {
        magnitudes_[ k ] = j;
    }
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator start
                                ,std::vector<double>::const_iterator end) 
{
    this -> size_ = end - start;
    this -> magnitudes_ = make_unique<double[]>( this -> size_ );
    int i = 0;
    for( auto it = start ; it < end ; it++ ) {
      this -> magnitudes_.get()[ i ] = *it;
      i++;
    }
}

EuclideanVector::EuclideanVector(EuclideanVector & another) : size_{ another.size_ }
{ 
    cout << "non copy construct\n";
    this -> magnitudes_ = make_unique<double[]>( this -> size_ );
    for( int i = 0 ; i < this -> size_ ; i++ ) {
      this -> magnitudes_.get()[ i ] = another.magnitudes_.get()[ i ];
    }
}

EuclideanVector::EuclideanVector(const EuclideanVector& another) : size_{ another.size_ } 
{ 
    cout << "const copy construct\n";
    this -> magnitudes_ = make_unique<double[]>( this -> size_ );
    for( int i = 0 ; i < this -> size_ ; i++ ) {
      this -> magnitudes_.get()[ i ] = another.magnitudes_.get()[ i ];
    }
}

EuclideanVector::EuclideanVector(EuclideanVector&& another) : size_{another.size_} 
                                                            ,magnitudes_{ move(another.magnitudes_) } 
{
    cout << "move construct\n";
}

EuclideanVector::~EuclideanVector() 
{
    cout << "destruction\n";
}

void DefinedSwap(EuclideanVector& first, EuclideanVector & second)
{
    swap( first.size_, second.size_ );
    swap( first.magnitudes_, second.magnitudes_ );
}

EuclideanVector & EuclideanVector::operator =( const EuclideanVector & rhs ) 
{
    EuclideanVector temp = EuclideanVector(rhs);
    DefinedSwap( *this, temp );
    return * this;
}

EuclideanVector & EuclideanVector::operator =( EuclideanVector & rhs ) 
{
    EuclideanVector temp = EuclideanVector(rhs);
    DefinedSwap( *this, temp );
    return * this;
}

EuclideanVector & EuclideanVector::operator =( EuclideanVector rhs ) 
{
    DefinedSwap( *this, rhs );
    return * this;
}

// EuclideanVector EuclideanVector::operator =( EuclideanVector&& ev ) 
// {
//     this -> size_ = ev.size_;
//     this -> magnitudes_ = move(ev.magnitudes_);
// }

// bool operator ==( EuclideanVector& ev ){
//     cout << ">>> ev" << ev << "\n";
//     cout << ">>> &ev" << &ev << "\n";
//     if( this == &ev ) {
//         return true;
//     }
//     return false;
// }
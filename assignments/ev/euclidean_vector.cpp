// #include "assignments/ev/euclidean_vector.h"
#include "euclidean_vector.h"

#include <cassert>
#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <vector>

using std::cout;
using std::swap;
using std::exception;
using std::vector;

EuclideanVector::EuclideanVector(int i) : EuclideanVector( i, 0 ) {}

EuclideanVector::EuclideanVector(int i, double j) : size_{i}, magnitudes_{ make_unique<double[]>( i ) }
{   
    cout << "second constuct\n";
    for( int k = 0 ; k < i ; k++ ) {
        magnitudes_[ k ] = j;
    }
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator start
                                ,std::vector<double>::const_iterator end) 
{
    cout << "iterator constuct\n";
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

/**
 * because we need to check self assignment, so parameter must be passed
 * by reference not value
 */
EuclideanVector & EuclideanVector::operator =( const EuclideanVector & rhs ) 
{
    cout << "const operator =\n";
    if( this == &rhs ) {
        cout << "self assignment\n";
        return *this;
    }
    EuclideanVector temp = EuclideanVector(rhs);
    DefinedSwap( *this, temp );
    return * this;
}

// EuclideanVector & EuclideanVector::operator =( EuclideanVector rhs ) 
// {
//     cout << "non const = operator\n";
//     DefinedSwap( *this, rhs );
//     return * this;
// }

/**
 * because move operator, so we can just direct swap
 * no need to care about what happens to rhs when finished
 */
EuclideanVector EuclideanVector::operator =( EuclideanVector&& rhs ) 
{
    cout << "move operator =\n";
    DefinedSwap( *this, rhs );
    return * this;
}

double& EuclideanVector::operator [](const int index)
{
    assert( index < this -> size_ );
    return this -> magnitudes_.get()[ index ];
}

EuclideanVector& EuclideanVector::operator +=(EuclideanVector& rhs)
{
    cout << "operator +=\n";
    if( this -> size_ != rhs.size_ ) {
        throw ("Dimensions of LHS(%d) and RHS(%d) do not match\n", this -> size_, rhs.size_);
    }
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        this -> magnitudes_.get()[ i ]  = this -> magnitudes_.get()[ i ] + rhs.magnitudes_.get()[ i ];
    }
    return * this;
}

EuclideanVector& EuclideanVector::operator -=(EuclideanVector& rhs)
{
    cout << "operator -=\n";
    if( this -> size_ != rhs.size_ ) {
        throw ("Dimensions of LHS(%d) and RHS(%d) do not match\n", this -> size_, rhs.size_);
    }
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        this -> magnitudes_.get()[ i ]  = this -> magnitudes_.get()[ i ] - rhs.magnitudes_.get()[ i ];
    }
    return * this;
}

EuclideanVector& EuclideanVector::operator *=(const int scalar)
{
    cout << "operator *=\n";
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        this -> magnitudes_.get()[ i ]  = this -> magnitudes_.get()[ i ] * scalar;
    }
    return * this;
}

EuclideanVector& EuclideanVector::operator /=(const int scalar)
{
    cout << "operator /=\n";
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        this -> magnitudes_.get()[ i ]  = this -> magnitudes_.get()[ i ] / scalar;
    }
    return * this;
}

EuclideanVector::operator std::vector<double>()
{
    vector<double> temp;
    for( int i = 0; i < this -> size_ ; i++ ) {
        temp.push_back( this -> magnitudes_.get()[ i ] );
    }
}

EuclideanVector::operator std::list<double>()
{
    list<double> temp;
    for( int i = 0; i < this -> size_ ; i++ ) {
        temp.push_back( this -> magnitudes_.get()[ i ] );
    }
}
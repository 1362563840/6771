// #include "assignments/ev/euclidean_vector.h"
#include "euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <cassert>
#include <cmath>
#include <list>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::exception;
using std::list;
using std::ostream;
using std::sqrt;
using std::string;
using std::swap;
using std::to_string;
using std::unique_ptr;
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
        throw "Dimensions of LHS(X) and RHS(Y) do not match";
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
        throw "Dimensions of LHS(X) and RHS(Y) do not match";
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
    if( scalar == 0 ) {
        throw "Invalid vector division by 0";
    }
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
    return temp;
}

EuclideanVector::operator std::list<double>()
{
    list<double> temp;
    for( int i = 0; i < this -> size_ ; i++ ) {
        temp.push_back( this -> magnitudes_.get()[ i ] );
    }
    return temp;
}

double EuclideanVector::at(int index)
{
    if( index < 0 || index >= this -> size_ ) {
        throw "Index X is not valid for this EuclideanVector object";
    }
    return this -> magnitudes_.get()[ index ];
}

int EuclideanVector::GetNumDimensions()
{
    return this -> size_;
}

double EuclideanVector::GetEuclideanNorm()
{
    double sum{0};
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        sum = sum + this -> magnitudes_.get()[ i ] * this -> magnitudes_.get()[ i ];
    }
    return sqrt( sum );
}

EuclideanVector EuclideanVector::CreateUnitVector()
{
    if( this -> size_ == 0 ) {
        throw "EuclideanVector with no dimensions does not have a unit vector";
    }
    double norm = this -> GetEuclideanNorm();

    EuclideanVector temp(*this);
    temp /= norm;
    return temp;
}

bool operator ==(const EuclideanVector& lhs, const EuclideanVector& rhs)
{
    if( lhs.size_ != rhs.size_ ) {
        return false;
    }
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        if( lhs.magnitudes_.get()[ i ] != rhs.magnitudes_.get()[ i ]  ) {
            return false;
        }
    }
    return true;
}

bool operator !=(const EuclideanVector& lhs, const EuclideanVector& rhs)
{
    if( lhs.size_ != rhs.size_ ) {
        return true;
    }
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        if( lhs.magnitudes_.get()[ i ] != rhs.magnitudes_.get()[ i ]  ) {
            return true;
        }
    }
    return false;
}

EuclideanVector operator +(const EuclideanVector& lhs, const EuclideanVector& rhs)
{
    cout << "operator +\n";
    if( lhs.size_ != rhs.size_ ) {
        throw "Dimensions of LHS(X) and RHS(Y) do not match";
    }
    EuclideanVector temp( lhs.size_ ); 
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp.magnitudes_.get()[ i ]  = lhs.magnitudes_.get()[ i ] + rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

EuclideanVector operator -(const EuclideanVector& lhs, const EuclideanVector& rhs)
{
    cout << "operator -\n";
    if( lhs.size_ != rhs.size_ ) {
        throw "Dimensions of LHS(X) and RHS(Y) do not match";
    }
    EuclideanVector temp( lhs.size_ ); 
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp.magnitudes_.get()[ i ]  = lhs.magnitudes_.get()[ i ] - rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

double operator *(const EuclideanVector& lhs, const EuclideanVector& rhs)
{
    cout << "dot product operator *\n";
    if( lhs.size_ != rhs.size_ ) {
        throw "Dimensions of LHS(X) and RHS(Y) do not match";
    }
    double temp{0};
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp  = lhs.magnitudes_.get()[ i ] * rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

EuclideanVector operator *(const EuclideanVector& lhs, const int scalar)
{
    cout << "operator non reference scalar on right *\n";
    EuclideanVector temp( lhs.size_ ); 
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp[ i ]  = lhs.magnitudes_.get()[ i ] * scalar;
    }
    return temp;
}

EuclideanVector operator *(const int scalar, const EuclideanVector& rhs)
{
    cout << "operator non reference scalar on left *\n";
    EuclideanVector temp( rhs.size_ ); 
    for( int i = 0 ; i < rhs.size_ ; i++ ) {
        temp[ i ]  = scalar * rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

EuclideanVector operator /(const EuclideanVector& lhs, const int division)
{
    cout << "operator arithmetic /\n";
    if( division == 0 ) {
        throw "Invalid vector division by 0";
    }
    EuclideanVector temp( lhs.size_ ); 
    for( int i = 0 ; i <  lhs.size_ ; i++ ) {
        temp.magnitudes_.get()[ i ] = lhs.magnitudes_.get()[ i ] / division;
    }
    return temp;
}

ostream& operator <<(ostream& out, const EuclideanVector& obj)
{   
    string output{""};
    for( int i = 0 ; i < obj.size_ ; i++ ) {
        if( i == 0 ) {
            output = output + to_string(obj.magnitudes_.get()[ i ]);
        } 
        else {
            output = output +  " " + to_string(obj.magnitudes_.get()[ i ]);
        }
    }
    out << "[" << output << "]\n";
    return out;
}

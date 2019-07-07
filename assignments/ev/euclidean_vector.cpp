#include "assignments/ev/euclidean_vector.h"
// #include "euclidean_vector.h"

#include <algorithm>  
#include <cassert>
#include <cmath>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::exception;
using std::list;
using std::ostream;
using std::sqrt;
using std::stringstream;
using std::string;
using std::swap;
using std::to_string;
using std::unique_ptr;
using std::vector;

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

EuclideanVector::EuclideanVector(const EuclideanVector& another) : size_{ another.size_ } 
{ 
    this -> magnitudes_ = make_unique<double[]>( this -> size_ );
    for( int i = 0 ; i < this -> size_ ; i++ ) {
      this -> magnitudes_.get()[ i ] = another.magnitudes_.get()[ i ];
    }
}

EuclideanVector::EuclideanVector(EuclideanVector&& another) noexcept : size_{another.size_} 
                                                            ,magnitudes_{ move(another.magnitudes_) } 
{
    another.size_ = 0;
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
    if( this == &rhs ) {
        return *this;
    }
    EuclideanVector temp = EuclideanVector(rhs);
    DefinedSwap( *this, temp );
    return * this;
}

EuclideanVector EuclideanVector::operator =(EuclideanVector&& rhs) noexcept
{
    this -> magnitudes_  = move(rhs.magnitudes_);
    size_ = rhs.size_;
    rhs.size_ = 0;
    return * this;
}

double& EuclideanVector::operator [](const int index)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    assert( index < this -> size_ );
    assert( index >= 0 );
    return this -> magnitudes_.get()[ index ];
}

EuclideanVector& EuclideanVector::operator +=(const EuclideanVector& rhs)
{
    if( this -> size_ != rhs.size_ ) {
        stringstream ss;
        ss << "Dimensions of LHS(" << this -> size_ << ") ";
        ss << "and RHS(" << rhs.size_ << ")" << " do not match";
        throw std::runtime_error( ss.str() );
    }
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        this -> magnitudes_.get()[ i ]  = this -> magnitudes_.get()[ i ] + rhs.magnitudes_.get()[ i ];
    }
    return * this;
}

EuclideanVector& EuclideanVector::operator -=(const EuclideanVector& rhs)
{
    if( this -> size_ != rhs.size_ ) {
        stringstream ss;
        ss << "Dimensions of LHS(" << this -> size_ << ") ";
        ss << "and RHS(" << rhs.size_ << ")" << " do not match";
        throw std::runtime_error( ss.str() );
    }
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        this -> magnitudes_.get()[ i ]  = this -> magnitudes_.get()[ i ] - rhs.magnitudes_.get()[ i ];
    }
    return * this;
}

EuclideanVector& EuclideanVector::operator *=(const double scalar)
{
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        this -> magnitudes_.get()[ i ]  = this -> magnitudes_.get()[ i ] * scalar;
    }
    return * this;
}

EuclideanVector& EuclideanVector::operator /=(const double scalar)
{
    if( scalar == 0 ) {
        stringstream ss;
        ss << "Invalid vector division by 0";
        throw std::runtime_error( ss.str() );
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

double EuclideanVector::at(int index) const
{
    if( index < 0 || index >= this -> size_ ) {
        stringstream ss;
        ss << "Index " << index << " is not valid for this EuclideanVector object";
        throw std::runtime_error( ss.str() );
    }
    return this -> magnitudes_.get()[ index ];
}

double& EuclideanVector::at(int index)
{
    if( index < 0 || index >= this -> size_ ) {
        stringstream ss;
        ss << "Index " << index << " is not valid for this EuclideanVector object";
        throw std::runtime_error( ss.str() );
    }
    return this -> magnitudes_.get()[ index ];
}

int EuclideanVector::GetNumDimensions()
{
    return this -> size_;
}

double EuclideanVector::GetEuclideanNorm()
{
    if( this -> GetNumDimensions() == 0 ) {
        stringstream ss;
        ss << "EuclideanVector with no dimensions does not have a norm";
        throw std::runtime_error( ss.str() );
    }
    double sum{0};
    for( int i = 0 ; i < this -> size_ ; i++ ) {
        sum = sum + this -> magnitudes_.get()[ i ] * this -> magnitudes_.get()[ i ];
    }
    return sqrt( sum );
}

EuclideanVector EuclideanVector::CreateUnitVector()
{
    if( this -> size_ == 0 ) {
        stringstream ss;
        ss << "EuclideanVector with no dimensions does not have a unit vector";
        throw std::runtime_error( ss.str() );
    }
    
    double norm = this -> GetEuclideanNorm();
    if( norm == 0 ) {
        stringstream ss;
        ss << "EuclideanVector with euclidean normal of 0 does not have a unit vector";
        throw std::runtime_error( ss.str() );
    }

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
    return !( operator==( lhs, rhs ) );
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
    if( lhs.size_ != rhs.size_ ) {
        stringstream ss;
        ss << "Dimensions of LHS(" << lhs.size_ << ") ";
        ss << "and RHS(" << rhs.size_ << ")" << " do not match";
        throw std::runtime_error( ss.str() );
    }
    EuclideanVector temp( lhs.size_ ); 
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp.magnitudes_.get()[ i ]  = lhs.magnitudes_.get()[ i ] + rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

EuclideanVector operator -(const EuclideanVector& lhs, const EuclideanVector& rhs)
{
    if( lhs.size_ != rhs.size_ ) {
        stringstream ss;
        ss << "Dimensions of LHS(" << lhs.size_ << ") ";
        ss << "and RHS(" << rhs.size_ << ")" << " do not match";
        throw std::runtime_error( ss.str() );
    }
    EuclideanVector temp( lhs.size_ ); 
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp.magnitudes_.get()[ i ]  = lhs.magnitudes_.get()[ i ] - rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

double operator *(const EuclideanVector& lhs, const EuclideanVector& rhs)
{
    if( lhs.size_ != rhs.size_ ) {
        stringstream ss;
        ss << "Dimensions of LHS(" << lhs.size_ << ") ";
        ss << "and RHS(" << rhs.size_ << ")" << " do not match";
        throw std::runtime_error( ss.str() );
    }
    double temp{0};
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp  = temp + lhs.magnitudes_.get()[ i ] * rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

EuclideanVector operator *(const EuclideanVector& lhs, const double scalar)
{
    EuclideanVector temp( lhs.size_ ); 
    for( int i = 0 ; i < lhs.size_ ; i++ ) {
        temp[ i ]  = lhs.magnitudes_.get()[ i ] * scalar;
    }
    return temp;
}

EuclideanVector operator *(const double scalar, const EuclideanVector& rhs)
{
    EuclideanVector temp( rhs.size_ ); 
    for( int i = 0 ; i < rhs.size_ ; i++ ) {
        temp[ i ]  = scalar * rhs.magnitudes_.get()[ i ];
    }
    return temp;
}

EuclideanVector operator /(const EuclideanVector& lhs, const double division)
{
    if( division == 0 ) {
        stringstream ss;
        ss << "Invalid vector division by 0";
        throw std::runtime_error( ss.str() );
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

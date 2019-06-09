#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include <algorithm>
#include <iterator>
#include "week01_copy/strSplit.h"

int main( int argc, char *argv[] ){
  string line;
  vector<string> arr;
  cout << "check vector arr" << "\n";
  display_vector(arr);
  cout << "<<" << "\n";
  cout << arr.size() << "\n";
  if ( argc != 2 ) {
    exit(1);
  }
  ifstream infile( argv[1] );
  if ( infile.is_open() == false ) {
    cout << "can not open file " << argv[1] << "\n";
    exit(1);
  }
  while ( getline(infile, line))
  {
    split4( line , arr, " ,|" );
    display_vector(arr);
    cout << "\n";
    cout << arr.size() << "\n";
  }
  cout << arr.size() << "\n";
  return 0;
}
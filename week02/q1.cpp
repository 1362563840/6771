#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include <algorithm>
#include <iterator>

template <class Container>
void split4(const std::string& str, Container& cont,
              const std::string& delims = " ")
{
    std::size_t current, previous = 0;
    current = str.find_first_of(delims);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find_first_of(delims, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}



void display_vector(const vector<string> &v)
{
    std::copy(v.begin(), v.end(),
        std::ostream_iterator<string>(std::cout, " "));
}


int main( int argc, char *argv[] ){
  string line;
  vector<string> arr;
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
  }
  return 0;
}
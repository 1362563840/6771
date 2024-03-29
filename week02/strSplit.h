//
// Created by matt on 9/06/19.
//

#ifndef INC_6771_STRSPLIT_H
#define INC_6771_STRSPLIT_H

#include <vector>
#include <sstream>
using namespace std;


template <class Container>
void split4(const std::string& str, Container& cont,
            const std::string& delims )
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

void display_vector(const vector<string> &v);


#endif //INC_6771_STRSPLIT_H

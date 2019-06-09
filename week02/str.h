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
            const std::string& delims = " ");

void display_vector(const vector<string> &v);


#endif //INC_6771_STRSPLIT_H

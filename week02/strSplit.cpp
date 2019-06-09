//
// Created by matt on 9/06/19.
//

//
// Created by matt on 9/06/19.
//

#include "week01_copy/strSplit.h"
#include <vector>
#include <iostream>
#include <iterator>
using namespace std;

void display_vector(const vector<string> &v)
{
  std::copy(v.begin(), v.end(),
            std::ostream_iterator<string>(std::cout, " "));
}
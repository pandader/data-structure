//
// Created by nirvana on 10/1/19.
//

#ifndef PROJECT_1_UTILITIES_H
#define PROJECT_1_UTILITIES_H

#include "iostream"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace utilities{

  void printStrVec(vector<string> const& strVec);
  vector<string> strSplit(string & inputStr, char const& delimiter = ',');
}

namespace mathUtils{

    double rounding45(double input, int idx);

}


#endif //PROJECT_1_UTILITIES_H

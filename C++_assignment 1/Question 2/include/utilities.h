//
// Created by nirvana on 10/1/19.
//

#ifndef PROJECT_1_UTILITIES_H
#define PROJECT_1_UTILITIES_H

#include "iostream"
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

namespace utilities{

		void printStrVec(vector<string> const& strVec);
		bool case_insensitive_match(string s1, string s2);
		vector<string> strSplit(string & inputStr, char const& delimiter = ',');
}

namespace mathUtils{

		double bisection(double a, double b, std::function<double(double)> const& f, double epsilon = 1e-6, double maxIter = 100);

		double secant(double x_0, double x_1, std::function<double(double)> const& f, double epsilon = 1e-6, double maxIter = 100);

		double polynomial(double var, vector<double> & parameters); //  [a_0, ..., a_n]

}


#endif //PROJECT_1_UTILITIES_H

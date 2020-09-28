//
// Created by nirvana on 10/1/19.
//

#include "utilities.h"
#include "cmath"

vector<string> utilities::strSplit(string &inputStr, const char &delimiter) {

  string token;
  vector<string> tokens;
  istringstream ss(inputStr);

  while(getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

void utilities::printStrVec(vector<string> const &strVec){

  for(size_t i = 0; i < strVec.size(); i++)
  {
    std::cout << strVec[i] << endl;
  }
}

bool utilities::case_insensitive_match(string s1, string s2) {

	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

	if(s1 == s2) return true;
	return false;
}

void utilities::exceptionHandle(string const& msg, bool doNotThrow)
{
	if(doNotThrow)
		cout << msg <<endl;
	else
		throw invalid_argument(msg);
}

double mathUtils::rounding45(double input, long digits) {
  // check this
  double res = std::round(input * pow(10, digits));
  return res / pow(10, digits);
}

double mathUtils::normalCDF(double var) {
		return erfc(- var / sqrt(2)) / 2;
}

double mathUtils::bisection(double lower, double upper, std::function<double(double)> const& f, double epsilon, size_t maxIter)
{

	double c;
	double curErr;
	size_t idx = 1;
	while (idx <= maxIter) {
		c = (upper + lower) / 2;
		curErr = f(c);
		if (abs(curErr) <= epsilon) {
			break;
		} else {
			if (f(c) < 0 && f(c) > f(lower)) lower = c;
			else if (f(c) > 0 && f(c) < f(upper)) upper = c;
		}
		idx++;
	}
	if(idx >= maxIter)
		cout << "Reach maximum itreation before converges!" << endl;
	return c;
}

//secant method
double mathUtils::secant(double x_0, double x_1, std::function<double(double)> const& f, double epsilon, size_t maxIter) {
	double root;
	double curErr;
	size_t idx = 1;
	vector<double> res = {x_0, x_1};
	while (idx <= maxIter) {
		for (long unsigned int i = 1; i < res.size(); i++) {
			root = res[i] - (f(res[i]) * (res[i] - res[i - 1])) / (f(res[i]) - f(res[i - 1]));
			curErr = f(root);
			if (abs(curErr) <= epsilon) {
				return root;
			} else
				res.push_back(root);
		}
		idx++;
	}
	cout << "Reach maximum itreation before converges!" << endl;
}

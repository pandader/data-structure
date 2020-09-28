//
// Created by nirvana on 10/1/19.
//

#include "utilities.h"


// parameters of n polynomial : [a_0, ..., a_n]
double mathUtils::polynomial(double var, vector<double> & parameters)
{
  double res = 0.;
  for(size_t i = 0; i < parameters.size(); i++){
    res += pow(var, i) * parameters[i];
  }
  return res;
}

double mathUtils::bisection(double a, double b, std::function<double(double)> const& f, double epsilon, double maxIter)
{
	assert(a < b);
	double c;
	double curErr;
	size_t it = 1;
	while (it <= maxIter) {
		c = (a + b) / 2;
		curErr = f(c);
		if (abs(curErr) <= epsilon) {
			break;
		}else if((f(a) > 0 && f(b) > 0)|| (f(a) < 0 && f(b) < 0))
			throw std::invalid_argument("f(a) and f(b) have the same sign!");
		else {
			if(f(a) < f(b)) {
				if (f(c) < 0 && f(c) > f(a)) a = c;
				else if (f(c) > 0 && f(c) < f(b)) b = c;
			}else{
				if (f(c) < 0 && f(c) > f(b)) b = c;
				else if(f(c) > 0 && f(c) < f(a)) a = c;
			}
		}
		it++;
	}

	if(it >= maxIter)
		cout << "Reach maximum itreation before converges!" << endl;
	return c;
}


//secant method
double mathUtils::secant(double x_0, double x_1, std::function<double(double)> const& f, double epsilon, double maxIter)
{

	double root;
	double curErr;
	size_t it = 1;
	vector<double> res = {x_0, x_1};
	while (it <= maxIter) {
		for (size_t i = 1; i < res.size(); i++) {
			root = res[i] - (f(res[i]) * (res[i] - res[i - 1])) / (f(res[i]) - f(res[i - 1]));
			curErr = f(root);
			if (abs(curErr) <= epsilon) {
				return root;
			} else
				res.push_back(root);
		}
		it++;
	}

	cout << "Reach maximum itreation before converges!" << endl;
	return nan("1");
}

vector<string> utilities::strSplit(string &inputStr, const char &delimiter) {

	string token;
	vector<string> tokens;
	istringstream ss(inputStr);

	while(getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

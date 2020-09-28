#include "utilities.h"
#include <fstream>
using namespace mathUtils;
using namespace utilities;



int main() {

	string line;
	ifstream myPoly("../../input/inputFile.txt");
	if (myPoly.is_open())
	{
		while ( getline (myPoly,line))
		{
			vector<string> content;
			vector<string> tokenize = strSplit(line, ',');
			for(auto each : tokenize){
				vector<string> temp = strSplit(each, ':');
				content.push_back(temp[1]);
			}
			vector<double> parameters, guess;
			size_t numOfParams = content.end() - content.begin() - 5;
			for_each(content.begin(), content.end() - 4, [&parameters](const string &ele) { parameters.push_back(stod(ele)); });
      for_each(content.begin() + numOfParams + 1, content.end(), [&guess](const string &ele) { guess.push_back(stod(ele)); });
			//	using std::function wrapper
			auto f = std::bind(&mathUtils::polynomial, std::placeholders::_1, parameters);
			// execution of solver
      double bisec_root = mathUtils::bisection(guess[0], guess[1], f);
      cout <<  "Bisection Gives :" << bisec_root << endl;
      double sec_root =  mathUtils::secant(guess[0], guess[1], f);
      cout << "Secant Gives: " << sec_root << endl;
		}
		myPoly.close();
	}

	return 0;

}










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

double mathUtils::rounding45(double input, int idx)
{
	vector<string> tokenize;
	string strRep = to_string(input);
	tokenize = utilities::strSplit(strRep, '.');
	std::string intStr = tokenize[0];
	std::string decStr = tokenize[1];

	// make up trailing zeros
	if(decStr.size() <= idx)
		for(size_t i = 0; i <= idx - decStr.size();i++) decStr += '0';

	long advance;
	double decSum = 0.0;
	advance = (decStr[idx] >= '5') ? 1 : 0;
	for(int i = idx - 1; i >= 0; i--)
	{

		size_t temp = decStr[i] - '0' + advance;
		if(temp > 9){
			advance = 1;
			temp = temp % 10;
		}else{
			advance = 0;
		}
		decSum += (double)temp * pow(10., -(double)(i+1));
	}

	double intSum = stod(intStr) + (double)advance + decSum;

	return intSum;
}
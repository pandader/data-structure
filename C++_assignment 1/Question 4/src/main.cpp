#include "EuropeanOption.h"

using namespace utilities;


int main()
{

	string line;
	ifstream myfile ("../../input/inputFile.txt");
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			vector<string> content;
			vector<string> tokenize = strSplit(line, ',');
			for(auto each : tokenize){
				vector<string> temp = strSplit(each, ':');
				content.push_back(temp[1]);
			}
			string optType = content[0];
      double spotPrice = stod(content[1]);
      double strikePrice = stod(content[2]);
      double interestRate= stod(content[3]);
			double volatility = stod(content[4]);
			double timeToMaturity = stod(content[5]);
			EuropeanOption option1(optType, spotPrice, strikePrice, interestRate, volatility, timeToMaturity);
			double res = option1.getPrice();
			cout << optType << " Option with Strike " << strikePrice << " and time to maturity "
			    << timeToMaturity << " is priced at : " << res <<endl;
		}
		myfile.close();
	}

	return 0;
}
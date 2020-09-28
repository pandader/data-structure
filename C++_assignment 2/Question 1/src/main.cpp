#include "AmericanOption.h"


void print2DArray(vector<vector<double>> const& vec2D)
{
	if(vec2D.size() == 0)
		cout << "Empty 2D Array!!" << endl;
	size_t width = vec2D[0].size();
	for(size_t i = 0; i < vec2D.size(); i++) {
		for (size_t j = 0; j < width; j++) {
			cout << vec2D[i][j] << " ";
		}
		cout << endl;
	}
}

int main()
{

	string optType = "call";
	double spotPrice = 158.28;
	double strikePrice = 172.5;
	double interestRate= 0.001;
	double volatility = 0.2153;
	double timeToMaturity = 1.;
	for (size_t i = 1; i <= 10; i++) {
		AmericanOption option(optType, spotPrice, strikePrice, interestRate, volatility, timeToMaturity, 10 * i);
		vector<vector<double>> lattice = option.getLattice();
		//print2DArray(lattice);
		double price = option.getPrice();
		cout << "American option price with step size " << 10 * i << " is :" << price << endl;
	}
	return 0;
}
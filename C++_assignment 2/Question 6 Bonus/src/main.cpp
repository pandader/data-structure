#include "Option.h"
#include "EuropeanOption.h"
#include "AmericanOption.h"
#include "BarrierOption.h"


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
	double strikePrice = 158.28;
	double interestRate= 0.001;
	double volatility = 0.2153;
	double timeToMaturity = 1.;
	string barType = "up-and-out";
  double barLevel = 200;
	std::shared_ptr<Option> ptr, ptr1, ptr2;
	ptr2 = std::shared_ptr<Option>(new BarrierOption(optType, barType, barLevel, spotPrice, strikePrice,
					                              interestRate, volatility, timeToMaturity, 252, 1e4));
	double price = ptr2->getPrice();
	double delta = ptr2->getDelta();
	double vega = ptr2->getVega();
	double rho = ptr2->getRho();
	double theta = ptr2->getTheta();
	cout << "Barrier option price is :" << price;
	cout << ", Delta is :" << delta;
	cout << ", Vega is :" << vega;
	cout << ", Theta is :" << theta;
	cout << ", Rho is :" << rho << endl;
	return 0;
}
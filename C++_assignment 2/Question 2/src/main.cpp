#include "Option.h"
#include "EuropeanOption.h"
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
	double strikePrice = 158.28;
	double interestRate= 0.001;
	double volatility = 0.2153;
	double timeToMaturity = 1.;
	std::shared_ptr<Option> ptr, ptr1;
	ptr = std::shared_ptr<Option>(new AmericanOption(optType, spotPrice, strikePrice, interestRate, volatility, timeToMaturity));
	ptr1 = std::shared_ptr<Option>(new EuropeanOption(optType, spotPrice, strikePrice, interestRate, volatility, timeToMaturity));
	double price = ptr->getPrice();
	double delta = ptr->getDelta();
	double vega = ptr->getVega();
	double theta = ptr->getTheta();
	double rho = ptr->getRho();
	cout << "American option price is :" << price << ", Detla is: " << delta;
	cout << ", Vega is: "<< vega << ", Theta is: " << theta << ", Rho is: " << rho << endl;

	price = ptr1->getPrice();
	delta = ptr1->getDelta();
	vega = ptr1->getVega();
	theta = ptr1->getTheta();
	rho = ptr1->getRho();
	cout << "European option price is :" << price << ", Detla is: " << delta;
	cout << ", Vega is: "<< vega << ", Theta is: " << theta << ", Rho is: " << rho << endl;
	return 0;

}
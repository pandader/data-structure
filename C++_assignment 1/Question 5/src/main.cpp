#include "EuropeanOption.h"

using namespace std;

int main() {
	string optType = "put";
	double spotPrice = 158.28;
	double strikePrice = 162.5;
	double interestRate= 0.0099;
	double timeToMaturity = 29./365.;
	double targetPrice = 0.5 * (5.80 + 6.05); // given opt value
	double strikeReset = 156;
	EuropeanOption option(optType, spotPrice, strikePrice, interestRate, timeToMaturity);
  double optPrice = option.getPrice(true);
	double impliedVol = option.getImpliedVol(targetPrice, "bisection", 0, 0.5);
	cout << "Using bisection method: " << endl;
	cout << "The implied volatlity of the option (strike = " << strikePrice << ") is: " << impliedVol << endl;
	impliedVol = option.getImpliedVol(targetPrice, "secant", 0, 0.5);
	cout << "Using secant method: " << endl;
	cout << "The implied volatlity of the option (strike = " << strikePrice << ") is: " << impliedVol << endl;
	option.setStrike(strikeReset);
	impliedVol = option.getImpliedVol(targetPrice, "bisection", 0, 1);
	cout << "Using bisection method: " << endl;
	cout << "The implied volatlity of the option (strike = " << strikeReset << ") is: " << impliedVol << endl;
	impliedVol = option.getImpliedVol(targetPrice, "secant", 0, 1);
	cout << "Using secant method: " << endl;
	cout << "The implied volatlity of the option (strike = " << strikeReset << ") is: " << impliedVol << endl;
}
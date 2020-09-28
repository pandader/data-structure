//
// Created by nirvana on 10/1/19.
//

#include "EuropeanOption.h"

using namespace utilities;
using namespace mathUtils;

EuropeanOption::EuropeanOption(
				string const& optType,
				double const& spotPrice,
				double const& strikePrice,
				double const& interestRate,
				double const& volatility,
				double const& timeToMaturity):
				m_optType(optType),
				m_spotPrice(spotPrice),
				m_strikePrice(strikePrice),
				m_interestRate(interestRate),
				m_vol(volatility),
				m_timeToMaturity(timeToMaturity)
{
	checkConstraints();
}

EuropeanOption::EuropeanOption(
				string const& optType,
				double const& spotPrice,
				double const& strikePrice,
				double const& interestRate,
				double const& timeToMaturity):
				m_optType(optType),
				m_spotPrice(spotPrice),
				m_strikePrice(strikePrice),
				m_interestRate(interestRate),
				m_vol(INF),
				m_timeToMaturity(timeToMaturity)
{
}

void EuropeanOption::checkConstraints()
{
	if(!(case_insensitive_match(m_optType, "Call") || case_insensitive_match(m_optType, "Put")))
		throw std::invalid_argument( "Option Type can only be Put or Call!!" );
	if(m_spotPrice < 0)
		throw std::invalid_argument( "Spot Price should be greater than or equal to 0!!" );
	if(m_strikePrice < 0)
		throw std::invalid_argument( "Strike Price should be greater than or equal to 0!!" );
	if(m_vol < 0)
		throw std::invalid_argument( "Volatility should be greater than or equal to 0!!" );
	if(m_timeToMaturity < 0 )
		throw std::invalid_argument( "Time should be greater than or equal to 0!!" );
}

void EuropeanOption::assertionVar(bool doNotThrow)
{
	if(m_optType.empty()) {
		exceptionHandle("Please Enter Valid Option Type!!", doNotThrow);
	}else{
		if(!(case_insensitive_match(m_optType, "Call") || case_insensitive_match(m_optType, "Put")))
			exceptionHandle("Option Type can only be Put or Call!!", doNotThrow);
	}
	if(m_spotPrice == INF)
		exceptionHandle("Spot Price is not Intialized", doNotThrow);
	else{
		if(m_spotPrice < 0)
			exceptionHandle( "Spot Price should be greater than or equal to 0!!", doNotThrow);
	}
	if(m_strikePrice == INF)
		exceptionHandle("Strike Price is not Intialized", doNotThrow);
	else{
		if(m_strikePrice < 0)
			exceptionHandle("strike Price should be greater than or equal to 0!!", doNotThrow);
	}
	if(m_vol == INF)
		exceptionHandle("Volatility is not Intialized", doNotThrow);
	else{
		if(m_vol < 0)
			exceptionHandle("Volatility should be greater than or equal to 0!!", doNotThrow);
	}
	if(m_timeToMaturity == INF)
		exceptionHandle("Time to maturity is not intialized", doNotThrow);
	else{
		if(m_timeToMaturity < 0)
			exceptionHandle( "Time To Maturity should be greater than or equal to 0!!", doNotThrow);
	}
}

double EuropeanOption::getPrice(bool doNotThrow)
{
  // throw exception if not initialized
	assertionVar(doNotThrow);

	return blackShcoles();
}

double EuropeanOption::getImpliedVol(double const& optPrice, string const& method,
				                             double lower, double upper, double const& epsilon)
{
	auto f = std::bind(&EuropeanOption::objectiveFun, *this, std::placeholders::_1, optPrice);
	if(case_insensitive_match(method, "bisection")) {
		return bisection(lower, upper, f, epsilon);
	}else {
		return secant(lower, upper, f, epsilon);
	}
}

double EuropeanOption::objectiveFun(double vol, double const& benchmark)
{
	return  blackShcoles(vol) - benchmark;
}

double EuropeanOption::blackShcoles(
				double volatility,
				double spotPrice,
				double strikePrice,
				double interestRate,
				double timeToMatrutiy,
				string   optType)
{
	volatility = (volatility == INF) ? m_vol : volatility;
	spotPrice = (spotPrice == INF) ? m_spotPrice : spotPrice;
	strikePrice = (strikePrice == INF) ? m_strikePrice : strikePrice;
	interestRate = (interestRate == INF) ? m_interestRate : interestRate;
	timeToMatrutiy = (timeToMatrutiy == INF) ? m_timeToMaturity : timeToMatrutiy;
  optType = (optType == "") ? m_optType : optType;

  // calculation
	double d1 = (1 / (volatility * sqrt(timeToMatrutiy))) * (log(spotPrice / strikePrice)
	                                                         + (interestRate + (pow(volatility,2) / 2)) * timeToMatrutiy);
	double d2 = d1 - volatility * sqrt(timeToMatrutiy);

	if (case_insensitive_match(optType, "call"))
		return normalCDF(d1) * spotPrice - normalCDF(d2) * strikePrice * exp(-interestRate * timeToMatrutiy);
	else
		return normalCDF(-d2) * strikePrice * exp(- interestRate * timeToMatrutiy) - normalCDF(-d1) * spotPrice;

}





//double EuropeanOption::getPrice(size_t scheme, double benchMark)
//{
//	// res
//	double res(0.0);

	// check scheme valid
//	if(scheme != BLACK_SCHOLES)
//		throw std::invalid_argument( "Please Provide an Valid Method" );

	// check all variables are provided
//	assertionVar();

//	switch(scheme) {
//		case BLACK_SCHOLES:
//			res = blackShcoles();
//			break;
//		default:
//			// default implementation is black shcoles
//			res = blackShcoles();
//	}
//
//	return res - benchMark; // in default mode, it is just pv
//}

//double EuropeanOption::getImpliedVol(double optPrice)
//{
//	auto f = std::bind(&EuropeanOption::meanSquareError, *this, std::placeholders::_1);
//	return bisection(0, 1e4, f);
//}

//double EuropeanOption::setStrike(double strike)
//{
//	return 0;
//}
//


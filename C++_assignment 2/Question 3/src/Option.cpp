//
// Created by nirvana on 11/10/19.
//

#include "Option.h"
using namespace utilities;

void Option::checkConstraints(bool doNotThrow)
{
	if(!(case_insensitive_match(m_optType, "Call") || case_insensitive_match(m_optType, "Put")))
		exceptionHandle( "Option Type can only be Put or Call!!", doNotThrow);
	if(m_spotPrice < 0)
		exceptionHandle( "Spot Price should be great than or equal to 0!!", doNotThrow);
	if(m_strikePrice <0)
		exceptionHandle( "strike Price should be great than or equal to 0!!", doNotThrow);
	if(m_vol < 0)
		exceptionHandle("volatility should be great than or equal to 0!!", doNotThrow);
	if(m_timeToMaturity < 0 )
		exceptionHandle( "time to maturity should be great than or equal to 0!!", doNotThrow);
}

// compute greeks
double Option::getDelta(const double &bumpSize)
{
	double basePrice = getPrice();
	m_spotPrice += bumpSize;
	double bumpedPrice = getPrice();
	m_spotPrice -= bumpSize;
	return (bumpedPrice - basePrice) / bumpSize;
}

// compute greeks
double Option::getRho(const double &bumpSize){
	double basePrice = getPrice();
	m_interestRate += bumpSize;
	double bumpedPrice = getPrice();
	m_interestRate -= bumpSize;
	return (bumpedPrice - basePrice) / bumpSize;
}

// compute greeks
double Option::getVega(const double &bumpSize){
	double basePrice = getPrice();
	m_vol += bumpSize;
	double bumpedPrice = getPrice();
	m_vol -= bumpSize;
	return (bumpedPrice - basePrice) / bumpSize;
}

// compute greeks
double Option::getTheta(const double &bumpSize){
	double basePrice = getPrice();
	m_timeToMaturity += bumpSize;
	double bumpedPrice = getPrice();
	m_timeToMaturity -= bumpSize;
	return (bumpedPrice - basePrice) / bumpSize;
}

//
// Created by nirvana on 10/1/19.
//

#include "EuropeanOption.h"

using namespace utilities;
using namespace mathUtils;

EuropeanOption::EuropeanOption(
				string      & optType,
				double const& spotPrice,
				double const& strikePrice,
				double const& interestRate,
				double const& volatility,
				double const& timeToMaturity,
				bool          doNotThrow):
				m_optType(optType),
				m_spotPrice(spotPrice),
				m_strikePrice(strikePrice),
				m_interestRate(interestRate),
				m_vol(volatility),
				m_timeToMaturity(timeToMaturity)
{
	checkConstraints(doNotThrow);
}

void EuropeanOption::checkConstraints(bool doNotThrow)
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

double EuropeanOption::getPrice()
{
	// calculation
	double d1 = (1 / (m_vol * sqrt(m_timeToMaturity))) * (log(m_spotPrice / m_strikePrice)
					  + (m_interestRate + (pow(m_vol,2) / 2)) * m_timeToMaturity);
	double d2 = d1 - m_vol * sqrt(m_timeToMaturity);

	if (case_insensitive_match(m_optType, "Call"))
		return normalCDF(d1) * m_spotPrice - normalCDF(d2) * m_strikePrice * exp(-m_interestRate * m_timeToMaturity);
	else
		return normalCDF(-d2) * m_strikePrice * exp(- m_interestRate * m_timeToMaturity) - normalCDF(-d1) * m_spotPrice;
}


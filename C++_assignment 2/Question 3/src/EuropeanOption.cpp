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
				bool          doNotThrow) :
				Option(optType, spotPrice, strikePrice, interestRate, volatility, timeToMaturity, doNotThrow)
{
	m_exerciseType = "EuropeanOption";
	checkConstraints(doNotThrow);
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


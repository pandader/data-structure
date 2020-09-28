//
// Created by nirvana on 11/10/19.
//

#include "AmericanOption.h"
#include "math.h"

using namespace utilities;
using namespace mathUtils;

AmericanOption::AmericanOption(
				string      & optType,
				double const& spotPrice,
				double const& strikePrice,
				double const& interestRate,
				double const& volatility,
				double const& timeToMaturity,
				size_t const& numOfSteps,
				bool          doNotThrow):
				m_optType(optType),
				m_spotPrice(spotPrice),
				m_strikePrice(strikePrice),
				m_interestRate(interestRate),
				m_vol(volatility),
				m_timeToMaturity(timeToMaturity),
				m_numOfSteps(numOfSteps)
{
	checkConstraints(doNotThrow);

	// initialize the 2d grid
	m_len = m_timeToMaturity / (double)m_numOfSteps;
	m_lattice.resize(m_numOfSteps + 1, vector<double>(m_numOfSteps + 1, 0));
  m_vlattice.resize(m_numOfSteps + 1, vector<double>(m_numOfSteps + 1, 0));
  // set default, will change to match volatility
  m_u = exp(m_vol * sqrt(m_len));
  m_d = 1 / m_u;
	m_p = 0.5 + m_interestRate / 2 / m_vol * sqrt(m_len);
	m_q = 1 - m_p;

	// evolve underlying
	forwardFeed();
}

void AmericanOption::checkConstraints(bool doNotThrow)
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

void AmericanOption::forwardFeed()
{
	m_lattice[0][0] = m_spotPrice;
	for(size_t i = 1; i <= m_numOfSteps; i++)
	{
		for(size_t j = 0; j <= i - 1; j++) {
			m_lattice[j][i] = m_lattice[j][i-1] * m_u;
		}
		m_lattice[i][i] = m_lattice[i - 1][i - 1] * m_d;
	}
}

void AmericanOption::backwardProp()
{
	double sign = case_insensitive_match(m_optType, "call") ? 1. : -1.;
	for(size_t i = 0; i <= m_numOfSteps; i++) {
		m_vlattice[i][m_numOfSteps] = max((m_lattice[i][m_numOfSteps] - m_strikePrice) * sign, 0.);
	}

	for(int i = m_numOfSteps - 1; i >= 0; i--)
	{
		double df = exp(-m_interestRate * m_len);
		for(size_t j = 0; j <= i; j++) {
			double stopValue = (m_lattice[j][i] - m_strikePrice) * sign;
			double contValue = (m_vlattice[j][i + 1] * m_p + m_vlattice[j + 1][i + 1] * m_q) * df;
			m_vlattice[j][i] = max(stopValue, contValue);
		}
	}
}

double AmericanOption::getPrice()
{
	// backward propogation
  backwardProp();
  return m_vlattice[0][0];
}
//
// Created by nirvana on 11/10/19.
//

#ifndef PROJECT_1_OPTION_H
#define PROJECT_1_OPTION_H

#include "std_lib_facilities.h"
#include "utilities.h"

// base class
class Option {

public:

		Option(string       & optType,
				   double const & spotPrice,
					 double  const & strikePrice,
					 double const & interestRate,
					 double const & volatility,
					 double const & timeToMaturity,
					 bool doNotThrow = true) :
					   m_optType(optType),
						 m_spotPrice(spotPrice),
						 m_strikePrice(strikePrice),
						 m_interestRate(interestRate),
						 m_vol(volatility),
						 m_timeToMaturity(timeToMaturity) {};

		// check constraints
		void checkConstraints(bool doNotThrow);

		// get_price
		virtual double getPrice() {return 0.;};

		// set price
		void setSpotPrice(double const& price) { m_spotPrice = price; }
		void setVolatility(double const& vol) { m_vol  =  vol; }
		void setTimeToMaturity(double const& t) { m_timeToMaturity  = t; }
		void setRiskFreeRate(double const& r) { m_interestRate  = r; }

		// compute greeks
		double getDelta(double const& bumpSize = 1e-4);
		double getVega(double const& bumpSize = 1e-4);
		double getRho(double const& bumpSize = 1e-4);
		double getTheta(double const& bumpSize = 1e-4);

		// member variable
		string  m_optType;
		double  m_spotPrice;
		double  m_strikePrice;
		double  m_interestRate;
		double  m_vol;
		double  m_timeToMaturity;

};


#endif //PROJECT_1_OPTION_H

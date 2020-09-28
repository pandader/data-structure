//
// Created by nirvana on 10/1/19.
//

#ifndef PROJECT_1_EUROPEANOPTION_H
#define PROJECT_1_EUROPEANOPTION_H

#include <functional>

#include "std_lib_facilities.h"
#include "utilities.h"

static double INF = std::numeric_limits<double>::infinity();

class EuropeanOption {

public:
    // member function
    // constructor 1
    EuropeanOption(string const & optType,
                   double const & spotPrice,
                   double const & strikePrice,
                   double const & interestRate,
                   double const & volatility,
                   double const & timeToMaturity);

    // constructor
    EuropeanOption(string const & optType,
                   double const & spotPrice,
                   double const & strikePrice,
                   double const & interestRate,
                   double const & timeToMaturity);

    // check constraints
    void checkConstraints();
    // check if var is initialized
    void assertionVar(bool doNotThrow);
    // get price
    double getPrice(bool doNotThrow = false);
	  // implied volatility
	  double getImpliedVol(double const& optPrice, string const& method,
	  				             double lower = 0., double upper = 1., double const& epsilon = 1e-6);
	  // get price 1, only allow volatility
	  double objectiveFun(double vol, double const& benchmark);
		// calculation funciton
	  double blackShcoles(double volatility = INF,
	                      double spotPrice = INF,
	  				            double strikePrice = INF,
	  				            double interestRate = INF,
	  				            double timeToMatrutiy = INF,
					              string optType = "");
	  // setters : strike set
		void setStrike(double strike) { m_strikePrice = strike; };


    // member variable
    string  m_optType;
    double  m_spotPrice;
    double  m_strikePrice;
    double  m_interestRate;
    double  m_vol;
    double  m_timeToMaturity;

};



#endif //PROJECT_1_EUROPEANOPTION_H

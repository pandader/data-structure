//
// Created by nirvana on 11/10/19.
//

#ifndef PROJECT_1_AMERICANOPTION_H
#define PROJECT_1_AMERICANOPTION_H


#include "utilities.h"

class AmericanOption {

public:
    // member function
    // constructor 1
    AmericanOption(string       & optType,
                   double const & spotPrice,
                   double const & strikePrice,
                   double const & interestRate,
                   double const & volatility,
                   double const & timeToMaturity,
                   size_t const & numOfSteps = 3,
                   bool doNotThrow = true);

    // check constraints
    void checkConstraints(bool doNotThrow);
		// get lattice
		vector<vector<double>> getLattice() const { return m_lattice; };
    // forward populate underlying price on lattice
    void forwardFeed();
    // backward propgation
	  void backwardProp();

    // get_price
    double getPrice();

    // member variable
    string  m_optType;
    double  m_spotPrice;
    double  m_strikePrice;
    double  m_interestRate;
    double  m_vol;
    double  m_timeToMaturity;

    size_t  m_numOfSteps;
    double  m_len;
    double  m_u;
    double  m_d;
    double  m_p; // up
    double  m_q; // dn
    vector<vector<double>> m_lattice;
		vector<vector<double>> m_vlattice;
};


#endif //PROJECT_1_AMERICANOPTION_H






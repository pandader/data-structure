//
// Created by nirvana on 10/1/19.
//

#ifndef PROJECT_1_EUROPEANOPTION_H
#define PROJECT_1_EUROPEANOPTION_H

#include "std_lib_facilities.h"
#include "utilities.h"

class EuropeanOption {

public:
    // member function
    // constructor 1
    EuropeanOption(string       & optType,
                   double const & spotPrice,
                   double const & strikePrice,
                   double const & interestRate,
                   double const & volatility,
                   double const & timeToMaturity,
                   bool doNotThrow = true);

    // check constraints
    void checkConstraints(bool doNotThrow);
    // get_price
    double getPrice();

    // member variable
    string  m_optType;
    double  m_spotPrice;
    double  m_strikePrice;
    double  m_interestRate;
    double  m_vol;
    double  m_timeToMaturity;

};



#endif //PROJECT_1_EUROPEANOPTION_H

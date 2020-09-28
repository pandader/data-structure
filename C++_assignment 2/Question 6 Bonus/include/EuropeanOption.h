//
// Created by nirvana on 10/1/19.
//

#ifndef PROJECT_1_EUROPEANOPTION_H
#define PROJECT_1_EUROPEANOPTION_H

#include "Option.h"
#include "std_lib_facilities.h"
#include "utilities.h"

class EuropeanOption : public Option{

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

    // get_price
    double getPrice() override;

};



#endif //PROJECT_1_EUROPEANOPTION_H

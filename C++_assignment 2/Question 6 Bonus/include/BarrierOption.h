//
// Created by nirvana on 11/11/19.
//

#ifndef PROJECT_1_BARRIEROPTION_H
#define PROJECT_1_BARRIEROPTION_H

#include "utilities.h"
#include "Option.h"

class BarrierOption : public Option {

public:
		// member function
		// constructor 1
		BarrierOption(string       & optType,
		              string       & barrierType,
		              double const & barrierLevel,
		              double const & spotPrice,
		              double const & strikePrice,
		              double const & interestRate,
		              double const & volatility,
		              double const & timeToMaturity,
		              size_t const & numOfSteps = 252,
		              size_t const & numOfPaths = 1e4,
		              bool doNotThrow = true);

		// simulate mc path
		double simulatePath();
		// get_price
		double getPrice() override;

		// barrier memebrs
		string m_barType;
		double m_barLevel;

		// extra members
		double  m_len;
		size_t  m_numOfSteps;
		size_t  m_numOfPaths;
};

#endif //PROJECT_1_BARRIEROPTION_H

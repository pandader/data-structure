//
// Created by nirvana on 11/11/19.
//

#include "BarrierOption.h"
#include "math.h"


using namespace utilities;
using namespace mathUtils;

BarrierOption::BarrierOption(
				string       & optType,
				string       & barrierType,
				double const & barrierLevel,
				double const & spotPrice,
				double const & strikePrice,
				double const & interestRate,
				double const & volatility,
				double const & timeToMaturity,
				size_t const & numOfSteps,
				size_t const & numOfPaths,
				bool doNotThrow):
				Option(optType, spotPrice, strikePrice, interestRate, volatility, timeToMaturity, doNotThrow),
				m_barType(barrierType),
				m_barLevel(barrierLevel),
				m_numOfSteps(numOfSteps),
				m_numOfPaths(numOfPaths)
{
	checkConstraints(doNotThrow);

	// check additional constraints
	if(!(case_insensitive_match(barrierType, "up-and-out")
	     ||case_insensitive_match(barrierType, "down-and-out")
	     ||case_insensitive_match(barrierType, "up-and-in")
	     ||case_insensitive_match(barrierType, "down-and-in")))
		exceptionHandle( "Option Type can only be Put or Call!!", doNotThrow);

	if(m_barLevel < 0)
		exceptionHandle( "Barrier Level should be great than or equal to 0!!", doNotThrow);

}

double BarrierOption::simulatePath()
{
	std::random_device rd;
	std::mt19937 gen(0);
	std::normal_distribution<double> distribution(0, sqrt(m_len));
	bool isValid = false;
	vector<double> path;
	path.push_back(m_spotPrice);
	for(size_t i = 1; i <= m_numOfSteps; i++)
	{
		double normalRV = distribution(gen);
		double s_next = path.back() + m_interestRate * path.back() * m_len + m_vol * normalRV * path.back();
    if(case_insensitive_match(m_barType, "up-and-out")){
    	if(s_next > m_barLevel) return -1.;
    	else path.push_back(s_next);
    }else if(case_insensitive_match(m_barType, "down-and-out")){
    	if(s_next < m_barLevel) return -1.;
    	else path.push_back(s_next);
    }else if(case_insensitive_match(m_barType, "up-and-in")
           ||case_insensitive_match(m_barType, "down-and-in")){
    	if(isValid){
    		path.push_back(s_next);
    	}else{
    		if(s_next > m_barLevel){
    			isValid = true;
    			path.push_back(s_next);
    		}else continue;
    	}
    }else if(case_insensitive_match(m_barType, "down-and-in")){
	    if(isValid){
		    path.push_back(s_next);
	    }else{
		    if(s_next < m_barLevel){
			    isValid = true;
			    path.push_back(s_next);
		    }else continue;
	    }
    } else
    	path.push_back(s_next);
	}

	if(case_insensitive_match(m_barType, "up-and-out") ||case_insensitive_match(m_barType, "down-and-out"))
		return path.back();
	else{
		if(!isValid) return -1.;
		else return path.back();
	}
}


double BarrierOption::getPrice()
{
	// time interval
	m_len = m_timeToMaturity / (double)m_numOfSteps;
	double runningSum = 0;
	for(size_t i = 0; i < m_numOfPaths; i++){
		double terminalValue = simulatePath();
		if(terminalValue == -1)
			continue;
		else{
			if(case_insensitive_match(m_optType, "call")) runningSum += max(terminalValue - m_strikePrice, 0.);
			else runningSum += max(m_strikePrice - terminalValue, 0.);
		}
	}
	return (runningSum / (double)m_numOfPaths) * exp(-m_interestRate * m_timeToMaturity);
}
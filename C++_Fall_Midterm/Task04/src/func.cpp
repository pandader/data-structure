//
// Created by nirvana on 10/25/19.
//
#include "std_lib_facilities.h"
#include "func.h"

double sampleMean(vector<double> const& v)
{
	if(v.size() == 0){
		error("the vector is empty!");}
	double runningSum = 0.;
	for(size_t i = 0; i < v.size(); i++)
	runningSum += v[i];

	return runningSum / v.size();
}


double sampleVar(vector<double> const& v)
{
	if(v.size() == 0){
		error("the vector is empty!");}
	double rawSum;
	double mean;
	mean = sampleMean(v);

	for(size_t i = 0; i < v.size(); i++)
	{
	rawSum += pow(v[i] - mean, 2);
	}

	return rawSum / v.size();
}
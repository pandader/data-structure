//
// Created by nirvana on 10/17/19.
//

#include "utilities.h"

vector<double> mathUtils::randomGen(vector<double> const &parameters, int const& sampleSize, string const& distType)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	vector<double> sample;

	if(distType == "Uniform")
	{
		double lowerBound = parameters[0];
		double upperBound = parameters[1];
		std::uniform_real_distribution<double> dist(lowerBound, upperBound);
		for(int i = 0; i < sampleSize; i++) sample.push_back(dist(gen));
	}else if(distType == "Normal") {

		double mu =  parameters[0];
		double sigma = parameters[1];
		std::normal_distribution<double> dist(mu, sigma);
		for(int i = 0; i < sampleSize; i++) sample.push_back(dist(gen));
	}
	else if(distType == "Poisson"){
		double average = parameters[0];
		std::poisson_distribution<> dist(average);
		for(size_t i=0; i < sampleSize; i++) sample.push_back(dist(gen));
	}
	else if(distType == "Binomial"){
		int trails = parameters[0];
		double success_prob = parameters[1];
		std::binomial_distribution<> dist(trails,success_prob);
		for(size_t i = 0; i < sampleSize; i++) sample.push_back(dist(gen));
	}
	else if(distType == "Gamma"){
		double alpha = parameters[0];
		double beta = parameters[1];
		std::gamma_distribution<> dist(alpha,beta);
		for(size_t i = 0;i < sampleSize; i++) sample.push_back(dist(gen));
	}

	return sample;
}

vector<double> mathUtils::statistics(vector<double> & sample)
{
	vector<double> result;
	double sum = 0.0;
	double variance = 0.0;
	double median = 0.0;

	// mean summation
	for(size_t i = 0; i < sample.size(); i++) sum += sample[i];
	double mean = sum/sample.size();

	// variance calculation
	for(size_t i = 0; i < sample.size(); ++i) variance += pow(sample[i] - mean, 2);
	variance = variance/sample.size();

	// medimum calculation
	sort(sample);
	median = (sample[sample.size()/2]+sample[sample.size()/2-1])/2;

	result.push_back(mean);
	result.push_back(sqrt(variance));
	result.push_back(median);
	return result;
}

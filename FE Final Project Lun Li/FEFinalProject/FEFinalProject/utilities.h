#pragma once
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <functional>
#include <iterator>
#include <algorithm>

using namespace std;

namespace genUtils {

	bool isTenor(string const& dateOrTenor);
	vector<string> strSplit(string const& inputStr, char const& delimiter);
	void KIRA_THROW_EXCEPTION(string const& msg, bool doNotThrow = false);
	void nvpDeserializer(string const& fileName, vector<vector<string>> & nvp);
	void data1Ddeserializer(string const& fileName, vector<string> & axis1, vector<double> & values);
	void data2Ddeserializer(string const& fileName, vector<string> & axis1, vector<string> & axis2, vector<vector<double>> & values);
}

namespace mathUtils {

	double interpLinear(double axis, double const& lx, double const& rx, double const& lv, double const& rv);

	class Interpolator1D
	{
	public:
		Interpolator1D(vector<double> const& axis1, vector<double> const& values, string const& method)
			: m_axis1(axis1), m_values(values), m_method(method) {
			if (axis1.size() == 0)
				genUtils::KIRA_THROW_EXCEPTION("Failed to create interpolator 1D");
			if (axis1.size() != values.size())
				genUtils::KIRA_THROW_EXCEPTION("Axis and Values are not consistent in dimension!");

		};

		double interpolate(double const& axis); // defalut extrapolation flat

	private:
		vector<double> m_axis1;
		vector<double> m_values;
		string m_method;
	};

	class Interpolator2D
	{
	public:
		Interpolator2D(vector<double> const& axis1, vector<double> const& axis2, vector<vector<double>> const& values, string const& method)
			: m_axis1(axis1), m_axis2(axis2), m_values(values), m_method(method) {
			if (axis1.size() == 0 || axis2.size() == 0 || values.size() == 0)
				genUtils::KIRA_THROW_EXCEPTION("Failed to create interpolator 2D");
			if (axis1.size() != values.size() || axis2.size() != values[0].size())
				genUtils::KIRA_THROW_EXCEPTION("Dimension Inconsistency!");
		};

		double interpolate(double const& axis1, double const& axis2) const; // defalut extrapolation flat

	private:
		vector<double> m_axis1;
		vector<double> m_axis2;
		vector<vector<double>> m_values;
		string m_method;
	};

	// get standard normal cdf
	double normalCDF(double x);

	// get standard normal pdf
	double normalPDF(double x);

	// simple solver newton-raphson
	double derivFunc(double x, double const& baseValue, function<double(double)> const& f, double epsilon = 1e-6);

	double newtonRaphson(double x, function<double(double)> const& f, double epsilon = 1e-6, size_t const& maxIter = 100);

  // black scholes formula
	double blackScholesCalculator(double const& fwd, double const& vol, double const& k, double const& T, string const& optType);

	// bachillier formula
	double bachillierCalculator(double const& fwd, double const& vol, double const& k, double const& T, string const& optType);

	// error function
	double errFun(double vol, double target, double fwd, double k, double T, string optType, bool isNormal);

	// implied vol
	double impliedVol(double const& target, double const& fwd, double const& k, double const& T, string const& optType, bool isNormal);

}

namespace sabrUtils
{
	double errVol(double alpha, double beta, double nu, double rho, double targetVol, double fwd, double T);

	double sabrEquivalentLogNormalVol(
		double const& fwd,
		double const& alpha,
		double const& beta,
		double const& nu,
		double const& rho,
		double const& k,
		double const& T);

	double sabrEquivalentAlphaFromNormalSigma(
		double const& fwd,
		double const& normalVol,
		double const& beta,
		double const& nu,
		double const& rho,
		double const& k,
		double const& T);


	double sabrNormalVol(
		double const& fwd,
		double const& normalVol,
		double const& beta,
		double const& nu,
		double const& rho,
		double const& k,
		double const& T, 
		string const& optType);

}


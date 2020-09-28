#pragma once
#include "utilities.h"

bool genUtils::isTenor(string const& dateOrTenor)
{

	return (dateOrTenor.find('Y') != string::npos) || 
		     (dateOrTenor.find('M') != string::npos) || 
		     (dateOrTenor.find('D') != string::npos);
}

vector<string> genUtils::strSplit(string const& inputStr, char const& delimiter) {

	string token;
	vector<string> tokens;
	istringstream ss(inputStr);

	while (getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

void genUtils::KIRA_THROW_EXCEPTION(string const& msg, bool doNotThrow)
{
	if (doNotThrow)
		cout << msg << endl;
	else
		throw invalid_argument("KIRA THROW ERROR: " + msg);
}

void genUtils::nvpDeserializer(string const& fileName, vector<vector<string>> & nvp)
{
	string str;
	ifstream inFile;
	inFile.open(fileName);
	while (inFile >> str) {
		vector<string> pair = strSplit(str, ':');
		nvp.push_back(pair);
	}
}

void genUtils::data1Ddeserializer(string const& fileName, vector<string> & axis1, vector<double> & values)
{
	string str;
	ifstream inFile;
	inFile.open(fileName);
	while (inFile >> str) {
		vector<string> row = strSplit(str, ',');
		axis1.push_back(row[0]);
		values.push_back(stod(row[1]));
	}
}

void genUtils::data2Ddeserializer(string const& fileName, vector<string> & axis1, vector<string> & axis2, vector<vector<double>> & values)
{
	string str;
	ifstream inFile;
	inFile.open(fileName);
	size_t count = 0;
	while (inFile >> str) {
		vector<double> tmp;
		vector<string> row = strSplit(str, ',');
		if(count == 0){
			transform(row.begin(), row.end(), back_inserter(axis1), [&](const string & arg) { return arg; });
		}else if (count == 1) {
			transform(row.begin(), row.end(), back_inserter(axis2), [&](const string & arg) { return arg; });
		}else{
			transform(row.begin(), row.end(), back_inserter(tmp), [&](string & arg) {return stod(arg); });
			values.push_back(tmp);
		}
		count += 1;
	}
}

double mathUtils::interpLinear(double axis, double const& lx, double const& rx, double const& lv, double const& rv)
{
	double slope = (rv - lv) / (rx - lx);
	double intercept = rv - slope * rx;
	return axis * slope + intercept;
}

double mathUtils::Interpolator1D::interpolate(double const& axis)
{
	//extraplation
	if (axis < m_axis1[0])
		return m_values[0];
	else if (axis > m_axis1.back())
		return m_values.back();
	else {
		// interpolation
		for (size_t i = 0; i < m_axis1.size() - 1; i++)
		{
			if (axis >= m_axis1[i] && axis <= m_axis1[i + 1]) {
				if (m_method == "PIECEWISE_LEFT")
					return m_values[i];
				else if (m_method == "PIECEWISE_RIGHT")
					return m_values[i + 1];
				else if (m_method == "LINEAR")
					return interpLinear(axis, m_axis1[i], m_axis1[i + 1], m_values[i], m_values[i + 1]);
			}
		}
	}
}


double mathUtils::Interpolator2D::interpolate(double const& axis1, double const& axis2) const
{
	// double extrapolators
	if (axis1 < m_axis1[0] && axis2 < m_axis2[0])
		return m_values[0][0];
	else if (axis1 < m_axis1[0] && axis2 > m_axis2.back())
		return m_values[0].back();
	else if (axis1 > m_axis1.back() && axis2 < m_axis2[0])
		return m_values.back()[0];
	else if (axis1 > m_axis1.back() && axis2 > m_axis2.back())
		return m_values.back().back();
	// single extrapolators
	else if (axis1 < m_axis1[0] && (axis2 >= m_axis2[0] && axis2 <= m_axis2.back())) {
		for (size_t i = 0; i < m_axis2.size() - 1; i++)
		{
			if (axis2 >= m_axis2[i] && axis2 <= m_axis2[i + 1]) {
				return interpLinear(axis2, m_axis2[i], m_axis2[i + 1], m_values[0][i], m_values[0][i + 1]);
				break;
			}
		}
	}
	else if (axis1 > m_axis1.back() && (axis2 >= m_axis2[0] && axis2 <= m_axis2.back())) {
		for (size_t i = 0; i < m_axis2.size() - 1; i++)
		{
			if (axis2 >= m_axis2[i] && axis2 <= m_axis2[i + 1]) {
				return interpLinear(axis2, m_axis2[i], m_axis2[i + 1], m_values.back()[i], m_values.back()[i + 1]);
				break;
			}
		}
	}
	else if (axis2 < m_axis2[0] && (axis1 >= m_axis1[0] && axis1 <= m_axis1.back())) {
		for (size_t i = 0; i < m_axis1.size() - 1; i++)
		{
			if (axis1 >= m_axis1[i] && axis1 <= m_axis1[i + 1]) {
				return interpLinear(axis1, m_axis1[i], m_axis1[i + 1], m_values[i][0], m_values[i+1][0]);
				break;
			}
		}
	}
	else if (axis2 > m_axis2.back() && (axis1 >= m_axis1[0] && axis1 <= m_axis1.back())) {
		for (size_t i = 0; i < m_axis1.size() - 1; i++)
		{
			if (axis1 >= m_axis1[i] && axis1 <= m_axis1[i + 1]) {
				return interpLinear(axis1, m_axis1[i], m_axis1[i + 1], m_values[i].back(), m_values[i + 1].back());
				break;
			}
		}
	}
	else {
		// in the center
		for (size_t i = 0; i < m_axis1.size(); i++) {
			if (axis1 >= m_axis1[i] && axis1 <= m_axis1[i + 1]) {
				for (size_t j = 0; j < m_axis2.size(); j++) {
					if (axis2 >= m_axis2[j] && axis2 <= m_axis2[j + 1])
					{
						// four points of interests
						// interpolate vertically
						double lv = interpLinear(axis1, m_axis1[i], m_axis1[i + 1], m_values[i][j], m_values[i + 1][j]);
						double rv = interpLinear(axis1, m_axis1[i], m_axis1[i + 1], m_values[i][j + 1], m_values[i + 1][j + 1]);
						return interpLinear(axis2, m_axis2[j], m_axis2[j + 1], lv, rv);
					}
				}
			}
		}
	}
}

// get model cdf
double mathUtils::normalCDF(double x) // Phi(-∞, x) aka N(x)
{
	return std::erfc(-x / std::sqrt(2)) / 2;
}

double mathUtils::normalPDF(double x)
{
	return 1 / sqrt(2 * atan(1.0) * 4) * exp(-pow(x, 2.) / 2);
}


// simple solver newton-raphson
double mathUtils::derivFunc(double x, double const& baseValue, function<double(double)> const& f, double epsilon) 
{
	x += epsilon;
	double perturb = f(x);
	return (perturb - baseValue) / epsilon;
}

double mathUtils::newtonRaphson(double x, function<double(double)> const& f, double epsilon, size_t const& maxIter) {
	size_t count = 0;
	double obj = f(x);
	double h = obj / mathUtils::derivFunc(x, obj, f, epsilon);
	while (abs(h) >= epsilon && count <= maxIter && abs(obj) >= 1e-10)
	{
		obj = f(x);
		h = obj / mathUtils::derivFunc(x, obj, f), epsilon;
		x -= h;
		count += 1;
	}
	return x;
}

// black scholes formula
double mathUtils::blackScholesCalculator(
	double const& fwd,
	double const& vol,
	double const& k,
	double const& T,
	string const& optType)
{
	if (vol < 0)
		genUtils::KIRA_THROW_EXCEPTION("Volatility cannot be negative!");
	if (T < 0)
		genUtils::KIRA_THROW_EXCEPTION("Expiry cannot be negative!");
	// calculation
	double d1 = (1 / (vol * sqrt(T))) * (log(fwd / k) + (pow(vol, 2) / 2) * T);
	double d2 = d1 - vol * sqrt(T);
	if (optType == "CALL")
		return mathUtils::normalCDF(d1) * fwd - mathUtils::normalCDF(d2) * k;
	else
		return mathUtils::normalCDF(-d2) * fwd - mathUtils::normalCDF(-d1) * fwd;
}

// bachillier formula
double mathUtils::bachillierCalculator(double const& fwd,
	double const& vol,
	double const& k,
	double const& T,
	string const& optType)
{
	if (vol < 0)
		genUtils::KIRA_THROW_EXCEPTION("Volatility cannot be negative!");
	if (T < 0)
		genUtils::KIRA_THROW_EXCEPTION("Expiry cannot be negative!");
	// calculation
	double d = (fwd - k) / (vol * sqrt(T));
	if (optType == "CALL")
		return (fwd - k) * mathUtils::normalCDF(d) + vol * sqrt(T) * mathUtils::normalPDF(d);
	else
		return (fwd - k) * (mathUtils::normalCDF(d) - 1) + vol * sqrt(T) * mathUtils::normalPDF(d);
}

double mathUtils::errFun(
	double vol,
	double target,
	double fwd,
	double k,
	double T,
	string optType,
	bool isNormal)
{
	double optPrice;
	if (isNormal)
		optPrice = mathUtils::bachillierCalculator(fwd, vol, k, T, optType);
	else
		optPrice = mathUtils::blackScholesCalculator(fwd, vol, k, T, optType);
	return pow((target - optPrice) * 1e4, 2.);
}

// implied vol
double mathUtils::impliedVol(
	double const& target,
	double const& fwd,
	double const& k,
	double const& T,
	string const& optType,
	bool isNormal)
{
	auto f = std::bind(&mathUtils::errFun, std::placeholders::_1, target, fwd, k, T, optType, isNormal);
	double impliedVol(0.01);
	return mathUtils::newtonRaphson(impliedVol, f);
}

double sabrUtils::sabrEquivalentLogNormalVol(
	double const& fwd,
	double const& alpha,
	double const& beta,
	double const& nu,
	double const& rho,
	double const& k,
	double const& T)
{
	if (alpha <= 0 || (beta < 0 || beta > 1) || (rho < -1 || rho > 1) || nu <= 0 || fwd < 0)
		genUtils::KIRA_THROW_EXCEPTION("Invalid SABR Parameters!");
	if (T < 0)
		genUtils::KIRA_THROW_EXCEPTION("Expiry cannot be negative!");

	double sigma;
	if (fwd == k) {
		double fab = alpha / pow(fwd, 1. - beta);
		double term1 = pow(1. - beta, 2.) / 24. * (alpha * alpha) / pow(fwd, 2. - 2 * beta);
		double term2 = 1. / 4. * (rho * beta * alpha * nu) / pow(fwd, 1. - beta);
		double term3 = (2. - 3. * rho * rho) / 24. * nu * nu;
		sigma = fab * (1 + (term1 + term2 + term3) * T);
	}else {
		double z = nu / alpha * pow(fwd * k, (1. - beta) / 2.) * log(fwd / k);
		double x_z = log((sqrt(1 - 2 * rho * z + z * z) + z - rho) / (1. - rho));
		double fkbeta = pow(fwd * k, (1. - beta) / 2.);
		double termX = pow(1. - beta, 2.) / 24. * pow(log(fwd / k), 2.);
		double termY = pow(1 - beta, 4.) / 1920. * pow(log(fwd / k), 4.);
		double term1 = alpha / (fkbeta * (1 + termX + termY));
		double term2 = term1 * (z / x_z);
		double termX1 = pow(1. - beta, 2.) / 24. * alpha * alpha / pow(fwd * k, (1. - beta));
		double termX2 = 1. / 4. * (rho * beta * nu * alpha) / fkbeta;
		double termX3 = (2. - 3 * rho * rho) / 24. * nu * nu;
		sigma = term2 * (1 + (termX1 + termX2 + termX3) * T);
	}
	return sigma;
}


double sabrUtils::errVol(double alpha, double beta, double nu, double rho, double targetVol, double fwd, double T)
{
	double logNormalVol;
	logNormalVol = sabrUtils::sabrEquivalentLogNormalVol(fwd, alpha, beta, nu, rho, fwd, T);
	return pow((targetVol - logNormalVol) * 1e4, 2.);
}

double sabrUtils::sabrEquivalentAlphaFromNormalSigma(
	double const& fwd,
	double const& normalVol,
	double const& beta,
	double const& nu,
	double const& rho,
	double const& k,
	double const& T)
{
	double targetPrice = mathUtils::bachillierCalculator(fwd, normalVol, fwd, T, "CALL");
	double logNormalVol = mathUtils::impliedVol(targetPrice, fwd, fwd, T, "CALL", false);
	double price = mathUtils::blackScholesCalculator(fwd, logNormalVol, k, T, "CALL");
	auto f = std::bind(&sabrUtils::errVol, std::placeholders::_1, beta, nu, rho, logNormalVol, fwd, T);
	double alpha(1e-4);
	return mathUtils::newtonRaphson(alpha, f);
}

double sabrUtils::sabrNormalVol(
	double const& fwd,
	double const& normalVol,
	double const& beta,
	double const& nu,
	double const& rho,
	double const& k,
	double const& T,
	string const& optType) {

	double alpha = sabrUtils::sabrEquivalentAlphaFromNormalSigma(fwd, normalVol, beta, nu, rho, fwd, T);
	double logNormalVol = sabrUtils::sabrEquivalentLogNormalVol(fwd, alpha, beta, nu, rho, k, T);
	return mathUtils::blackScholesCalculator(fwd, logNormalVol, k, T, optType);
}


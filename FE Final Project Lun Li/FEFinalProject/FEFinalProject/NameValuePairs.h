#pragma once
#include "utilities.h"

using namespace genUtils;

class NameValuePair
{
public:
	
	NameValuePair(string const& uniqueName, vector<vector<string>> const& nvp)
		: m_uniqueName(uniqueName), m_nvp(nvp), m_nvpType("") {
		initialise(); 
	};
	NameValuePair(string const& uniqueName, string const& fileName);
	void initialise();
	string getUniqueName() const { return m_uniqueName; }
	size_t numItems() const { return m_nvp.size(); };
	void displayNVP() const;
	vector<vector<string>> getNVP() const { return m_nvp; };
	void setType(string const& type) {
		m_nvpType = type;
	}
	string getValueByKey(string const& key) const { return m_map.at(key); };
	virtual string getType() const = 0;

private:
	string m_uniqueName;
	string m_nvpType;
	vector<vector<string>> m_nvp;
	map<string, string> m_map;
};

class dataConvention : public NameValuePair
{
public:
	dataConvention(string const& uniqueName, vector<vector<string>> const& nvp) :
		NameValuePair(uniqueName, nvp) {
		setType("DataConvention");
	}
	dataConvention(string const& uniqueName, string const& fileName) : NameValuePair(uniqueName, fileName) {
		setType("DataConvention");
	}
	string getType() const { return "DataConvention"; }
};

class BuildMethod : public NameValuePair
{
public:
	BuildMethod(string const& uniqueName, vector<vector<string>> const& nvp) :
		NameValuePair(uniqueName, nvp) {
		setType("BuildMethod");
	}
	BuildMethod(string const& uniqueName, string const& fileName) : NameValuePair(uniqueName, fileName) { 
		setType("BuildMethod");  
	}
	string getType() const { return "BuildMethod"; }
};

class YieldCurveBuildMethod : public BuildMethod
{
public:
	
	YieldCurveBuildMethod(string const& uniqueName, vector<vector<string>> const& nvp);
	YieldCurveBuildMethod(string const& uniqueName, string const& fileName);
	vector<string> getCalibInsts() const { return m_calibInst; }
private:
	string m_target;
	string m_cashDeposit;
	string m_fra;
	string m_swap;
	vector<string> m_calibInst;
};

class ModelBachelierModelBuildMethod : public BuildMethod
{
public:
	ModelBachelierModelBuildMethod(string const& uniqueName, vector<vector<string>> const& nvp);
	ModelBachelierModelBuildMethod(string const& uniqueName, string const& fileName);
  string getNormalVol() const { return m_normalVol; }
	string getInterpolationMethod() const { return m_interpMethod; }

private:
	string m_target;
	string m_normalVol;
	string m_interpMethod;
};

class ModelStochasticAlphaBetaRhoBuildMethod : public BuildMethod
{
public:
	ModelStochasticAlphaBetaRhoBuildMethod(string const& uniqueName, vector<vector<string>> const& nvp);
	ModelStochasticAlphaBetaRhoBuildMethod(string const& uniqueName, string const& fileName);
	string getNormalVolConvention() const { return m_normalVol; }
	string getBetaConvention() const{ return m_beta; }
	string getNuConvention() const { return m_nu; }
	string getRhoConvention() const { return m_rho; }
	string getInterpolationMethod() const { return m_interpMethod; }

private:
	string m_target;
	string m_normalVol;
	string m_beta;
	string m_rho;
	string m_nu;
	string m_interpMethod;
};
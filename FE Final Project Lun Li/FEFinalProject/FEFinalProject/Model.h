#pragma once
#include "utilities.h"
#include "Date.h"
#include "Data.h"
#include "Product.h"
#include "NameValuePairs.h"
#include "valuationEngine.h"

class valuationEngine;
class valuationEngineCashDeposit;
class valuationEngineFRA;
class valuationEngineSwap;

using namespace mathUtils;

class Model
{
public:
	virtual shared_ptr<const Model> getSubModel() const = 0;
	virtual string getModelType() const = 0;
	virtual Date getModelValueDate() const = 0;
	virtual size_t getNumOfParameters() const = 0;
	virtual void calibration() = 0;
};

class ModelYieldCurve : public Model
{
public:

	ModelYieldCurve(string & uniqueName,
		Date   & valueDate,
		shared_ptr<const YieldCurveBuildMethod> const& ycBM,
		shared_ptr<const DataCollection> const& ycDC)
		:m_uniqueName(uniqueName),
		m_valueDate(valueDate),
		m_ycBM(ycBM),
		m_ycDC(ycDC){ initialise(); };

	ModelYieldCurve(ModelYieldCurve* rhs);

	void initialise();
	shared_ptr<const Model> getSubModel() const { return shared_ptr<const ModelYieldCurve>(); };
	string getModelType() const { return "YIELD_CURVE"; }
	Date getModelValueDate() const { return m_valueDate; }
	size_t getNumOfParameters() const { return m_numParams; }
	size_t getInternalParameterPos(string const& dateOrTenor) const;
	double getDiscountingFactor(string const& dateOrTenor) const;

	// main calibration loop
	void calibration();
	// cash despoit
	void calibCashDepositUtil(string & dateOrTenor, double const& rate, string const& dataConvention);
	// fra 
	void calibFRA(string & expiryOrTenor, double const& strike, string const& dataConvention);
	// swap
	void calibSwap(string & expiryOrTenor, string & termDateOrTenor, double const& strike, string const& dataConvention);
	// Derivative of a function by bump reval
	double derivFunc(double& x, double const& baseValue, std::shared_ptr<valuationEngine> const& ve);
	// Newton Raphson
	void newtonRaphson(double& x, std::shared_ptr<valuationEngine> const& ve, double epsilon = 1e-6, size_t const& maxIter = 100);

private:
	string m_uniqueName;
	Date m_valueDate;
	shared_ptr<const DataCollection> m_ycDC;
	shared_ptr<const YieldCurveBuildMethod> m_ycBM;
	size_t m_numParams;
	vector<Date> m_dates;
	vector<string> m_tenors;
	vector<double> m_ifr;
};

class ModelBachelier : public Model
{
public:

	ModelBachelier(
		string const& uniqueName,
		Date   const& valueDate,
		shared_ptr<ModelYieldCurve const> const& ycModel,
		shared_ptr<const ModelBachelierModelBuildMethod> const& bcBM,
		shared_ptr<const DataCollection> const& bcDC);
	
	void initialise();
	shared_ptr<const Model> getSubModel() const { return dynamic_pointer_cast<const ModelYieldCurve>(m_ycModel); };
	string getModelType() const { return "BACHELIER"; }
	Date getModelValueDate() const { return m_valueDate; }
	size_t getNumOfParameters() const { return m_expiries.size() * m_tenors.size(); };
	double getNormalVol (string const& expiryDate, string const& tenorOrDate) const;
	virtual void calibration() {
	  // NOT IMPLEMENTED
	};

private:
	string m_uniqueName;
	Date m_valueDate;
	shared_ptr<ModelYieldCurve const> m_ycModel;
	shared_ptr<const DataCollection> m_bcDC;
	shared_ptr<const ModelBachelierModelBuildMethod> m_bcBM;
	shared_ptr<const Data2D> m_normalVol;
	shared_ptr<const Interpolator2D> m_volInterp;
	size_t m_numParams;
	vector<string> m_expiries;
	vector<string> m_tenors;
	vector<vector<double>> m_vols;
};

class ModelStochasticAlphaBetaRho : public Model
{
public:

	ModelStochasticAlphaBetaRho(
		string const& uniqueName,
		Date   const& valueDate,
		shared_ptr<ModelYieldCurve const> const& ycModel,
		shared_ptr<const ModelStochasticAlphaBetaRhoBuildMethod> const& sabrBM,
		shared_ptr<const DataCollection> const& sabrDC);

	void initialise();
	shared_ptr<const Model> getSubModel() const { return dynamic_pointer_cast<const Model>(m_ycModel); };
	string getModelType() const { return "SABR"; }
	Date getModelValueDate() const { return m_valueDate; }
	size_t getNumOfParameters() const { return m_expiries.size() * m_tenors.size(); };
	double getSabrParameter(string const& expiryDate, string const& tenorOrDate, string const& dataType) const;
	virtual void calibration() {
		// NOT IMPLEMENTED
	};

private:
	string m_uniqueName;
	Date m_valueDate;
	shared_ptr<ModelYieldCurve const> m_ycModel;
	shared_ptr<const DataCollection> m_sabrDC;
	shared_ptr<const ModelStochasticAlphaBetaRhoBuildMethod> m_sabrBM;
	shared_ptr<const Data2D> m_normalVol2D;
	shared_ptr<const Data2D> m_beta2D;
	shared_ptr<const Data2D> m_nu2D;
	shared_ptr<const Data2D> m_rho2D;
	shared_ptr<const Interpolator2D> m_volInterp;
	shared_ptr<const Interpolator2D> m_betaInterp;
	shared_ptr<const Interpolator2D> m_nuInterp;
	shared_ptr<const Interpolator2D> m_rhoInterp;
	size_t m_numParams;
	vector<string> m_expiries;
	vector<string> m_tenors;
	vector<vector<double>> m_vols;
	vector<vector<double>> m_beta;
	vector<vector<double>> m_nu;
	vector<vector<double>> m_rho;
};
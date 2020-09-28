#pragma once

#include "Model.h"

ModelYieldCurve::ModelYieldCurve(ModelYieldCurve* rhs)
{
	m_uniqueName = rhs->m_uniqueName;
	m_valueDate = rhs->m_valueDate;
	m_ycDC = rhs->m_ycDC;
	m_ycBM = rhs->m_ycBM;
  m_numParams = rhs->m_numParams;
	m_dates = rhs->m_dates;
	m_tenors = rhs->m_tenors;
	m_ifr = rhs->m_ifr;
}

void ModelYieldCurve::initialise()
{
	// temp var
	Date curDate;
	string dt, dc;
	vector<string> axis1;

  // loop through build methods
	for (auto& each : m_ycBM->getCalibInsts()) {
		dt = each; dc = m_ycBM->getValueByKey(dt);
		m_numParams += m_ycDC->getDataObject(dt, dc)->getNumItems();
		m_ycDC->getDataObject(dt, dc)->getAxis1(axis1);
		for (auto it = axis1.begin(); it != axis1.end(); it++)
		{
			if (isTenor(*it)) {
				if(each == "CASH_DEPOSIT"){
					std::shared_ptr<const ProductCashDeposit> tmp(new ProductCashDeposit("dummy", m_valueDate.isoFormat(), 0., *it, 1, "Buy", dc));
					m_dates.push_back(tmp->termDate());
				}else if (each == "FRA") {
					std::shared_ptr<const ProductFRA> tmp(new ProductFRA("dummy", m_valueDate.isoFormat(), 0., *it, "Buy", 1, dc));
					m_dates.push_back(tmp->termDate());
				}else if (each == "SWAP") {
					vector<string> tokenized = strSplit(*it, ' x ');
					if (tokenized.size() == 1) {
						tokenized.push_back(tokenized[0]);
						tokenized[0] = "2D"; // hard coded;
					}
					std::shared_ptr<const ProductSwap> tmp(new ProductSwap("dummy", m_valueDate.isoFormat(), 0., tokenized[0], tokenized[1], "Pay", "Buy", 1, dc));
					m_dates.push_back(tmp->termDate());
				}else
					KIRA_THROW_EXCEPTION(each + " is not supported yet!");
			}else
			  m_dates.push_back(Date(*it));
			m_ifr.push_back(0.0);
		}
		axis1.clear();
	}

	calibration();
}

void ModelYieldCurve::calibration()
{
	// temp var
	vector<string> axis1;
	vector<double> values;
	string dt, dc;
	std::shared_ptr<const Data1D> cd;
	vector<std::shared_ptr<const ProductCashDeposit>> calibCashVec;

	for (auto& each : m_ycBM->getCalibInsts())
	{
		dt = each; dc = m_ycBM->getValueByKey(each);
		if (each == "CASH_DEPOSIT")
		{
			cd = std::dynamic_pointer_cast<const Data1D>(m_ycDC->getDataObject(dt, dc));
			cd->getAxis1(axis1); cd->getValues(values);
			for (size_t i = 0; i < axis1.size(); i++) 
			{
				calibCashDepositUtil(axis1[i], values[i], dc);
			}
		}else if (each == "FRA"){
			cd = std::dynamic_pointer_cast<const Data1D>(m_ycDC->getDataObject(dt, dc));
			cd->getAxis1(axis1); cd->getValues(values);
			for (size_t i = 0; i < axis1.size(); i++)
			{
				calibFRA(axis1[i], values[i], dc);
			}
		}else if (each == "SWAP") {
			cd = std::dynamic_pointer_cast<const Data1D>(m_ycDC->getDataObject(dt, dc));
			cd->getAxis1(axis1); cd->getValues(values);
			for (size_t i = 0; i < axis1.size(); i++)
			{
				vector<string> tokenized = strSplit(axis1[i], ' x ');
				if (tokenized.size() == 1) {
					tokenized.push_back(tokenized[0]);
					tokenized[0] = "2D"; // hard coded;
				}
				calibSwap(tokenized[0], tokenized[1], values[i], dc);
			}
		}else
			KIRA_THROW_EXCEPTION(each + " is not supported yet!");
		// house keeping
		axis1.clear(); values.clear();
	}
}

// cash despoit calibration
void ModelYieldCurve::calibCashDepositUtil(string & dateOrTenor, double const& rate, string const& dataConvention) 
{
	std::shared_ptr<const ProductCashDeposit> calibCash(new ProductCashDeposit("calib_cash", m_valueDate.isoFormat(), rate, dateOrTenor, 100., "Buy", dataConvention));
	string termDate = calibCash->termDate().isoFormat();
	size_t pos = getInternalParameterPos(termDate);
	std::shared_ptr<ModelYieldCurve> calibYC = shared_ptr<ModelYieldCurve>(new ModelYieldCurve(this));
	std::shared_ptr<valuationEngineCashDeposit> ve(new valuationEngineCashDeposit(calibYC, calibCash));
	newtonRaphson((calibYC->m_ifr[pos]), ve, 1e-8, 100);
	this->m_ifr[pos] = calibYC->m_ifr[pos];
};

// fra calibration
void ModelYieldCurve::calibFRA(string & expiryOrTenor, double const& strike, string const& dataConvention)
{
	std::shared_ptr<const ProductFRA> calibFRA(new ProductFRA("calib_fra", m_valueDate.isoFormat(), strike, expiryOrTenor, "Buy", 100, dataConvention));
	string termDate = calibFRA->termDate().isoFormat();
	size_t pos = getInternalParameterPos(termDate);
	std::shared_ptr<ModelYieldCurve> calibYC = shared_ptr<ModelYieldCurve>(new ModelYieldCurve(this));
	std::shared_ptr<valuationEngineFRA> ve(new valuationEngineFRA(calibYC, calibFRA));
	newtonRaphson((calibYC->m_ifr[pos]), ve, 1e-8, 100);
	this->m_ifr[pos] = calibYC->m_ifr[pos];
};

// swap calibration
void ModelYieldCurve::calibSwap(string & expiryOrTenor, string & termDateOrTenor, double const& strike, string const& dataConvention)
{
	std::shared_ptr<const ProductSwap> calibSwap(new ProductSwap("calib_swap", m_valueDate.isoFormat(), strike, expiryOrTenor, termDateOrTenor, "Pay", "Buy", 100, dataConvention));
	string termDate = calibSwap->termDate().isoFormat();
	size_t pos = getInternalParameterPos(termDate);
	std::shared_ptr<ModelYieldCurve> calibYC = shared_ptr<ModelYieldCurve>(new ModelYieldCurve(this));
	std::shared_ptr<valuationEngineSwap> ve(new valuationEngineSwap(calibYC, calibSwap));
	newtonRaphson((calibYC->m_ifr[pos]), ve, 1e-8, 100);
	this->m_ifr[pos] = calibYC->m_ifr[pos];
}

size_t ModelYieldCurve::getInternalParameterPos(string const& dateOrTenor) const
{
	Date targetDate = m_valueDate;
	if (isTenor(dateOrTenor)) targetDate.addPeriod(dateOrTenor);
	else targetDate = Date(dateOrTenor);

	for (auto it = m_dates.begin(); it != m_dates.end() - 1; it++)
	{ 
		if (targetDate < *it && it == m_dates.begin()) {
			return 0;
		}else if (targetDate >= *it && targetDate < *(it + 1)) {
			return std::distance(m_dates.begin(), it);
		}
	}
	return m_ifr.size() - 1;
}


double ModelYieldCurve::getDiscountingFactor(string const& dateOrTenor) const
{
	Date date = m_valueDate;
	if (isTenor(dateOrTenor))
		date.addPeriod(dateOrTenor);
	else
		date = Date(dateOrTenor);
	Date curDate = m_valueDate;
	double x = m_ifr[getInternalParameterPos(dateOrTenor)];
	double dcf = Date::dateYearFraction(curDate, date);
	return exp(- x * dcf);
}


double ModelYieldCurve::derivFunc(double& x, double const& baseValue, std::shared_ptr<valuationEngine> const& ve) {
	x += 1e-6;
	ve->calculateValue();
	x -= 1e-6;
	return (ve->value() - baseValue) / 1e-6;
};


void ModelYieldCurve::newtonRaphson(double& x, std::shared_ptr<valuationEngine> const& ve, double epsilon, size_t const& maxIter) {
	size_t count = 0;
	ve->calculateValue();
	double obj = ve->value();
	double h = obj / derivFunc(x, obj, ve);
	while (abs(h) >= epsilon && count <= maxIter)
	{
		ve->calculateValue();
		obj = ve->value();
		h = obj / derivFunc(x, obj, ve);
		x -= h;
		count += 1;
	}
};


ModelBachelier::ModelBachelier(
	string const& uniqueName,
	Date   const& valueDate,
	shared_ptr<ModelYieldCurve const> const& ycModel,
	shared_ptr<const ModelBachelierModelBuildMethod> const& bcBM,
	shared_ptr<const DataCollection> const& bcDC)
	:m_uniqueName(uniqueName),
	m_valueDate(valueDate),
	m_ycModel(ycModel),
	m_bcBM(bcBM),
	m_bcDC(bcDC)
{
	initialise();
};

void ModelBachelier::initialise()
{
	vector<double> axis1Vec, axis2Vec;
	m_normalVol = std::dynamic_pointer_cast<Data2D const>(m_bcDC->getDataObject("NORMAL_VOLATILITY", m_bcBM->getValueByKey("NORMAL_VOLATILITY")));
	m_normalVol->getAxis1(m_expiries); m_normalVol->getAxis2(m_tenors); m_normalVol->getValues(m_vols);
	transform(m_expiries.begin(), m_expiries.end(), back_inserter(axis1Vec),
		[&](string const& arg) {return Date::tenorToDate(m_valueDate, arg); });
	transform(m_tenors.begin(), m_tenors.end(), back_inserter(axis2Vec),
		[&](string const& arg) {return Date::tenorToDate(m_valueDate,arg); });
	m_volInterp = shared_ptr<const Interpolator2D>(new Interpolator2D(axis1Vec, axis2Vec, m_vols, m_bcBM->getInterpolationMethod()));
}

double ModelBachelier::getNormalVol(string const& expiryDate, string const& tenorOrDate) const
{
	Date tmpDate;
	double axis1 = Date::dateYearFraction(m_valueDate, Date(expiryDate));
	if (isTenor(tenorOrDate)) {
		tmpDate = m_valueDate; tmpDate.addPeriod(tenorOrDate);
	}else
		tmpDate = Date(tenorOrDate);
	double axis2 = Date::dateYearFraction(m_valueDate, tmpDate);
	double tmp = m_volInterp->interpolate(axis1, axis2);
	if (tmp < 0)
		KIRA_THROW_EXCEPTION("Normal Volatility cannot be negative!");
	else
		return tmp;
}


ModelStochasticAlphaBetaRho::ModelStochasticAlphaBetaRho(
	string const& uniqueName,
	Date   const& valueDate,
	shared_ptr<ModelYieldCurve const> const& ycModel,
	shared_ptr<const ModelStochasticAlphaBetaRhoBuildMethod> const& sabrBM,
	shared_ptr<const DataCollection> const& sabrDC)
	:m_uniqueName(uniqueName),
	m_valueDate(valueDate),
	m_ycModel(ycModel),
	m_sabrBM(sabrBM),
	m_sabrDC(sabrDC)
{
	initialise();
};

void ModelStochasticAlphaBetaRho::initialise()
{
	vector<double> axis1Vec, axis2Vec;
	shared_ptr<const Data2D> tmp2D;
	// set up axes
	tmp2D = std::dynamic_pointer_cast<Data2D const>(m_sabrDC->getDataObject("SABR_NORMAL_VOL", m_sabrBM->getValueByKey("SABR_NORMAL_VOL")));
	tmp2D->getAxis1(m_expiries); tmp2D->getAxis2(m_tenors); 
	transform(m_expiries.begin(), m_expiries.end(), back_inserter(axis1Vec),
		[&](string const& arg) {return Date::tenorToDate(m_valueDate, arg); });
	transform(m_tenors.begin(), m_tenors.end(), back_inserter(axis2Vec),
		[&](string const& arg) {return Date::tenorToDate(m_valueDate, arg); });
	//// sabr parameters
	// normalVol
	tmp2D->getValues(m_vols);
	m_volInterp = shared_ptr<const Interpolator2D>(new Interpolator2D(axis1Vec, axis2Vec, m_vols, m_sabrBM->getInterpolationMethod()));
	// beta
	tmp2D = std::dynamic_pointer_cast<Data2D const>(m_sabrDC->getDataObject("SABR_BETA", m_sabrBM->getValueByKey("SABR_BETA")));
	tmp2D->getValues(m_beta);
	m_betaInterp = shared_ptr<const Interpolator2D>(new Interpolator2D(axis1Vec, axis2Vec, m_beta, m_sabrBM->getInterpolationMethod()));
	// nu
	tmp2D = std::dynamic_pointer_cast<Data2D const>(m_sabrDC->getDataObject("SABR_NU", m_sabrBM->getValueByKey("SABR_NU")));
	tmp2D->getValues(m_nu);
	m_nuInterp = shared_ptr<const Interpolator2D>(new Interpolator2D(axis1Vec, axis2Vec, m_nu, m_sabrBM->getInterpolationMethod()));
	// rho
	tmp2D = std::dynamic_pointer_cast<Data2D const>(m_sabrDC->getDataObject("SABR_RHO", m_sabrBM->getValueByKey("SABR_RHO")));
	tmp2D->getValues(m_rho);
	m_rhoInterp = shared_ptr<const Interpolator2D>(new Interpolator2D(axis1Vec, axis2Vec, m_rho, m_sabrBM->getInterpolationMethod()));
}

double ModelStochasticAlphaBetaRho::getSabrParameter(string const& expiryDate, string const& tenorOrDate, string const& dataType) const
{
	Date tmpDate;
	double axis1 = Date::dateYearFraction(m_valueDate, Date(expiryDate));
	if (isTenor(tenorOrDate)) {
		tmpDate = m_valueDate; tmpDate.addPeriod(tenorOrDate);
	}
	else
		tmpDate = Date(tenorOrDate);
	double axis2 = Date::dateYearFraction(m_valueDate, tmpDate);
	
	double tmp;
	if (dataType == "SABR_NORMAL_VOL") {
		tmp = m_volInterp->interpolate(axis1, axis2);
		if (tmp < 0)
			KIRA_THROW_EXCEPTION("Normal Volatility cannot be negative!");
		else
			return tmp;
	}else if (dataType == "SABR_BETA") {
		tmp = m_betaInterp->interpolate(axis1, axis2);
		if (tmp < 0 || tmp > 1)
			KIRA_THROW_EXCEPTION("Beta is in [0, 1]!");
		return tmp;
	}else if (dataType == "SABR_NU") {
		tmp = m_nuInterp->interpolate(axis1, axis2);
		if (tmp < 0)
			KIRA_THROW_EXCEPTION("Nu cannot be negative!");
		return tmp;
	}else if (dataType == "SABR_RHO") {
		tmp =  m_rhoInterp->interpolate(axis1, axis2);
		if (tmp < -1 || tmp > 1)
			KIRA_THROW_EXCEPTION("Rho is between -1 and 1!");
		else
			return tmp;
	}
}
#include "valuationEngine.h"

valuationEngineCashDeposit::valuationEngineCashDeposit(
	shared_ptr<ModelYieldCurve const> const& ycModel, 
	shared_ptr<ProductCashDeposit const> const& cdProduct)
	: m_model(ycModel), m_product(cdProduct)
{
	m_valueDate = m_model->getModelValueDate();
	// from product
	m_spotDate = m_product->spotDate();
	m_termDate = m_product->termDate();
	m_ntl = m_product->notional();
	m_rate = m_product->rate();
	m_buyOrSell = m_product->buyOrSell();
	m_sign = (m_product->buyOrSell() == "BUY") ? 1 : -1;
	m_accrued = Date::dateYearFraction(m_spotDate, m_termDate);
}

void valuationEngineCashDeposit::calculateValue()
{
	// calculate pv
	double dfMat = m_model->getDiscountingFactor(m_termDate.isoFormat());
	double spotCashFlow = m_model->getDiscountingFactor(m_spotDate.isoFormat());
	double terminalCashFlow = -(1 + m_rate * m_accrued) * dfMat;
	m_value = m_sign * m_ntl * (spotCashFlow + terminalCashFlow);
	m_pv01 = -m_sign * m_ntl * m_accrued * dfMat;
}

double valuationEngineCashDeposit::parRateOrSpread()
{
  double spotCashFlow = m_model->getDiscountingFactor(m_spotDate.isoFormat());
	return (spotCashFlow / m_model->getDiscountingFactor(m_termDate.isoFormat()) - 1) / m_accrued;
}

valuationEngineFRA::valuationEngineFRA(shared_ptr<ModelYieldCurve const> const& ycModel, shared_ptr<ProductFRA const> const& fraProduct)
	: m_model(ycModel), m_product(fraProduct)
{
	m_valueDate = ycModel->getModelValueDate();
	// from product
	m_spotDate = m_product->spotDate();
	m_settlementDate = m_product->settlementDate();
	m_termDate = m_product->termDate();
	m_ntl = m_product->notional();
	m_strike = m_product->rate();
	m_buyOrSell = m_product->buyOrSell();
	m_sign = (m_product->buyOrSell() == "BUY") ? 1 : -1;
	m_accrued = Date::dateYearFraction(m_settlementDate, m_termDate);
}

void valuationEngineFRA::calculateValue()
{
	double dfSet = m_model->getDiscountingFactor(m_settlementDate.isoFormat());
	double dfMat = m_model->getDiscountingFactor(m_termDate.isoFormat());
	m_fwdRate = 1 / m_accrued * (dfSet / dfMat - 1);
	m_value = m_sign * m_ntl * m_accrued * dfSet * (m_fwdRate - m_strike);
	m_pv01 = m_sign * m_ntl * m_accrued * dfSet;
}

valuationEngineSwap::valuationEngineSwap(shared_ptr<ModelYieldCurve const> const& ycModel, shared_ptr<ProductSwap const> const& swapProduct)
	: m_model(ycModel), m_product(swapProduct)
{
	m_valueDate = ycModel->getModelValueDate();
	// from product
	m_spotDate = m_product->spotDate();
	m_settlementDate = m_product->settlementDate();
	m_termDate = m_product->termDate();
	m_ntl = m_product->notional();
	m_strike = m_product->rate();
	m_payOrRec = m_product->payOrRec();
	m_buyOrSell = m_product->buyOrSell();
	m_sign = (m_product->buyOrSell() == "BUY") ? 1 : -1;
	m_paymentDates = m_product->paymentDates();
	m_fixingDates = m_product->fixingDates();
	m_accruedVec = m_product->accruedVec();
}

void valuationEngineSwap::calculateValue()
{
	double dfSet = m_model->getDiscountingFactor(m_settlementDate.isoFormat());
	double dfMat = m_model->getDiscountingFactor(m_termDate.isoFormat());
	double fltLeg = (dfSet - dfMat) * (m_payOrRec == "PAY" ? 1 : -1);
	m_annuity = 0.;
	for (size_t i = 0; i < m_paymentDates.size(); i++)
	{ 
		double df = m_model->getDiscountingFactor(m_paymentDates[i].isoFormat());
		m_annuity += df * m_accruedVec[i];
	}
	double fixLeg = m_strike * m_annuity * (m_payOrRec == "PAY" ? -1 : 1);
	m_value = m_sign * m_ntl * (fltLeg + fixLeg);
	m_swapRate = (dfSet - dfMat) / m_annuity;
	m_pv01 = m_sign * m_ntl * (fixLeg / m_strike);
}

valuationEngineCapFloorLet::valuationEngineCapFloorLet(
	shared_ptr<ModelBachelier const> const& bcModel,
	shared_ptr<ProductCapFloorLet const> const& cfProduct) :
	m_model(bcModel), m_product(cfProduct)
{
	m_valueDate = bcModel->getModelValueDate();
	// from product
	m_spotDate = m_product->spotDate();
	m_expiryDate = m_product->expiryDate();
	m_settlementDate = m_product->settlementDate();
	m_termDate = m_product->termDate();
	m_ntl = m_product->notional();
	m_strike = m_product->rate();
	m_callOrPut = m_product->capOrFloor() == "CAP" ? "CALL" : "PUT";
	m_buyOrSell = m_product->buyOrSell();
	m_sign = (m_product->buyOrSell() == "BUY") ? 1 : -1;
	m_accrued = Date::dateYearFraction(m_settlementDate, m_termDate);
	// generate underlying product FRA
	m_fraProduct = shared_ptr<const ProductFRA>(new ProductFRA("dummyFRA", m_valueDate.isoFormat(), 0., m_expiryDate.isoFormat(),
		                                                         "BUY", 1., m_product->getDataConvention()->getUniqueName()));
	m_subModel = std::dynamic_pointer_cast<ModelYieldCurve const>(m_model->getSubModel());
}

void valuationEngineCapFloorLet::calculateValue()
{
	double dfSet = m_subModel->getDiscountingFactor(m_settlementDate.isoFormat());
	shared_ptr<valuationEngineFRA> ve(new valuationEngineFRA(m_subModel, m_fraProduct));
	ve->calculateValue();
	m_fwdRate = ve->parRateOrSpread();
	m_normalVol = m_model->getNormalVol(m_expiryDate.isoFormat(), m_product->tenorOrDate());
	m_value = bachillierCalculator(m_fwdRate, m_normalVol, m_strike, Date::dateYearFraction(m_valueDate, m_expiryDate.isoFormat()), m_callOrPut);
	m_impliedVol = impliedVol(m_value, m_fwdRate, m_strike, Date::dateYearFraction(m_valueDate, m_expiryDate.isoFormat()), m_callOrPut, true);
	m_value *= m_sign * m_ntl	* dfSet * m_accrued;
	m_pv01 = m_sign * m_ntl * dfSet * m_accrued;
}

valuationEngineSwaption::valuationEngineSwaption(
	shared_ptr<ModelBachelier const> const& bcModel,
	shared_ptr<ProductSwaption const> const& swptProduct) :
	m_model(bcModel), m_product(swptProduct)
{
	m_valueDate = bcModel->getModelValueDate();
	// from product
	m_spotDate = m_product->spotDate();
	m_expiryDate = m_product->expiryDate();
	m_settlementDate = m_product->settlementDate();
	m_termDate = m_product->termDate();
	m_ntl = m_product->notional();
	m_strike = m_product->rate();
	m_callOrPut = m_product->payOrRec() == "PAY" ? "CALL" : "PUT";
	m_buyOrSell = m_product->buyOrSell();
	m_sign = (m_product->buyOrSell() == "BUY") ? 1 : -1;
	// generate underlying product FRA
	m_swapProduct = shared_ptr<const ProductSwap>(new ProductSwap("dummySwap", m_valueDate.isoFormat(), m_strike, m_settlementDate.isoFormat(), m_termDate.isoFormat(), 
		                                                            "REC",  "BUY", m_sign * m_ntl, m_product->getDataConvention()->getUniqueName()));
	m_subModel = std::dynamic_pointer_cast<ModelYieldCurve const>(m_model->getSubModel());
}

void valuationEngineSwaption::calculateValue()
{
	shared_ptr<valuationEngineSwap> ve(new valuationEngineSwap(m_subModel, m_swapProduct));
	ve->calculateValue();  m_swapRate = ve->parRateOrSpread();
	m_annuity = ve->annuity();

	// tenor approximation
	size_t numOfDays = Date::dateYearFraction(m_settlementDate, m_termDate);
	Date curDate = m_valueDate; curDate.addPeriod(to_string(numOfDays) + "D");
	m_normalVol = m_model->getNormalVol(m_expiryDate.isoFormat(), curDate.isoFormat());
	m_value = bachillierCalculator(m_swapRate, m_normalVol, m_strike, Date::dateYearFraction(m_valueDate, m_expiryDate.isoFormat()), m_callOrPut);
	m_impliedVol = impliedVol(m_value, m_swapRate, m_strike, Date::dateYearFraction(m_valueDate, m_expiryDate.isoFormat()), m_callOrPut, true);
	m_value *= m_sign * m_ntl	* m_annuity;
	m_pv01 = m_sign * m_ntl * m_annuity;
}

valuationEngineSABRCapFloorLet::valuationEngineSABRCapFloorLet(
	shared_ptr<ModelStochasticAlphaBetaRho const> const& sabrModel,
	shared_ptr<ProductCapFloorLet const> const& cfProduct) :
	m_model(sabrModel), m_product(cfProduct)
{
	m_valueDate = m_model->getModelValueDate();
	// from product
	m_spotDate = m_product->spotDate();
	m_expiryDate = m_product->expiryDate();
	m_settlementDate = m_product->settlementDate();
	m_termDate = m_product->termDate();
	m_ntl = m_product->notional();
	m_strike = m_product->rate();
	m_callOrPut = m_product->capOrFloor() == "CAP" ? "CALL" : "PUT";
	m_buyOrSell = m_product->buyOrSell();
	m_sign = (m_product->buyOrSell() == "BUY") ? 1 : -1;
	m_accrued = Date::dateYearFraction(m_settlementDate, m_termDate);
	// generate underlying product FRA
	m_fraProduct = shared_ptr<const ProductFRA>(new ProductFRA("dummyFRA", m_valueDate.isoFormat(), 0., m_expiryDate.isoFormat(), 
		                                                         "BUY", 1., m_product->getDataConvention()->getUniqueName()));
	m_subModel = std::dynamic_pointer_cast<ModelYieldCurve const>(m_model->getSubModel());
}

void valuationEngineSABRCapFloorLet::calculateValue()
{
	double dfSet = m_subModel->getDiscountingFactor(m_settlementDate.isoFormat());
	shared_ptr<valuationEngineFRA> ve(new valuationEngineFRA(m_subModel, m_fraProduct));
	ve->calculateValue();
	m_fwdRate = ve->parRateOrSpread();
	m_normalVol = m_model->getSabrParameter(m_expiryDate.isoFormat(), m_product->tenorOrDate(), "SABR_NORMAL_VOL");
	m_beta = m_model->getSabrParameter(m_expiryDate.isoFormat(), m_product->tenorOrDate(), "SABR_BETA");
	m_nu = m_model->getSabrParameter(m_expiryDate.isoFormat(), m_product->tenorOrDate(), "SABR_NU");
	m_rho = m_model->getSabrParameter(m_expiryDate.isoFormat(), m_product->tenorOrDate(), "SABR_RHO");
	m_value = sabrNormalVol(m_fwdRate, m_normalVol, m_beta, m_nu, m_rho, m_strike, Date::dateYearFraction(m_valueDate, m_expiryDate.isoFormat()), m_callOrPut);
	m_impliedVol = impliedVol(m_value, m_fwdRate, m_strike, Date::dateYearFraction(m_valueDate, m_expiryDate.isoFormat()), m_callOrPut, true);
	m_value *= m_sign * m_ntl	* dfSet * m_accrued;
	m_pv01 = m_sign * m_ntl * dfSet * m_accrued;
}

valuationEngineSABRSwaption::valuationEngineSABRSwaption(
	shared_ptr<ModelStochasticAlphaBetaRho const> const& sabrModel,
	shared_ptr<ProductSwaption const> const& swptProduct) :
	m_model(sabrModel), m_product(swptProduct)
{
	m_valueDate = m_model->getModelValueDate();
	// from product
	m_spotDate = m_product->spotDate();
	m_expiryDate = m_product->expiryDate();
	m_settlementDate = m_product->settlementDate();
	m_termDate = m_product->termDate();
	m_ntl = m_product->notional();
	m_strike = m_product->rate();
	m_callOrPut = m_product->payOrRec() == "PAY" ? "CALL" : "PUT";
	m_buyOrSell = m_product->buyOrSell();
	m_sign = (m_product->buyOrSell() == "BUY") ? 1 : -1;
	// generate underlying product FRA
	m_swapProduct = shared_ptr<const ProductSwap>(new ProductSwap("dummySwap", m_valueDate.isoFormat(), m_strike, m_settlementDate.isoFormat(), m_termDate.isoFormat(), 
		                                                            "REC",  "BUY", m_sign * m_ntl, m_product->getDataConvention()->getUniqueName()));
	m_subModel = std::dynamic_pointer_cast<ModelYieldCurve const>(m_model->getSubModel());
}

void valuationEngineSABRSwaption::calculateValue()
{
	shared_ptr<valuationEngineSwap> ve(new valuationEngineSwap(m_subModel, m_swapProduct));
	ve->calculateValue();  m_swapRate = ve->parRateOrSpread();
	m_annuity = ve->annuity();

	// tenor approximation
	size_t numOfDays = Date::dateYearFraction(m_settlementDate, m_termDate);
	Date curDate = m_valueDate; curDate.addPeriod(to_string(numOfDays) + "D");
	double expiryTime = Date::dateYearFraction(m_valueDate, m_expiryDate.isoFormat());
	m_normalVol = m_model->getSabrParameter(m_expiryDate.isoFormat(), curDate.isoFormat(), "SABR_NORMAL_VOL");
	m_beta = m_model->getSabrParameter(m_expiryDate.isoFormat(), curDate.isoFormat(), "SABR_BETA");
	m_nu = m_model->getSabrParameter(m_expiryDate.isoFormat(), curDate.isoFormat(), "SABR_NU");
	m_rho = m_model->getSabrParameter(m_expiryDate.isoFormat(), curDate.isoFormat(), "SABR_RHO");
	m_value = sabrNormalVol(m_swapRate, m_normalVol, m_beta, m_nu, m_rho, m_strike, expiryTime, m_callOrPut);
  m_impliedVol = impliedVol(m_value, m_swapRate, m_strike, expiryTime, m_callOrPut, true);
	m_value *= m_sign * m_ntl	* m_annuity;
	m_pv01 = m_sign * m_ntl * m_annuity;
}
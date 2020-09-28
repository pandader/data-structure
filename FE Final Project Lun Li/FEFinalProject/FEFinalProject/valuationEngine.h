#pragma once
#include "utilities.h"
#include "Model.h"
#include "Product.h"

class ModelYieldCurve;
class ModelBachelier;
class ModelStochasticAlphaBetaRho;
class ProductCashDeposit;
class ProductFRA;
class ProductSwap;
class ProductCapFloorLet;
class ProductSwaption;

using namespace mathUtils;
using namespace sabrUtils;

class valuationEngine
{
public:
	virtual void calculateValue() = 0;
	virtual string getValuationEngineType() const = 0;
	virtual double parRateOrSpread() = 0;
	virtual double pv01() const = 0;
	virtual double value() const = 0;
	virtual double normalVol() const = 0;
};

class valuationEngineCashDeposit : public valuationEngine
{
public:
	valuationEngineCashDeposit(shared_ptr<ModelYieldCurve const> const& ycCurve, shared_ptr<ProductCashDeposit const> const& cdProduct);
	string getValuationEngineType() const { return "YC_CASH_DEPOSIT_VE"; }
	void calculateValue();
	double parRateOrSpread();
	double pv01() const { return m_pv01; }
	double value() const { return m_value; }
	virtual double normalVol() const { return 0; }
private:
	shared_ptr<ModelYieldCurve const> m_model;
	shared_ptr<ProductCashDeposit const> m_product;
	Date m_valueDate;
	Date m_spotDate;
	Date m_termDate;
	string m_buyOrSell;
	double m_ntl;
	double m_sign;
	double m_accrued;
	double m_rate;
	double m_pv01;
	double m_value;
};

class valuationEngineFRA : public valuationEngine
{
public:
	valuationEngineFRA(shared_ptr<ModelYieldCurve const> const& ycCurve, shared_ptr<ProductFRA const> const& fraProduct);
	string getValuationEngineType() const { return "YC_FRA_VE"; }
	void calculateValue();
	double parRateOrSpread() { return m_fwdRate; }
	double pv01() const { return m_pv01;  }
	double value() const { return m_value; }
	virtual double normalVol() const { return 0; }
private:
	shared_ptr<ModelYieldCurve const> m_model;
	shared_ptr<ProductFRA const> m_product;
	Date m_valueDate;
	Date m_spotDate;
	Date m_settlementDate;
	Date m_termDate;
	string m_buyOrSell;
	double m_ntl;
	double m_sign;
	double m_accrued;
	double m_strike;
	double m_pv01;
	double m_value;
	double m_fwdRate;
};

class valuationEngineSwap : public valuationEngine
{
public:
	valuationEngineSwap(shared_ptr<ModelYieldCurve const> const& ycCurve, shared_ptr<ProductSwap const> const& swapProduct);
	string getValuationEngineType() const { return "YC_SWAP_VE"; }
	void calculateValue();
	double parRateOrSpread() { return m_swapRate; }
	double annuity() const { return m_annuity;  }
	double pv01() const { return m_pv01; }
	double value() const { return m_value; }
	virtual double normalVol() const { return 0; }
private:
	shared_ptr<ModelYieldCurve const> m_model;
	shared_ptr<ProductSwap const> m_product;
	Date m_valueDate;
	Date m_spotDate;
	Date m_settlementDate;
	Date m_termDate;
	string m_buyOrSell;
	string m_payOrRec;
	double m_ntl;
	double m_sign;
	double m_strike;
	double m_pv01;
	double m_annuity;
	double m_value;
	double m_swapRate;
	vector<double> m_accruedVec;
	vector<Date> m_paymentDates;
	vector<Date> m_fixingDates;
};

class valuationEngineCapFloorLet : public valuationEngine
{
public:
	valuationEngineCapFloorLet(shared_ptr<ModelBachelier const> const& bcModel, shared_ptr<ProductCapFloorLet const> const& cfProduct);
	string getValuationEngineType() const { return "BC_CAPFLOOR_VE"; }
	void calculateValue();
	double parRateOrSpread() { return m_fwdRate; }
	double pv01() const { return m_pv01; }
	double normalVol() const { return m_impliedVol; }
	double value() const { return m_value; }

private:
	shared_ptr<ModelBachelier const> m_model;
	shared_ptr<ModelYieldCurve const> m_subModel;
	shared_ptr<const ProductFRA> m_fraProduct;
	shared_ptr<ProductCapFloorLet const> m_product;
	Date m_valueDate;
	Date m_spotDate;
	Date m_expiryDate;
	Date m_settlementDate;
	Date m_termDate;
	string m_callOrPut;
	string m_buyOrSell;
	double m_ntl;
	double m_sign;
	double m_accrued;
	double m_strike;
	double m_pv01;
	double m_value;
	double m_fwdRate;
	double m_normalVol;
	double m_impliedVol;
};

class valuationEngineSwaption : public valuationEngine
{
public:
	valuationEngineSwaption(shared_ptr<ModelBachelier const> const& bcModel, shared_ptr<ProductSwaption const> const& swptProduct);
	string getValuationEngineType() const { return "BC_SWAPTION_VE"; }
	void calculateValue();
	double parRateOrSpread() { return m_swapRate; }
	double pv01() const { return m_pv01; }
	double annuity() const { return m_annuity; }
	double normalVol() const { return m_impliedVol; }
	double value() const { return m_value; }

private:
	shared_ptr<ModelBachelier const> m_model;
	shared_ptr<ModelYieldCurve const> m_subModel;
	shared_ptr<ProductSwap const> m_swapProduct;
	shared_ptr<ProductSwaption const> m_product;
	Date m_valueDate;
	Date m_spotDate;
	Date m_expiryDate;
	Date m_settlementDate;
	Date m_termDate;
	string m_callOrPut;
	string m_buyOrSell;
	double m_ntl;
	double m_sign;
	double m_accrued;
	double m_strike;
	double m_pv01;
	double m_swapRate;
	double m_annuity;
	double m_value;
	double m_normalVol;
	double m_impliedVol;
};

class valuationEngineSABRCapFloorLet : public valuationEngine
{
public:
	valuationEngineSABRCapFloorLet(shared_ptr<ModelStochasticAlphaBetaRho const> const& sabrModel, shared_ptr<ProductCapFloorLet const> const& cfProduct);
	string getValuationEngineType() const { return "SABR_CAPFLOOR_VE"; }
	void calculateValue();
	double parRateOrSpread() { return m_fwdRate; }
	double pv01() const { return m_pv01; }
	double normalVol() const { return m_impliedVol; }
	double value() const { return m_value; }

private:
	shared_ptr<ModelStochasticAlphaBetaRho const> m_model;
	shared_ptr<ModelYieldCurve const> m_subModel;
	shared_ptr<const ProductFRA> m_fraProduct;
	shared_ptr<ProductCapFloorLet const> m_product;
	Date m_valueDate;
	Date m_spotDate;
	Date m_expiryDate;
	Date m_settlementDate;
	Date m_termDate;
	string m_callOrPut;
	string m_buyOrSell;
	double m_ntl;
	double m_sign;
	double m_accrued;
	double m_strike;
	double m_pv01;
	double m_value;
	double m_fwdRate;
	double m_normalVol;
	double m_impliedVol;
	double m_beta;
	double m_nu;
	double m_rho;
};

class valuationEngineSABRSwaption : public valuationEngine
{
public:
	valuationEngineSABRSwaption(shared_ptr<ModelStochasticAlphaBetaRho const> const& sabrModel, shared_ptr<ProductSwaption const> const& swptProduct);
	string getValuationEngineType() const { return "SABR_SWAPTION_VE"; }
	void calculateValue();
	double parRateOrSpread() { return m_swapRate; }
	double pv01() const { return m_pv01; }
	double annuity() const { return m_annuity; }
	double normalVol() const { return m_impliedVol; }
	double value() const { return m_value; }

private:
	shared_ptr<ModelStochasticAlphaBetaRho const> m_model;
	shared_ptr<ModelYieldCurve const> m_subModel;
	shared_ptr<const ProductSwap> m_swapProduct;
	shared_ptr<ProductSwaption const> m_product;
	Date m_valueDate;
	Date m_spotDate;
	Date m_expiryDate;
	Date m_settlementDate;
	Date m_termDate;
	string m_callOrPut;
	string m_buyOrSell;
	double m_ntl;
	double m_sign;
	double m_accrued;
	double m_strike;
	double m_pv01;
	double m_swapRate;
	double m_annuity;
	double m_value;
	double m_normalVol;
	double m_impliedVol;
	double m_beta;
	double m_nu;
	double m_rho;
};





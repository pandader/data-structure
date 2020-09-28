#pragma once
#include "utilities.h"
#include "Date.h"
#include "NameValuePairs.h"
#include "registra.h"

class KIRA;

class Product
{

public:

	Product(string const& uniqueName, string const& valueDate, double const& ntl, string const& buyOrSell, string const& dataConv, string const& productType);

	string uniqueName() const { return m_uniqueName; }
	string valueDate() const { return m_valueDate; }
	double notional() const { return m_notional; }
	string buyOrSell() const { return m_buyOrSell; }
	string productType() const { return m_productType; }
	Date spotDate() const { return m_spotDate; }
	Date termDate() const { return m_termDate; }
	shared_ptr<dataConvention> getDataConvention() const { return m_dataConvention; }
	void setSpotDate(Date const& d) { m_spotDate = d; };
	void setTermDate(Date const& d) { m_termDate = d; }

	virtual string tenorOrDate() const = 0;
	virtual double rate() const = 0;

private:
	string m_uniqueName;
	string m_valueDate;
	Date m_spotDate;
	Date m_termDate;
	double m_notional;
	string m_buyOrSell;
	string m_productType;
	shared_ptr<dataConvention> m_dataConvention;
};

class ProductCashDeposit : public Product
{

public:
	ProductCashDeposit(
		string const& uniqueName,
		string const& valueDate,
		double const& rate,
		string const& tenorOrDate,
		double const& notional,
		string const& buyOrSell,
		string const& dataConvention);

	string offSet() const { return m_offSet; }
	string tenorOrDate() const { return m_tenorOrDate; }
	double rate() const { return m_rate; }

private:
	double m_rate;
	string m_tenorOrDate;
	string m_offSet;
};

class ProductFRA : public Product
{
public:
	ProductFRA(
		string const& uniqueName,
		string const& valueDate, 
		double const& strike,
		string const& expiryOrTenor,
		string const& buyOrSell,
		double const& ntl,
		string const& dataConvention
		);

	string offSet() const { return m_offSet; }
	string settlementDate() const { return m_settlementDate;  }
	string tenorOrDate() const { return m_tenor; }
	double rate() const { return m_strike; }

private:
	string m_settlementDate;
	string m_tenor;
	string m_offSet;
	double m_strike;
};

class ProductSwap : public Product
{
public:
	ProductSwap(
		string const& uniqueName,
		string const& valueDate,
		double const& strike,
		string const& expiryOrTenor,
		string const& tenorOrDate,
		string const& payOrRec,
		string const& buyOrSell,
		double const& ntl,
		string const& dataConvention);

  void generatePaymentDates();
	string accrualFreq() const { return m_accrualFreq; }
	string offSet() const { return m_offSet; }
	string settlementDate() const { return m_settlementDate; }
	string tenorOrDate() const { return m_tenorOrDate; }
	string payOrRec() const { return m_payOrRec; }
	double rate() const { return m_strike; }
	vector<Date> fixingDates() const { return m_fixingDates; }
	vector<Date> paymentDates() const { return m_paymentDates; }
	vector<double> accruedVec() const { return m_accured; }

private:
	double m_strike;
	string m_tenorOrDate;
	string m_settlementDate;
	string m_offSet;
	string m_accrualFreq;
	string m_payOrRec;
	vector<double> m_accured;
	vector<Date> m_fixingDates;
	vector<Date> m_paymentDates;
};

class ProductCapFloorLet : public Product
{
public:
	ProductCapFloorLet(
		string const& uniqueName,
		string const& valueDate,
		double const& strike,
		string const& expiryOrTenor,
		string const& capOrFloor,
		string const& buyOrSell,
		double const& ntl,
		string const& dataCovention
		);

	string expiryDate() const { return m_expiryDate; }
	string capOrFloor() const { return m_capOrFloor; }
	string offSet() const { return m_offSet; }
	string settlementDate() const { return m_settlementDate; }
	string tenorOrDate() const { return m_tenor; }
	double rate() const { return m_strike; }

private:
	string m_expiryDate;
	string m_settlementDate;
	string m_tenor;
	string m_offSet;
	string m_capOrFloor;
	double m_strike;
};

class ProductSwaption : public Product
{
public:
	ProductSwaption(
		string const& uniqueName,
		string const& valueDate,
		double const& strike,
		string const& expiryOrTenor,
		string const& tenorOrDate,
		string const& payOrRec,
		string const& buyOrSell,
		double const& ntl,
		string const& dataConvention
		);

	string accrualFreq() const { return m_accrualFreq; }
	string expiryDate() const { return m_expiryDate; }
	string payOrRec() const { return m_payOrRec; }
	string offSet() const { return m_offSet; }
	string settlementDate() const { return m_settlementDate; }
	string tenorOrDate() const { return m_tenor; }
	double rate() const { return m_strike; }

private:
	string m_expiryDate;
	string m_settlementDate;
	string m_tenorOrDate;
	string m_tenor;
	string m_offSet;
	string m_payOrRec;
	string m_accrualFreq;
	double m_strike;
};
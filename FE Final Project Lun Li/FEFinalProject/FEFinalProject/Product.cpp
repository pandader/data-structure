#include "Product.h"

Product::Product(string const& uniqueName, string const& valueDate, double const& ntl, string const& buyOrSell, string const& dataConv, string const& productType)
	: m_uniqueName(uniqueName),
	m_valueDate(valueDate),
	m_notional(ntl),
	m_buyOrSell(buyOrSell),
	m_productType(productType)
{
	m_dataConvention = KIRA::getInstance()->getConvention(dataConv);
};

ProductCashDeposit::ProductCashDeposit(
	string const& uniqueName,
	string const& valueDate,
	double const& rate,
	string const& tenorOrDate,
	double const& notional,
	string const& buyOrSell,
	string const& dataConvention) 
	: Product(uniqueName, valueDate, notional, buyOrSell, dataConvention, dataConvention), 
	  m_rate(rate),
	  m_tenorOrDate(tenorOrDate)
{
  // offset
	m_offSet = getDataConvention()->getValueByKey("SETTLEMENT_OFFSET");

	// calculate spot date
	Date tmpDate = Date(this->valueDate());
	tmpDate.addPeriod(m_offSet);
	setSpotDate(Date(tmpDate));

	// calculate term date
	if (isTenor(m_tenorOrDate)) {
		tmpDate = this->spotDate();
		tmpDate.addPeriod(m_tenorOrDate);
		setTermDate(Date(tmpDate));
	}
	else
		setTermDate(Date(m_tenorOrDate));
}

ProductFRA::ProductFRA(
	string const& uniqueName,
	string const& valueDate, 
	double const& strike,
	string const& expiryOrTenor,
	string const& buyOrSell,
	double const& ntl,
	string const& dataConvention
	) : Product(uniqueName, valueDate, ntl, buyOrSell, dataConvention, "FRA"),
	m_strike(strike)
{
	// tenor and offset
	m_tenor = getDataConvention()->getValueByKey("TENOR");
	m_offSet = getDataConvention()->getValueByKey("SETTLEMENT_OFFSET");

	// calculate spot date
	Date tmpDate = Date(this->valueDate());
	tmpDate.addPeriod(m_offSet);
	setSpotDate(Date(tmpDate));

	// calculate settlementDate
	if(isTenor(expiryOrTenor)){
		tmpDate.addPeriod(expiryOrTenor);
		m_settlementDate = tmpDate.isoFormat();
	}else
	  m_settlementDate = expiryOrTenor;

	// calculate term date
	tmpDate = Date(m_settlementDate);
	tmpDate.addPeriod(m_tenor);
	setTermDate(tmpDate);
}

ProductSwap::ProductSwap(
	string const& uniqueName,
	string const& valueDate,
	double const& strike,
	string const& expiryOrTenor,
	string const& tenorOrDate,
	string const& payOrRec,
	string const& buyOrSell,
	double const& ntl,
	string const& dataConvention
	) : Product(uniqueName, valueDate, ntl, buyOrSell, dataConvention, "SWAP"),
	m_strike(strike),
	m_tenorOrDate(tenorOrDate),
	m_payOrRec(payOrRec)
{

	m_offSet = getDataConvention()->getValueByKey("SETTLEMENT_OFFSET");
	m_accrualFreq = getDataConvention()->getValueByKey("TENOR");

	// calculate spot date
	Date tmpDate = Date(this->valueDate());
	tmpDate.addPeriod(m_offSet);
	setSpotDate(Date(tmpDate));

	// calculate settlementDate
	if (isTenor(expiryOrTenor)) {
		tmpDate.addPeriod(expiryOrTenor);
		m_settlementDate = tmpDate.isoFormat();
	}
	else
		m_settlementDate = expiryOrTenor;

	// calculate term date
	if (isTenor(tenorOrDate)) {
		tmpDate = Date(m_settlementDate);
		tmpDate.addPeriod(m_tenorOrDate);
		setTermDate(tmpDate);
	}
	else
		setTermDate(m_tenorOrDate);

	// calculate payment date
	generatePaymentDates();
}

void ProductSwap::generatePaymentDates()
{
	Date curDate(m_settlementDate);
	Date termDate(this->termDate());
	while (true) {
		m_fixingDates.push_back(curDate);
		curDate.addPeriod(m_accrualFreq);
		if (curDate >= termDate) {
			m_paymentDates.push_back(termDate);
			m_accured.push_back(Date::dateYearFraction(m_fixingDates.back(), termDate));
			break;
		}else{ 
			m_paymentDates.push_back(curDate);
			m_accured.push_back(Date::dateYearFraction(m_fixingDates.back(), curDate));
		}
	}
}

ProductCapFloorLet::ProductCapFloorLet(
	string const& uniqueName,
	string const& valueDate,
	double const& strike,
	string const& expiryOrTenor,
	string const& capOrFloor,
	string const& buyOrSell,
	double const& ntl,
	string const& dataConvention
	) : Product(uniqueName, valueDate, ntl, buyOrSell, dataConvention, "CAPFLOOR"),
	m_strike(strike),
	m_capOrFloor(capOrFloor)
{
	// set up
	m_tenor = getDataConvention()->getValueByKey("TENOR");
	m_offSet = getDataConvention()->getValueByKey("SETTLEMENT_OFFSET");

	// calculate spot date
	Date tmpDate = Date(this->valueDate());
	tmpDate.addPeriod(m_offSet);
	setSpotDate(Date(tmpDate));

	// calculate settlementDate
	Date setDate;
	if (isTenor(expiryOrTenor)) {
		tmpDate.addPeriod(expiryOrTenor);
		m_expiryDate = tmpDate.isoFormat();
		setDate = Date(m_expiryDate);
	}else {
		setDate = Date(expiryOrTenor);
		m_expiryDate = setDate.isoFormat();
	}
	setDate.addPeriod(m_offSet);
	m_settlementDate = setDate.isoFormat();

	// calculate term date
	setDate.addPeriod(m_tenor);
	setTermDate(setDate);
}

ProductSwaption::ProductSwaption(
	string const& uniqueName,
	string const& valueDate,
	double const& strike,
	string const& expiryOrTenor,
	string const& tenorOrDate,
	string const& payOrRec,
	string const& buyOrSell,
	double const& ntl,
  string const& dataConvention
	) : Product(uniqueName, valueDate, ntl, buyOrSell, dataConvention, "SWAPTION"),
	m_strike(strike),
	m_tenorOrDate(tenorOrDate),
	m_payOrRec(payOrRec)
{
	m_offSet = getDataConvention()->getValueByKey("SETTLEMENT_OFFSET");
	m_accrualFreq = getDataConvention()->getValueByKey("TENOR");

	// calculate spot date
	Date tmpDate = Date(this->valueDate());
	tmpDate.addPeriod(m_offSet);
	setSpotDate(Date(tmpDate));

	// calculate settlementDate
	Date setDate;
	if (isTenor(expiryOrTenor)) {
		tmpDate.addPeriod(expiryOrTenor);
		m_expiryDate = tmpDate.isoFormat();
		setDate = Date(m_expiryDate);
	}
	else {
		setDate = Date(expiryOrTenor);
		m_expiryDate = setDate.isoFormat();
	}
	setDate.addPeriod(m_offSet);
	m_settlementDate = setDate.isoFormat();

	// calculate term date
	if (isTenor(tenorOrDate)) {
		tmpDate = Date(m_settlementDate);
		tmpDate.addPeriod(m_tenorOrDate);
		setTermDate(tmpDate);
	}
	else
		setTermDate(m_tenorOrDate);

}
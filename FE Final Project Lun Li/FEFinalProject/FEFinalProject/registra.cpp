#include "registra.h"

bool KIRA::instanceFlag = false;
KIRA* KIRA::kira = NULL;

KIRA::KIRA()
{
	// data convention
	string path = STATIC_DATA + "DataConvention\\";
	vector<string> tmpList = {"CASH-USD", "USD-LIBOR-BBA-3M-FRA", "USD-LIBOR-BBA-3M", "USD-SWAP-SEMI-BOND", "USD-SWAPTION"};
	for (auto& each : tmpList) {
		shared_ptr<dataConvention> dc(new dataConvention(each, path + each + ".txt"));
		m_dataConventionMap[dc->getUniqueName()] = dc;
	}

	// valuation engine
	string modelType, productType;
	modelType = "YIELD_CURVE"; productType = "CASH_DEPOSIT";
	m_veMap[modelType + "-" + productType] = "YC_CASH_DEPOSIT_VE";
	productType = "FRA";
	m_veMap[modelType + "-" + productType] = "YC_FRA_VE";
	productType = "SWAP";
	m_veMap[modelType + "-" + productType] = "YC_SWAP_VE";
	modelType = "BACHELIER";  productType = "CAPFLOOR";
	m_veMap[modelType + "-" + productType] = "BC_CAPFLOOR_VE";
	modelType = "BACHELIER";  productType = "SWAPTION";
	m_veMap[modelType + "-" + productType] = "BC_SWAPTION_VE";
	modelType = "SABR"; productType = "CAPFLOOR";
	m_veMap[modelType + "-" + productType] = "SABR_CAPFLOOR_VE";
	productType = "SWAPTION";
	m_veMap[modelType + "-" + productType] = "SABR_SWAPTION_VE";
}

shared_ptr<BuildMethod> KIRA::getBuildMethod(string const& bm, string const& bmType) const
{
	// build methods
	string path = STATIC_DATA + "BuildMethod\\";
	return shared_ptr<BuildMethod>(new BuildMethod(bm, STATIC_DATA + "BuildMethod\\" + bmType + ".txt"));
}



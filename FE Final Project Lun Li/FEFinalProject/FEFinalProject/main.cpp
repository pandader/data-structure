#include "Model.h"
#include "Product.h"
#include "Data.h"
#include "registra.h"
#include "interface.h"

using namespace std;
using namespace interface;

int main()
{
	// load library
	KIRA *kira = kira->getInstance();

	// declare a yield curve model and vol model built on top of it
	kiraCreateModel("LIVE-YC", "2019-11-26", "YIELD_CURVE", "MarketData", "BuildMethod");
  kiraCreateVolModel("LIVE-SABR", "LIVE-YC", "SABR", "MarketData", "BuildMethod");

	// create product
	vector<double> res;
	kiraCreateProduct("Swap1", "SWAP", "2019-11-26", 3.3*1e-2, "2020-03-03", 1e6, "BUY", "USD-SWAP-SEMI-BOND", "5Y", "REC");
	kiraCreateProduct("Swap2", "SWAP", "2019-11-26", 2.56*1e-2, "2D", 2*1e4, "SELL", "USD-SWAP-SEMI-BOND", "2Y", "PAY");
	kiraCreateProduct("Fra1", "FRA", "2019-11-26", 2.35*1e-2, "4M", 1e6, "BUY", "USD-LIBOR-BBA-3M-FRA", "3M", "PAY");
	kiraCreateProduct("Fra2", "FRA", "2019-11-26", 2.4*1e-2, "6M", 3e6, "BUY", "USD-LIBOR-BBA-3M-FRA", "3M", "REC");
	kiraCreateProduct("cap", "CAPFLOOR", "2019-11-26", 2.65*1e-2, "2021-05-02", 4.0*1e6, "BUY", "USD-LIBOR-BBA-3M-FRA", "3M", "CAP");
	kiraCreateProduct("swpt", "SWAPTION", "2019-11-26", 3.43*1e-2, "10Y", 1.0*1e6, "SELL", "USD-SWAPTION", "10Y", "PAY");
	kiraCreateValueReport(res, "LIVE-SABR", "swpt", "PARRATEORSPREAD"); cout << res.back() << endl; res.clear();
	//kiraCreateValueReport(res, "LIVE-SABR", "swpt", "PV"); cout << res.back() << endl; res.clear();
	//kiraCreateValueReport(res, "LIVE-SABR", "swpt", "PV01"); cout << res.back() << endl; res.clear();
  
	// vol smile
	vector<double> strikes = { 0.0309204, 0.0314204, 0.0319204, 0.0324204, 0.0329204,
														 0.0334204, 0.0339204, 0.03454204, 0.0349204, 0.03542040};
	vector<string> optType = { "REC", "REC", "REC", "REC", "REC", "PAY", "PAY", "PAY", "PAY", "PAY" };

	for (size_t i = 0; i < 10; i++) {
		kiraCreateProduct("swpt " + to_string(i), "SWAPTION", "2019-11-26", strikes[i], "10Y", 1.0*1e6, "SELL", "USD-SWAPTION", "10Y", optType[i]);
		kiraCreateValueReport(res, "LIVE-SABR", "swpt " + to_string(i), "IMPLIED_VOL"); cout << res.back() << endl; res.clear();
	}

	//// USEFUL APIS

	//// create models
	//kiraCreateModel("LIVE-SABR", "2019-11-26", "SABR", "MarketData", "BuildMethod");
	//kiraCreateModel("LIVE-BACHELIER", "2019-11-26", "BACHELIER", "MarketData", "BuildMethod");

	//// get forward levels
	////vector<string> tenors = { "3M", "6M", "9M", "1Y", "15M", "18M", "21M", "2Y", };
	//for (size_t i = 0; i < 80; i++) {
	//	string tenor = to_string((i + 1) * 3) + "M";
	//	Date tmpDate = Date("2019-11-26"); tmpDate.addPeriod(tenor);
	//	cout << tmpDate.isoFormat() << " " << kiraValueFwd("LIVE-YC", tenor, "3M") << endl;
	//}

	//cout << "-------------------------------------" << endl;

	//// get discounting factors
	////vector<string> tenors = { "3M", "6M", "9M", "1Y", "15M", "18M", "21M", "2Y", };
	//for (size_t i = 0; i < 80; i++) {
	//	string tenor = to_string((i + 1) * 3) + "M";
	//	Date tmpDate = Date("2019-11-26"); tmpDate.addPeriod(tenor);
	//	cout << tmpDate.isoFormat() << " " << kiraGetDiscountFactor("LIVE-YC", tenor) << endl;
	//}

	system("pause");
	return 0;
}


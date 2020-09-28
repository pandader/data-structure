#include "interface.h"

void interface::kiraCreateModel(string uniqueName, string const& valueDate, string const& type, string const& dataCollection, string const& buildMethod)
{
	string path = MKT_DATA + dataCollection + "\\";

	string ycName = (type == "YIELD_CURVE") ? uniqueName : uniqueName + "_YC";
	vector<std::shared_ptr<const Data>> ycDcVec;
	// gather data => data collection
	std::shared_ptr<const Data1D> cashDataObject(new Data1D("CASH_DEPOSIT::CASH-USD", "CASH_DEPOSIT", "CASH-USD", path + "cash.csv"));
	std::shared_ptr<const Data1D> fraDataObject(new Data1D("FRA::USD-LIBOR-BBA-3M-FRA", "FRA", "USD-LIBOR-BBA-3M-FRA", path + "fra.csv"));
	std::shared_ptr<const Data1D> swapDataObject(new Data1D("SWAP::USD-SWAP-SEMI-BOND", "SWAP", "USD-SWAP-SEMI-BOND", path + "swap.csv"));
	ycDcVec.push_back(cashDataObject); ycDcVec.push_back(fraDataObject); ycDcVec.push_back(swapDataObject);
	std::shared_ptr<const DataCollection> ycDC(new DataCollection("YC_DC", ycDcVec));
	// get BM
	shared_ptr<BuildMethod> bm = KIRA::getInstance()->getBuildMethod(buildMethod, "YIELD_CURVE");
	shared_ptr<YieldCurveBuildMethod> ycBM(new YieldCurveBuildMethod("YC_BM", bm->getNVP()));
	// build yield curve
	shared_ptr<Model> model(new ModelYieldCurve(ycName, Date(valueDate), ycBM, ycDC));
	KIRA::getInstance()->cacheModel(ycName, model);
	
	if (type == "BACHELIER") {
	  vector<std::shared_ptr<const Data>> bachillierDCVec;
		std::shared_ptr<const Data2D> normalVol(new Data2D("NORMAL_VOLATILITY::USD-SWAPTION", "NORMAL_VOLATILITY", "USD-SWAPTION", path + "normalVol.csv"));
		bachillierDCVec.push_back(normalVol);
		std::shared_ptr<const DataCollection> bachillierDC(new DataCollection("BACHILLIER_DC", bachillierDCVec));
		// gather build method => read txt
		shared_ptr<BuildMethod> bm = KIRA::getInstance()->getBuildMethod(buildMethod, type);
		shared_ptr<ModelBachelierModelBuildMethod> bcBuildMethod(new ModelBachelierModelBuildMethod("BC_BM", bm->getNVP()));
		// build bachillier curve
		shared_ptr<ModelYieldCurve> yc = dynamic_pointer_cast<ModelYieldCurve>(model);
		std::shared_ptr<Model> bcModel(new ModelBachelier(uniqueName, Date(valueDate), yc, bcBuildMethod, bachillierDC));
		KIRA::getInstance()->cacheModel(uniqueName, bcModel);
	}
	else if (type == "SABR")
	{
		vector<std::shared_ptr<const Data>> sabrDCVec;
		std::shared_ptr<const Data2D> normalVol(new Data2D("SABR_NORMAL_VOL::USD-SWAPTION", "SABR_NORMAL_VOL", "USD-SWAPTION", path + "normalVol.csv"));
		std::shared_ptr<const Data2D> beta(new Data2D("SABR_BETA::USD-SWAPTION", "SABR_BETA", "USD-SWAPTION", path + "beta.csv"));
		std::shared_ptr<const Data2D> nu(new Data2D("SABR_NU::USD-SWAPTION", "SABR_NU", "USD-SWAPTION", path + "nu.csv"));
		std::shared_ptr<const Data2D> rho(new Data2D("SABR_RHO::USD-SWAPTION", "SABR_RHO", "USD-SWAPTION", path + "rho.csv"));
		sabrDCVec.push_back(normalVol); sabrDCVec.push_back(beta); sabrDCVec.push_back(nu); sabrDCVec.push_back(rho);
		std::shared_ptr<const DataCollection> sabrDC(new DataCollection("SABR_DC", sabrDCVec));
		// gather build method => read txt
		shared_ptr<BuildMethod> bm = KIRA::getInstance()->getBuildMethod(buildMethod, type);
		shared_ptr<ModelStochasticAlphaBetaRhoBuildMethod> sabrBuildMethod(new ModelStochasticAlphaBetaRhoBuildMethod("SABR_BM", bm->getNVP()));
		// build sabr curve
		shared_ptr<ModelYieldCurve> yc = dynamic_pointer_cast<ModelYieldCurve>(model);
		std::shared_ptr<Model> sabrModel(new ModelStochasticAlphaBetaRho(uniqueName, Date(valueDate), yc, sabrBuildMethod, sabrDC));
		KIRA::getInstance()->cacheModel(uniqueName, sabrModel);
	}
}

void interface::kiraCreateVolModel(string uniqueName, string const& ycModel, string const& type, string const& dataCollection, string const& buildMethod)
{
	string path = MKT_DATA + dataCollection + "\\";
	shared_ptr<Model> model = KIRA::getInstance()->getModel(ycModel);
	string valueDate = model->getModelValueDate().isoFormat();

	if (type == "BACHELIER") {
		vector<std::shared_ptr<const Data>> bachillierDCVec;
		std::shared_ptr<const Data2D> normalVol(new Data2D("NORMAL_VOLATILITY::USD-SWAPTION", "NORMAL_VOLATILITY", "USD-SWAPTION", path + "normalVol.csv"));
		bachillierDCVec.push_back(normalVol);
		std::shared_ptr<const DataCollection> bachillierDC(new DataCollection("BACHILLIER_DC", bachillierDCVec));
		// gather build method => read txt
		shared_ptr<BuildMethod> bm = KIRA::getInstance()->getBuildMethod(buildMethod, type);
		shared_ptr<ModelBachelierModelBuildMethod> bcBuildMethod(new ModelBachelierModelBuildMethod("BC_BM", bm->getNVP()));
		// build bachillier curve
		shared_ptr<ModelYieldCurve> yc = dynamic_pointer_cast<ModelYieldCurve>(model);
		std::shared_ptr<Model> bcModel(new ModelBachelier(uniqueName, Date(valueDate), yc, bcBuildMethod, bachillierDC));
		KIRA::getInstance()->cacheModel(uniqueName, bcModel);
	}
	else if (type == "SABR")
	{
		vector<std::shared_ptr<const Data>> sabrDCVec;
		std::shared_ptr<const Data2D> normalVol(new Data2D("SABR_NORMAL_VOL::USD-SWAPTION", "SABR_NORMAL_VOL", "USD-SWAPTION", path + "normalVol.csv"));
		std::shared_ptr<const Data2D> beta(new Data2D("SABR_BETA::USD-SWAPTION", "SABR_BETA", "USD-SWAPTION", path + "beta.csv"));
		std::shared_ptr<const Data2D> nu(new Data2D("SABR_NU::USD-SWAPTION", "SABR_NU", "USD-SWAPTION", path + "nu.csv"));
		std::shared_ptr<const Data2D> rho(new Data2D("SABR_RHO::USD-SWAPTION", "SABR_RHO", "USD-SWAPTION", path + "rho.csv"));
		sabrDCVec.push_back(normalVol); sabrDCVec.push_back(beta); sabrDCVec.push_back(nu); sabrDCVec.push_back(rho);
		std::shared_ptr<const DataCollection> sabrDC(new DataCollection("SABR_DC", sabrDCVec));
		// gather build method => read txt
		shared_ptr<BuildMethod> bm = KIRA::getInstance()->getBuildMethod(buildMethod, type);
		shared_ptr<ModelStochasticAlphaBetaRhoBuildMethod> sabrBuildMethod(new ModelStochasticAlphaBetaRhoBuildMethod("SABR_BM", bm->getNVP()));
		// build sabr curve
		shared_ptr<ModelYieldCurve> yc = dynamic_pointer_cast<ModelYieldCurve>(model);
		std::shared_ptr<Model> sabrModel(new ModelStochasticAlphaBetaRho(uniqueName, Date(valueDate), yc, sabrBuildMethod, sabrDC));
		KIRA::getInstance()->cacheModel(uniqueName, sabrModel);
	}
}

double interface::kiraGetDiscountFactor(string const& modelName, string const& termOrTenor)
{

	shared_ptr<ModelYieldCurve> ycModel = dynamic_pointer_cast<ModelYieldCurve>(KIRA::getInstance()->getModel(modelName));
	return ycModel->getDiscountingFactor(termOrTenor);
}


double interface::kiraValueFwd(string const& modelName, string const& termOrTenor, string const& fwdPeriod)
{
	shared_ptr<ModelYieldCurve> ycModel = dynamic_pointer_cast<ModelYieldCurve>(KIRA::getInstance()->getModel(modelName));
	std::shared_ptr<const ProductFRA> fra(new ProductFRA("fra", ycModel->getModelValueDate().isoFormat(), 0, termOrTenor, "BUY", 1, KIRA::getInstance()->getConvention("USD-LIBOR-BBA-3M-FRA")->getUniqueName()));
	Date settlementDate = fra->settlementDate();
	Date termDate = fra->termDate();
	double accrued = Date::dateYearFraction(settlementDate, termDate);
	double dfSet = ycModel->getDiscountingFactor(settlementDate.isoFormat());
	double dfMat = ycModel->getDiscountingFactor(termDate.isoFormat());
	return 1 / accrued * (dfSet / dfMat - 1);
}

// kiraCreateProduct
void interface::kiraCreateProduct(string const& uniqueName,
	string const& productType,
	string const& valueDate,
	double const& rate,
	string const& expiryOrSet,
	double const& notional,
	string const& buyOrSell,
	string const& dataConvention,
	string const& termination,
	string const& payOrRec)
{
	shared_ptr<Product> holder;
	if (productType == "CASH_DEPOSIT")
		holder =  shared_ptr<Product>(new ProductCashDeposit(uniqueName, valueDate, rate, expiryOrSet, notional, buyOrSell, dataConvention));
	else if (productType == "FRA")
		holder = shared_ptr<Product>(new ProductFRA(uniqueName, valueDate, rate, expiryOrSet, buyOrSell, notional, dataConvention));
	else if (productType == "SWAP")
		holder = shared_ptr<Product>(new ProductSwap(uniqueName, valueDate, rate, expiryOrSet, termination, payOrRec, buyOrSell, notional, dataConvention));
	else if (productType == "CAPFLOOR")
		holder = shared_ptr<Product>(new ProductCapFloorLet(uniqueName, valueDate, rate, expiryOrSet, payOrRec, buyOrSell, notional, dataConvention));
	else if (productType == "SWAPTION")
		holder = shared_ptr<Product>(new ProductSwaption(uniqueName, valueDate, rate, expiryOrSet, termination, payOrRec, buyOrSell, notional, dataConvention));
	else
		KIRA_THROW_EXCEPTION("Product Type " + productType + " is not supported!");
	KIRA::getInstance()->cacheProduct(uniqueName, holder);
}

void interface::kiraCreateValueReport(vector<double> & res, string const& modelName, string const& productName, string const& request)
{
	shared_ptr<Model> model = KIRA::getInstance()->getModel(modelName);
	//if (KIRA::getInstance()->getModel(modelName)->getModelType() != "YIELD_CURVE" &&
	//	  (productName == "CASH_DEPOSIT" || productName == "FRA" || productName == "SWAP")) 
	//	model = dynamic_pointer_cast<shared_ptr<Model>>(KIRA::getInstance()->getModel(modelName)->getSubModel());
	//else model = KIRA::getInstance()->getModel(modelName);
	shared_ptr<Product> product = KIRA::getInstance()->getProduct(productName);
	string veType = KIRA::getInstance()->getValuationEngine(model->getModelType(), product->productType());

	if (veType == "YC_CASH_DEPOSIT_VE") {
		shared_ptr<ModelYieldCurve> yc = dynamic_pointer_cast<ModelYieldCurve>(model);
		shared_ptr<ProductCashDeposit> cd = dynamic_pointer_cast<ProductCashDeposit>(product);
		shared_ptr<valuationEngineCashDeposit> ve(new valuationEngineCashDeposit(yc, cd));
		interface::executionOfValuation<valuationEngineCashDeposit>(res, ve, request);
	}
	else if (veType == "YC_FRA_VE") {
		shared_ptr<ModelYieldCurve> yc = dynamic_pointer_cast<ModelYieldCurve>(model);
		shared_ptr<ProductFRA> fra = dynamic_pointer_cast<ProductFRA>(product);
		shared_ptr<valuationEngineFRA> ve(new valuationEngineFRA(yc, fra));
		interface::executionOfValuation<valuationEngineFRA>(res, ve, request);
	}
	else if (veType == "YC_SWAP_VE") {
		shared_ptr<ModelYieldCurve> yc = dynamic_pointer_cast<ModelYieldCurve>(model);
		shared_ptr<ProductSwap> swap = dynamic_pointer_cast<ProductSwap>(product);
		shared_ptr<valuationEngineSwap> ve(new valuationEngineSwap(yc, swap));
		interface::executionOfValuation<valuationEngineSwap>(res, ve, request);
	}
	else if (veType == "BC_CAPFLOOR_VE") {
		shared_ptr<ModelBachelier> bc = dynamic_pointer_cast<ModelBachelier>(model);
		shared_ptr<ProductCapFloorLet> cf = dynamic_pointer_cast<ProductCapFloorLet>(product);
		shared_ptr<valuationEngineCapFloorLet> ve(new valuationEngineCapFloorLet(bc, cf));
		interface::executionOfValuation<valuationEngineCapFloorLet>(res, ve, request);
	}
	else if (veType == "BC_SWAPTION_VE") {
		shared_ptr<ModelBachelier> bc = dynamic_pointer_cast<ModelBachelier>(model);
		shared_ptr<ProductSwaption> swpt = dynamic_pointer_cast<ProductSwaption>(product);
		shared_ptr<valuationEngineSwaption> ve(new valuationEngineSwaption(bc, swpt));
		interface::executionOfValuation<valuationEngineSwaption>(res, ve, request);
	}
	else if (veType == "SABR_CAPFLOOR_VE") {
		shared_ptr<ModelStochasticAlphaBetaRho> sabr = dynamic_pointer_cast<ModelStochasticAlphaBetaRho>(model);
		shared_ptr<ProductCapFloorLet> cf = dynamic_pointer_cast<ProductCapFloorLet>(product);
		shared_ptr <valuationEngineSABRCapFloorLet> ve(new valuationEngineSABRCapFloorLet(sabr, cf));
		interface::executionOfValuation<valuationEngineSABRCapFloorLet>(res, ve, request);
	}
	else if (veType == "SABR_SWAPTION_VE") {
		shared_ptr<ModelStochasticAlphaBetaRho> sabr = dynamic_pointer_cast<ModelStochasticAlphaBetaRho>(model);
		shared_ptr<ProductSwaption> swpt = dynamic_pointer_cast<ProductSwaption>(product);
		shared_ptr<valuationEngineSABRSwaption> ve(new valuationEngineSABRSwaption(sabr, swpt));
		interface::executionOfValuation<valuationEngineSABRSwaption>(res, ve, request);
	}
}


template <class V>
void interface::executionOfValuation(vector<double> & res, shared_ptr<V> const& ve, string const& request)
{
	ve->calculateValue();
	if (request == "PV")
	  res.push_back(ve->value());
	else if (request == "PARRATEORSPREAD") 
		res.push_back(ve->parRateOrSpread());
	else if (request == "PV01")
		res.push_back(ve->pv01());
	else if (request == "IMPLIED_VOL") {
		if (ve->getValuationEngineType() == "BC_CAPFLOOR_VE" || ve->getValuationEngineType() == "BC_SWAPTION_VE"
			|| ve->getValuationEngineType() == "SABR_CAPFLOOR_VE" || ve->getValuationEngineType() == "SABR_SWAPTION_VE")
			res.push_back(ve->normalVol());
	}
}
#pragma once

#include "registra.h"
#include "Model.h"
#include "Product.h"

class Model;
class Product;

namespace interface
{
	// kiraCreateModel
	void kiraCreateModel(string uniqueName, string const& valueDate, string const& type, string const& dataCollection, string const& buildMethod);

	// kiraCreateVolModel (yc provided)
	void kiraCreateVolModel(string uniqueName, string const& ycModel, string const& type, string const& dataCollection, string const& buildMethod);

	// kiraGetDiscountFactor
	double kiraGetDiscountFactor(string const& modelName, string const& termOrTenor);

	// kiraValueIndexFwd
	double kiraValueFwd(string const& modelName, string const& termOrTenor, string const& fwdPeriod);

	// kiraCreateProduct
  void kiraCreateProduct(string const& uniqueName,
		string const& productType,
		string const& valueDate,
		double const& rate,
		string const& expiryOrSet,
		double const& notional,
		string const& buyOrSell,
		string const& dataConvention,
		string const& termination = "",
		string const& payOrRec = "");

	// kiraCreateValueReport
	void kiraCreateValueReport(vector<double> & res, string const& modelName, string const& productName, string const& request);

	// template
	template <class V>
	void executionOfValuation(vector<double> & res, shared_ptr<V> const& ve, string const& request);
};
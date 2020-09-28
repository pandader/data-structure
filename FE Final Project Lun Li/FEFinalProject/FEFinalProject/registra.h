#pragma once
#include "utilities.h"
#include "Model.h"
#include "Product.h"
#include "NameValuePairs.h"


static string STATIC_DATA = "c:\\temp\\KIRA\\";
static string MKT_DATA = "c:\\temp\\KIRA\\";

class Product;
class Model;

class KIRA
{
private:
	KIRA();
	
	static bool instanceFlag;
	static KIRA *kira;
	map<string, shared_ptr<dataConvention>> m_dataConventionMap;
	map<string, shared_ptr<Model>> m_modelMap;
	map<string, shared_ptr<Product>> m_productMap;
	map<string, string> m_veMap;

public:
	static KIRA* getInstance()
	{
		if (!instanceFlag)
		{
			kira = new KIRA();
			instanceFlag = true;
			return kira;
		}
		else return kira;
	}

	~KIRA()
	{
		instanceFlag = false;
	}

	void cacheModel(string const& modelName, shared_ptr<Model> const& model) { m_modelMap[modelName] = model; }
	void cacheProduct(string const& productName, shared_ptr<Product> const& product) { m_productMap[productName] = product; }

	string getValuationEngine(string const& modelType, string const& productType) const { return m_veMap.at(modelType + "-" + productType); }
	shared_ptr<Model> getModel(string const& modelName) const { return m_modelMap.at(modelName); }
	shared_ptr<Product> getProduct(string const& productName) const { return m_productMap.at(productName); }
	shared_ptr<dataConvention> getConvention(string const& dc) const { return m_dataConventionMap.at(dc); };
	shared_ptr<BuildMethod> getBuildMethod(string const& bm, string const& bmType) const;
};


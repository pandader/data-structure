#include "Data.h"

Data1D::Data1D(string const& uniqueName, string const& dataType, string const& dataConvention, string const& fileName)
	: Data(uniqueName, dataType, dataConvention, vector<string>(), vector<double>())
{
	vector<string> axis1;
	vector<double> values;
	genUtils::data1Ddeserializer(fileName, axis1, values);
	setAxis1(axis1);
	setValues(values);
}

size_t Data1D::getNumItems() const
{
	vector<string> tmp;
	this->getAxis1(tmp);
	return tmp.size();
}

Data2D::Data2D(string const& uniqueName, string const& dataType, string const& dataConvention, string const& fileName)
	: Data(uniqueName, dataType, dataConvention, vector<string>(), vector<double>())
{
	vector<string> axis1;
	genUtils::data2Ddeserializer(fileName, axis1, m_axis2, m_values2D);
	setAxis1(axis1);
}

size_t Data2D::getNumItems() const
{
	vector<string> tmp, tmp1;
	this->getAxis1(tmp);
	this->getAxis2(tmp1);
	return tmp.size() * tmp1.size();
}

DataCollection::DataCollection(string const& uniqueName, vector<std::shared_ptr<Data const>> & dataObjects)
	: m_uniqueName(uniqueName)
{

	if (dataObjects.empty()) genUtils::KIRA_THROW_EXCEPTION("Empty data collection!");
	for (auto& each : dataObjects) {
		string dt = each->getDataType();
		string dc = each->getDataconvention();
		m_uniqueIDs.push_back(dt + "::" + dc);
		m_dcMap[dt + "::" + dc] = each;
	}
};



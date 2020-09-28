#pragma once

#include "utilities.h"

class Data {
public:
	explicit Data(string const& uniqueName, string const& dataType, string const& dataConvention, vector<string> const& axis1, vector<double> const& values)
		: m_uniqueName(uniqueName), m_dataType(dataType), m_dataConvention(dataConvention), m_axis1(axis1), m_values(values) {} 
	string getUniqueName() const { return m_uniqueName; }
	string getDataType() const { return m_dataType; }
	string getDataconvention() const { return m_dataConvention; }
	void getAxis1(vector<string> & axis1) const { axis1 = m_axis1; }
	void getValues(vector<double> & values) const { values = m_values; }
	virtual void getAxis2(vector<string> & axis2) const = 0;
	virtual size_t getDim() const = 0;
	virtual size_t getNumItems() const = 0;
	void setAxis1(vector<string> & axis1) { m_axis1 = axis1; }
	void setValues(vector<double> & values) { m_values = values; }
	virtual void setAxis2(vector<string> & axis2) = 0;
private:
	string m_uniqueName;
	string m_dataType;
	string m_dataConvention;
	vector<string> m_axis1;
	vector<double> m_values;
};

class Data1D : public Data {
public:
	Data1D(string const& uniqueName, string const& dataType, string const& dataConvention, vector<string> const& axis1, vector<double> const& values)
		: Data(uniqueName, dataType, dataConvention, axis1, values) {}; 
	Data1D(string const& uniqueName, string const& dataType, string const& dataConvention, string const& fileName);
	void getAxis2(vector<string> &axis2) const {
	  // NOT IMPLEMENTED
	};
	void setAxis2(vector<string> & axis2) {
	  // NOT IMPLEMENTED
	}
	size_t getDim() const { return 1; }
	size_t getNumItems() const;
};

class Data2D : public Data {
public:
	Data2D(string const& uniqueName, string const& dataType, string const& dataConvention, vector<string> const& axis1, vector<string> const& axis2, vector<vector<double>> const& values)
		: Data(uniqueName, dataType, dataConvention, axis1, vector<double>()), m_axis2(axis2), m_values2D(values) {}
	Data2D(string const& uniqueName, string const& dataType, string const& dataConvention, string const& fileName);
	void getAxis2(vector<string> & axis2) const { axis2 = m_axis2; };
	void getValues(vector<vector<double>> & values2D) const { values2D = m_values2D; }
	size_t getDim() const { return 2; }
	size_t getNumItems() const;
	void setAxis2(vector<string> & axis2) { m_axis2 = axis2; }
private:
	vector<string> m_axis2;
	vector<vector<double>> m_values2D;
};

class DataCollection
{
public:
	DataCollection(string const& uniqueName, vector<std::shared_ptr<Data const>> & dataObjects);
	size_t getNumOfDataObjects() const { return m_dcMap.size(); }
	std::shared_ptr<Data const> getDataObject(string const& dataType, string const& dataConvention) const { return m_dcMap.at(dataType + "::" + dataConvention); };
	void getUniqueIdentifiers(vector<string> & uniqueIdentifiers) const { uniqueIdentifiers = m_uniqueIDs; };

private:
	string         m_uniqueName;
	vector<string> m_uniqueIDs;
	map<string, std::shared_ptr<Data const>> m_dcMap;
};
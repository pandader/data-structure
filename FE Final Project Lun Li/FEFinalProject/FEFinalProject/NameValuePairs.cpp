#pragma once

#include "NameValuePairs.h"

NameValuePair::NameValuePair(string const& uniqueName, string const& fileName) : m_uniqueName(uniqueName) {
	m_nvpType = "";
	
	m_uniqueName = strSplit(strSplit(fileName, '\\').back(), '.')[0];
	nvpDeserializer(fileName, m_nvp);
	initialise();
};

void NameValuePair::initialise()
{
	if (m_nvp.empty()) genUtils::KIRA_THROW_EXCEPTION("Empty " + m_nvpType + " !");
	for (size_t i = 0; i < m_nvp.size(); i++) 
		m_map[m_nvp[i][0]] = m_nvp[i][1];
}

void NameValuePair::displayNVP() const
{
	if (m_nvp.empty()) genUtils::KIRA_THROW_EXCEPTION("Empty " + m_nvpType + " !");
	for (auto it = m_map.begin(); it != m_map.end(); it++) 
		cout << it->first << " : " << it->second << endl;
}


YieldCurveBuildMethod::YieldCurveBuildMethod(string const& uniqueName, vector<vector<string>> const& nvp)
	: BuildMethod(uniqueName, nvp)
{
	if (this->getValueByKey("TARGET").empty()) KIRA_THROW_EXCEPTION("Target needs to be specified!!");
	else m_target = this->getValueByKey("TARGET");
	m_cashDeposit = this->getValueByKey("CASH_DEPOSIT");
	if (m_cashDeposit != "N/A") m_calibInst.push_back("CASH_DEPOSIT");
	m_fra = this->getValueByKey("FRA");
	if (m_fra != "N/A") m_calibInst.push_back("FRA");
	m_swap = this->getValueByKey("SWAP");
	if (m_swap != "N/A") m_calibInst.push_back("SWAP");

	if (m_calibInst.size() == 0) KIRA_THROW_EXCEPTION("Calibration instruments is empty!");
}

YieldCurveBuildMethod::YieldCurveBuildMethod(string const& uniqueName, string const& fileName)
	: BuildMethod(uniqueName, fileName)
{
	if (this->getValueByKey("TARGET").empty()) KIRA_THROW_EXCEPTION("Target needs to be specified!!");
	else m_target = this->getValueByKey("TARGET");
	m_cashDeposit = this->getValueByKey("CASH_DEPOSIT");
	if (m_cashDeposit != "N/A") m_calibInst.push_back("CASH_DEPOSIT");
	m_fra = this->getValueByKey("FRA");
	if (m_fra != "N/A") m_calibInst.push_back("FRA");
	m_swap = this->getValueByKey("SWAP");
	if (m_swap != "N/A") m_calibInst.push_back("SWAP");
	
	if (m_calibInst.size() == 0) KIRA_THROW_EXCEPTION("Calibration instruments is empty!");
}

ModelBachelierModelBuildMethod::ModelBachelierModelBuildMethod(string const& uniqueName, vector<vector<string>> const& nvp)
	: BuildMethod(uniqueName, nvp)
{
	if (this->getValueByKey("TARGET").empty()) KIRA_THROW_EXCEPTION("Target needs to be specified!!");
	else m_target = this->getValueByKey("TARGET");
	m_normalVol = this->getValueByKey("NORMAL_VOLATILITY");
	m_interpMethod = this->getValueByKey("INTERP_METHOD");
}

ModelBachelierModelBuildMethod::ModelBachelierModelBuildMethod(string const& uniqueName, string const& fileName)
	: BuildMethod(uniqueName, fileName)
{
	if (this->getValueByKey("TARGET").empty()) KIRA_THROW_EXCEPTION("Target needs to be specified!!");
	else m_target = this->getValueByKey("TARGET");
	m_normalVol = this->getValueByKey("NORMAL_VOLATILITY");
	m_interpMethod = this->getValueByKey("INTERP_METHOD");

}


ModelStochasticAlphaBetaRhoBuildMethod::ModelStochasticAlphaBetaRhoBuildMethod(string const& uniqueName, vector<vector<string>> const& nvp)
	: BuildMethod(uniqueName, nvp)
{
	if (this->getValueByKey("TARGET").empty()) KIRA_THROW_EXCEPTION("Target needs to be specified!!");
	else m_target = this->getValueByKey("TARGET");
	m_normalVol = this->getValueByKey("SABR_NORMAL_VOL");
	m_beta = this->getValueByKey("SABR_BETA");
	m_nu = this->getValueByKey("SABR_NU");
	m_rho = this->getValueByKey("SABR_RHO");
	m_interpMethod = this->getValueByKey("INTERP_METHOD");
}

ModelStochasticAlphaBetaRhoBuildMethod::ModelStochasticAlphaBetaRhoBuildMethod(string const& uniqueName, string const& fileName)
	: BuildMethod(uniqueName, fileName)
{
	if (this->getValueByKey("TARGET").empty()) KIRA_THROW_EXCEPTION("Target needs to be specified!!");
	else m_target = this->getValueByKey("TARGET");
	m_normalVol = this->getValueByKey("SABR_NORMAL_VOL");
	m_beta = this->getValueByKey("SABR_BETA");
	m_nu = this->getValueByKey("SABR_NU");
	m_rho = this->getValueByKey("SABR_RHO");
	m_interpMethod = this->getValueByKey("INTERP_METHOD");
}

//
// Created by nirvana on 10/1/19.
//

#include "money.h"

using namespace utilities;
using namespace mathUtils;

money::money(string & extRep) {

	m_internalRep = strToLng(extRep);

}

bool money::operator==(money const &m) {
  return m_internalRep == m.m_internalRep;
}

bool money::operator!=(money const &m) {
	return m_internalRep != m.m_internalRep;
}

money money::operator+(money const &m) {

 long res = m_internalRep + m.m_internalRep;
 string resStr = longToStr(res);
 money resM(resStr);
 return resM;

}

money money::operator-(money const &m) {

  long res = m_internalRep - m.m_internalRep;
  string resStr = longToStr(res);
  money resM(resStr);
  return resM;

}

money money::operator*(int n) {
	string resStr = longToStr(m_internalRep * n);
	money resM(resStr);
	return resM;
}

money money::operator*(double n) {
	long res1 = (long)(rounding45((double)m_internalRep * n, 0));
	string resStr = longToStr(res1);
	money resM(resStr);
	return resM;
}

money money::operator/(int n) {
	long res = m_internalRep / n;
	string resStr = longToStr(res);
	money resM(resStr);
	return resM;
}

money money::operator/(double n) {
	long res1 = (long)(rounding45((double)m_internalRep / n, 0));
	string resStr = longToStr(res1);
	money resM(resStr);
	return resM;
}

ostream& operator<<(ostream& os, money const & m) {
	string tempStr = money::longToStr(m.m_internalRep);
	os << tempStr;
	return os;
}

istream& operator>>(istream& input, money & m)
{
	string tempStr;
	input >> tempStr;
	m.m_internalRep = money::strToLng(tempStr);

	return input;
}

long money::strToLng(string &strRep){
	vector<string> tokenize1;
	tokenize1 = strSplit(strRep, '$');
	double roundTdbl = rounding45(stod(tokenize1[1]),2);
	return (long)(roundTdbl * 100);
}

string money::longToStr(long longRep){
  long dollarPart = longRep / 100;
  long centPart = longRep % 100;
  string strRepDollar = std::to_string(dollarPart);
  string strRepCents = (centPart == 0) ? "00" : to_string(centPart);
  return "$" + strRepDollar + "." + strRepCents;
}

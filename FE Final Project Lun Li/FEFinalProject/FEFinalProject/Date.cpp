#include "Date.h"

Date::Date(string isoFormat) {

	vector<string> tokenized = strSplit(isoFormat, '-');
	if (tokenized.size() != 3)
		KIRA_THROW_EXCEPTION(isoFormat + " is not in ISO format, e.g., YYYY-MM-DD!");
	else {
		m_year = stoi(tokenized[0]);
		m_month = stoi(tokenized[1]);
		m_day = stoi(tokenized[2]);
	}
}

void Date::addPeriod(string const& tenor)
{	
	if (tenor.back() == 'Y')
		add_year(stoi(strSplit(tenor, 'Y')[0]));
	else if (tenor.back() == 'M')
		add_month(stoi(strSplit(tenor, 'M')[0]));
	else if (tenor.back() == 'D')
		add_day(stoi(strSplit(tenor, 'D')[0]));
	else
		KIRA_THROW_EXCEPTION(tenor + " is not legtimate tenor!");
}

void Date::add_day(int n)
{

	int year = m_year + n / 360;
	n %= 360;
	int month = m_month + n / 30;
	n %= 30;
	int day = m_day + n;

	if (day > 30) {
		day -= 30;
		++month;
	}else if (day < 1) {
		day += 30;
		--month;
	}

	if (month > 12) {
		month -= 12;
		++year;
	}else if (month < 1) {
		month += 12;
		--year;
	}

	if (!is_valid(year, month, day))
		KIRA_THROW_EXCEPTION("Not valid date!");

	m_year = year;
	m_month = month;
	m_day = day;
}

void Date::add_month(int n) {

	int year = m_year + n / 12;
	n %= 12;
	int month = m_month + n;

	if (month > 12) {
		month -= 12;
		++year;
	}else if (month < 1) {
		month += 12;
		--year;
	}

	if (!is_valid(year, month, 1))
		KIRA_THROW_EXCEPTION("Not valid date!");

	m_year = year;
	m_month = month;
}

void Date::add_year(int n) {
	m_year += n;
}

bool Date::is_valid(int y, int m, int d)
{
	if (m < 1 || m > 12) return false;
	return !(d < 1 || d > 30);
}

bool Date::operator==(Date const& d)
{
	return m_year == d.getYear() && m_month == d.getMonth() && m_day == d.getDay();
}

bool Date::operator!=(Date const& d)
{
	return !(*this == d);
}

bool Date::operator<(Date const& d)
{
	// year compare
	if (m_year < d.getYear()) return true;
	else if (m_year > d.getYear()) return false;
	// month compare
	if (m_month < d.getMonth()) return true;
	else if (m_month > d.getMonth()) return false;
	// day compare
	return m_day < d.getDay();
}

bool Date::operator>(Date const& d)
{
	return !(*this == d) && !(*this < d);
}

bool Date::operator<=(Date const& d)
{
	return (*this == d) || (*this < d);
}

bool Date::operator>=(Date const& d)
{
	return (*this == d) || (*this > d);
}

double Date::dateYearFraction(Date const& startDate, Date const& endDate)
{
	size_t count = 0;
	Date curDate = startDate;
	while (curDate <= endDate) {
		curDate.add_day(1);
		count++;
	}
	return (double)count / 365.;
}

double Date::tenorToDate(Date const& baseDate, string const& tenor)
{
	Date curDate = baseDate;
	curDate.addPeriod(tenor);
	return Date::dateYearFraction(baseDate, curDate);
}
#pragma once
#include "utilities.h"

using namespace genUtils;

class Date
{
public:

	// constructor
	Date() {};
	Date(string isoFormat); // YYYY-MM-DD
	Date(int year, int month, int day) : m_year(year), m_month(month), m_day(day) {};

	// add period
	void addPeriod(string const& tenor);
	void add_day(int n);
	void add_month(int n);
	void add_year(int n);

	// operator overloading
	bool operator==(Date const& d);
	bool operator!=(Date const& d);
	bool Date::operator<(Date const& d);
	bool Date::operator>(Date const& d);
	bool Date::operator<=(Date const& d);
	bool Date::operator>=(Date const& d);

	// Getters
	string isoFormat() const { return to_string(m_year) + "-" + to_string(m_month) + "-" + to_string(m_day); }
	int getYear() const { return m_year; };
	int getMonth() const { return m_month; };
	int getDay() const { return m_day; };

	// Setters
	void setYear(int year) { m_year = year; };
	void setMonth(int month) { m_month = month; };
	void setDay(int day) { m_day = day; };

	// check if valid
	bool is_valid(int year, int month, int day);

  // date year fraction
	static double dateYearFraction(Date const& startDate, Date const& endDate);

	// term to date
	static double tenorToDate(Date const& baseDate, string const& tenor);

private:

	int m_year;
	int m_month;
	int m_day;
};
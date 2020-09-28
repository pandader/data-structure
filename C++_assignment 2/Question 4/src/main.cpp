#include "Link.h"
#include "EuropeanOption.h"
#include "AmericanOption.h"

using namespace std;


string setID(Option* ptr){
	return ptr->m_optType + "-" + to_string(ptr->m_timeToMaturity) + "-" + to_string(ptr->m_strikePrice);
}

void printSingleOpt(Option* ptr) {
	cout << "This option is of type " << ptr->m_optType << ", ";
	cout << "has spot price " << ptr->m_spotPrice << ", ";
	cout << "strike price " << ptr->m_strikePrice << ", ";
	cout << "risk free rate " << ptr->m_interestRate << ", ";
	cout << "volatility " << ptr->m_vol << ", ";
	cout << "and time to maturity " << ptr->m_timeToMaturity << "." << endl;
}

int main()
{
	string optType1 = "call"; string optType2 = "put";
	Option* ptr1; Option* ptr2; Option* ptr3; Option* ptr4;
	//Option* ptr5; Option* ptr6; Option* ptr7; Option* ptr8;
	ptr1 = new AmericanOption(optType1, 100, 100, 0.01, 0.2, 1);
	ptr2 = new AmericanOption(optType1, 100, 101, 0.01, 0.3, 2);
	ptr3 = new AmericanOption(optType1, 100, 102, 0.01, 0.4, 3);
	ptr4 = new AmericanOption(optType1, 100, 103, 0.01, 0.5, 4);

	//aList->printAll(head, printSingleOpt);

	Link<Option>* aList = new Link<Option>(ptr3, setID);
	aList = aList->addOrdered(new Link<Option>(ptr4, setID));
	aList = aList->addOrdered(new Link<Option>(ptr2, setID));
	aList = aList->addOrdered(new Link<Option>(ptr1, setID));

	aList->printAll(aList, printSingleOpt);

	return 0;
}

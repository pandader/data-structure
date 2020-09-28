#include "Link.h"
#include "EuropeanOption.h"
#include "AmericanOption.h"

using namespace std;

string setID(Option* ptr){
	return ptr->m_exerciseType + "-" + ptr->m_optType + "-" + to_string(ptr->m_timeToMaturity)
	                           + "-" + to_string(ptr->m_strikePrice);
}

int main()
{
	string optType1 = "call"; string optType2 = "put";
	Option* ptr1; Option* ptr2; Option* ptr3; Option* ptr4;
	Option* ptr5; Option* ptr6;
	ptr1 = new AmericanOption(optType1, 100, 100, 0.01, 0.2, 1);
	ptr2 = new AmericanOption(optType1, 100, 101, 0.01, 0.3, 2);
	ptr3 = new AmericanOption(optType1, 100, 102, 0.01, 0.4, 3);
	ptr4 = new AmericanOption(optType1, 100, 103, 0.01, 0.5, 4);
	ptr5 = new EuropeanOption(optType1, 100, 103, 0.01, 0.5, 4);
	ptr6 = new EuropeanOption(optType1, 100, 104, 0.01, 0.5, 4);

//	Link* aList = new Link(ptr1, setID);
//	Link* head = aList;
//	aList = aList->add(new Link(ptr2, setID));
//	aList = aList->add(new Link(ptr3, setID));
//	aList = aList->add(new Link(ptr4, setID));

	Link* aList = new Link(ptr3, setID);
	aList = aList->addOrdered(new Link(ptr4, setID));
	aList = aList->addOrdered(new Link(ptr2, setID));
	aList = aList->addOrdered(new Link(ptr1, setID));
	aList = aList->addOrdered(new Link(ptr5, setID));
	aList = aList->addOrdered(new Link(ptr6, setID));

	aList->printAll(aList);

	return 0;
}

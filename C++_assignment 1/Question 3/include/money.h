//
// Created by nirvana on 10/1/19.
//

#ifndef PROJECT_1_MONEY_H
#define PROJECT_1_MONEY_H

#include "utilities.h"

class money {

public:
    // constructor
    money() = default;
    explicit money(string & exRep);
    // overload == operator
    bool operator ==(money const& m);
		// overload != operator
		bool operator !=(money const& m);
    // overload + operator
    money operator +(money const& m);
    // overload - operator
    money operator -(money const& m);
		// overload * operator
		money operator *(double n);
		// overload / operator
		money operator /(double n);
		// overload * operator
		money operator *(int n);
		// overload / operator
		money operator /(int n);

    // friends
    // overload << operator (output)
    friend ostream& operator<<(ostream & os, money const & m);
    // overload >> operator (input)
    friend istream& operator>>(istream & input, money & m);

    // conversion function
    static long strToLng(string & strRep);
    static string longToStr(long longRep);

    // member variables
    long m_internalRep;
};



#endif //PROJECT_1_MONEY_H

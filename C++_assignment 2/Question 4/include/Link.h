//
// Created by nirvana on 11/11/19.
//

#ifndef PROJECT_1_LINK_H
#define PROJECT_1_LINK_H

#include "Option.h"

template <class T> class Link{

public:
		// constructor
		Link(T* const&  v, string (*func)(T*), Link* p = nullptr, Link* s = nullptr)
						: ptr(v), prev(p), succ(s) {
			idFun = func;
			uniqueID = (*idFun)(v);
		};
		// insert n before this object
		Link* insert(Link* n);
		// insert n after this object
		  Link* add(Link* n);
		// add in an ordered way
		Link* addOrdered(Link* n);
		// erase p from list
		Link* erase(Link* p);
		// find s in list
		Link* find(Link* p, T* const& s);
		// move n positinos in list
		Link* advance(Link* p, int n);
		// return successor
		Link* next() const { return succ; }
		// return previous
		Link* previous() const { return prev; }
		// set previous
		void setPre(Link* p) { prev = p; }
		void setNext(Link* p) { succ = p; }
		// printall
		void printAll(Link *p, void (*printf)(T*));
		//public member: value
		T* ptr;
		string uniqueID;
		string (*idFun)(T*);

private:
		Link* prev;
		Link* succ;
};


#endif //PROJECT_1_LINK_H

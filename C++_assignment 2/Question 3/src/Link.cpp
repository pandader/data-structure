//
// Created by nirvana on 11/11/19.
//

#include "Link.h"

Link* Link::insert(Link *n)
{
	if(n == nullptr) return this;
	if(this == nullptr) return n;
	n->succ = this;
	if(prev) prev->succ = n;
	n->prev = prev;
	prev = n;
	return n;
}

Link* Link::add(Link *n)
{
  if(n == nullptr) return this;
  if(this == nullptr) return n;
  n->prev = this;
  if(succ) succ->prev = n;
  n->succ = succ;
  succ = n;
  return n;
}

Link* Link::addOrdered(Link *n)
{
	if(n == nullptr) return this;
	if(this == nullptr) return n;

	// for non-corner case
	Link* cur = this;
	string id = n->uniqueID;
	if(id >= this->uniqueID){
		while(cur->next())
		{
			cur = cur->next();
		  if(id <= cur->uniqueID) return cur->insert(n);
		}
		cur->setNext(n);
		n->setPre(cur);
		return n;
	}else{
		while(cur->previous())
		{
			cur = cur->previous();
			if(id >= cur->uniqueID) return cur->add(n);
		}
		cur->setPre(n);
		n->setNext(cur);
		return n;
	}
}

Link* Link::erase(Link *p)
{
	if(p == nullptr) return nullptr;
	if(p->succ) p->succ->prev = p->prev;
	if(p->prev) p->prev->succ = p->succ;
	return p->succ;
}

Link* Link::find(Link* p, Option* const& s)
{
	while (p)
	{
		if(p->optPtr == s) return p;
		p = p->succ;
	}
	return nullptr;
}

Link* Link::advance(Link* p, int n)
{
	if(p == nullptr) return nullptr;
	if(n > 0)
	{
		while(n--)
		{
			if(p->succ == nullptr) return nullptr;
			p = p->succ;
		}
	}else if(n < 0)
	{
		while(n++)
		{
			if(p->prev == nullptr) return nullptr;
			p = p->prev;
		}
	}
	return p;
}

void Link::printAll(Link *p)
{

	if(p == nullptr)
		utilities::exceptionHandle("Empty List!!", true);
	// get to the head of the list
	Link* cur = p;
	while(cur->previous() != nullptr)
		cur = cur->previous();

	while(cur)
	{
		Option* ptr = cur->optPtr;
		cout << "This option is of type " << ptr->m_exerciseType << "-" << ptr->m_optType << ", ";
		cout << "has spot price " << ptr->m_spotPrice << ", ";
		cout << "strike price " << ptr->m_strikePrice << ", ";
		cout << "risk free rate " << ptr->m_interestRate << ", ";
		cout << "volatility " << ptr->m_vol << ", ";
		cout << "and time to maturity " << ptr->m_timeToMaturity <<  "." << endl;
		cur = cur->next();
	}
}
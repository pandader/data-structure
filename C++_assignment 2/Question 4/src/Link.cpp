//
// Created by nirvana on 11/11/19.
//

#include "Link.h"

template <class T>
Link<T>* Link<T>::insert(Link<T> *n)
{
	if(n == nullptr) return this;
	if(this == nullptr) return n;
	n->succ = this;
	if(prev) prev->succ = n;
	n->prev = prev;
	prev = n;
	return n;
}

template <class T>
Link<T>* Link<T>::add(Link<T>* n)
{
	if(n == nullptr) return this;
	if(this == nullptr) return n;
	n->prev = this;
	if(succ) succ->prev = n;
	n->succ = succ;
	succ = n;
}

template <class T>
Link<T>* Link<T>::addOrdered(Link<T> *n)
{
	if(n == nullptr) return this;
	if(this == nullptr) return n;

	// for non-corner case
	Link<T>* cur = this;
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

template <class T>
Link<T>* Link<T>::erase(Link<T> *p)
{
	if(p == nullptr) return nullptr;
	if(p->succ) p->succ->prev = p->prev;
	if(p->prev) p->prev->succ = p->succ;
	return p->succ;
}

template <class T>
Link<T>* Link<T>::find(Link<T>* p, T* const& s)
{
	while (p)
	{
		if(p->ptr == s) return p;
		p = p->succ;
	}
	return nullptr;
}

template <class T>
Link<T>* Link<T>::advance(Link<T>* p, int n)
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

template <class T>
void Link<T>::printAll(Link<T> *p, void (*printf)(T*))
{

	if(p == nullptr) cout << "Empty List !" << endl;
	// get to the head of the list
	Link<T>* cur = p;
	while(cur->previous() != nullptr) cur = p->previous();

	while(cur)
	{
		T* ptr = cur->ptr;
		printf(ptr);
		cur = cur->next();
	}
}

template class Link<Option>;
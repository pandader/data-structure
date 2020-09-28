#include "testHead.h"


void B1::vf() {
	cout << "vf" << endl;
}

void B1::f()
{
	cout << "f" << endl;
}

void D1::vf()
{
	cout << "vf_mod" << endl;
}

void D1::f()
{
	cout << "f_mod" << endl;
}

void D1::pvf()
{
	cout << "pvf" << endl;
}

void D2::pvf()
{
	cout << "pvf_d2" << endl;
}

void D21::pvf()
{
	cout << m_str << endl;
}

void D22::f(B2& b2)
{
	b2.pvf();
}

void D22::pvf()
{
	cout << m_int << endl;
}
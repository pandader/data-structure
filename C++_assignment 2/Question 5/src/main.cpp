#include "utilities.h"

using namespace std;

int lenOfChar(const char* cStr)
{
	int numChar = 0;
	const char* cur = cStr;
	while (*cur) {
		numChar++;
		cur++;
	}
	return numChar;
}

char* strdup(char const * cStr)
{
	char *temp = (char*)malloc(lenOfChar(cStr) + 1);

	char* ptr = temp;
	while(*cStr != '\0')
	{
		*ptr = *cStr;
		ptr++; cStr++;
	}
	*ptr = '\0';
	return temp;
}

bool compareStr(const char* cur, const char* x)
{
	const char* p1 = cur;
	const char* p2 = x;
	while(*p2 != '\0')
	{
		if(*p1 == *p2){
			p1++;
			p2++;
		}else{
		  break;
		}
	}
	return *p2 == '\0';
}

// find first occurence of the C-stye string x in s
const char* findx(const char* s, const char* x)
{
	const char* cur = s;
	while(*cur != '\0')
	{
		if(compareStr(cur, x))
			break;
		else
			cur++;
	}
	return cur;
}

int strCmp(const char *a, const char *b)
{
	for(; *a && *b && *a == *b; ++a, ++b)
		;
	if (*b == *a)
		return 0;
	else if(*b > *a)
		return -1;
	else
		return 1;
}

int main()
{
	char p[6] = "Hello";
	char p1[4] = "ell";
//	char* ptr = strdup(p);
//	while(*ptr) {
//		cout << *ptr++;
//	}
//
//  const char* ptr = findx(p, p1);
//  cout << *ptr << endl;
  char s1[4] = "abc";
  char s2[3] = "aa";
  cout << strCmp(s1, s2) << endl;


}
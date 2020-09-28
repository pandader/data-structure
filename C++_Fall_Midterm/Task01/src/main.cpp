#include "std_lib_facilities.h"
#include <iostream>

string transString(const string& s){
	string fixed="";
	char c;
	int length = s.length( );
	for (int i = 0; i < length; i++){
		c = s.at(i);
		if (!ispunct(c) && !isspace(c))
			fixed += tolower(c);
	}
	return fixed;
}

bool isPalindrome(const string& s)
{
	string str=transString(s);
	int len = str.length();
	int half = len/2;
	for (int i=0; i< half; i++){
		if (str.at(i) != str.at(len-1-i))
			return false;
	}
	return true;
}



int main()
{
    cout << "Task 1:" << '\n';

    cout << "This should return true: " << (isPalindrome("Rats live on no evil star") ? "true" : "false") << '\n';

    cout << "This should return false: " << (isPalindrome("To be or not to be") ? "true" : "false") << '\n';

		cout << "Task 2:" << '\n';

		cout << "This should return false: " << (isPalindrome("Dogs live on no evil star") ? "true" : "false") << '\n';

		cout << "This should return true: " << (isPalindrome("racecar") ? "true" : "false") << '\n';

		cout << "Task 3:" << '\n';

		cout << "This should return false: " << (isPalindrome("Do geese see Dog") ? "true" : "false") << '\n';

		cout << "This should return true: " << (isPalindrome("Do geese see God") ? "true" : "false") << '\n';

		cout << "Task 4:" << '\n';

		cout << "This should return true: " << (isPalindrome("Mr. Owl ate my metal worm") ? "true" : "false") << '\n';

		cout << "This should return true: " << (isPalindrome("Go hang a salami, I'm a lasagna hog") ? "true" : "false") << '\n';

	keep_window_open();

    return 0;
}

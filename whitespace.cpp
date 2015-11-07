#include <iostream>
#include <string>
#include <cctype> 
using namespace std;

bool is_white_space(string s) {

    for (size_t i = 0; i < s.size(); i++) {
        if (!isspace(s[i]))
            return false;
    }
    return true;
}

int main() {

    string s1("red");
    string s2("     ");
    string s3("  \t");
    string s4("\t  ");
    string s5("\t hi ");

    cout << "is_white_space(red): " << is_white_space(s1) << endl;
    cout << "is_white_space(    ): " << is_white_space(s2) << endl;
    cout << "is_white_space(    \t): " << is_white_space(s3) << endl;
    cout << "is_white_space(\t   ): " << is_white_space(s4) << endl;
    cout << "is_white_space(\t hi ): " << is_white_space(s5) << endl;
}
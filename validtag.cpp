#include <iostream>
#include <string>
#include <locale> 
using namespace std;

bool is_valid_tag(string s) {

    for (size_t i = 0; i < s.size(); i++) {
        if (!isalnum(s[i]))
            return false;
    }
    return true;
}

int main() {

    string s1("red");
    string s2("red!");
    string s3("!red");
    string s4("r?d");

    cout << "is_valid_tag(red): " << is_valid_tag(s1) << endl;
    cout << "is_valid_tag(red!): " << is_valid_tag(s2) << endl;
    cout << "is_valid_tag(!red): " << is_valid_tag(s3) << endl;
    cout << "is_valid_tag(r?d): " << is_valid_tag(s4) << endl;
}
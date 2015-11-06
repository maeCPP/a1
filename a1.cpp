/**
 *  COMP 3512 - C++
 *  Assignment #1
 *  Fall 2015
 *  
 *  Mae Yee, A00077546
 *  a1.cpp
 */
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <cstdlib>

using namespace std;

bool is_valid_tag(string s) {

    for (size_t i = 0; i < s.size(); i++) {
        if (!isalnum(s[i]))
            return false;
    }
    return true;
}


int main(int argc, char* argv[]) {

    string config_file;
    string line, word;
    string tag, code;
    map<string, string> config_map;
    vector<string> v;

    smatch m;

    //regex re1(R"((<(.+)>)(\s*[a-z]+\s*)(</(.+)>))");
    regex re1(R"(<(.+?)>)");
    regex re2(R"(\\e)");

    // bool textflag = 0;

    if (argc == 1) {
        config_file = "config.txt";
    } else {
        config_file = argv[1];
    }
      
    ifstream fs(config_file);

    // config file cannot be opened
    if (!fs) {                       
        cerr << "\n\nERROR:  Cannot open file " << argv[1] << ".  Exiting program." << endl;
        exit(1);                    
    }

    while (getline(fs, line)) {

        istringstream iss( line );

        if (iss >> tag >> code) {

            if (is_valid_tag(tag)) {
                config_map.insert ( pair<string, string>(tag, code) );
            } else {
                cerr << "\n\nERROR:  " << tag << " is an invalid tag." << endl;
            }
        }
    }

    /*
    for (auto it = config_map.begin(); it != config_map.end(); ++it) {
        cout << it->first << "\t => " << it->second << '\n';
    }*/

    auto it = config_map.find("text");
    if (it == config_map.end()) {
        cerr << "\n\nERROR:  Config file does not contain 'text' tag.  Exiting program." << endl;
        exit(1);
    }

    cin >> word;

    /* TO-DO:  handle case where <text> is not followed by space */
    if (word.substr(0,6) != "<text>") {
        cerr << "\n\nERROR:  Input text does not start with <text> tag.  Exiting program." << endl;
         exit(1);
    } else {
        v.push_back("text");
    }

    

    /* TO-DO:  handle case where input text does not end with </text> tag */

    /* TO-DO:  handle case where <text> or </text> occur more than once */

    while (getline(cin, line)) {

    /* TO-DO:  preserve empty new lines */

        for (auto it = line.cbegin(), end = line.cend(); regex_search(it, end, m, re1); it = m.suffix().first) 
        {
            
            cout << m.prefix();                         // print everything before the tag

            string tagname = m.str(1);

            if (tagname[0] != '/') {                    // it is a start tag 

                auto it = config_map.find(tagname);     // lookup tag in config map

                if (it != config_map.end()) {           // tag is found

                    cout << regex_replace(it->second, re2, "\033");
                    v.push_back(tagname);
                    
                } else {
                    cerr << "\n\nERROR:  " << tagname << " is an invalid tag.  Exiting program." << endl;
                    exit(1);
                }

            } else {                                    // it is an end tag

                /*
                if (tagname == "/text") {
                    cout << "-------- found /text" << endl;
                    break;
                }
                */

                if (!v.empty()) {

                    /*
                    cerr << "v = [";
                    for (auto vit = v.begin(); vit != v.end()-1; ++vit)
                        cerr << *vit << ", ";
                    cerr << v.back() << "]" << endl;
                    */
                   

                    
                    // check if tagname is the last element of the vector
                    if (v.back() == tagname.substr(1)) {

                        // remove the last tagname from the vector
                        v.pop_back();
                        
                        // revert to previous color   
                        if (!v.empty()) {
                            auto it = config_map.find(v.back());
                            cout << regex_replace(it->second, re2, "\033");
                            cout << m.suffix() << endl;
                            
                        }

                    } else {                            // invalid nesting of tags
                        cerr << "\n\nERROR:  Invalid nesting of tags.  Exiting program." << endl;
                        break;
                    } 
                }
            } 
        } 
    }

}
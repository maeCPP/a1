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

/**
 * Returns true if string s (which represents a tag) comprises of 
 * only alphanumeric characters; returns false otherwise.
 */
bool is_valid_tag(string s) {

    for (size_t i = 0; i < s.size(); i++) {
        if (!isalnum(s[i]))
            return false;
    }
    return true;
}

bool is_white_space(string s) {

    for (size_t i = 0; i < s.size(); i++) {
        if (!isspace(s[i]))
            return false;
    }
    return true;
}

/**
 * Checks command-line arguments for a configuration file name.  If
 * none provided, uses the default "config.txt" file.  Opens the 
 * config file and parses the file line-by-line to read in valid
 * tags and codes into a map.
 *
 * Returns a map of tags and codes upon success.
 */
map<string, string> load_config(int argc, char* argv[]) {

    string config_file;
    string line;
    string tag;                         // must be all alphanumeric
    string code;                        
    map<string, string> config_map;

    // check number of arguments
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

    // parse config file, line by line, for valid tags and codes
    while (getline(fs, line)) {

        istringstream iss( line );

        if (iss >> tag >> code) {

            if (is_valid_tag(tag)) {
                config_map.insert ( pair<string, string>(tag, code) );
            } else {
                cerr << "\n\nIgnoring invalid tag: " << tag << ".\n\n";
            }
        }
    }
    
    // print out config_map for testing
    // 
    // for (auto it = config_map.begin(); it != config_map.end(); ++it) {
    //     cout << it->first << "\t => " << it->second << '\n';
    // }
    
    // check that config file contains the "text" tag
    auto it = config_map.find("text");
    if (it == config_map.end()) {
        cerr << "\n\nERROR:  Config file does not contain 'text' tag.  Exiting program." << endl;
        exit(1);
    }

    return config_map;
}


int main(int argc, char* argv[]) {

    string line, word;
    map<string, string> config_map;
    vector<string> v;
    smatch m;
    //regex re1(R"((<(.+)>)(\s*[a-z]+\s*)(</(.+)>))");
    regex re1(R"(<(.+?)>)");
    regex re2(R"(\\e)");
    bool ot_flag = false;               // true if open text tag  <text>  found
    bool ct_flag = false;               // true if close text tag </text> found

    //cout << ot_flag << ' ' << ct_flag << endl;

    // load configuration file
    config_map = load_config(argc, argv);

    size_t ln = 1;
    while (getline(cin, line)) {
    
    /* TO-DO:  handle case where </text> occur more than once */

        // preserve lines with only white space
        //if (is_white_space(line)) {
        //    cout << "line number: " << ln << endl;
        //}
        //cout << "line number: " << ln << endl;


        for (auto it = line.cbegin(), end = line.cend(); regex_search(it, end, m, re1); it = m.suffix().first) 
        {
            string tagname = m.str(1);

            if (tagname == "text") {

                // check that <text> tag not duplicated
                if (ot_flag) {
                    cerr << "\033[39;49m\n\nERROR:  duplicate <text> tag.  Exiting program." << endl;
                    exit(1);
                }
                ot_flag = true;

                // check that the <text> tag is not preceded by non white space characters
                if (!is_white_space(m.prefix())) {
                    cerr << "\033[39;49m\n\nERROR:  Input text does not start with <text> tag.  Exiting program." << endl;
                    exit(1);
                }
            } else {
            
            // print everything before the tag unless it is the <text> tag 
            //if (tagname != "text" ) {
                cout << m.prefix();                      
            }

            // check if it is a start tag
            if (tagname[0] != '/') {                 

                auto it = config_map.find(tagname);     // lookup tag in config map

                if (it != config_map.end()) {           // tag is found

                    cout << regex_replace(it->second, re2, "\033");
                    v.push_back(tagname);
                    
                } else {
                    cerr << "\033[39;49m\n\nERROR:  " << m.str() << " is an invalid tag.  Exiting program." << endl;
                    exit(1);
                }

            // otherwise it must be a close tag
            } else {       

                // TO-DO:  check that the </text> tag is not followed by any non-white space characters                      

                //if (!v.empty()) {
                    
                    // check if tagname is same as the last element of the vector
                    if (v.back() == tagname.substr(1)) {

                        // remove the last tagname from the vector
                        v.pop_back();

                        //if (tagname == "/text") {
                            //cout << "END:  <" << tagname << ">" << endl;
                            //ct_flag = true;
                        //}
                        
                        // revert to previous color   
                        //if (!v.empty()) {
                        //
                        auto it = config_map.find(v.back());
                        cout << regex_replace(it->second, re2, "\033");
                        
                        /*
                        if (!is_white_space(m.suffix())) {
                            cerr << "\033[39;49m\n\nERROR:  Input text does not end with </text> tag.  Exiting program." << endl;
                            exit(1);
                        }
                        */
                        
                        //if (!ct_flag) {
                            cout << m.suffix() << endl;
                        //}
                        //}

                    } else {                            // invalid nesting of tags
                        cerr << "\033[39;49m\n\nERROR:  Invalid nesting of tags.  Exiting program." << endl;
                        break;
                    } 
                }
            //} 
        } 

        ln++;
    }

}

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
#define  DEFAULT_COLOR "\033[39;49m"
 using   namespace std;

/**
 * Returns true if string s (which represents a tag) comprises of 
 * only alphanumeric characters; returns false otherwise.
 */
bool is_valid_tag(const string& s) 
{
    for (size_t i = 0; i < s.size(); i++) 
    {
        if (!isalnum(s[i]))
            return false;
    }
    return true;
}

/**
 * Returns true if string s comprises entirely of white space;
 * returns false otherwise.
 */
bool is_white_space(const string& s) 
{
    for (size_t i = 0; i < s.size(); i++) 
    {
        if (!isspace(s[i]))
            return false;
    }
    return true;
}

/**
 *  Replaces the \\e part of an ansi code with \033 
 */
void replace_ecode(string& ansi) 
{
    if (ansi.substr(0,2) == "\\e")
        ansi = '\033' + ansi.substr(2);
}

/**
 * Checks command-line arguments for a configuration file name.  If
 * none provided, uses the default "config.txt" file.  Opens the 
 * config file and parses the file line-by-line to read in valid
 * tags and codes into a map.
 *
 * Returns a map of tags and codes upon success.
 */
map<string, string> load_config(int argc, char* argv[]) 
{
    string config_file;
    string line;
    string tag;                         // must be all alphanumeric
    string code;
    size_t lineNum = 0;                        
    map<string, string> config;

    // check number of arguments
    if (argc == 1) 
    {
        config_file = "config.txt";
    } else {
        config_file = argv[1];
    }
      
    ifstream fs(config_file);
    // config file cannot be opened
    if (!fs)
    {                       
        cerr << "\n\nERROR:  Cannot open file " << argv[1] << ".  Exiting program." << endl;
        exit(1);                    
    }

    // parse config file, line by line, for valid tags and codes
    while (getline(fs, line))
    {
        lineNum++;
        istringstream iss(line);

        if (iss >> tag >> code)
        {
            if (is_valid_tag(tag)) 
            {
                replace_ecode(code);
                config.insert ( pair<string, string>(tag, code) );
            } else {
                cerr << "\n\nIgnoring invalid tag " << tag << " on line " << lineNum << ".\n\n";
            }
        }
    }
    
    // check that config file contains the "text" tag
    auto it = config.find("text");
    if (it == config.end())
    {
        cerr << "\n\nERROR:  Config file does not contain 'text' tag.  Exiting program." << endl;
        exit(1);
    }

    #ifdef DEBUG
        for (auto x: config)
            cout << x.second << "<" << x.first << ">" << it->second << endl;
        exit(1);
    #endif
    
    return config;
}

void process_first_word(map<string, string>& config, vector<string>& tags, string& word, bool& ot_flag) {

    // get first word and check that it is the <text> tag
    cin >> word;

    if (word.substr(0,6) != "<text>") 
    {
        cerr << "\n\nERROR:  Input text does not start with <text> tag.  Exiting program." << endl;
         exit(1);
    } else {
        tags.push_back("text");
        ot_flag = true;
        word = word.substr(6);
        auto it = config.find("text");
        cout << it->second;                   
    }
}

int main(int argc, char* argv[]) 
{
    string line, word;
    map<string, string> config;
    vector<string> tags;

    bool ot_flag = false;               // true if open text tag  <text>  found
    bool ct_flag = false;               // true if close text tag </text> found
    size_t lineNum = 0;                 // line number

    // load configuration file
    config = load_config(argc, argv);

    // read in the first word and handle any related errors
    process_first_word(config, tags, word, ot_flag);

    // process all the rest
    while (getline(cin, line)) 
    {
        string prefix, suffix;
        smatch m;
        regex reg_tag(R"(<(.+?)>)");
        lineNum++;

        if (lineNum == 1)
            line = word + line;

        for (auto it = line.cbegin(), end = line.cend(); regex_search(it, end, m, reg_tag); it = m.suffix().first) 
        {
            prefix = m.prefix();
            suffix = m.suffix();
            string tagname = m.str(1);

            cout << prefix;
            
            if (tagname[0] != '/')                      // it is a start tag 
            {
                // check that there is no duplication of <text> tag
                if (tagname == "text" && ot_flag)
                {  
                    cerr << DEFAULT_COLOR << "\n\nERROR:  duplicate <text> tag.  Exiting program." << endl;
                    exit(1);
                }

                auto it = config.find(tagname);     // lookup tag in config map

                if (it != config.end())             // tag is found
                {           
                    cout << it->second;
                    tags.push_back(tagname);    
                } else {
                    cerr << DEFAULT_COLOR << "\n\nERROR:  <" << tagname << "> is an invalid tag.  Exiting program." << endl;
                    exit(1);
                }

            } else {                                    // it is an end tag

                if (tagname == "/text") 
                {
                    ct_flag = true;
                }
                
                // check if tagname is the last element of the vector
                if (tags.back() == tagname.substr(1))
                {
                    // remove the last tagname from the vector
                    tags.pop_back();
                    
                    // revert to previous color   
                    if (!tags.empty())
                    {
                        auto it = config.find(tags.back());
                        cout << it->second;                            
                    }

                } else {                                // invalid nesting of tags
                    cerr << DEFAULT_COLOR << "\n\nERROR:  Invalid nesting of tags.  Exiting program." << endl;
                    exit(1);
                } 
            } 
            
        } //END: for loop


        if (!is_white_space(line) && !ct_flag) 
            //cout << " ?FLAG? " << ct_flag << " length: " << suffix.length() << "line " << lineNum << "   "<< suffix << endl;
            cout << suffix << endl;

        //if (!is_white_space(line) && !ct_flag && suffix.length() == 0)
            //cout << lineNum << ":" << suffix; 
            //cout << suffix; 

        // no tags found in line
        if (!regex_search(line, m, reg_tag))
        {
            if (!ct_flag)
                cout << line << endl;

            if (ct_flag && !is_white_space(line)) 
            {
                cerr << DEFAULT_COLOR << "\n\nERROR:  Text found after </text> tag.  Exiting program." << endl;
                exit(1);
            }
        } 

    } // END: while loop

    // check if </text> tag found after reading in all input text
    if (!ct_flag) {
        cerr << DEFAULT_COLOR << "ERROR:  No </text> tag found.  Exiting program." << endl;
        exit(1);
    }

}
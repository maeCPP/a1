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
    map<string, string> config_map;

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
                config_map.insert ( pair<string, string>(tag, code) );
            } else {
                cerr << "\n\nIgnoring invalid tag " << tag << " on line " << lineNum << ".\n\n";
            }
        }
    }
    
    // check that config file contains the "text" tag
    auto it = config_map.find("text");
    if (it == config_map.end())
    {
        cerr << "\n\nERROR:  Config file does not contain 'text' tag.  Exiting program." << endl;
        exit(1);
    }

/*  
    !!! TESTING MAP -- REMOVE CODE BEFORE SUBMITTING

    cout << "------------------------------------------------------" << endl;
    for (auto& i: config_map) {
        cout << i.first << "\t=> " << i.second << endl;
    }
    cout << "------------------------------------------------------" << endl;

*/
    #ifdef DEBUG
        for (auto x: config_map)
            cout << x.second << "<" << x.first << ">" << it->second << endl;
        exit(1);
    #endif
    
    return config_map;
}


int main(int argc, char* argv[]) 
{
    string line, word;
    map<string, string> config_map;
    vector<string> v;
    smatch m;
    string prefix, suffix;

    regex reg_tag(R"(<(.+?)>)");
    regex reg_ot(R"(<text>)");
    regex reg_ct(R"(</text>)");
    bool ot_flag = false;               // true if open text tag  <text>  found
    bool ct_flag = false;               // true if close text tag </text> found
    size_t lineNum = 0;                 // line number
    size_t tagNum = 0;                  // tag number

    // load configuration file
    config_map = load_config(argc, argv);

    //cout << "152:  (ot_flag, ct_flag):  (" << ot_flag << ',' << ct_flag << ')' << endl;
    // get first word and check that it is the <text> tag
    cin >> word;

    if (word.substr(0,6) != "<text>") {
        cerr << "\n\nERROR:  Input text does not start with <text> tag.  Exiting program." << endl;
         exit(1);
    } else {
        v.push_back("text");
        tagNum++;
        ot_flag = true;
        word = word.substr(6);
        auto it = config_map.find("text");
        cout << it->second;                   
        // cout << "\nnew word: " << word << endl;
    }


    /* TO-DO:  handle case where input text does not end with </text> tag */

    /* TO-DO:  handle case where <text> or </text> occur more than once */

    //cout << "173:  (ot_flag, ct_flag):  (" << ot_flag << ',' << ct_flag << ')' << endl;
    // process all the rest
    while (getline(cin, line)) 
    {
        lineNum++;
        //cout << "line: " << lineNum << "    ";
        
        // preserve lines with only white space
        //if (is_white_space(line)) {
        //    // cout << "line number: " << lineNum << endl;
        //    cout << endl;
        //}

        if (lineNum == 1)
            line = word + line;

        for (auto it = line.cbegin(), end = line.cend(); regex_search(it, end, m, reg_tag); it = m.suffix().first) 
        {
            prefix = m.prefix();
            suffix = m.suffix();
            string tagname = m.str(1);

            cout << prefix;

            //cout << "\n167:  <" << tagname << ">  lineNum:  " << lineNum << "  ot_flag? " << ot_flag << ' ' << "tagNum: " << tagNum << endl; 

            // // in first line
            // if (lineNum == 1) 
            // {
            //     // in first line, found <text> and no previous tags have been scanned in
            //     if (tagname == "text" && tagNum == 0)
            //     {
            //         tagNum++;
            //         ot_flag = true;


            //         // check that <text> is preceeded only by white space
            //         if (!is_white_space(m.prefix())) 
            //         {
            //             cerr << "\033[39;49m\n\nERROR:  182  Input text does not start with <text> tag.  Exiting program." << endl;
            //             exit(1);
            //         }

            //         // check that <text> tag not duplicated
            //         //if (ot_flag) {
            //         //    cerr << "\033[39;49m\n\nERROR:  duplicate <text> tag on line " << lineNum << ".  Exiting program." << endl;
            //         //    exit(1);
            //         //}
                    

            //         //cout << "191:  " << tagname << "  ot_flag? " << ot_flag << ' ' << "tagNum: " << tagNum << endl; 

            //         // check that the <text> tag is not preceded by non white space characters
                    
            //     // in first line, found <text> but it's not the first tag in that line
            //     } else if (tagname == "text" && tagNum != 0) {
            //         cerr << "\033[39;49m\n\nERROR:  198  Input text does not start with <text> tag.  Exiting program." << endl;
            //         exit(1);
            //     // tag is anything but <text>
            //     } else {
            
            if (tagname[0] != '/')                      // it is a start tag 
            {
                // check that there is no duplication of <text> tag
                if (tagname == "text" && ot_flag)
                {  
                    cerr << "\033[39;49m\n\nERROR:  duplicate <text> tag.  Exiting program." << endl;
                    exit(1);
                }

                tagNum++;
                auto it = config_map.find(tagname);     // lookup tag in config map

                if (it != config_map.end())             // tag is found
                {           
                    cout << it->second;
                    v.push_back(tagname);    
                } else {
                    cerr << "\033[39;49m\n\nERROR:  <" << tagname << "> is an invalid tag.  Exiting program." << endl;
                    exit(1);
                }
            //cout << "254:  (ot_flag, ct_flag):  (" << ot_flag << ',' << ct_flag << ')' << endl;
            } else {                                    // it is an end tag

                if (tagname == "/text") {
                    //cout << "-------- found /text" << endl;
                    ct_flag = true;
                }
                

                // if (!v.empty()) {

                    /*
                    cerr << "v = [";
                    for (auto vit = v.begin(); vit != v.end()-1; ++vit)
                        cerr << *vit << ", ";
                    cerr << v.back() << "]" << endl;
                    */
                   

                    
                    // check if tagname is the last element of the vector
                    if (v.back() == tagname.substr(1))
                    {
                        // remove the last tagname from the vector
                        v.pop_back();
                        
                        // revert to previous color   
                        if (!v.empty())
                        {
                            auto it = config_map.find(v.back());
                            cout << it->second;                            
                        }

                    } else {                            // invalid nesting of tags
                        cerr << "\033[39;49m\n\nERROR:  Invalid nesting of tags.  Exiting program." << endl;
                        exit(1);
                    } 
                // }
                //cout << "292:  (ot_flag, ct_flag):  (" << ot_flag << ',' << ct_flag << ')' << endl;
            } 
            
        } // closes for loop

        //cout << "this is the last one:  " << suffix << endl;
        if (!is_white_space(line)) 
            cout << suffix << endl;
        else
            cout << endl;

        //cout << "300: (ot_flag, ct_flag):  (" << ot_flag << ',' << ct_flag << ')' << endl;

        if (!regex_search(line, m, reg_tag))
        {
            if (!ct_flag)
                cout << line;

            if (ct_flag && !is_white_space(line)) 
            {
                cerr << "\033[39;49m\n\nERROR:  Text found after </text> tag.  Exiting program." << endl;
                exit(1);
            }
        } 

    } // closes while loop

    if (!ct_flag) {
        cerr << "\033[39;49m\n\nERROR:  No </text> tag found.  Exiting program." << endl;
        exit(1);
    }

}
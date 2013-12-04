//No to C style strings

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <castConfig.h>


using namespace std;

castConfig::castConfig(string fname)
{
    string castPath;
    castPath = getenv("CAST_PATH");
    string file2read = castPath +"/inputs/"+ fname;
    //cout << "Config File: " << file2read << endl;

    ifstream cfgFile;                   // create object for config file
    cfgFile.open(file2read.c_str());    // Opening the file

    //Checking if file is opened without error
    if (!cfgFile.is_open()) { cout << "Could not open the file " << file2read << " -> Terminating..." << endl; exit(EXIT_FAILURE); }

    string key, value;
    string line;
    size_t found;

    // Looping over lines in config file
    for (int j = 0; cfgFile.good(); j++)
    {
        //Reading line
        getline (cfgFile,line);

        // ignoring Commented lines
        if (line[0]=='#')
            continue;

        //cout << line << endl;

        // Getting the position of space
        found=line.find(' ');

        //If space is not found ignoring
        if (found==std::string::npos)
            continue;

        key = line.substr (0,found);
        value = line.substr (found+1);
        //cout << "Key: " << key << endl;
        //cout << "Value: " << value << endl;

        // Path Options{{{
        if (key=="OUT_PATH")
            outpath = value;
        //}}}

        // castProfile options {{{
        if (key=="PCENTER")
            pCenter = bool(atoi(value.c_str()));

        if (key=="INCREMENT")
            increment = double(atof(value.c_str()));

        if (key=="USEPROFILE")
            useProfile = bool(atoi(value.c_str()));
        if (!useProfile)
            increment = 0.0;

        //}}}

    }

}

castConfig::~castConfig(){}

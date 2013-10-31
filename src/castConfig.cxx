

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
    string  file2read = castPath +"/inputs/"+ fname;
    cout << file2read << endl;

    ifstream cfgFile;               // create object for output
    cfgFile.open(file2read.c_str());

    //Checking if file is opened without error
    if (!cfgFile.is_open()) { cout << "Could not open the file " << file2read << " -> Terminating..." << endl; exit(EXIT_FAILURE); }

    string key, value;
    string line;
    for (int j = 0; cfgFile.good(); j++)
    {

        getline (cfgFile,line);
        if (line[0]=='#')
            continue;

        cout << line << endl;

        size_t found;
        found=line.find(' ');
        if (found!=std::string::npos)
            std::cout << "Period found at: " << found << '\n';
        else
            continue;

        key = line.substr (0,found);
        value = line.substr (found);
        cout << "Key: " << key << endl;
        cout << "Value: " << value << endl;

        if (key=="PCENTER")
            pCenter = bool(atoi(value.c_str()));

        if (key=="USEPROFILE")
            useProfile = bool(atoi(value.c_str()));

    }

}

castConfig::~castConfig(){}

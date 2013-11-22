/**********************************************************************
Class to read config file
Based on Javier Galan's micromegas analysis library

Author: Cenk Yıldız
Bogazici University

Date: 31 Oct 2013
***********************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>

#ifndef _CAST_CONFIG_
#define _CAST_CONFIG_

using namespace std;

class castConfig
{

    private:
        double a;
    
    public:

	castConfig(string fname);
	~castConfig();

        // castProfile options {{{

        //castProfile options
	bool pCenter;

        // Use Density Profile to calculate Conversion Probability
        bool useProfile;

        // Increment to integrate density profile
        double increment;
        //}}}

};
#endif


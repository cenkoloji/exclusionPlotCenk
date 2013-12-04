/**********************************************************************
Density profile inside the magnet

Author: Cenk Yıldız
Bogazici University

Date: 28 Oct 2013
***********************************************************************/

#ifndef _CASTPROFILE_
#define _CASTPROFILE_

#include <iostream>
#include<castGas.h>
#include<castConfig.h>


class castProfile
{

    public:

        //Constructor from PCB(Pa), Tmag(K), angle(deg), cfg(castConfig)
        castProfile(castGas * cG, double PCB, double Tmag, double angle, castConfig * cfg);
        ~castProfile();

        castGas *gas;       //The parameters of the gas
        castConfig *cfg;    //The config class instant

        double press;         //Pressure measured at PCB sensor position or in the center of cold bore in Pascals
        double Tmag;        //Magnet temperature in Kelvins
        double angle;       //Magnet inclination angle in degrees
        double lenstart;    //Start length of the density profile (-5 - +5 m)
        double lenend;      //End length of the density profile (-5 - +5 m)
        double increment;   //Increment in the profile
        double centerdensity; //Density in the center of the coldbore
        double centerpressure; //Pressure in the center of the coldbore
        bool pCenter;           // If true, input pressure is central PCB
        int elements;       //Number of points in the profile
        double * density;   //Density array in [kg/m3]
        double * pressure;  //Pressure array in [Pa]

};
#endif


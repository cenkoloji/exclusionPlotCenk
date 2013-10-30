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


class castProfile
{

    public:

        // PCB(Pa), Tmag(K), angle(deg)
        castProfile(castGas * cG, double PCB, double Tmag, double angle);
        ~castProfile();

        castGas *gas;       //The parameters of the gas

        double PCB;         //Pressure measured by PCB sensor in Pascals
        double Tmag;        //Magnet temperature in Kelvins
        double angle;       //Magnet inclination angle in degrees
        double lenstart;    //Start length of the density profile (-5 - +5 m)
        double lenend;      //End length of the density profile (-5 - +5 m)
        double increment;   //Increment in the profile
        double centerdensity; //Density in the center of the coldbore
        double centerpressure; //Pressure in the center of the coldbore
        int elements;       //Number of points in the profile
        double * density;   //Density array in [kg/m3]
        double * pressure;  //Pressure array in [Pa]

};
#endif


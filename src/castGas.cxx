// castGas class methods

#include <iostream>
#include <castGas.h>
#include <cmath>

using namespace std;

//ClassImp(castGas)

//castGas::castGas(double atw, castMagnet *CASTM, int t) // Constructor {{{
castGas::castGas(double atw, castMagnet *CASTM, int t)
{
    atomicwg = atw;
    type=t;
    mag = CASTM;
} // }}}

castGas::~castGas(){ }

//getPhotonMass(double pressure,double angle) return the expected mass of the axion in eV type 0 for 4He(ideal gas); type 1 for 3He HW (Jaime's formula) and type 2 for CW (Jaime's formula) {{{
double castGas::getPhotonMass(double pressure,double angle){
    // Same as Juanan and Theodoros, In my calculation const is 23.5 (compatible)
    // ***ANGLE

    if(type==0)
        return sqrt( 0.020 * pressure / mag->getTMag());
    else if(type==1)
        return 23.46550825*sqrt(getGasDensity(pressure)); // 23.5 would be enough I think. I found this constant a bit different
    else if(type==2)
        return 23.46550825*sqrt(getGasDensity(pressure)); // 23.5 would be enough I think. I found this constant a bit different
    else 
        return 0;
} // }}}

//getGasDensity(double pressure, double angle) return the density in (gr*cm-3) inside the coldbore type 0 for 4He(ideal gas); type 1 for 3He HW (Jaime's formula) and type 2 for CW (Jaime's formula) {{{
double castGas::getGasDensity(double pressure,double angle){
    // Formulas are from Jaime(same as Juanan and Theodoros)
    // ***ANGLE

    if(type==0)
        return pressure*atomicwg/(mag->getTMag()*83144.72);
    else if(type==1)
    {
        double den = 1E-3*(0.02099*pressure-0.004914);
        if(den>=0.)
            return den;
        else return 0;
    }
    else if(type==2)
    {
        double den = 1E-3*(2.87E-5*pressure*pressure+0.02005*pressure-0.001571);
        if(den>=0.)
            return den;
        else return 0;
    }
    else 
        return 0;
} // }}}

//getMagnetLength(double pressure,double angle) return the magnet lengt(m) for a given pressure inside de Coldbore {{{
double castGas::getMagnetLength(double pressure, double angle){  
    // TODO: Find correct formulas!
    // ***ANGLE

    if(type==0)
        return mag->getLenMag();
    else if(type==1)
        return (922.3-4.8*pressure)*1E-2;
        //return (815.7-2.694*pressure)*1E-2;
    else if (type==2)
        return (861.1-2.1*pressure)*1E-2; // Nuno CFD
        //return 7.82; // Jaime
    else 
        return 0;

} // }}}

//Show() Simple function to print basic information about gas {{{
void castGas::Show() // 
{
cout << "\nGas Information" << endl;
cout << "================" << endl;
cout << "Gas Type: " << type << endl; 
cout << "Atomic Weight: " << atomicwg << endl; 
} // }}}

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

//getPhotonMass(double density) return the expected mass of the axion in eV type 0 for 4He(ideal gas); type 1 for 3He HW (Jaime's formula) and type 2 for CW (Jaime's formula) {{{
double castGas::getPhotonMass(double density){
    // Same as Juanan and Theodoros, In my calculation const is 23.5 (compatible)

    return 23.46550825*sqrt(density); // 23.5 would be enough I think. I found this constant a bit different

} // }}}

//getMagnetLength(double pressure,double angle) return the magnet lengt(m) for a given pressure inside de Coldbore {{{
double castGas::getMagnetLength(double pressure, double angle)
{
    // ***ANGLE may be added later

    if(type==0)
        return mag->getLenMag();
    else if(type==1)
        return (922.3-4.8*pressure)*1E-2; // Nuno CFD
    else if (type==2)
        return (861.1-2.1*pressure)*1E-2; // Nuno CFD
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

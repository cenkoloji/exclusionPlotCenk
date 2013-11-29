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

    rgas = 8314.34 / 3.016;
    a0 = 0.457235*rgas*rgas*TCRIT*TCRIT/PCRIT;  /*coefficients from Wikipedia, sligthly different values in Fluent user guide ... */
    b0 = 0.077796*rgas*TCRIT/PCRIT;
    k0 = 0.37464+1.54226*ACENT-0.26992*ACENT*ACENT;


} // }}}

castGas::~castGas(){ }

//getPhotonMass(double density) return the expected mass of the axion in eV type 0 for 4He(ideal gas); type 1 for 3He HW (Jaime's formula) and type 2 for CW (Jaime's formula) for density(gr/cm3) {{{
double castGas::getPhotonMass(double density){
    // Same as Juanan and Theodoros, In my calculation const is 23.5 (compatible)

    return 23.46550825*sqrt(density); // 23.5 would be enough I think. I found this constant a bit different

} // }}}

//getPhotonMass(double density) return the expected mass of the axion in eV type 0 for 4He(ideal gas); type 1 for 3He HW (Jaime's formula) and type 2 for CW (Jaime's formula) for density(gr/cm3) {{{
double castGas::getDensityFromPhotonMass(double ma){
    // Same as Juanan and Theodoros, In my calculation const is 23.5 (compatible)

    return pow(ma/23.46550825,2); // 23.5 would be enough I think. I found this constant a bit different

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

double castGas::getDensity(double temp, double press) // {{{
{
    // Taken from Enrico Da Riva's code used in Fluent CFD  simulations
    double a1,a2,a3;   /* v^3 + a1 v^2 + a2 v^3 + a3 = 0*/
    double vv,vv1,vv2,vv3;
    double qq,qq3,sqq,rr,rrabs,tt,dd;
    /*old     double afun = a0*pow(TCRIT/temp,NRK);*/
    double alfapr = (1.+k0*(1.-sqrt(temp/TCRIT))) * (1.+k0*(1.-sqrt(temp/TCRIT)));
    a1 = b0-rgas*temp/press;
    a2 = a0*alfapr/press - 3.*b0*b0 - 2.*b0*rgas*temp/press;
    a3 = b0*b0*b0 - a0*b0*alfapr/press + rgas*temp*b0*b0/press;
    /*old     a1 = c0-rgas*temp/press;
    *      a2 = -(bb*b0+rgas*temp*b0/press-afun/press);
    *           a3 = -afun*bb/press;*/
    /* Solve cubic equation for specific volume */
    qq = (a1*a1-3.*a2)/9.;
    rr = (2.*a1*a1*a1-9.*a1*a2+27.*a3)/54.;
    qq3 = qq*qq*qq;
    dd = qq3-rr*rr;
    /* If dd < 0, then we have one real root THIS PART WAS WRONG IN THE ORIGINAL UDF FROM FLUENT!!!!!*/
    /* If dd >= 0, then we have three roots -> choose largest root */
    if (dd < 0.) 
    {
        /*      tt = sqrt(-dd)+pow(fabs(rr),0.333333);*/ /* original Fluent UDF */
        /*      tt = -1.*pow(rr + sqrt(-1.*dd),0.3333333333);*/ /*this was wrong because it may be negative!!!!*/
        rrabs = fabs(rr);
        tt = -1.*rr/rrabs*pow(rrabs + sqrt(-1.*dd),0.3333333333);
        vv = (tt+qq/tt)-a1/3.;
    }
    else
    {
        tt = acos(rr/sqrt(qq3));
        sqq = sqrt(qq);
        vv1 = -2.*sqq*cos(tt/3.)-a1/3. ;

        vv2 = -2.*sqq*cos((tt+2.*PI)/3.)-a1/3. ;

        vv3 = -2.*sqq*cos((tt+4.*PI)/3.)-a1/3. ;

        vv = (vv1 > vv2) ? vv1 : vv2;
        vv = (vv > vv3) ? vv : vv3;
    }
    return 1./vv;
} //}}}

double castGas::getPressure(double temp, double density) // {{{
{
     double v = 1./density;
     double alfapr = (1.+k0*(1.-sqrt(temp/TCRIT))) * (1.+k0*(1.-sqrt(temp/TCRIT)));
     return rgas*temp/(v-b0)-a0*alfapr/(v*v+2.*b0*v-b0*b0);
} // }}}

/*
double castGas::getHydrostatic(double press, double temp, double x, double angle) // {{{
{

    // dens(kg/m3) * h(m) * g(m/s2)

    double density = getDensity(temp,press);

    double height = sin(angle*PI/180.) * ( POSPCB - x );

    return density * height * GRAVITY;

    //P(x) = PCB + Phydro

} // }}}
*/

double castGas::getHydrostatic(double PCB, double temp, double x, double angle) // {{{
{

    //double hydro1 = getHydrostatic(PCB, temp, POSPCB, x, angle);

    // dens(kg/m3) * h(m) * g(m/s2)
    
    double increment = 0.01; //m

    double height = sin(angle*PI/180.) * increment;

    double density;
    double phydro = 0;
    double pressure = PCB;

    for (double position = POSPCB; position < x; position = position + increment)
    {
        density = getDensity(temp,pressure);
        phydro = phydro + density * height * GRAVITY;
        pressure = pressure + phydro;
    }

    // cout << hydro1 << " " <<  phydro << " " << hydro1- phydro   << " " << (hydro1 - phydro)*100/phydro  << endl;
    return phydro;
   

} // }}}

double castGas::getHydrostatic(double press, double temp, double x0, double x1, double angle) // {{{
{

    // dens(kg/m3) * h(m) * g(m/s2)

    double density = getDensity(temp,press);
    double height = sin(angle*PI/180.) * ( x1 - x0 );
    return density * height * GRAVITY;

} // }}}


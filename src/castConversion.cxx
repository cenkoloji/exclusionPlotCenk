#include <cmath>
#include <castConversion.h>
//#include <castGas.h>
//#include <castMagnet.h>

//ClassImp(castConversion)

//castConversion::castConversion(castMagnet *castMag, castGas *Cg) Constructor with the classes CASTMagnet and CASTGas {{{
castConversion::castConversion(castMagnet *castMag, castGas *Cg){

mag = castMag;
gas = Cg;

} // }}} 

castConversion::~castConversion(){}

//ConversionProbability(double ma, double Ea, double mgamma,double angle) Probability of conversion for a axion mass (eV), a energy (keV), a pressure (mbar) and angle(deg) {{{
double castConversion::ConversionProbability(double ma, double Ea, double press,double angle){
// ANGLE***

if( ma == 0.0) return CalculateNormalizedFactor(press);

double mgamma = gas->getPhotonMass(press); // ***ANGLE

double q = (ma*ma - mgamma*mgamma)/2./Ea/1000.0; // in eV
double l = gas->getMagnetLength(press); // in m // ***ANGLE
double phi = q * (l * Lconversion); // l*Lconversion is in eV^{-1}

double Gamma = 100 * AbsorptionCoeff( Ea, press ); // cm-1 to m-1 // ***ANGLE
double GammaL = Gamma * gas->getMagnetLength(press);

//if(GammaL==0.0)return CalculateNormalizedFactor()*4*sin(phi/2.)*sin(phi/2.)/(phi*phi);

double MFactor = phi*phi + GammaL*GammaL/4.0; // (qL)^2 + (\Gamma L)^2
MFactor = 1.0/MFactor;

double sol = MFactor * CalculateNormalizedFactor(press)*( 1.0 + std::exp(-1.0*GammaL) - 2.0 * std::exp(-0.5 * GammaL) * std::cos(phi) );
//Above line was as below in Juanan code. But mgamma doesn't make sense there
//double sol = MFactor * CalculateNormalizedFactor(mgamma)*( 1.0 + TMath::Exp(-1.0*GammaL) - 2.0 * TMath::Exp(-0.5 * GammaL) * TMath::Cos(phi) );

    return sol;

} // }}}

//CalculateNormalizedFactor() return the normalized factor for the conversion probability ( g10^2(BL/2)^2 ) {{{
double castConversion::CalculateNormalizedFactor(double press,double angle){

    //double tm = lightSpeed / naturalElectron * 1.0e-9; // gev
    double sol = gas->getMagnetLength(press) * mag->getBMag() * BLconversion / 2.0; 
    sol = sol* sol * 1.0e-20; // 10e-20: for normalization g_agamma to g_10
    return sol;

}// }}}

//AbsorptionCoeff(double en, double p, double angle) absortion coefficient factor(cm-1) for a pressure, angle and a energy in the magnet (energy in keV,angle in deg and p in mbar) {{{
double castConversion::AbsorptionCoeff(double en, double p,double angle)
{

    double densityHe = gas->getGasDensity(p);//in [g/cm^3] // ***ANGLE
    double mu;
 
    //Cenk's implementation (For some reason, Juanan and Javier's used Annika's version, which is worse)
    mu = std::exp(-1.5832 + 5.9195 * std::exp(-0.353808*en)+4.03598*std::exp(-0.970557*en)); // mu/ro in NIST

/*  {{{ Annika's version (worse fit)  
    double x,f1,mu;

    x = log10(en);

    //fit for mu(en) using data from NIST database
    //mu(en) is the sum of atomic photoabsorption and inelastic scattering
    //cross section, including Rayleigh cross section as well mu[cm^2/g],

    f1=1.8101-3.2108*x-0.007392*TMath::Power(x,2.0)-1.8815*TMath::Power(x,3.0)+4.8621*TMath::Power(x,4.0)-2.1921*TMath::Power(x,5.0);
    mu = TMath::Power(10,f1);

*/ // }}}

    //Gamma = mu/ro * density
    double absorpcoeff = densityHe * mu;
    return absorpcoeff; // cm-1 

} // }}}

//getAxionFlux( double en ) Axion flux cm-2 s-1 keV-1 (energy in eV) {{{
double castConversion::getAxionFlux( double en ){

    double factor = 6.020e10;
    double sol = factor * std::pow( en, 2.481) * std::exp( -en/1.205);
    return sol;

} // }}}

//ExpectedNumberOfCounts( double Ei, double Ef, double ma, double press, double time ){{{
double castConversion::ExpectedNumberOfCounts( double Ei, double Ef, double ma, double press, double time ){

double totalCounts = 0.0,Enrg;

    double Estep = 0.05;
    for ( double E = Ei; E <=Ef-Estep; E = E + Estep )
    {
	    Enrg = E + 0.5 * Estep;
	    totalCounts +=  ConversionProbability( ma, Enrg, press ) * getAxionFlux( Enrg ) * time * Estep;
	    //printf("%lf\n",Enrg);
    }

    return totalCounts;

} // }}}


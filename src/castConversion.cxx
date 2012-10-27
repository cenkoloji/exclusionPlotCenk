#include <cmath>
#include <castConversion.h>
//#include <castGas.h>
//#include <castMagnet.h>

//ClassImp(castConversion)

//castConversion::castConversion(castMagnet *castMag, castGas *Cg) Constructor with the classes CASTMagnet and CASTGas {{{
castConversion::castConversion(castMagnet *castMag, castGas *Cg)
{
    mag = castMag;
    gas = Cg;
} // }}} 

castConversion::~castConversion(){}

//ConversionProbability(double ma, double Ea, double mgamma,double angle) Probability of conversion for a axion mass (eV), a energy (keV), a pressure (mbar) and angle(deg)  {{{
double castConversion::ConversionProbability(double ma, double Ea, double press,double angle)
{
    // ANGLE***

    if( ma == 0.0) return CalculateNormalizedFactor(press);

    double mgamma = gas->getPhotonMass(press); // ***ANGLE

    double q = (ma*ma - mgamma*mgamma)/2./Ea/1000.0; // in eV
    double l = gas->getMagnetLength(press); // in m // ***ANGLE
    double qL = q * (l * Lconversion); // l*Lconversion is in eV^{-1}

    double Gamma = 100 * AbsorptionCoeff( Ea, press ); // cm-1 to m-1 // ***ANGLE
    double GammaL = Gamma * gas->getMagnetLength(press);

    //if(GammaL==0.0)return CalculateNormalizedFactor()*4*sin(phi/2.)*sin(phi/2.)/(phi*phi);

    double MFactor = qL*qL + GammaL*GammaL/4.0; // (qL)^2 + (\Gamma L)^2
    MFactor = 1.0/MFactor;

    double fac = MFactor * CalculateNormalizedFactor(press)*( 1.0 + std::exp(-1.0*GammaL) - 2.0 * std::exp(-0.5 * GammaL) * std::cos(qL) );
    //Above line was as below in Juanan code. He confirmed it is faulty
    //double sol = MFactor * CalculateNormalizedFactor(mgamma)*( 1.0 + TMath::Exp(-1.0*GammaL) - 2.0 * TMath::Exp(-0.5 * GammaL) * TMath::Cos(phi) );

    return fac;

} // }}}

//CalculateNormalizedFactor() return the normalized factor for the conversion probability ( g10^2(BL/2)^2 ) {{{
double castConversion::CalculateNormalizedFactor(double press,double angle)
{
    //*** ANGLE

    //double tm = lightSpeed / naturalElectron * 1.0e-9; // gev
    double fac = gas->getMagnetLength(press) * mag->getBMag() * BLconversion / 2.0; 
    fac = fac* fac * 1.0e-20; // 10e-20: for normalization g_agamma to g_10
    return fac;

}// }}}

//AbsorptionCoeff(double en, double p, double angle) absortion coefficient factor(cm-1) for a pressure, angle and a energy in the magnet (energy in keV,angle in deg and p in mbar) {{{
double castConversion::AbsorptionCoeff(double en, double p,double angle)
{

    double densityHe = gas->getGasDensity(p);//in [g/cm^3] // ***ANGLE
    double mu;
 
    //Cenk's implementation (For some reason, Juanan and Javier's used Annika's version, which is worse)
    mu = std::exp(-1.5832 + 5.9195 * std::exp(-0.353808*en)+4.03598*std::exp(-0.970557*en)); // expressed as mu/ro in NIST

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

//ExpectedNumberOfCounts( double Ei, double Ef, double ma, double press, double time ) Ei and Ef(keV), ma(eV), press(mbar), time(sec){{{
double castConversion::ExpectedNumberOfCounts( double Ei, double Ef, double ma, double press, double time )
{

    //TODO: Step structure can be removed and made single step
    double totalCounts = 0.0;
    double Enrg;

    double Estep = 0.05;
    for ( double E = Ei; E <=Ef-Estep; E = E + Estep )
    {
	    Enrg = E + 0.5 * Estep;
	    totalCounts +=  ConversionProbability( ma, Enrg, press ) * getAxionFlux( Enrg ) * time * Estep;
	    //printf("%lf\n",Enrg);
    }

    return totalCounts;

} // }}}


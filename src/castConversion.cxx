#include <cmath>
#include <complex>
#include <castConversion.h>
#include <castProfile.h>

//ClassImp(castConversion)

/*
//castConversion::castConversion(castMagnet *castMag, castGas *Cg) Constructor with the classes CASTMagnet and CASTGas {{{
castConversion::castConversion(castMagnet *castMag, castGas *Cg, castProfile * castProf)
{
    mag = castMag;
    gas = Cg;
    prof = castProf;
} // }}} 
*/
castConversion::~castConversion(){}

//ConversionProbability(double ma, double Ea, double pressure, double density, double angle) Probability of conversion for a axion mass (eV), a energy (keV), a pressure (mbar), density(g/cm3) and angle(deg)  {{{
double castConversion::ConversionProbability(double ma, double Ea, double pressure, double density, double angle)
{
    // ANGLE***

    if( ma == 0.0) return CalculateNormalizedFactor(pressure);

    double mgamma = gas->getPhotonMass(density);

    double q = (ma*ma - mgamma*mgamma)/2./Ea/1000.0; // in eV
    double l = gas->getMagnetLength(pressure); // in m // ***ANGLE
    double qL = q * (l * Lconversion); // l*Lconversion is in eV^{-1}

    double Gamma = 100 * AbsorptionCoeff( Ea, density); // cm-1 to m-1 // ***ANGLE
    double GammaL = Gamma * gas->getMagnetLength(pressure);

    //if(GammaL==0.0)return CalculateNormalizedFactor()*4*sin(phi/2.)*sin(phi/2.)/(phi*phi);

    double MFactor = qL*qL + GammaL*GammaL/4.0; // (qL)^2 + (\Gamma L)^2
    MFactor = 1.0/MFactor;

    double fac = MFactor * CalculateNormalizedFactor(pressure)*( 1.0 + std::exp(-1.0*GammaL) - 2.0 * std::exp(-0.5 * GammaL) * std::cos(qL) );
    //Above line was as below in Juanan code. He confirmed it is faulty
    //double sol = MFactor * CalculateNormalizedFactor(mgamma)*( 1.0 + TMath::Exp(-1.0*GammaL) - 2.0 * TMath::Exp(-0.5 * GammaL) * TMath::Cos(phi) );

    return fac;

} // }}}

double castConversion::ConversionProbabilityFromLength(double ma, double Ea, double pressure, double temperature, double angle) // {{{
{
    double L0,L1,L,dL;
    int N;

    castProfile * profile = new castProfile(gas, pressure*100, temperature, angle,cfg);

    L0 = profile->lenstart;
    L1 = profile->lenend;
    L = L1 - L0;        //magnet length
    dL = profile->increment;
    N = profile->elements;
    
    double normfac = L * mag->getBMag() * BLconversion / 2.0; 
    normfac = normfac * normfac * 1.0e-20; // 10e-20: for normalization g_agamma to g_10

    double centerdensity = profile->centerdensity*1E-3;

    double mgamma = gas->getPhotonMass(centerdensity);

    double q = (ma*ma - mgamma*mgamma)/2./Ea/1000.0; // in eV
    double qL = q * (L * Lconversion); // l*Lconversion is in eV^{-1}

    double Gamma = 100 * AbsorptionCoeff( Ea, centerdensity); // cm-1 to m-1 // ***ANGLE
    double GammaL = Gamma * L;

    //if(GammaL==0.0)return CalculateNormalizedFactor()*4*sin(phi/2.)*sin(phi/2.)/(phi*phi);

    double MFactor = qL*qL + GammaL*GammaL/4.0; // (qL)^2 + (\Gamma L)^2
    MFactor = 1.0/MFactor;

    double fac = MFactor * normfac * ( 1.0 + std::exp(-1.0*GammaL) - 2.0 * std::exp(-0.5 * GammaL) * std::cos(qL) );

    return fac;

} // }}}

double castConversion::ConversionProbabilityFromProfile(double ma, double Ea, double pressure, double temperature, double angle) //  {{{
{

    double P,d,L0,L1,L,dL,position,mgamma,sum1,q,amplitude,exponent;
    complex<double> sum2 (0.0,0.0);
    int N;
    castProfile * profile = new castProfile(gas, pressure*100, temperature, angle,cfg);

    L0 = profile->lenstart;
    L1 = profile->lenend;
    L = L1 - L0;        //magnet length
    dL = profile->increment;
    N = profile->elements;

    double fac = mag->getBMag() * BLconversion / 2.0;    
    fac = fac * fac * 1.0E-20;  // 10e-20: for normalization g_agamma to g_10
    
    double density = profile->density[0]*1E-3;
    double Gamma = 100 * AbsorptionCoeff( Ea, density); // cm-1 to m-1

    double term1 = std::exp(-0.5 * Gamma * L);

    // Calculating integrals over the length
    for (int i = 0; i < N; i++) {
        position = i*dL;
        P = profile->pressure[i];
        d = profile->density[i]*1E-3;
        mgamma = gas->getPhotonMass(d);

        //cout << L0 + i*dL << "m : " << P << " " << d  << " " << gas->getPhotonMass(d) <<endl;
        sum1 = 0;
        for (int j = 0; j < i; j++)
        {
            P = profile->pressure[j];
            d = profile->density[j]*1E-3;
            mgamma = gas->getPhotonMass(d);
            q = std::abs(ma*ma - mgamma*mgamma)/2./Ea/1000.0; // 1000 for eV
            sum1 += q * dL * Lconversion;
        }
        
        amplitude =  std::exp(0.5 * Gamma * position)*dL;
        exponent = sum1;
        //if (i%100==0)
            //cout << " i: " <<  i << " , position:"  <<  position << " , Amplitude: " << amplitude << " , exponent: " << exponent << endl;
        sum2 += std::polar(amplitude,exponent);
    }

    double abscomplex = std::abs(sum2);
    //cout << "sum 2:" << sum2 << "abs 2:" << abscomplex  << endl;
    
    fac = fac * term1 * term1 * abscomplex * abscomplex;

    return fac;

    // complex<double> forth = polar(1.0,3.1415/2)
/*
    // Normalized factor
    double fac = gas->getMagnetLength(pressure) * mag->getBMag() * BLconversion / 2.0; 
    fac = fac* fac * 1.0e-20; // 10e-20: for normalization g_agamma to g_10
    return fac;

    if( ma == 0.0) return CalculateNormalizedFactor(pressure);

    double mgamma = gas->getPhotonMass(density);

    double q = (ma*ma - mgamma*mgamma)/2./Ea/1000.0; // in eV
    double l = gas->getMagnetLength(pressure); // in m // ***ANGLE
    double qL = q * (l * Lconversion); // l*Lconversion is in eV^{-1}

    double Gamma = 100 * AbsorptionCoeff( Ea, density); // cm-1 to m-1 // ***ANGLE
    double GammaL = Gamma * gas->getMagnetLength(pressure);

    //if(GammaL==0.0)return CalculateNormalizedFactor()*4*sin(phi/2.)*sin(phi/2.)/(phi*phi);

    double MFactor = qL*qL + GammaL*GammaL/4.0; // (qL)^2 + (\Gamma L)^2
    MFactor = 1.0/MFactor;

    double fac = MFactor * CalculateNormalizedFactor(pressure)*( 1.0 + std::exp(-1.0*GammaL) - 2.0 * std::exp(-0.5 * GammaL) * std::cos(qL) );
    //Above line was as below in Juanan code. He confirmed it is faulty
    //double sol = MFactor * CalculateNormalizedFactor(mgamma)*( 1.0 + TMath::Exp(-1.0*GammaL) - 2.0 * TMath::Exp(-0.5 * GammaL) * TMath::Cos(phi) );

    return fac;

*/

} // }}}

//CalculateNormalizedFactor(double pressure,double angle) return the normalized factor for the conversion probability ( g10^2(BL/2)^2 ) {{{
double castConversion::CalculateNormalizedFactor(double pressure,double angle)
{
    //*** ANGLE

    //double tm = lightSpeed / naturalElectron * 1.0e-9; // gev
    double fac = gas->getMagnetLength(pressure) * mag->getBMag() * BLconversion / 2.0; 
    fac = fac* fac * 1.0e-20; // 10e-20: for normalization g_agamma to g_10
    return fac;

}// }}}

//AbsorptionCoeff(double en, double density, double angle) absortion coefficient factor(cm-1) for a density, angle and a energy in the magnet (energy in keV,angle in deg and density in g/cm3) {{{
double castConversion::AbsorptionCoeff(double en, double density,double angle)
{

    double mu;

    //Cenk's implementation (For some reason, Juanan and Javier's used Annika's version, which is worse)
    mu = std::exp(-1.5832 + 5.9195 * std::exp(-0.353808*en)+4.03598*std::exp(-0.970557*en)); // expressed as mu/ro in NIST

    //Gamma = mu/ro * density
    double absorpcoeff = density * mu;
    return absorpcoeff; // cm-1 

} // }}}

//getAxionFlux( double en ) Axion flux cm-2 s-1 keV-1 (energy in eV) {{{
double castConversion::getAxionFlux( double en ){

    double factor = 6.020e10;
    double sol = factor * std::pow( en, 2.481) * std::exp( -en/1.205);
    return sol;

} // }}}

//ExpectedNumberOfCounts( double Ei, double Ef, double ma, double pressure, double temperature, double angle, double time ) Ei and Ef(keV), ma(eV), press(mbar), temp(K), angle(deg), time(sec){{{
double castConversion::ExpectedNumberOfCounts( double Ei, double Ef, double ma, double pressure, double temperature, double angle, double time )
{

    //cout << "calculating Exp Num count" << endl;
    double totalCounts = 0.0;
    double Enrg;
    double Estep = 0.05;
    //cout << pressure << " " << temperature  << " " <<angle << " " << endl;
    for ( double E = Ei; E <=Ef-Estep; E = E + Estep )
    {
	    Enrg = E + 0.5 * Estep;
	    totalCounts +=  ConversionProbabilityFromProfile( ma, Enrg, pressure,temperature,angle ) * getAxionFlux( Enrg ) * time * Estep;
	    //printf("%lf\n",Enrg);
    }

    return totalCounts;

} // }}}

//ExpectedNumberOfCounts( double Ei, double Ef, double ma, double pressure, double density, double time ) Ei and Ef(keV), ma(eV), press(mbar), time(sec){{{
double castConversion::ExpectedNumberOfCounts( double Ei, double Ef, double ma, double pressure, double density, double time )
{

    double totalCounts = 0.0;
    double Enrg;
    double Estep = 0.05;
    for ( double E = Ei; E <=Ef-Estep; E = E + Estep )
    {
	    Enrg = E + 0.5 * Estep;
	    totalCounts +=  ConversionProbability( ma, Enrg, pressure,density ) * getAxionFlux( Enrg ) * time * Estep;
	    //printf("%lf\n",Enrg);
    }

    return totalCounts;

} // }}}

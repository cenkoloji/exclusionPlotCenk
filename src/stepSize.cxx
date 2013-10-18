#include <iostream>
#include <fstream>
#include <cmath>
#include<castDetector.h>
#include<castConversion.h>
#include<castMagnet.h>
#include<castGas.h>
//#include<castUtils.h>
#include<castExposure.h>
#include<castTracking.h>
using std::cout;
using std::endl;



int main(int argc,const char *argv[])
{

    //Setting Paths {{{
    char inputPath[256];
    sprintf(inputPath,"%s/inputs/",getenv("CAST_PATH"));
    cout << "\nInput Path: " << inputPath  <<endl ;
    // }}}

    //castMagnet instance
    cout << "\nCreating magnet instance..." <<endl ;
    castMagnet *mag = new castMagnet();
    mag->Show();

    //castDetector instance
    char mMEffFile[256];
    sprintf(mMEffFile,"%s/mMEfficiency.txt",inputPath);

    char softwareEfficiencyFile[256];
    sprintf(softwareEfficiencyFile,"%s/mMSoftEfficiency1.txt",inputPath);

    cout << "\nCreating detector instance..." <<endl ;
    castDetector *det = new castDetector();
    det->setDetEfficiency(mMEffFile,softwareEfficiencyFile);
    det->Show();

    castGas *gas = new castGas(3.0160293,mag,1);

    castConversion *conv = new castConversion(mag,gas);

    char fname[256];

    int Npoints = 100;
    double density,ma,ma_common,nphoton, m;
    double densities[] = {0.5*1E-3,0.5018*1E-3}; // gr/cm3

    ma_common = gas->getPhotonMass(densities[0]);

    for (int i = 0; i < 2; i++)
    {
        density = densities[i];

        ma = gas->getPhotonMass(density);
        cout << ma <<endl ;
        cout << "===================================" <<endl ;

        ofstream outFile;
        sprintf(fname,"stepOut%.4f_%.3f",ma,density*1E3);
        outFile.open(fname);

        for (int k = 0; k < Npoints; k++) 
        {
            m = ma_common + 0.01/100 * (k-Npoints/2);
            nphoton = conv->ExpectedNumberOfCounts( 2.0, 7.0, m, 24.7, density, 1800.0 );
            //cout << "m = " << m << " n: " << nphoton << endl;
            outFile << m << "\t" << nphoton << endl;
        }
    }


    return 0;
}

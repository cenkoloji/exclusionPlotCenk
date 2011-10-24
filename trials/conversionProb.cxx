//program to create text files for plotting axion-photon conversion probability
//Used together with plotConversionProb.py

#include <iostream>
#include <fstream>
#include "castGas.h"
#include "castMagnet.h"
#include "castConversion.h"
using namespace std;

double * constructLinearArray(double min,double max,int N);

int main()
{

    // Defining number of elements of arrays {{{
    int Nma = 10000; // Number of axion masses
    int NEp = 7; // Number of photon energies
    int Npres = 11; // Number of pressures }}}

    // Initializing arrays {{{ 
    double * maArray;           // Axion mass array (Created later)
    double * EphotonArray;      // Photon energy array (keV)
    double * pressureArray;     // Pressure array (mbar)
    EphotonArray = constructLinearArray(1.0,7.0,NEp);
    pressureArray = constructLinearArray(0.0,100.0,Npres);
    // }}}

    //creating  castMagnet and castGas instances {{{
    castMagnet * cm = new castMagnet();
    castGas * cg = new castGas(3.0,cm,1); // 3He , Hot window
    cm->Show();
    cg->Show();
    std::cout << endl;
    // }}}

    //Creating castConversion instance
    castConversion * cc = new castConversion(cm,cg);

    for (int i = 0; i <Npres ; i++)  // Loop over pressures {{{
    {
        double pressure = pressureArray[i];
        cout << "Pressure: " << pressure << ", mGamma: " << cg->getPhotonMass(pressure) << endl;

        for (int j = 0; j < NEp; j++) // Loop over photon energies
        {
            double energy = EphotonArray[j];

            // Opening file to be written
            char filename[256];
            ofstream outFile;                                       // create object for output
            sprintf(filename,"out/%g_%g.txt", pressure, energy);    // File name convention: Pressure_Energy.txt
            outFile.open(filename);

            double photonMass = cg->getPhotonMass(pressure);
            if (photonMass == 0.0)
                maArray = constructLinearArray(1e-7,0.1,Nma);         // Axion mass array for vacuum (eV)
            else
                maArray = constructLinearArray(photonMass*0.8,photonMass*1.2,Nma);         // Axion mass array around mgamma (eV)

            for (int k = 0; k < Nma; k++)  // Loop over axion masses
            {
                double ma = maArray[k];
                double prob = cc->ConversionProbability(ma,energy, pressure);
                outFile << ma << "\t" << prob << endl;
            }

            delete maArray;         // freeing memory allocated for maArray
            outFile.close();        // closing the
        }
    }// }}}

    // Delete's {{{
    delete cm;
    delete cg;
    delete cc;
    delete pressureArray;
    delete EphotonArray;
    // }}}

    return 0;
}

//constructLinearArray(double min,double max,int N) Does the job of linspace in python {{{
double * constructLinearArray(double min,double max,int N)
{
// Creates an array with N values between min and max (including both)
// You have to delete pointers in the end!

    double * arr = new double[N];
    for (int i = 0; i < N; i++) 
    {
        arr[i] = min + 1.0*i*(max-min)/(N-1);
    }

    return arr;
} // }}}

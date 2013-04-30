//Trial program to test castLike class 

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include<castLike.h>
#include<castDetector.h>
#include<castConversion.h>
#include<castMagnet.h>
#include<castGas.h>
//#include<castUtils.h>
#include<castExposure.h>
#include<castTracking.h>
using std::cout;
using std::endl;


int main( int argc, char *argv[])
{

    double ma=0.38;


    // Command Line Arguments {{{
    if(argc>=2)
    {
        for(int i = 1; i < argc; i++)
        {
            if( *argv[i] == '-')
            {
                argv[i]++;
                if( *argv[i] == '-') argv[i]++;
                {
                    switch ( *argv[i] )
                    {
                        case 'a' : ma=atof(argv[i+1]); break;
                        default : return 0;
                    }
                }
            }
        }
    }// }}}

    cout << "\nStarting Limit Calculation for 2008 data with ma= " << ma <<endl ;

    char namePrefix[]={'1','2','3'}; // Name prefix for files of different detectors

    //Setting Paths {{{
    char outputPath[256],inputPath[256];
    sprintf(inputPath,"%s/inputs/",getenv("CAST_PATH"));
    cout << "\nInput Path: " << inputPath  <<endl ;
    sprintf(outputPath,"%s/outputs/",getenv("CAST_PATH"));
    cout << "Output Path: " << outputPath  <<endl ;
    // }}}

    //castMagnet instance
    cout << "\nCreating magnet instance..." <<endl ;
    castMagnet *mag = new castMagnet();
    mag->Show();

    //castDetector instances, eff file and initialization of each instance {{{

    int ndetectors = 3;
    char mMEffFile[256];
    char softwareEfficiencyFile[256];
    sprintf(mMEffFile,"%s/mMEfficiency.txt",inputPath); // TODO: Change

    cout << "\nCreating detector instances..." <<endl ;
    castDetector *det[ndetectors];

    for(int i=0;i<ndetectors; i++)
    {
        det[i]= new castDetector();
        sprintf(softwareEfficiencyFile,"%s/mMSoftEfficiency%c.txt",inputPath,namePrefix[i]);
        det[i]->setDetEfficiency(mMEffFile,softwareEfficiencyFile);
        det[i]->Show();
    }
    // }}}

    // Reading Exposure and Tracking Files, creating vector arrays {{{

    char expFileName[256];      // File name of exposure file
    ifstream expFile;           // file object to read
    char trkFileName[256];      // File name of tracking file
    ifstream trkFile;           // file object to read

    //creating array of vector of castExposure objects
    vector<castExposure> vecExposure[ndetectors];
    castExposure exp = castExposure();

    //creating array of vector of castTracking objects
    vector<castTracking> vecTracking[ndetectors];
    castTracking trk = castTracking();

    for(int i=0;i<ndetectors; i++)
    {
        
        sprintf(expFileName,"%s/2008tckInfoSunset%c_OUT.txt",inputPath,namePrefix[i]);
        expFile.open(expFileName);  // associate with a file
        cout << expFileName << endl;

        //Checking if file is opened without error
        if (!expFile.is_open()) { cout << "Could not open the file " << expFileName << " -> Terminating..." << endl; exit(EXIT_FAILURE); }

        for (int j = 0; expFile.good(); j++)
        {

            expFile >> exp.countNo >> exp.ntrack >> exp.nstep >> exp.labviewtime >> exp.timeExp >> exp.angle >> exp.tmag >> exp.pressure >> exp.density;

            //Converting density from kg/m3 to g/cm3
            exp.density = exp.density * 1E-3;

            //if (exp.timeExp!= 0)
                //cout << "dens: " << exp.density << ", expTime: " << exp.timeExp << endl;
            vecExposure[i].push_back(exp);
        }

        expFile.close();         // Closing the file

        sprintf(trkFileName,"%s/finalTrackingcounts_2008.Sunset%c",inputPath,namePrefix[i]);
        trkFile.open(trkFileName);  // associate with a file
        cout << trkFileName << endl;

        //Checking if file is opened without error
        if (!trkFile.is_open()) { cout << "Could not open the file " << trkFileName << " -> Terminating..." << endl; exit(EXIT_FAILURE); }

        for (int j = 0; trkFile.good(); j++)
        {

            trkFile >> trk.timestamp >> trk.energy >> trk.density >> trk.bckLevel >> trk.angle >> trk.tmag >> trk.pressure;

            trk.density = trk.density * 1E-3;

            cout <<" Dens: " <<  trk.density <<", En: " << trk.energy <<  ", bck: " << trk.bckLevel << endl;
            vecTracking[i].push_back(trk);
        }

        trkFile.close();         // Closing the file

    }

    // }}}

    // Testing to see if the read values are ok.
    cout << "pres: " << vecExposure[0][33122].pressure  << " - exp: " << vecExposure[0][33122].timeExp  <<endl ;
    cout << "pres: " << vecTracking[0][212].pressure  << " - en: " << vecTracking[0][212].energy << " - bck: " << vecTracking[0][212].bckLevel <<endl ;

    //castGas instance
    castGas *gas = new castGas(3.0160293,mag,1);

    //castConversion instance
    castConversion *conv = new castConversion(mag,gas);

    // castLike instance
    castLike *like = new castLike(conv,mag,gas,det,ndetectors);
    like->Show();

    char tmp[256];
    sprintf( tmp, "%s/aM%lf.txt",outputPath,ma);

    double gL4 = 0.0;

    gL4=like->GetgL4(ma, vecExposure,vecTracking);
    //double gRange[]={-300000,30000.}; //Ranges to plot gl4
    //like->plot_gL4(ma, vecExposure,vecTracking,10000,gRange);

    like->GetMaxLike(ma, vecExposure, vecTracking, gL4, 0);

    cout << "   ma:" << ma << endl;
    cout << "   nGamma:" << like->nGamma << endl;
    cout << "   gL4:" << gL4 << "*10^(-40)" << endl;
    cout << "=> gL:" << std::sqrt(std::sqrt(gL4)) << "*10^(-10)" << endl;

    char outFileName[256];      // File name of output
    ofstream outFile;           // file object to write
    sprintf(outFileName,"%s/ma/%lf.txt",outputPath,ma);
    outFile.open(outFileName);

    outFile << ma << "\t" << like->nGamma << "\t" << gL4 << "\t" <<  endl;

    outFile.close();

    return 0;
}

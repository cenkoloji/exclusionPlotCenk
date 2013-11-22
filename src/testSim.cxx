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

    cout << "\nStarting Limit Calculation for Simulated data with ma= " << ma <<endl ;

    //Setting Paths {{{
    char inputPath[256];
    sprintf(inputPath,"%s/inputs/",getenv("CAST_PATH"));
    cout << "\nInput Path: " << inputPath  <<endl ;
    // }}}

    //castMagnet instance
    cout << "\nCreating magnet instance..." <<endl ;
    castMagnet *mag = new castMagnet();
    mag->Show();

    //castDetector instances, eff file and initialization of each instance {{{

    int ndetectors = 1;
    char mMEffFile[256];
    char softwareEfficiencyFile[256];
    sprintf(mMEffFile,"%s/mMEfficiency.txt",inputPath); // TODO: Change


    cout << "\nCreating detector instances..." <<endl ;
    castDetector *det[ndetectors];
    char namePrefix[] = {'1'};

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
        
        sprintf(expFileName,"%s/simexp%c.txt",inputPath,namePrefix[i]);
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

        sprintf(trkFileName,"%s/simtrk%c.txt",inputPath,namePrefix[i]);
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
    cout << "pres: " << vecExposure[0][1].pressure  << " - exp: " << vecExposure[0][1].timeExp  <<endl ;
    cout << "pres: " << vecTracking[0][2].pressure  << " - en: " << vecTracking[0][2].energy << " - bck: " << vecTracking[0][2].bckLevel <<endl ;

    //castGas instance
    castGas *gas = new castGas(3.0160293,mag,1);

    //castConfig instance
    castConfig * cfg1 = new castConfig("config3.cfg");

    //castConversion instance
    castConversion *conv = new castConversion(mag,gas,cfg1);

    // castLike instance
    castLike *like = new castLike(conv,mag,gas,det,ndetectors);

    sprintf(like->outputPath,"%s/simoutputs/",getenv("CAST_PATH"));
    cout << "Output Path: " << like->outputPath  <<endl ;

    like->Show();


    double gL4 = 0.0;

    gL4=like->GetgL4(ma, vecExposure, vecTracking);

    like->GetMaxLike(ma, vecExposure, vecTracking, gL4, 1);

    double gRange[]={like->maxg4 - like->sigmaLeft, gL4*1.2}; //Ranges to plot gl4
    //double gRange[]={-100,10 }; //Ranges to plot gl4 // TODO remove
    like->plot_gL4(ma, vecExposure,vecTracking,gRange);

    cout << "   ma:" << ma << endl;
    cout << "   nGamma:" << like->nGamma << endl;
    cout << "   gL^4:" << gL4 << "*10^(-40)" << endl;
    cout << "=> gL:" << std::sqrt(std::sqrt(gL4)) << "*10^(-10)" << endl;

    char outFileName[256];      // File name of output
    ofstream outFile;           // file object to write
    sprintf(outFileName,"%s/ma/%lf.txt",like->outputPath,ma);
    outFile.open(outFileName);

    outFile << ma << "\t" << like->nGamma << "\t" << gL4 << "\t" <<  endl;

    outFile.close();
    /**/

    return 0;
}

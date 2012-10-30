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

    char namePrefix[]={'1','2'}; // Name prefix for files of different detectors

    //Setting Paths {{{
    char outputPath[256],inputPath[256];
    sprintf(outputPath,"./"); // TODO: Change
    sprintf(inputPath,"./");  // TODO: Change
    // }}}

    //castMagnet instance
    castMagnet *mag = new castMagnet();
    mag->Show();

    //castDetector instances, eff file and initialization of each instance {{{

    double Einitial=2.,Efinal=7.;
    int ndetectors = 2;
    double softwareEfficiencies[]={0.5,0.5};
    char mMEffFile[256];
    sprintf(mMEffFile,"%s/mMEfficiency.txt",inputPath); // TODO: Change

    castDetector *det[ndetectors];

    for(int i=0;i<ndetectors; i++)
    {
        det[i]= new castDetector();
        det[i]->setDetEfficiency(mMEffFile,softwareEfficiencies[i]);
        det[i]->Show();
    }
    // }}}

    // Reading Exposure and Tracking Trees, creating vector arrays {{{

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
        sprintf(expFileName,"pressureExposureSunset%c.txt",namePrefix[i]);
        expFile.open(expFileName);  // associate with a file
        cout << expFileName << endl;

        //Checking if file is opened without error
        if (!expFile.is_open()) { cout << "Could not open the file " << expFileName << " -> Terminating..." << endl; exit(EXIT_FAILURE); }

        for (int j = 0; expFile.good(); j++)
        {
            expFile >> exp.pressure >> exp.timeExp;
            //if (exp.timeExp!= 0)
            //    cout << exp.pressure << endl;
            vecExposure[i].push_back(exp);
        }

        expFile.close();         // Closing the file

        sprintf(trkFileName,"finalTrackingcounts_2008.Sunset%c",namePrefix[i]);
        trkFile.open(trkFileName);  // associate with a file
        cout << trkFileName << endl;

        //Checking if file is opened without error
        if (!trkFile.is_open()) { cout << "Could not open the file " << trkFileName << " -> Terminating..." << endl; exit(EXIT_FAILURE); }

        for (int j = 0; trkFile.good(); j++)
        {
            trkFile >> trk.pressure >> trk.energy >> trk.bckLevel ;
            //cout << trk.pressure <<" " << trk.energy <<  " " << trk.bckLevel << endl;
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
    sprintf( tmp, "%s/sunset2008/comb/aM%lf.txt",outputPath,ma);
    FILE *fg = fopen( tmp, "w" );

    double gL4;

    gL4=like->GetgL4(ma, vecExposure,vecTracking);
    //gL4=like->GetgL4_Dummy(ma, vecExposure,vecTracking);

    cout << "   ma:" << ma << endl;
    cout << "   gL4:" << gL4 << "*10^(-40)" << endl;
    cout << "=> gL:" << std::sqrt(std::sqrt(gL4)) << "*10^(-10)" << endl;

    //fprintf( fg, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", ma,sqrt(sqrt(gL4)),gL4,minGL4,sigmaLeft,sigmaRight);
    //fflush( fg );
    //printf( "ma: %lf\tgL: %lf\tgL4: %lf\tMingL4: %lf\tsigmaL: %lf\tsigmaR: %lf\n", ma,sqrt(sqrt(gL4)), gL4,minGL4,sigmaLeft,sigmaRight);

    //fclose(fg);

    return 0;
}


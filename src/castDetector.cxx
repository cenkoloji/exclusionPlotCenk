#include <castDetector.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

// Default constructor, setting efficiency points to 0, initializing Einitial, Efinal, focusArea {{{
castDetector::castDetector(double Ei, double Ef, double focusarea, double oeff): Einitial(Ei), Efinal(Ef), focusArea(focusarea),opticsEfficiency(oeff)
{ 
    cout << Einitial <<endl ;
    for(int i=0;i<EFF_POINTS;i++)
    {
        detEfficiency[i]=0; 
        softwareEfficiency[i]=0;
    }

}// }}}

castDetector::~castDetector( ) { }

void castDetector::setDetEfficiency(char *effFileName, char *softEffFileName) //{{{
{

    int i = 0;
    double energy;

    ifstream effFile;           // create object to read
    effFile.open(softEffFileName);  // associate with a file

    if (!effFile.is_open()) { cout << "Could not open the file " << softEffFileName<< " -> Terminating..." << endl; exit(EXIT_FAILURE); }

    while  (effFile.good()) // while input good and not at EOF
    {
        effFile >> energy >> softwareEfficiency[i];
        //cout << "i: " << i << " En: " << energy << " Eff: " << softwareEfficiency[i] << endl;
        i++;
    }
    //cout << "Software Efficiency points : " << i << endl;

    effFile.close();         // Closing the file

    i = 0;
    effFile.open(effFileName);  // associate with a file

    if (!effFile.is_open()) { cout << "Could not open the file " << effFileName<< " -> Terminating..." << endl; exit(EXIT_FAILURE); }

    while  (effFile.good()) // while input good and not at EOF
    {
        effFile >> energy >> detEfficiency[i];
        detEfficiency[i] =  detEfficiency[i] * softwareEfficiency[i];
        //cout << "i: " << i << " En: " << energy << " Eff: " << detEfficiency[i] << endl;
        i++;
    }
    //cout << "Efficiency points : " << i << endl;

    effFile.close();         // Closing the file

} //}}}

double castDetector::getDetEfficiency(double e,bool interpolate)//{{{
{

    double en,eff;

        if (interpolate == true) // Interpolation: Is better if efficiency is given for fixed energies,(it is not!) {{{
        {
            for( int j = 0; j < EFF_POINTS; j++ )
            {
                    en = 0.1 + stepsize * (double)(j);
                    if( e >= en && e < en + stepsize )
                    {
                        cout << en << " " << e << " " << en + stepsize << " " << endl;
                        cout << detEfficiency[j] << " " << detEfficiency[j + 1] << endl;
                        eff = detEfficiency[j+1] - (en + stepsize -e)*(detEfficiency[j+1]-detEfficiency[j])/stepsize;
                        break;
                    }
            }
            if( eff == 0 ) { printf("EFFICIENCY ZERO!!  %lf\n",e); exit(0); }

        } //  }}}

        else // No interpolation: Is better if efficiency is given as a histogram. USE THIS!  {{{
        {
            for( int j = 0; j < EFF_POINTS; j++ )
            {
                    en = stepsize * (double)(j);
                    if( e >= en && e < en + stepsize)
                    {
                        eff = detEfficiency[j];
                        break;
                    }
            }
            if( eff == 0 ) { printf("EFFICIENCY ZERO!!  %lf\n",e); exit(0); }
        } // }}}

    //printf("En %lf Eff %lf\n",e,eff);

    return eff;
} // }}}

double castDetector::getMeanEfficiency() //{{{
{

    double nEff=0,eff=0,en;

    for( int j = 0; j < EFF_POINTS; j++ )
    {
        en = stepsize * (double)(j);
        if( en>=Einitial && en <Efinal)
        {
            eff+= detEfficiency[j];
            nEff++;
        }
    }

    //printf("Mean Eff %lf\n",eff/nEff);

    return eff/nEff;

} // }}}

void castDetector::setCCDEfficiency(char *effFile) // Not used! {{{
{

int nEff=0,line=0;
double a,b,meanEff;

FILE *fEff = fopen(effFile, "r" );
	while( fscanf( fEff, "%lf\t%lf\n", &a, &b ) != EOF )
	{	
		meanEff+=b;
		if((line+1)%2==0 && line!=0){
		detEfficiency[nEff+5] = meanEff/2.;
		printf(" En %1.1lf Eff %.3lf\n",(double)(nEff+5)*0.2,detEfficiency[nEff+5]);
		nEff++;
		meanEff=0;
		}
	line++;
	}

cout << "Efficiency points : " << nEff << endl;
fclose(fEff);

} // }}}

void castDetector::Show() // {{{
{

    cout << "\nDetector Information" << endl;
    cout << "====================" << endl;
    cout << "Optics Eff: "  << opticsEfficiency << endl; 
    cout << "Focus Area: "  << focusArea        << endl; 
    cout << "E[i]: "        << Einitial  << endl; 
    cout << "E[f]: "        << Efinal    << endl; 
    cout << "Mean Eff(Ei - Ef keV): "    << getMeanEfficiency()  << endl; 
                
}// }}}

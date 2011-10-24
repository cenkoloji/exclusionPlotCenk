
#include <castDetector.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


//ClassImp(castDetector)

// Default constructor {{{
castDetector::castDetector( ) 
{

for(int i=0;i<EFF_POINTS;i++)detEfficiency[i]=0;

}// }}}

castDetector::~castDetector( ) { }


//setDetEfficiency(char *effFile,double softEff) set the efficiency of the detector via text file in format (energy	efficiency) the energy bins start at 0.1 keV and ends at 11.9 keV with a step of 0.2 keV {{{
void castDetector::setDetEfficiency(char *effFile,double softEff){

int nEff = 0;
double a,b;

FILE *fEff = fopen(effFile, "r" );
	while( fscanf( fEff, "%lf\t%lf\n", &a, &b ) != EOF )
	{
		detEfficiency[nEff] = softEff * b;
		printf(" En %1.1lf Eff %.3lf\n",(double)nEff*0.2,detEfficiency[nEff]);
		nEff++;
	}

cout << "Efficiency points : " << nEff << endl;
fclose(fEff);

} //}}}

//getDetEfficiency(double e) return the efficiency of the detector for a given energy (in keV)
double castDetector::getDetEfficiency(double e){

double en,eff;

	for( int j = 0; j < EFF_POINTS; j++ )
	{
		en = 0.2 * (double)(j);
		if( e >= en && e < en + 0.2 ){
			eff = detEfficiency[j];
			break;
			}
	}
	if( eff == 0 ) { printf("EFFICIENCY ZERO!!  %lf\n",e); exit(0); }

//printf("En %lf Eff %lf\n",e,eff);

return eff;
}

//getMeanEfficiency() return the mean efficiency of the detector in the energy range of operation for the detector
double castDetector::getMeanEfficiency(){

double nEff=0,eff=0,en;

	for( int j = 0; j < EFF_POINTS; j++ )
	{
		en = 0.2 * (double)(j);
			if( en>=Einitial && en <Efinal){
			eff+= detEfficiency[j];
			nEff++;
			}
	}
	//if( eff == 0 ) { printf("EFFICIENCY ZERO!!\n"); exit(0); }

//printf("Mean Eff %lf\n",eff/nEff);

return eff/nEff;


}

void castDetector::setCCDEfficiency(char *effFile){

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



}





#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <math.h>
#include <vector>
#include <cstdlib>

#include<castLike.h>
//#include<TMath.h>
#include<castExposure.h>
#include<castTracking.h>
//#include<TGraph.h>

//ClassImp(CASTLike)

//castLike(castConversion *castconv, castMagnet *castmag, castGas *Cgas,castDetector *castdet[],int ndet) Constructor with definitions of conversion, magnet, gas and number of detectors {{{
castLike::castLike(castConversion *cconv, castMagnet *cmag, castGas *cgas, castDetector *cdet[], int ndet)
{

    for(int i=0;i<ndet;i++)
        det[i]=cdet[i];

    ndetectors = ndet;
    conv=cconv;
    mag=cmag;
    gas=cgas;
    gasTypes = 0;
    sprintf(outputPath,"./"); //TODO
    nGamma = 0;

    printf("Number of detectors %d\n",ndetectors);
}// }}}

castLike::~castLike(){}

double castLike::GetNgammaCounts(double ma, const vector<castExposure> vecExp[])// return the total expected number of counts for a axion mass (eV), uses as input the exposure in a tree {{{
{

    double E0,Ef; //,mgammaCount;
    int nbins;

    for(int d= 0;d<ndetectors;d++)
    {
	
	nbins=vecExp[d].size();

        for(int i=0;i<nbins;i++)
        {
            //If there is no exposure for a pressure, continue
            if(vecExp[d][i].timeExp==0)
                continue;

            //If there are different gasses, read the type of exposure tree
            if(gasTypes)
                gas->setType(vecExp[d][i].gasType);

            // Next 2 lines can be used to lower computation time, by not calculating if pressure is far from the sensitivity region
            //mgammaCount = gas->GetPhotonMass(vecExp[d][i]->pressure);
            //if(TMath::Abs(mgammaCount-ma)>0.02)continue;

            // Summing contribution of each enery for this 
            for( E0=det[d]->getEinitial(); E0<det[d]->getEfinal() ; E0+=0.5 )
            {
                Ef = E0 + 0.5;
                nGamma +=   det[d]->getDetEfficiency((E0+Ef)/2.)
                          * det[d]->getOpticsEfficiency()
                          * mag->getAreaCB()
                          * conv->ExpectedNumberOfCounts(E0,Ef,ma,vecExp[d][i].pressure,vecExp[d][i].timeExp);
                //if(i==)cout<<E0<<"  "<<Ef<<"   "<<nGamma<<endl;
            }
        }

    }

    printf("nGamma %lf\n",nGamma);
    return nGamma;

}// }}}

double castLike::GetgL4(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[],bool writeToFile)//This function implements the likelihood for a given axion; trackings counts and exposure for one or more detectors {{{
{

    double g4Step = 10000., g4,E,E0,Ef;
    double chi[MAX_ITERATIONS];//,mgammaCount;   // chi := -1/2 Chi**2
    int nChi,nCounts;

    if(ma>1.5)
        g4Step=1.E9;

    //castTracking *tck = new castTracking();
    printf("Axion mass %.4f\n",ma);

    // Get the total expected counts
    GetNgammaCounts(ma,vecExp);
    cout << "nGamma: " << nGamma <<endl ;


    ofstream outFileValues;
    if (writeToFile)
    {
        char fnameValues[256];
        sprintf(fnameValues,"%s/%.4f_values",outputPath,ma);
        cout << fnameValues<< endl;
        outFileValues.open(fnameValues);
    }


    double term2 = 0;
    double term1;

    // Some Notes on the method:
    // We start at a really high g4 value, iterating g4 value by stepsize, and calculating Chi for each value.
    //Step size decrease if the [ chi[j] - chi[0] < -10 ], where chi[0] is the value at the beginning of the iterations with this step size
    //
    //Note that chi is always extremely high negative value, and when you take exp(chi[0]), you get almost 0.
    //chi[0] is used like a reference value, where g = 0. It has only contributions of background levels.
    //
    //While integrating for limit, you can always use exp(chi[j]-chi[0]) instead of exp(chi[j]). (since you'll divide two integrals, and chi[0] terms will disappear)
    //
    //K = exp(chi[j]-chi[0]) starts close to 1 (since chi[1]-chi[0]~0 and exp(0) = 1) and decrease as chi[j] gets smaller
    //
    //[ chi[j] - chi[0] < -10 ]  means this: [ K < 4.5E-5 ]
    //
    //When this K value becomes extremely small, step size is decreased. The whole process is repeated
    //
    //If it takes more iterations than the  CONVERGENVE limit for k to be less than 4.5E-5,(max 5000 iteration), the process is stopped.
    //
    //Think like this: If in only few iterations you have K<4.5E-5, it means you have really little amount of points. By decreasing the step size, you are finding the optimal resolution,
    //where you have as much points before the K goes as low, so you can calculate integral more accurately
    //
    //Loop is ended if the number of iterations with same step size passed the convergence limit
    do
    {
        g4Step = g4Step/4.;
        for(int j=0;j<MAX_ITERATIONS;j++)
        {
            g4=(double) j * g4Step;

            // First term of Chi: g4 * nGamma
            term1 = -1.0 * g4 * nGamma;
            term2 = 0.0;

            // Second term of Chi: ln( bck + g4 * nExpect )
            for(int d =0;d<ndetectors;d++)
            {
                nCounts=vecTrk[d].size();
                //printf("nCounts %d",nCounts);

                // Summing the contribution of each count
                for(int i =0; i<nCounts;i++)
                {

                    if(gasTypes)
                        gas->setType(vecTrk[d][i].gasType);

                    //mgammaCount = gas->GetPhotonMass(vecTrk[d][i]->pressure);
                    //if(TMath::Abs(mgammaCount-ma)>0.02)continue;

                    // Finding the relevant energy  bin to calculate expected number of photons
                    for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5)
                    {
                        if((vecTrk[d][i].energy)>E && (vecTrk[d][i].energy)<E+0.5)
                        {
                            E0=E;
                            Ef= E+0.5;
                        }
                    }

                    double bcklevel = vecTrk[d][i].bckLevel * det[d]->getFocusArea() * (Ef-E0);
                    double expCounts = det[d]->getDetEfficiency(vecTrk[d][i].energy) * det[d]->getOpticsEfficiency() * mag->getAreaCB() * conv->ExpectedNumberOfCounts( E0, Ef, ma, vecTrk[d][i].pressure, 1.0 );
                    term2+=  std::log ( bcklevel + g4 * expCounts );

                    //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;
                }

            }

            chi[j]  = term1 + term2;
            //cout<<"\tj: " << j<< ",  g: " << g4 << ",  chi[j] - chi[0]: "<< chi[j] - chi[0] << "  term1,2: " << term1<<", "<< term2 <<  "    chi = " << chi[j] <<  "    exp(chi) = " << std::exp(chi[j]) << "    exp(chi-chi0) = " << std::exp(chi[j]-chi[0]) << endl;

            if (writeToFile)
            {
            outFileValues   << j<< "\t" << g4 << "\t"<< chi[j] - chi[0] << "\t"
                            << term1<<"\t"<< term2 <<"\t" << chi[j] <<  "\t"
                            << std::exp(chi[j]) << "\t" << std::exp(chi[j]-chi[0]) << endl;
            }
            if(j%1000==0)
            {
                if (j!= 0)
                    cout<<"j: " << j<< ",  g: " << g4 << ",  chi[j] - chi[0]: "<< chi[j] - chi[0] << "  term1,2: " << term1<<", "<< term2 <<  "    chi = " << chi[j] <<  "    exp(chi) = " << std::exp(chi[j]) << "    exp(chi-chi0) = " << std::exp(chi[j]-chi[0]) << endl;
                    cout << "";
            }

            if( chi[j] - chi[0] < -10 )
            {
                nChi = j;
                cout<<"** j: " << j<< ",  g: " << g4 << ",  chi[j] - chi[0]: "<< chi[j] - chi[0] << "  term1,2: " << term1<<", "<< term2 <<  "    chi = " << chi[j] <<  "    exp(chi) = " << std::exp(chi[j]) << "    exp(chi-chi0) = " << std::exp(chi[j]-chi[0]) << endl;
                break;
            }

        }

    }while( nChi < CONVERGENCE );

    double integral = IntegrateChi( chi, nChi );
    double gL4 = g4Step * GetLimit( chi, integral );

    //cout<<"gL4: "<<gL4<<endl;

    //delete tck;

    if (writeToFile)
        outFileValues.close();

    return gL4;
    //return 0;

}// }}}

double castLike::plot_gL4(double ma, const vector<castExposure> vecExp[], const vector<castTracking> vecTrk[], double g4Step, double *range)//This function implements the likelihood for a given axion; trackings counts and exposure for one or more detectors {{{
{

    double g4,E,E0,Ef;
    double chi[20000];//,mgammaCount;   // chi := -1/2 Chi**2
    int nChi,nCounts;

    ofstream outFileValues;
    char fnameValues[256];
    sprintf(fnameValues,"%s/%.4f_valuesForPlot",outputPath,ma);
    cout << fnameValues<< endl;
    outFileValues.open(fnameValues);

    //castTracking *tck = new castTracking();
    printf("Axion mass %lf\n",ma);

    // Get the total expected counts
    double nGamma = GetNgammaCounts(ma,vecExp);
    cout << "nGamma: " << nGamma <<endl ;

    double term2 = 0;
    double term1;

    g4 = range[0];

    // Iterating over g for some values
    for (int j = 0; j < MAX_ITERATIONS; j++)
    {
        g4 += double(j) * 0.01;

        // First term of Chi: g4 * nGamma
        term1 = -1.0 * g4 * nGamma;
        term2 = 0.0;

        // Second term of Chi: ln( bck + g4 * nExpect )
        for(int d =0;d<ndetectors;d++)
        {
            nCounts=vecTrk[d].size();
            //printf("nCounts %d",nCounts);

            // Summing the contribution of each count
            for(int i =0; i<nCounts;i++)
            {

                if(gasTypes)
                    gas->setType(vecTrk[d][i].gasType);

                //mgammaCount = gas->GetPhotonMass(vecTrk[d][i]->pressure);
                //if(TMath::Abs(mgammaCount-ma)>0.02)continue;

                // Finding the relevant energy  bin to calculate expected number of photons
                for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5)
                {
                    if((vecTrk[d][i].energy)>E && (vecTrk[d][i].energy)<E+0.5)
                    {
                        E0=E;
                        Ef= E+0.5;
                    }
                }

                double bcklevel = vecTrk[d][i].bckLevel * det[d]->getFocusArea() * (Ef-E0);
                double expCounts = det[d]->getDetEfficiency(vecTrk[d][i].energy) * det[d]->getOpticsEfficiency() * mag->getAreaCB() * conv->ExpectedNumberOfCounts( E0, Ef, ma, vecTrk[d][i].pressure, 1.0 );
                term2+=  std::log ( bcklevel + g4 * expCounts );

                //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;
            }

        }

        chi[j]  = term1 + term2;
        cout<<"\tj: " << j<< ",  g: " << g4 << ",  chi[j] - chi[0]: "<< chi[j] - chi[0] << "  term1,2: " << term1<<", "<< term2 <<  "    chi = " << chi[j] <<  "    exp(chi) = " << std::exp(chi[j]) << "    exp(chi-chi0) = " << std::exp(chi[j]-chi[0]) << endl;

        outFileValues << std::setprecision(9);
        outFileValues   << j<< "\t" << g4 << "\t"<< chi[j] - chi[0] << "\t"
          << term1<<"\t"<< term2 <<"\t" << chi[j] <<  "\t" << std::exp(chi[j])
          << "\t" << std::exp(chi[j]-chi[0]) << endl;

        if (g4>range[1])
            break;
    }

    outFileValues.close();

    //double integral = IntegrateChi( chi, nChi );
    //double gL4 = g4Step * GetLimit( chi, integral );

    //cout<<"gL4: "<<gL4<<endl;

    //delete tck;

    //return gL4;
    return 0;

}// }}}

//IntegrateChi( double *chi, int n ) return the integral of exp(-chi2) for the likelihood function {{{
double castLike::IntegrateChi( double *chi, int n )
{
    double offset = chi[0];
    double integral = 0;

    for( int i = 0; i < n; i++ )
        integral = integral + std::exp( chi[i] - offset );

    return integral;

} // }}}

//GetLimit( double *chi, double integral ) return the value of gL4 from the likelihood with a 95% confidence level {{{
double castLike::GetLimit( double *chi, double integral )
{

    double offset = chi[0];

    double newInt = 0;
    int i = 0;

    while( newInt < 0.95 * integral )
    {
        newInt = newInt + std::exp( chi[i] - offset );
        i++;
    }

    return (double) i;

}// }}}

void castLike::Show() //{{{
{

    cout << "\nLikelihood instance information:" <<endl ;
    cout << "================================" <<endl ;
    cout << "Number of detectors:        " << ndetectors  << endl ;
    cout << "Use different gas types?    " << bool(gasTypes)  << endl ;
    cout << "castConversion instance at: " << conv  << endl ;
    cout << "castMagnet instance at:     " << mag  << endl ;
    cout << "castGas instance at:        " << gas  << endl ;

}//}}}

/*
//I don't use it for now. I don't really care for minimum
//GetMinLike(double ma,TTree *expTree[],TTree *tckTree[],TTree &gL4Tree) this function try to obtain the value of gL4 that minimize the likelihood, also try to obtain the curve (under development) {{{
double castLike::GetMinLike(double ma,TTree *expTree[],TTree *tckTree[],TTree &gL4Tree,double g4,double &sigmaLeft,double &sigmaRight)
{

double g,E,E0,Ef,minChi;
double chi0,chi,j,prevChi;//,mgammaCount;
int nCounts;

castTracking *tck = new castTracking();
printf("Axion mass %lf\n",ma);

double nGamma = GetNgammaCounts(ma,expTree);
TBranch *branch;

double mingL4,minLike=1000.;

int iterations=0;

double gL4Limit[2];
gL4Limit[0]=-1E9;
gL4Limit[1]=g4;

double g4Step = g4/1000.;

cout<<"Searching in ranges ["<<gL4Limit[0]<<" - "<<gL4Limit[1]<<"]"<<endl;
cout<<"Step "<<g4Step<<endl;
CASTUtils *ut = new CASTUtils();
gL4Tree.Branch("like",ut);

while(g4Step>g4*1e-8&&iterations<1000000){
j=0;
	while(g>gL4Limit[0]){
	
	g=gL4Limit[1]+j* g4Step;
	chi = -1.0 * g * nGamma;
		
		for(int d =0;d<ndetectors;d++){
		branch=tckTree[d]->GetBranch("tck");
		branch->SetAddress(&tck);
		nCounts=branch->GetEntries();
		//printf("nCounts %d\n",d,nCounts);
			for(int i =0; i<nCounts;i++){
			branch->GetEvent(i);
			if(gasTypes)gas->SetType(tck->gasType);
			//mgammaCount = gas->GetPhotonMass(tck->pressure);
				//if(TMath::Abs(mgammaCount-ma)>0.02)continue;
				for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5){
					if((tck->energy)>E&&(tck->energy)<E+0.5){
					E0=E;
					Ef= E+0.5;
					}
				}
			//printf("%lf  %lf  %lf\n",tck->energy,E0,Ef);
			chi+= TMath::Log(TMath::Abs(tck->bckLevel*det[d]->getFocusArea()*(Ef-E0) + g * det[d]->getDetEfficiency(tck->energy) * det[d]->getOpticsEfficiency()*mag->getACB()* conv->ExpectedNumberOfCounts( E0, Ef, ma, tck->pressure, 1.0 )));
			//printf("Eff %lf\n",det[d]->getDetEfficiency(tck->energy));
			}
		}
		if(j==0)prevChi=chi;
		if(iterations==0)chi0=chi;
		ut->gL4=g;
		ut->Like=chi0-chi;
		gL4Tree.Fill();
		//cout<<g<<"  "<<chi<<"  "<<prevChi<<"  "<<chi-prevChi<<endl;
		if(ut->Like<minLike){mingL4=g;minLike=ut->Like;}
		if((ut->Like-minLike)>0.1)break;
		
		if((ut->Like-minLike)>0)cout<<g<<"  "<<ut->Like-minLike<<endl;
		
		j--;
		iterations++;
	}
gL4Limit[1]=mingL4+5.*g4Step;
gL4Limit[0]=mingL4-5.*g4Step;
cout<<"Searching in ranges ["<<gL4Limit[0]<<" - "<<gL4Limit[1]<<"]"<<endl;
g4Step=g4Step/5.;
cout<<"Step "<<g4Step<<endl;
}
cout<<"Iterations "<<iterations<<endl;
printf("MingL4 %e\n",mingL4);

g=mingL4;
chi = -1.0 * g * nGamma;

	for(int d =0;d<ndetectors;d++){
	branch=tckTree[d]->GetBranch("tck");
	branch->SetAddress(&tck);
	nCounts=branch->GetEntries();
	//printf("nCounts %d\n",d,nCounts);
		for(int i =0; i<nCounts;i++){
		branch->GetEvent(i);
		if(gasTypes)gas->SetType(tck->gasType);
		//mgammaCount = gas->GetPhotonMass(tck->pressure);
			//if(TMath::Abs(mgammaCount-ma)>0.02)continue;
			for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5){
				if((tck->energy)>E&&(tck->energy)<E+0.5){
				E0=E;
				Ef= E+0.5;
				}
			}
		//printf("%lf  %lf  %lf\n",tck->energy,E0,Ef);
		chi+= TMath::Log(TMath::Abs(tck->bckLevel*det[d]->getFocusArea()*(Ef-E0) + g * det[d]->getDetEfficiency(tck->energy) * det[d]->getOpticsEfficiency()*mag->getACB()* conv->ExpectedNumberOfCounts( E0, Ef, ma, tck->pressure, 1.0 )));
		//printf("Eff %lf\n",det[d]->getDetEfficiency(tck->energy));
		}
	}
g4Step=g4Step*20000.;
minChi=chi;
j=0;
while((minChi-chi)<0.5){
	g=mingL4+j* g4Step;
	chi = -1.0 * g * nGamma;
	
		for(int d =0;d<ndetectors;d++){
		branch=tckTree[d]->GetBranch("tck");
		branch->SetAddress(&tck);
		nCounts=branch->GetEntries();
		//printf("nCounts %d\n",d,nCounts);
			for(int i =0; i<nCounts;i++){
			branch->GetEvent(i);
			if(gasTypes)gas->SetType(tck->gasType);
			//mgammaCount = gas->GetPhotonMass(tck->pressure);
				//if(TMath::Abs(mgammaCount-ma)>0.02)continue;
				for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5){
					if((tck->energy)>E&&(tck->energy)<E+0.5){
					E0=E;
					Ef= E+0.5;
					}
				}
			//printf("%lf  %lf  %lf\n",tck->energy,E0,Ef);
			chi+= TMath::Log(TMath::Abs(tck->bckLevel*det[d]->getFocusArea()*(Ef-E0) + g * det[d]->getDetEfficiency(tck->energy) * det[d]->getOpticsEfficiency()*mag->getACB()* conv->ExpectedNumberOfCounts( E0, Ef, ma,tck->pressure, 1.0 )));
			//printf("Eff %lf\n",det[d]->getDetEfficiency(tck->energy));
			}
		}
		ut->gL4=g;
		ut->Like=chi0-chi;
		gL4Tree.Fill();
		if((int)j%1000==0)cout<<g<<"  "<<(minChi-chi)<<endl;
		j--;
}

sigmaLeft=mingL4-g;
chi = minChi;
j=0;

while((minChi-chi)<0.5){
	g=mingL4+j* g4Step;
	chi = -1.0 * g * nGamma;
	
		for(int d =0;d<ndetectors;d++){
		branch=tckTree[d]->GetBranch("tck");
		branch->SetAddress(&tck);
		nCounts=branch->GetEntries();
		//printf("nCounts %d\n",d,nCounts);
			for(int i =0; i<nCounts;i++){
			branch->GetEvent(i);
			if(gasTypes)gas->SetType(tck->gasType);
			//mgammaCount = gas->GetPhotonMass(tck->pressure);
				//if(TMath::Abs(mgammaCount-ma)>0.02)continue;
				for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5){
					if((tck->energy)>E&&(tck->energy)<E+0.5){
					E0=E;
					Ef= E+0.5;
					}
				}
			//printf("%lf  %lf  %lf\n",tck->energy,E0,Ef);
			chi+= TMath::Log(TMath::Abs(tck->bckLevel*det[d]->getFocusArea()*(Ef-E0) + g * det[d]->getDetEfficiency(tck->energy) * det[d]->getOpticsEfficiency()*mag->getACB()* conv->ExpectedNumberOfCounts( E0, Ef, ma, tck->pressure, 1.0 )));
			//printf("Eff %lf\n",det[d]->getDetEfficiency(tck->energy));
			}
		}
		ut->gL4=g;
		ut->Like=chi0-chi;
		gL4Tree.Fill();
		if((int)j%1000==0)cout<<g<<"  "<<(minChi-chi)<<endl;
		j++;
}

sigmaRight=g-mingL4;


delete tck;
delete ut;

//if(mingL4>=0)return TMath::Sqrt(TMath::Sqrt(mingL4));
//else return -1*TMath::Sqrt(TMath::Sqrt(-1*mingL4));

return mingL4;

}// }}}
*/


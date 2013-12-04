
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
#include<castExposure.h>
#include<castTracking.h>


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
    //sprintf(outputPath,"%s/outputs/",getenv("CAST_PATH"));
    nGamma = 0;

    printf("Number of detectors %d\n",ndetectors);
}// }}}

castLike::~castLike(){}

void castLike::FillTrackingVectors(double ma,vector<castTracking> vecTrk[]) // {{{
{

    cout << "Filling Tracking Vector!" <<endl ;
    // Calculating stuff:
    double bckCnts, expCnts,E0,Ef,E;
    for(int d = 0; d<ndetectors ; d++)
    {
        int nCounts=vecTrk[d].size();
        //printf("nCounts %d",nCounts);

        // Summing the contribution of each count
        for(int i =0; i<nCounts;i++)
        {

            if(gasTypes)
                gas->setType(vecTrk[d][i].gasType);

            // Finding the relevant energy  bin to calculate expected number of photons
            for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5)
            {
                if((vecTrk[d][i].energy)>E && (vecTrk[d][i].energy)<E+0.5)
                {
                    E0=E;
                    Ef= E+0.5;
                }
            }

            bckCnts = vecTrk[d][i].bckLevel * det[d]->getFocusArea() * (Ef-E0);
            expCnts = det[d]->getDetEfficiency(vecTrk[d][i].energy) * det[d]->getOpticsEfficiency() * mag->getAreaCB() * conv->ExpectedNumberOfCounts( E0, Ef, ma, vecTrk[d][i].pressure,vecTrk[d][i].tmag,vecTrk[d][i].angle, 1.0 );
            vecTrk[d][i].bckCnts = bckCnts;
            vecTrk[d][i].expCnts = expCnts;
            //cout << bckCnts << " " << expCnts << endl;
        }
    }

    cout << "Filled Tracking Vectors!" <<endl ;
}// }}}

double castLike::GetNgammaCounts(double ma, const vector<castExposure> vecExp[])// {{{
{
    if (nGamma != 0)
        return nGamma;

    double E0,Ef;
    int nbins;

    for(int d= 0;d<ndetectors;d++)
    {

	nbins=vecExp[d].size();
        cout << nbins << endl;
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
            //if(std::abs(mgammaCount-ma)>0.02)continue;

            // Summing contribution of each enery for this 
            for( E0=det[d]->getEinitial(); E0<det[d]->getEfinal() ; E0+=0.5 )
            {
                Ef = E0 + 0.5;
                nGamma +=   det[d]->getDetEfficiency((E0+Ef)/2.)
                          * det[d]->getOpticsEfficiency()
                          * mag->getAreaCB()
                          * conv->ExpectedNumberOfCounts(E0,Ef,ma,vecExp[d][i].pressure,vecExp[d][i].tmag,vecExp[d][i].angle,vecExp[d][i].timeExp);
                //if(i==)cout<<E0<<"  "<<Ef<<"   "<<nGamma<<endl;
            }
        //cout << i << " " << nGamma << endl;
        }

    }

    printf("nGamma %lf\n",nGamma);
    return nGamma;

}// }}}

double castLike::GetgL4(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[],bool writeToFile)// {{{
{

    double g4Step = 10000., g4,E,E0,Ef;
    double chi[MAX_ITERATIONS];// chi := -1/2 Chi**2
    int nChi,nCounts;

    if(ma>1.5)
        g4Step=1.E9;

    // Get the total expected counts
    cout << "getting nGamma" << endl;
    nGamma = GetNgammaCounts(ma,vecExp);
    cout << "nGamma: " << nGamma <<endl ;


    ofstream outFileValues;
    if (writeToFile)
    {
        char fnameValues[256];
        sprintf(fnameValues,"%s/values/%lf",outputPath,ma);
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

                    double bcklevel = vecTrk[d][i].bckCnts;
                    double expCounts = vecTrk[d][i].expCnts;
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

    if (writeToFile)
        outFileValues.close();

    return gL4;

}// }}}

double castLike::plot_gL4(double ma, const vector<castExposure> vecExp[], const vector<castTracking> vecTrk[], double *range)//This function implements the likelihood for a given axion; trackings counts and exposure for one or more detectors {{{
{

    double g4,E,E0,Ef;
    double chi[MAX_ITERATIONS]; // chi := -1/2 Chi**2
    int nChi,nCounts;

    ofstream outFileValues;
    char fnameValues[256];
    sprintf(fnameValues,"%s/plots/%lf",outputPath,ma);
    outFileValues.open(fnameValues);

    // Get the total expected counts
    nGamma = GetNgammaCounts(ma,vecExp);

    cout << "Plotting:" << endl;
    cout << "=========" << endl;
    cout << fnameValues<< endl;
    printf("Axion mass %lf\n",ma);
    cout << "nGamma: " << nGamma <<endl ;
    cout << "Ranges: " << range[0] << " - " << range[1] << endl;

    double step = (range[1] - range[0])/1000.;
    double term2 = 0;
    double term1;

    cout << "step: " << step << endl;

    // Iterating over g for some values
    for (int j = 0; j < MAX_ITERATIONS; j++)
    {
        g4 = range[0] + double(j)* step;

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

                double bcklevel = vecTrk[d][i].bckCnts;
                double expCounts = vecTrk[d][i].expCnts;
                term2+=  std::log ( std::abs( bcklevel + g4 * expCounts ) );

                //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;
            }

        }

        chi[j]  = term1 + term2;
        cout<<"\tj: " << j<< ",  g: " << g4 << ",  chi[j] - chi[0]: "<< chi[j] - chi[0] << "  term1,2: " << term1<<", "<< term2 <<  "    chi = " << chi[j] <<  "    exp(chi) = " << std::exp(chi[j]) << "    exp(chi-chi0) = " << std::exp(chi[j]-chi[0]) << ", step:"  << step << endl;



        //Header:   j   g4      chi[j] - chi[0]      term1      term2       chi[j]      std::exp(chi[j])    std::exp(chi[j]-chi[0])
        outFileValues << std::setprecision(9);
        outFileValues   << j<< "\t" << g4 << "\t"<< chi[j] - chi[0] << "\t"
          << term1<<"\t"<< term2 <<"\t" << chi[j] <<  "\t" << std::exp(chi[j])
          << "\t" << std::exp(chi[j]-chi[0]) << endl;

        if (g4>range[1])
            break;
    }

    outFileValues.close();

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
    cout << "OutputPath:                 " << outputPath  << endl ;
    cout << "castConversion instance at: " << conv  << endl ;
    cout << "castMagnet instance at:     " << mag  << endl ;
    cout << "castGas instance at:        " << gas  << endl ;

}//}}}

double castLike::GetMaxLike(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[], double g4max, bool writeToFile)//{{{
{

    double g4Step = 10000., g4, E,E0,Ef;
    double chi,prevChi,chi0; // chi := -1/2 Chi**2
    int nChi,nCounts;
    int iterations;

    cout << "Trying to find maximum" << endl;
    cout << "======================" << endl;

    // Get the total expected counts
    nGamma = GetNgammaCounts(ma,vecExp);
    cout << "nGamma: " << nGamma <<endl ;

    double range[2];
    double maxGl4;
    double like;
    double maxLike = -1000;
    g4Step = g4max*1e-3;

    range[0] = -1E10;
    range[1] = g4max;

    ofstream outFileValues;
    if (writeToFile)
    {
        char fnameValues[256];
        sprintf(fnameValues,"%s/sigmas/%lf",outputPath,ma);
        cout << fnameValues << endl;
        outFileValues.open(fnameValues);
    }

    double term2 = 0;
    double term1;
    int j;

    iterations = 0;

    //Getting maximum of likelihood function (going back from g4 limit) {{{
    while (g4Step > g4max*1e-7 && iterations < MAX_ITERATIONS)
    {
        j = 0;
	g4 = range[1] + j * g4Step;

        while (g4 > range[0])
        {
	    g4 = range[1] + j * g4Step;

            // First term of Chi: g4 * nGamma
            term1 = -1.0 * g4 * nGamma;
            term2 = 0.0;

            // Second term of Chi: ln( bck + g4 * nExpect ) {{{
            for(int d =0;d<ndetectors;d++)
            {
                nCounts=vecTrk[d].size();
                //printf("nCounts %d",nCounts);

                // Summing the contribution of each count
                for(int i =0; i<nCounts;i++)
                {
                    if(gasTypes)
                        gas->setType(vecTrk[d][i].gasType);

                    // Finding the relevant energy  bin to calculate expected number of photons
                    for(E=det[d]->getEinitial();E<det[d]->getEfinal();E+=0.5)
                    {
                        if((vecTrk[d][i].energy)>E && (vecTrk[d][i].energy)<E+0.5)
                        {
                            E0=E;
                            Ef= E+0.5;
                        }
                    }


                double bcklevel = vecTrk[d][i].bckCnts;
                double expCounts = vecTrk[d][i].expCnts;
                    term2+=  std::log ( std::abs( bcklevel + g4 * expCounts ) );

                    //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;

                }

            }//}}}

            chi = term1 + term2;

            if(j==0) prevChi = chi;
            if(iterations==0) chi0 = chi;
            like = chi - chi0;

            //cout<<g<<"  "<<chi<<"  "<<prevChi<<"  "<<chi-prevChi<<endl;
            if(like>maxLike)
            {
                maxGl4=g4;
                maxLike=like;
                //cout << "iter: " << iterations <<", new maxGl4: " << maxGl4 << " , maxlike " << maxLike << endl;;
            }

            if((maxLike - like)>0.1)
                break;

            //if((maxLike-like)>0) cout << "iter: " << iterations <<", g4:" <<g4 << " , max-like " << maxLike - like << endl;

            j--;
            iterations++;

        }

        range[1]=maxGl4+5.*g4Step;
        range[0]=maxGl4-5.*g4Step;
        cout<< "Iter: " << iterations << "Searching in ranges ["<<range[0]<<" - "<<range[1]<<"]"<<endl;
        g4Step=g4Step/5.;
        cout<<"Step "<<g4Step<<endl;
    }
    // }}}

    maxg4 = maxGl4;

    cout<<"Iterations: "<<iterations<<endl;
    cout<<"max g4: "<<maxGl4<<endl;

    g4 = maxGl4;

    term1 = -1.0 * g4 * nGamma;
    term2 = 0.0;

    // Second term of Chi: ln( bck + g4 * nExpect ) {{{
    for(int d =0;d<ndetectors;d++)
    {
        nCounts=vecTrk[d].size();
        //printf("nCounts %d",nCounts);

        // Summing the contribution of each count
        for(int i =0; i<nCounts;i++)
        {

            double bcklevel = vecTrk[d][i].bckCnts;
            double expCounts = vecTrk[d][i].expCnts;
            term2+=  std::log ( std::abs( bcklevel + g4 * expCounts ) );

            //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;

        }

    }//}}}

    chi = term1 + term2;

    // Sigma calculation by second derivative{{{
        double step_sigma = g4Step*15;
 
        double der1r = (CalculateLikelihood(ma, maxg4 + step_sigma, nGamma, vecExp,vecTrk) - CalculateLikelihood(ma, maxg4, nGamma, vecExp,vecTrk))/step_sigma;
        double der1l = (CalculateLikelihood(ma, maxg4, nGamma, vecExp,vecTrk) - CalculateLikelihood(ma, maxg4-step_sigma, nGamma, vecExp,vecTrk))/step_sigma;

        double der2 = (der1r - der1l)/step_sigma;
        double sigder = 1./std::sqrt(-der2);

        cout << "der1r: " <<der1r << ", der1l: " << der1l << ", der2: " << der2 << ", sigder: " <<sigder << endl;

    //}}}

    // Getting right sigma {{{
    cout << "\nGetting the right sigma" << endl;

    g4Step=g4Step*1E7;
    int dummyg4Step = g4Step;
    int direction = +1;
    double maxChi=chi ;
    g4 = maxGl4;
    cout << "Max Chi: " << maxChi << endl ;
    j=0;
    int kk = 0;


    while (std::abs(maxChi - chi - 0.5) > (1E-3))
    {
        kk++;
        g4 = g4 + g4Step*direction;


        term1 = -1.0 * g4 * nGamma;
        term2 = 0.0;

        // Second term of C hi: ln( bck + g4 * nExpect ) {{{
        for(int d =0;d<ndetectors;d++)
        {
            nCounts=vecTrk[d].size();
            //printf("nCounts %d",nCounts);

            // Summing the contribution of each count
            for(int i =0; i<nCounts;i++)
            {

            double bcklevel = vecTrk[d][i].bckCnts;
            double expCounts = vecTrk[d][i].expCnts;
                term2+=  std::log ( std::abs( bcklevel + g4 * expCounts ) );

                //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;

            }

        }//}}}

        chi = term1 + term2;

        cout << "step: " << g4Step <<", dir: " << direction << " " << j << ": g4: " <<  g4 << ", maxchi - chi: " << maxChi - chi << endl;
        if (direction * (maxChi - chi - 0.5) > 0)
        {
            g4Step = g4Step/2.;
            direction = direction * -1;
            cout << "Direction Change!" << endl;
            j = 1;
            continue;
        }

        j++;

    }

    cout << "kk: " << kk << endl;
    cout << "step: " << g4Step <<", dir: " << direction << " " << j << ": g4: " <<  g4 << ", maxchi - chi: " << maxChi - chi << endl;
    cout << "right: " << g4 << ", chi: " <<  chi << endl;

    sigmaRight = g4 - maxGl4;
    //}}}

    // Getting left sigma {{{
    cout << "\nGetting the left sigma" << endl;
    //g4Step=dummyg4Step;
    direction = -1;
    chi=maxChi;
    g4 = maxGl4;
    cout << "Max Chi: " << maxChi << endl ;
    j=0;
    kk = 0;

    while (std::abs(maxChi - chi - 0.5) > (1E-3))
    {
        kk++;
        g4 = g4 + g4Step*direction;


        term1 = -1.0 * g4 * nGamma;
        term2 = 0.0;

        // Second term of Chi: ln( bck + g4 * nExpect ) {{{
        for(int d =0;d<ndetectors;d++)
        {
            nCounts=vecTrk[d].size();
            //printf("nCounts %d",nCounts);

            // Summing the contribution of each count
            for(int i =0; i<nCounts;i++)
            {
                /*
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
                }*/


            double bcklevel = vecTrk[d][i].bckCnts;//vecTrk[d][i].bckLevel * det[d]->getFocusArea() * (Ef-E0);
            double expCounts = vecTrk[d][i].expCnts;//det[d]->getDetEfficiency(vecTrk[d][i].energy) * det[d]->getOpticsEfficiency() * mag->getAreaCB() * conv->ExpectedNumberOfCounts( E0, Ef, ma, vecTrk[d][i].pressure,vecTrk[d][i].tmag,vecTrk[d][i].angle, 1.0 );
                term2+=  std::log ( std::abs( bcklevel + g4 * expCounts ) );

                //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;

            }

        }//}}}

        chi = term1 + term2;

        cout << "step: " << g4Step <<", dir: " << direction << " " << j << ": g4: " <<  g4 << ", maxchi - chi: " << maxChi - chi << endl;
        if (direction * (-1) * (maxChi - chi - 0.5) > 0)
        {
            g4Step = g4Step/2.;
            direction = direction * -1;
            cout << "Direction Change!" << endl;
            j = 1;
            continue;
        }

        if (g4>maxGl4)
        {
            cout << "Only Direction Change!" << endl;
            direction = direction * -1;
            continue;
        }

        j++;


    }

    cout << "kk: " << kk << endl;
    cout << "step: " << g4Step <<", dir: " << direction << " " << j << ": g4: " <<  g4 << ", maxchi - chi: " << maxChi - chi << endl;
    cout << "left: " << g4 << ", chi: " <<  chi << endl;

    sigmaLeft = maxGl4 - g4;
    //}}}

    // May need to write also sigma of g
    outFileValues << std::setprecision(9);
    outFileValues   << maxGl4  << "\t"<< sigmaLeft << "\t"<< sigmaRight << endl;

    if (writeToFile)
        outFileValues.close();

    cout << "max: " << maxGl4  << "\t, left:"<< sigmaLeft << "\t, right:"<< sigmaRight << "\t, der:" << sigder<< endl;

}// }}}

double castLike::CalculateLikelihood(double ma, double g4, double nGamma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[] ) //{{{
{
        double term1, term2, chi, E0,E,Ef;
        int nCounts;
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

            double bcklevel = vecTrk[d][i].bckCnts;
            double expCounts = vecTrk[d][i].expCnts;
                term2+=  std::log ( std::abs( bcklevel + g4 * expCounts ) );

                //cout << "Event: " << i << "\ten: " << vecTrk[d][i].energy  << "\tbck Level :" << bcklevel <<  "\texpCounts: "  << expCounts << "\tg*expCounts: " << g4*expCounts <<  endl ;
            }

        }

        chi  = term1 + term2;
        return chi;
}//}}}


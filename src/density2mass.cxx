//Simple program to convert density to photon mass

#include <stdlib.h>     /* atof */
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include<castGas.h>
using std::cout;
using std::endl;


int main( int argc, char *argv[])
{

    double dens;

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
                        case 'd' : dens=atof(argv[i+1]); break;
                        default : return 0;
                    }
                }
            }
        }
    }
    // }}}

    //cout << "\nCreating castMagnet instance..." <<endl ;
    castMagnet *mag = new castMagnet();
    //mag->Show();


    //cout << "\nCreating castGas instance..." <<endl ;
    castGas *gas = new castGas(3.0160293,mag,1);

    cout << gas->getPhotonMass(dens) << endl;

    return 0;
}

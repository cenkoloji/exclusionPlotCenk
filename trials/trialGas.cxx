//Trial program to test castGas class

#include "castGas.h"

int main()
{
    double ar = 14.3;
    double T = 2.4;
    double len = 10.0;
    int n = 2;
    double B = 8.9;

    castMagnet * cm = new castMagnet(ar,T,len,n,B);
    castMagnet * cm2;

    castGas * cg = new castGas(3.0,cm,1);
    castGas * cg2 = new castGas(4.0,cm2,2);

    cg->Show();
    cg2->Show();

    return 0;
}


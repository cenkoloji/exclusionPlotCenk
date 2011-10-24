//Trial program to test castMagnet class

#include "castMagnet.h"

int main()
{
    
    double ar = 14.3;
    double T = 2.4;
    double len = 10.0;
    int n = 2;
    double B = 8.9;

    castMagnet * cm = new castMagnet(ar,T,len,n,B);
    cm->setBMag(10.1);

    castMagnet cm2;

    cm->Show();

    cm2.Show();

    return 0;
}


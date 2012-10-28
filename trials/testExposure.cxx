//Trial program to test castGas class

#include <iostream>
#include <fstream>
#include "castExposure.h"
using std::cout;
using std::endl;


void printExposures(castExposure *ar[],int n);

int main()
{

    char expFileName[256];
    sprintf(expFileName,"pressureExposureSunset1.txt");

    //Getting number of lines in the file
    char command[256];
    sprintf(command,"wc -l %s",expFileName);
    int nExpLines;
    FILE *p = popen(command, "r");
    fscanf (p,"%d",&nExpLines);
    cout << nExpLines <<endl ;

    //creating castExposure objects
    castExposure  * expArray[nExpLines];

    ifstream expFile;           // create object to read
    expFile.open(expFileName);  // associate with a file

    if (!expFile.is_open())
    {
        cout << "Could not open the file " << expFileName << " -> Terminating..." << endl; exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nExpLines && expFile.good(); i++)
    {
        expArray[i] = new castExposure();
        expFile >> expArray[i]->pressure >> expArray[i]->timeExp;
    }

    // Example of reading the array
    printExposures(expArray,nExpLines);

    expFile.close();         // Closing the file

    return 0;

}

void    printExposures(castExposure *ar[],int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << ar[i]->pressure << "  " << ar[i]->timeExp << endl;
    }
}

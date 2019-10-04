
#include "Tx.h"
#include "Rx.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>

using namespace std;

#define N 64 //64 bytes

int main(void)
{
    /* Generate initial data */
    int init_data[N];
    int i;

    srand((unsigned)time(NULL)); //Generate random seeds
    for(i=0; i<N; i++)
    {
        init_data[i]=(rand()%256)-128; //range:-128~127
        cout <<"element No."<<i<<" "<< init_data[i]<<endl;
    }
    
    Transmitter Tr;
    Receiver Re;
    //fftw_complex *Tr_out;
	float *Tr_out;
    
    Tr_out  = Tr.tran(N,init_data);
    Re.rece(N,Tr_out);
    
    //fftw_cleanup();
    return 0;
}

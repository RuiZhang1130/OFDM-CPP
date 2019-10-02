#ifndef TRANSMITTER_H
#define TRANSMITTER_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>

using namespace std;

class Transmitter
{
   public:
   fftw_complex * tran(int n, int data[])
         {
	    fftw_plan p;
	    fftw_complex *in=new fftw_complex[n];
	    fftw_complex *out=new fftw_complex[n];

            int i;
            for (i=0; i<n; i++)  //read array
            {
                in[i][0] = data[i]; //real
                in[i][1] = 0;  //imaginary
            }
            p = fftw_plan_dft_1d(n, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);  //IFFT
            fftw_execute(p);
            
	    for (i=0; i<n ; i++)
            {
                cout<<"after IFFT : "<<"element No."<<i<<" "<<out[i][0]<<" "<<out[i][1]<<endl;
	    }
            fftw_destroy_plan(p);
	    return out;
	  }
};


#endif


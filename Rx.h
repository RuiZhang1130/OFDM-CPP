#ifndef RECEIVER_H
#define RECEIVER_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>

using namespace std;

class Receiver
{
   public:
   
   void rece(int n, fftw_complex tr_out[])
        {
            fftw_plan q;
	    fftw_complex *in=new fftw_complex[n];
	    fftw_complex *out=new fftw_complex[n];
            int i;
	    // in = tr_out; 
            
            q = fftw_plan_dft_1d(n, tr_out, out, FFTW_FORWARD, FFTW_ESTIMATE);  //FFT
            fftw_execute(q);
    
            for (i=0; i<n; i++)  // normalize 
            {
                out[i][0] *= 1./n; 
                out[i][1] *= 1./n; 
            }
    
            for (i=0; i<n; i++)
            {
                 cout<<"after FFT (real part) : "<<"element No."<<i<<" "<<out[i][0]<<endl;
            }
            fftw_destroy_plan(q);   
        }
};


#endif


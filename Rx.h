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
   
   //void rece(int n, fftw_complex tr_out[])
	void rece(int n, float modu_out[])
        {
            fftw_plan q;
	    fftw_complex *in=new fftw_complex[n];
	    fftw_complex *out=new fftw_complex[n];
            int i;
	for (i=0; i<n; i++)
	{
	in[i][0] = modu_out[2*i];
	in[i][1] = modu_out[2*i+1];}
	    // in = tr_out; 
		
            
            //q = fftw_plan_dft_1d(n, tr_out, out, FFTW_FORWARD, FFTW_ESTIMATE);  //FFT
	q = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
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


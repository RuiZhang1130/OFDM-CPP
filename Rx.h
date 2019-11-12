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
	char * rece(int n, /*float//fftw_complex//char*/double Tx_out[])
        {
            fftw_plan q;
	    fftw_complex *in=new fftw_complex[n];
	    fftw_complex *out=new fftw_complex[n];
	//float *demodu = new float[2*n];
char *demodu = new char[2*n];
            int i;
	/*for (i=0; i<n; i++)
	{
	in[i][0] = modu_out[2*i];
	in[i][1] = modu_out[2*i+1];}*/
	   /* in = Tx_out; */
		
            
            //q = fftw_plan_dft_1d(n, tr_out, out, FFTW_FORWARD, FFTW_ESTIMATE);  //FFT
	/*q = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
            fftw_execute(q);*/
    
            /*for (i=0; i<n; i++)  // normalize 
            {
                out[i][0] *= 1./n; 
                out[i][1] *= 1./n; 
            }*/

		/*/////////for (i=0; i<n; i++)
		{
			if(out[i][0]>0)
				demodu[2*i] = 1;
			else
				demodu[2*i] = 0;
		}
		for (i=0; i<n; i++)
		{
			if(out[i][1]>0)
				demodu[2*i+1] = 1;
			else
				demodu[2*i+1] = 0;
		}///////////////////*/
    
           // for (i=0; i<2*n; i++)
           // {
                 /*cout<<"after FFT (real part) : "<<"element No."<<i<<" "<<out[i][0]<<endl;*/
		//cout<<"after FFT : "<<"element No."<<i<<" "<<demodu[i]<<endl;
           // }

//decode_cp
int de_cp_l = 2*n/4;
double *de_cp = new double[2*n];
for(i=0;i<2*n;i++)
{
	de_cp[i] = Tx_out[i+de_cp_l];
}

//demodulation
int j;
for(i=0; i<n; i++)
{
j=i%2;
	if(j=0)
	{
	in[i][0] = de_cp[2*i];
	in[i][1] = de_cp[2*i+1];
	}
	if(j=1)
	{
	in[i][0] = -1*de_cp[2*i];
	in[i][1] = -1*de_cp[2*i+1];
	}
}
q = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);//FFT
fftw_execute(q);

		for (i=0; i<n; i++)
		{
			if(out[i][0]>0)
				demodu[2*i] = 1;
			else
				demodu[2*i] = 0;
		}
		for (i=0; i<n; i++)
		{
			if(out[i][1]>0)
				demodu[2*i+1] = 1;
			else
				demodu[2*i+1] = 0;
		}
//for (i=0; i<4; i++)
//{
                 /*cout<<"after FFT (real part) : "<<"element No."<<i<<" "<<out[i][0]<<endl;*/
//cout<<"after FFT : "<<"element No."<<i<<" "<<in[i][0]<<" "<<in[i][1]<<" "<<demodu[2*i]<<" "<<demodu[2*i+1]<<endl;
//}


            fftw_destroy_plan(q);   
	return demodu;
        }
};


#endif



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
   
	char * rece(int N_pt, int wav_l, short wav_out[], short pt_start0, short pt_start1, short pt_end0, short pt_end1)
        {
		int i;
		int j;
		int data_cp= N_pt+(N_pt/4);
		int n = N_pt/2;
		int data_l = N_pt-4;

		fftw_plan q;
		fftw_complex *in=new fftw_complex[n];
		fftw_complex *out=new fftw_complex[n];

		char *demodu = new char[N_pt];
		char *data = new char[data_l];
	
		//Positioning pilot
		int pt;
		for (i=0; i<wav_l; i++)
		{
		if((wav_out[i]==pt_start0)&&(wav_out[i+1]==pt_start1)&&(wav_out[i+N_pt-2]==pt_end0)&&(wav_out[i+N_pt-1]==pt_end1))
		{
			pt = i;
		}
		}

		//decode_cp
		short *de_cp = new short[N_pt];
		for (i=0; i<N_pt; i++)
		{
			de_cp[i] = wav_out[pt+i];
		}

		//demodulation
		for (i=0; i<n; i++)
		{
			j = i%2;
			if(j==0)
			{
				in[i][0] = de_cp[2*i];
				in[i][1] = de_cp[2*i+1];
			}
			if(j==1)
			{
				in[i][0] = -1*de_cp[2*i];
				in[i][1] = -1*de_cp[2*i+1];
			}
		}

		q = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE); //FFT
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

		//rm_pt
		for(i=0;i<data_l;i++)
		{
			data[i]=demodu[i+2];
		}


            	fftw_destroy_plan(q);   
		return data;
	}

};


#endif


#ifndef TRANSMITTER_H
#define TRANSMITTER_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>
#include <math.h>

using namespace std;


class Transmitter
{
   public:
   //fftw_complex * tran(int n, int data[])
/*float * tran(int n, int data[])*/
	/*char*/double * tran(int n, char data[])
         {
	    fftw_plan p;
	    fftw_complex *in=new fftw_complex[n];
	    fftw_complex *out=new fftw_complex[n];
	    //float *modu = new float[2*n];
		//char *modu = new char[2*n];
double *modu = new double[2*n];

            int i;
            /*for (i=0; i<n; i++)  //read array
            {
                in[i][0] = data[i]; //real
                in[i][1] = 0;  //imaginary
            }
            p = fftw_plan_dft_1d(n, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);  //IFFT
            fftw_execute(p);*/
            
	    //modulation
		/*for (i=0; i<n; i++)
		{
			modu[i] = 2*data[i]-1;
		}*/
	    /*for (i=0; i<n; i++)
	    {
		modu[2*i] = out[i][0];
		modu[2*i+1] = out[i][1];
            }*/
		/*for (i=0; i<(n/2); i++)
		{
			in[i][0] = modu[2*i];
			in[i][1] = modu[2*i+1];
	    	}
		p = fftw_plan_dft_1d(n/2, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);  //IFFT
		fftw_execute(p);*/
	    
	    /*for (i=0; i<(n/2); i++)
            {
                cout<<"after IFFT : "<<"element No."<<i<<" "<<out[i][0]<<" "<<out[i][1]<<" "<<modu[2*i]<<" "<<modu[2*i+1]<<endl;
	    }*/
char *data2 = new char[2*n];
for (i=0; i<2*n; i++)
{
	data2[i] = 2*data[i]-1;
}
for (i=0; i<n; i++)
{
	in[i][0] = data2[2*i];
	in[i][1] = data2[2*i+1];
}
p = fftw_plan_dft_1d(n, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);  //IFFT
fftw_execute(p);


//modulation
int j;
for (i=0; i<n; i++)
{
j=i%2;
	if(j=0)
	{
	modu[2*i] = out[i][0];
	modu[2*i+1] = out[i][1];
	}
	if(j=1)
	{
	modu[2*i] = -out[i][0];
	modu[2*i+1] = -out[i][1];
	}
}

	for (i=0; i<n; i++)
            {
                cout<<"after IFFT : "<<"element No."<<i<<" "<<out[i][0]<<" "<<out[i][1]<<" "<<modu[2*i]<<" "<<modu[2*i+1]<<endl;
	    }


            fftw_destroy_plan(p);
	    //return out;
	return modu;
	  }
};


#endif



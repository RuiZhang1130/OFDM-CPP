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

	short * tran(int n, char data[])
	{
	int pt_l= 2*n+4;
	int pt_e= pt_l-3;
	int n2= pt_l/2;

	    fftw_plan p;
	    fftw_complex *in=new fftw_complex[n2];
	    fftw_complex *out=new fftw_complex[n2];

	double *modu = new double[pt_l];
	int i;
            
char *data2 = new char[2*n];
char *data_pt = new char[pt_l];
for (i=0; i<2*n; i++)
{
	data2[i] = 2*data[i]-1;  //1->1,0->-1
}

//Pilot tones
for (i=0; i<pt_l; i++)
{
	if(i<2)
	{data_pt[i]=2;}
	else if(i>pt_e)
	{data_pt[i]=2;}
	else
	{data_pt[i]=data2[i-2];}
}


for (i=0; i<n2; i++)
{
	in[i][0] = data_pt[2*i];
	in[i][1] = data_pt[2*i+1];
}

/*for(i=0;i<10;i++)
{
cout<<i<<": "<<in[i][0]<<endl;
}*/

p = fftw_plan_dft_1d(n2, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);  //IFFT
fftw_execute(p);
/*for(i=10000;i<n2;i++)
{
cout<<i<<": "<<out[i][0]<<endl;
}*/

//modulation
int j;
for (i=0; i<n2; i++)
{
j=i%2;
	if(j==0)
	{
	modu[2*i] = out[i][0];
	modu[2*i+1] = out[i][1];
	}
	if(j==1)
	{
	modu[2*i] = -out[i][0];
	modu[2*i+1] = -out[i][1];
	}
}

//double->short int
short *modu_short = new short[pt_l];
for(i=0; i<pt_l; i++)
{
	modu_short[i]=short(modu[i]);
}

//Pilot tones _value
short pt_start0 = modu_short[0];
short pt_start1 = modu_short[1];
short pt_end0 = modu_short[pt_l-2];
short pt_end1 = modu_short[pt_l-1];

//cyclic prefix
//////int modu_l = 2*n2;
int cp_l = pt_l/4; //1/4 data
int cp_t = pt_l+cp_l; //total
short *cp = new short[cp_t];
for (i=0;i<cp_t;i++)
{
	if(i<cp_l)
	{
	cp[i]=modu_short[i+pt_l-cp_l];
	}
	else
	{
	cp[i]=modu_short[i-cp_l];
	}
}
//add pt_value
int pt_cp_l = cp_t+4;
short *pt_cp = new short[pt_cp_l];

pt_cp[0] = pt_start0;
pt_cp[1] = pt_start1;
pt_cp[2] = pt_end0;
pt_cp[3] = pt_end1;
for (i=4;i<pt_cp_l;i++)
{
	pt_cp[i] = cp[i-4];
}

            fftw_destroy_plan(p);
	return pt_cp;
	  }
};


#endif


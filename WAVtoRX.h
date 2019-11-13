#ifndef WAVTORX_H
#define WAVTORX_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>

#include <math.h>

using namespace std;

class WAVto
{
   public:

double * r_wav(void)
{
	int wav_l;
	FILE* fp;
	
	fp = fopen("test.wav", "rb");
	fseek(fp, 0, SEEK_END);
	wav_l=ftell(fp);
	rewind(fp);
	
	int wav_d = wav_l/8;

	double* wavBuffer=(double*)malloc(wav_d* sizeof(double));
	fread(wavBuffer, wav_d, 8, fp);
	fclose(fp);
	
	int rm_head = wav_d-6;

	//char *a=(char*)malloc(rm_head* sizeof(char));
	double *b=(double*)malloc(rm_head* sizeof(double));
	//double *c=(double*)malloc(data_l* sizeof(double));
	double *p= wavBuffer;

	for(int i=0;i<wav_d;i++)
	{
	if(i>5)  //rm 48 bytes (head+sup[4])
		{
		b[i-6]=(double)*p;
		}
	p+=sizeof(char); //1byte
	}

	//free(b);
//cout<<rm_head<<endl;
	return b;
}


};


#endif

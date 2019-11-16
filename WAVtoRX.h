#ifndef WAVTORX_H
#define WAVTORX_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>


using namespace std;

class WAVto
{
	public:

	int data_l(void)
	{
		int length;
		int wav_l;
		FILE* fp;
	
		fp = fopen("record.wav", "rb");
		fseek(fp, 0, SEEK_END);
		length=ftell(fp);
		rewind(fp);

		fclose(fp);
		wav_l=length/2; //short int: 2 bytes
		return wav_l;
	}

	short * r_wav(void)
	{
		int length;
		int wav_l;
		FILE* fp;
	
		//read record file
		fp = fopen("record.wav", "rb");
		fseek(fp, 0, SEEK_END);
		wav_l=ftell(fp);
		rewind(fp);
		wav_l=length/2;
		short* wavBuffer=(short*)malloc(wav_l* sizeof(short));
		fread(wavBuffer, length, 1, fp);
		fclose(fp);
	
		short *b=(short*)malloc(wav_l* sizeof(short));
		short *p= wavBuffer;

		for(int i=0;i<wav_l;i++)
		{
			b[i]=(short)*p;
			p+=1;
		}

		return b;
	}

};


#endif

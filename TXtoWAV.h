#ifndef TXTOWAV_H
#define TXTOWAV_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>

#include <math.h>

using namespace std;

class toWAV
{
   public:

void w_wav(int n, short Tx_out[])
{
	struct wavHead
	{
		char RIFF[4];
		/*long*/ int size0;
		char WAVE[4];
		char FMT[4];
		/*long*/ int size1;
		short int fmttag;
		short int channel;  
		/*long*/ int samplepersec;
		/*long*/int bytepersec;
		short int blockalign;
		short int bitpersample; //16bit
		char DATA[4];
		/*long*/ int size2;
	};
wavHead head = {{'R','I','F','F'},0,{'W','A','V','E'},{'f','m','t',' '},16,1,1,44100,88200,2,16,{'d','a','t','a'},0};//initialization
	head.size2=2*n;
	head.size0=(2*n)+44-8;

struct wavHead *point = &head;

	FILE *fp;
	fp = fopen("test.wav","wb");
	fwrite(point, sizeof(head),1,fp);
	fwrite(Tx_out, n,1,fp);
	fclose(fp);
}


};


#endif

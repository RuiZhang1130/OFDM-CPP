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

void w_wav(int n, double Tx_out[])
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
		short int bitpersample;
		char DATA[4];
		/*long*/ int size2;
	};
wavHead head = {{'R','I','F','F'},0,{'W','A','V','E'},{'f','m','t',' '},16,1,1,44100,44100,1,8,{'d','a','t','a'},0};//initialization
	head.size0=n+16+8;
	head.size2=n;

struct wavHead *point = &head;
//long int a = sizeof(Tx_out);=n
	char sup[4] = {'s','u','p','p'};

	//char body[head.size2];
	FILE *fp;
	fp = fopen("test.wav","wb");
	fwrite(point, sizeof(head),1,fp);
	fwrite(sup,4,1,fp);
	fwrite(Tx_out, n,8,fp);
	fclose(fp);
}


};


#endif

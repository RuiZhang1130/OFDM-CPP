
#include "Tx.h"
#include "Rx.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>
#include "TXtoWAV.h"
#include "WAVtoRX.h"

using namespace std;

//#define N 64 //64 bytes
//#define Ns N/2 //symbol_length

int main(void)
{
	int length;
	FILE* fp;
	
	fp = fopen("source01.jpg", "rb");
	fseek(fp, 0, SEEK_END);
	length=ftell(fp);
	rewind(fp);
	
	char* ImgBuffer=(char*)malloc(length* sizeof(char));
	fread(ImgBuffer, length, 1, fp);
	fclose(fp);

int bl=8*length;
char data[bl];
int i;

	char *b=(char*)malloc(length* sizeof(char));
	char *p= ImgBuffer;
	for(i=0;i<length;i++){
	b[i]=(char)*p;
data[8*i] = (b[i] & 0x80) >> 7;
data[8*i+1] = (b[i] & 0x40) >> 6;
data[8*i+2] = (b[i] & 0x20) >> 5;
data[8*i+3] = (b[i] & 0x10) >> 4;
data[8*i+4] = (b[i] & 0x08) >> 3;
data[8*i+5] = (b[i] & 0x04) >> 2;
data[8*i+6] = (b[i] & 0x02) >> 1;
data[8*i+7] = (b[i] & 0x01);
	p+=sizeof(char);
	}
	//free(b);



int N= bl;
int Ns= bl/2;
int N_pt= N+4;
int N_cp= N_pt+(N_pt/4);
int N_ptcp=N_cp+4;
/////////////////////////////////////////////////
    Transmitter Tr;
    Receiver Re;
toWAV W_wav;
WAVto R_wav;

short *Tr_out=new short[N_ptcp];
char *Re_out=new char[N];

    
	Tr_out = Tr.tran(Ns,data);
short pt_start0 = Tr_out[0];
short pt_start1 = Tr_out[1];
short pt_end0 = Tr_out[2];
short pt_end1 = Tr_out[3];
short *Tr_out2=new short[N_cp];
for (i=0;i<N_cp;i++)
{
	Tr_out2[i] = Tr_out[i+4];
}

	W_wav.w_wav(N_cp,Tr_out2);

int wav_l;
	wav_l = R_wav.data_l();

short *wav_out=new short[wav_l];
	wav_out = R_wav.r_wav();

	Re_out = Re.rece(N_pt,wav_l,wav_out,pt_start0,pt_start1,pt_end0,pt_end1);//Ns

//////////////////////////////////////////////////
//////
char output[length];

for(int j=0;j<length;j++){
output[j]=(Re_out[8*j]<<7)|(Re_out[8*j+1]<<6)|(Re_out[8*j+2]<<5)|(Re_out[8*j+3]<<4)|(Re_out[8*j+4]<<3)|(Re_out[8*j+5]<<2)|(Re_out[8*j+6]<<1)|(Re_out[8*j+7]);}


//////////////////////////////////////
fp=fopen("output.jpg","wb");
fwrite(output, length, 1, fp);
fclose(fp);

//free(ImgBuffer);
//free(b);
//free(Re_out);


    return 0;
}

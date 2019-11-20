#ifndef TRANSMITTER_H
#define TRANSMITTER_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>
#include <math.h>
#include <vector>

//#include <unistd.h>
using namespace std;

class Transmitter
{
	public:

	void tran(int img_size, int symbol_n)
	{
		int k;
		int k2;
		int length;
		FILE* fp;
	
		fp = fopen("source01.jpg", "rb"); //read image
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		rewind(fp);
	
		char* ImgBuffer=(char*)malloc(length* sizeof(char));
		fread(ImgBuffer, length, 1, fp);
		fclose(fp);

		int bl = 8*length;      //bit stream length
		char data[bl];

		char *b=(char*)malloc(length* sizeof(char));
		char *p= ImgBuffer;
	
		//char->bit
		for(k=0; k<length; k++)
		{
			b[k]=(char)*p;
			data[8*k] = (b[k] & 0x80) >> 7;
			data[8*k+1] = (b[k] & 0x40) >> 6;
			data[8*k+2] = (b[k] & 0x20) >> 5;
			data[8*k+3] = (b[k] & 0x10) >> 4;
			data[8*k+4] = (b[k] & 0x08) >> 3;
			data[8*k+5] = (b[k] & 0x04) >> 2;
			data[8*k+6] = (b[k] & 0x02) >> 1;
			data[8*k+7] = (b[k] & 0x01);
			p+=sizeof(char);
		}

		//Symbol
		int sl = bl/symbol_n;   //each symbol length
		vector<vector<char> > symbol(symbol_n, vector<char>(sl));
		for(k=0; k<symbol_n; k++)
		{
			for(k2=0; k2<sl; k2++)
			{
				symbol[k][k2] = data[(sl*k)+k2];
			}
		}

		char *rand_n = new char[sl];
		char *data_xor = new char[sl];
		int N = sl/2;
		int bbl = 2*sl;	//2*sl		
		char bb[bbl];
		fftw_complex *data_qam = new fftw_complex[sl];		
		fftw_complex *out = new fftw_complex[sl];
		double *modu = new double[bbl];
		//short *modu_short = new short[bbl];
		int cpl = bbl/4;   //1/4 data
		int tl = bbl+cpl;  //total length
		double *data_cp = new double[tl];
		vector<vector<double> > symbol_out(symbol_n, vector<double>(tl));
		//short *symbol_out = new short[tl];


		for(int n=0; n<symbol_n; n++)
		{
				
			//Energy dispersal
			srand(4);                  //generate random seeds
			for(k=0; k<sl; k++)
			{
				rand_n[k] = rand()%2;        //generate a random number of 0 or 1
			}
			for(k=0; k<sl; k++)
			{
				data_xor[k] = symbol[n][k]^rand_n[k];   //XOR
			}

			//*****************************************
			for(k=0; k<bbl; k++)
			{
				bb[k] = 0;
			}
			for(k=0; k<sl; k++)
			{
				data_xor[k] = 2*data_xor[k]-1;  //1->1,0->-1
			}
			
			for(k=0; k<sl;k++)
			{
				bb[N+k] = data_xor[k];
			}
			
			//PT
			bb[N-2] = 2;
			bb[N-1] = 0;
			//bb[bbl-N-2] =2;
			//bb[bbl-N-1] =0;
			
			//QAM
			for(k=0; k<sl; k++)
			{
				data_qam[k][0] = bb[2*k];
				data_qam[k][1] = bb[2*k+1];
			}
	
			//IFFT ("data_qam" is input)
			fftw_plan p_ifft;
			p_ifft = fftw_plan_dft_1d(sl, data_qam, out, FFTW_BACKWARD, FFTW_ESTIMATE);
			fftw_execute(p_ifft);

	
			//Bandpass quadrature modulation
			for (k=0; k<sl; k++)
			{
				k2 = k%2;
				if(k2==0)
				{
					modu[2*k] = out[k][0];
					modu[2*k+1] = out[k][1];
				}
				else if(k2==1)
				{
					modu[2*k] = -out[k][0];
					modu[2*k+1] = -out[k][1];
				}
			}
	
			//Cyclic Prefix	
			for(k=0; k<tl; k++)
			{
				if(k<cpl)
				{
					data_cp[k] = modu[k+bbl-cpl];
				}
				else
				{
					data_cp[k] = modu[k-cpl];
				}
			}

			for(k=0; k<tl; k++)
			{
				symbol_out[n][k] = data_cp[k];
			}
			
			fftw_destroy_plan(p_ifft);
		}
	
		//Save as WAV
		struct wavHead
		{
			char RIFF[4];
			int size0;
			char WAVE[4];
			char FMT[4];
			int size1;
			short int fmttag;
			short int channel;      //1-channel
			int samplepersec;       //44100Hz
			int bytepersec;
			short int blockalign;
			short int bitpersample; //32bit
			char DATA[4];
			int size2;              //length of data
		};
		
		wavHead head = {{'R','I','F','F'},0,{'W','A','V','E'},{'f','m','t',' '},16,1,1,44100,176400,4,32,{'d','a','t','a'},0};//initialization
		head.size2 = (8*tl)*symbol_n;
		head.size0 = (8*tl)*symbol_n+44-8;
	
		struct wavHead *point = &head;
		fp = fopen("test.wav","wb");
		fwrite(point, sizeof(head), 1, fp);   //write head

		double *wav = new double[symbol_n*tl];
		vector<double>::iterator it;
		vector<vector<double>>::iterator iter;

		k = 0;
		for(iter=symbol_out.begin(); iter!=symbol_out.end(); iter++)
		{
			for(it=iter->begin(); it!=iter->end(); it++)
			{
				wav[k] = *it;
				k = k+1;
			}
		}
		fwrite(wav, 8*symbol_n*tl, 1, fp);             //write data

		fclose(fp);
	}
};


#endif


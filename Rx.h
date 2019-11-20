#ifndef RECEIVER_H
#define RECEIVER_H


#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>
#include <vector>

#include <unistd.h>

using namespace std;

class Receiver
{
	public:
   
	void rece(int img_size, int symbol_n)
        {
		int k, k2, n, n2, kk;
		int wav_l;
		int wav_double;
		FILE* fp;
	
		//Read record file
		fp = fopen("record.wav", "rb");
		fseek(fp, 0, SEEK_END);
		wav_l = ftell(fp);      //1byte
		rewind(fp);
		wav_double = wav_l/8;   //8bytes(double)
		char* wavBuffer=(char*)malloc(wav_l* sizeof(char));
		fread(wavBuffer, wav_l, 1, fp);
		fclose(fp);
	
		char *wav_data=(char*)malloc(wav_l* sizeof(char));
		char *p= wavBuffer;

		for(k=0; k<wav_l; k++)
		{
			wav_data[k]=(char)*p; 
			p+=sizeof(char);
		}

		/*The length of the data stream before modulatiom can be 
		  obtained from the information of the original image.*/
		int ori_ml = 2*8*img_size/symbol_n;

		int cp_l = ori_ml/4;      //length of CP
		int sl = ori_ml+cp_l;  	  //length of each symbol
		int pos_end = wav_double-(sl*(symbol_n))+1;
		double *symbol = new double[sl];
		double *s_decp = new double[ori_ml];
		int demodu_l = ori_ml/2;
		int N = demodu_l/4;
		fftw_complex *demodu = new fftw_complex[demodu_l];
		fftw_complex *out = new fftw_complex[demodu_l];
		fftw_complex *BB = new fftw_complex[cp_l];

		char *de_qam = new char[demodu_l];
		char *rand_n = new char[demodu_l];	
		vector<vector<char> > s_out(symbol_n, vector<char>(demodu_l));
 
		int pos_start; 
		int pos0 = 0;

		char chs[8];
		double data_double[wav_double];
	

		for(n=0; n<symbol_n; n++)
		{ 
			do{
				for(kk=0; kk<wav_double; kk++)
				{
					chs[0] = wav_data[8*kk+pos0];
					chs[1] = wav_data[8*kk+1+pos0];
					chs[2] = wav_data[8*kk+2+pos0];
					chs[3] = wav_data[8*kk+3+pos0];
					chs[4] = wav_data[8*kk+4+pos0];
					chs[5] = wav_data[8*kk+5+pos0];
					chs[6] = wav_data[8*kk+6+pos0];
					chs[7] = wav_data[8*kk+7+pos0];
					data_double[kk] = *(double*)chs; 
				}	pos_start = 0;
				do
				{
	
					for(k=0; k<sl; k++)
					{
						symbol[k] = data_double[pos_start+k];
					}
		
					//Decode_ Cyclic Prefix
					for(k=0; k<ori_ml; k++)
					{
						s_decp[k] = symbol[k+cp_l];
					}
	
					//Demodulation
					for(k=0; k<demodu_l; k++)
					{
						k2 = k%2;
						if(k2==0)
						{
							demodu[k][0] = s_decp[2*k];
							demodu[k][1] = s_decp[2*k+1];
						}
						else if(k2==1)
						{
							demodu[k][0] = -1*s_decp[2*k];
							demodu[k][1] = -1*s_decp[2*k+1];
						}
					}
		
					//FFT ("demodu" is input)
					fftw_plan q_fft;
					q_fft = fftw_plan_dft_1d(demodu_l, demodu, out, FFTW_FORWARD, FFTW_ESTIMATE);
					fftw_execute(q_fft);
					fftw_destroy_plan(q_fft);
	
					// normalize
					out[N-1][0] *= 1./demodu_l;
	
					if(out[N-1][0]==2)
					{
	
						for(k=0; k<cp_l; k++)
						{
							BB[k][0] = out[k+N][0];
							BB[k][1] = out[k+N][1];
						}
						//DeCODE_ QAM
						for (k=0; k<cp_l; k++)
						{
							if(BB[k][0]>0)
								de_qam[2*k] = 1;
							else
								de_qam[2*k] = 0;
						}
						for (k=0; k<cp_l; k++)
						{
							if(BB[k][1]>0)
								de_qam[2*k+1] = 1;
							else
								de_qam[2*k+1] = 0;
						}
						
						//XOR
						srand(4);                  //random seeds same Tx
						for(k=0; k<demodu_l; k++)
						{
							rand_n[k] = rand()%2;        
						}
						for(k=0; k<demodu_l; k++)
						{
							s_out[n][k] = de_qam[k]^rand_n[k];   
						}
						
						break;
					}
					
				pos_start=pos_start+1;
				}while(pos_start<pos_end);
		
			pos0 = pos0+1;
			}while(pos0<8);	

		}		
		

		//output image
		char output_b[symbol_n*demodu_l];
		char output[img_size];

		vector<char>::iterator it;
		vector<vector<char>>::iterator iter;

		k=0;
		for(iter=s_out.begin(); iter!=s_out.end(); iter++)
		{
			for(it=iter->begin(); it!=iter->end(); it++)
			{
				output_b[k]=*it;
				k = k+1;
			}
		}

		for(k=0; k<img_size; k++)
		{
			output[k] = (output_b[8*k]<<7)|(output_b[8*k+1]<<6)|(output_b[8*k+2]<<5)|(output_b[8*k+3]<<4)|(output_b[8*k+4]<<3)|(output_b[8*k+5]<<2)|(output_b[8*k+6]<<1)|(output_b[8*k+7]);
		}
	

		fp=fopen("output.jpg","wb");
		fwrite(output, img_size, 1, fp);
		
		fclose(fp);
	}

};


#endif


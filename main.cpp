
#include "Tx.h"
#include "Rx.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>

using namespace std;

//#define N 64 //64 bytes
//#define Ns N/2 //symbol_length

int main(void)
{
    /* //Generate initial data 
    int init_data[N];
    int i;

    srand((unsigned)time(NULL)); //Generate random seeds
    
	//for(i=0; i<N; i++)
    //{
       // init_data[i]=(rand()%256)-128; //range:-128~127
      //  cout <<"element No."<<i<<" "<< init_data[i]<<endl;
    //}
	
    for(i=0; i<N; i++)
	{
	init_data[i]=rand()%2; //[0,1]
	cout <<"element No."<<i<<" "<< init_data[i]<<endl;
	}
    */

	int length;
	FILE* fp;
	
	fp = fopen("source01.jpg", "rb");
	fseek(fp, 0, SEEK_END);
	length=ftell(fp);
	rewind(fp);
	
	char* ImgBuffer=(char*)malloc(length* sizeof(char));
	fread(ImgBuffer, length, 1, fp);
	fclose(fp);

	char *b=(char*)malloc(length* sizeof(char));
	char *p= ImgBuffer;
	for(int i=0;i<length;i++){
	b[i]=(char)*p;
	//cout<<b[i]<<endl;
	p+=sizeof(char);
	}
	//cout<<length<<endl;
	//free(b);

int N= length;
int Ns= length/2;
/////////////////////////////////////////////////
    Transmitter Tr;
    Receiver Re;
    //fftw_complex *Tr_out;
	/*float *Tr_out;*/
	fftw_complex *Tr_out=new fftw_complex[Ns];
char *Re_out=new char[N];
    
    Tr_out = Tr.tran(N,b);
    Re_out = Re.rece(Ns,Tr_out);
    
    //fftw_cleanup();
//////////////////////////////////////////////////
fp=fopen("source02.jpg","wb");
fwrite(Re_out, length, 1, fp);
fclose(fp);

free(ImgBuffer);
free(b);
free(Re_out);


    return 0;
}

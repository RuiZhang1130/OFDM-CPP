
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




//int N= length;
//int Ns= length/2;
int N= bl;
int Ns= bl/2;
/////////////////////////////////////////////////
    Transmitter Tr;
    Receiver Re;
    //fftw_complex *Tr_out;
	/*float *Tr_out;*/
	//fftw_complex *Tr_out=new fftw_complex[Ns];
//char *Tr_out=new char[N];
//char *Re_out=new char[N];
double *Tr_out=new double[N];
char *Re_out=new char[N];
    
    Tr_out = Tr.tran(Ns,data);

/*int dn = 8*N; 
char str[dn];
for(i=0; i<N; i++){
gvct(Tr_out[i], 8, str[]);*/

    Re_out = Re.rece(Ns,Tr_out);
    
    //fftw_cleanup();
//////////////////////////////////////////////////
//////
//char *output =new char[length];
//char *output = (char*)malloc(length* sizeof(char));
char output[length];

for(int j=0;j<length;j++){
output[j]=(Re_out[8*j]<<7)|(Re_out[8*j+1]<<6)|(Re_out[8*j+2]<<5)|(Re_out[8*j+3]<<4)|(Re_out[8*j+4]<<3)|(Re_out[8*j+5]<<2)|(Re_out[8*j+6]<<1)|(Re_out[8*j+7]);}
//char *output2 = (char *)output;

//cout<<data[8*12065]<<endl;

//cout<<N<<endl;
//cout<<sizeof(data)<<endl;
//cout<<output2<<endl;


//////////////////////////////////////
fp=fopen("source02.jpg","wb");
fwrite(output, length, 1, fp);
fclose(fp);

free(ImgBuffer);
free(b);
free(Re_out);


    return 0;
}

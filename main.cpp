
#include "Tx.h"
#include "Rx.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fftw3.h>

using namespace std;

#define img_size 12066  //image size (byte)
#define symbol_n 1    //number of symbols

int main(void)
{

	Transmitter Tr;
	Receiver Re;

	FILE* fh;

	fh = fopen("source01.jpg", "r");
	if(fh==NULL)
	{
		fclose(fh);
		cout<<"There is no image 'source01.jpg' in this folder."<<endl;
	}
	else
	{
		fclose(fh);

		Tr.tran(img_size, symbol_n);
		cout<<"The transmitter is running successfully."<<endl;
	}
	
	fh = fopen("record.wav", "r");
	if(fh==NULL)
	{
		fclose(fh);
		cout<<"There is no audio 'test.wav' in this folder."<<endl;
	}
	else
	{
		fclose(fh);
		Re.rece(img_size, symbol_n);
		cout<<"The receiver is running successfully."<<endl;
	}

	return 0;
}

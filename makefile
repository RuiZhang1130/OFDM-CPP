myTest: main.cpp
	g++ main.cpp -o myTest -lfftw3 -L/home/rrbb/fftw/lib -I/home/rrbb/fftw/include


clean:
	rm myTest


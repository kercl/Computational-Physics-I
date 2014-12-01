#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>

#include "lib/transform.h"
#include "lib/csv.h"
#include "lib/converter.h"

#define ROUNDS 5
#define MAX_ARRAY_SIZE 15

using namespace std;

int main(int argc, char **argv) {
	vector<complex<float>> data, result, tf;
	vector<int> dfttime, ffttime;
	
	dft<complex<float>> ftransform;
	fft fasttransform;
	
	srand(time(NULL));
	
	cerr << "dft performance: transforming vector " << ROUNDS << " times" << endl
	     << "=================================================================" << endl;
	
	int len = 1;
	for(int k = 0; k < MAX_ARRAY_SIZE; ++k) {
		cerr << " -- measuring for " << len << " elements..." << endl;
	
		int t = clock();
		for(int i = 0; i < ROUNDS; ++i) {
			data.clear();
			
			for(int j = 0; j < len; ++j)
				data.push_back(complex<float>(rand(), rand()));
		
			ftransform.forward_transform(result, data);
		}
		dfttime.push_back(clock() - t);
		len *= 2;
	}
	
	cerr << "fft performance: transforming vector " << ROUNDS << " times" << endl
	     << "=================================================================" << endl;
	
	len = 1;
	for(int k = 0; k < MAX_ARRAY_SIZE; ++k) {
		cerr << " -- measuring for " << len << " elements..." << endl;
	
		int t = clock();
		for(int i = 0; i < ROUNDS; ++i) {
			data.clear();
		
			for(int j = 0; j < len; ++j)
				data.push_back(complex<float>(rand(), rand()));
		
			fasttransform.forward_transform(result, data);
		}
		ffttime.push_back(clock() - t);
		
		len *= 2;
	}
	
	cout << "N FFT DFT\n";
	int l = 1;
	for(int k = 0; k < MAX_ARRAY_SIZE; ++k) {
		cout << l << " " << (float)ffttime[k] / ((float)ffttime[MAX_ARRAY_SIZE-1]) 
		          << " " << (float)dfttime[k] / ((float)dfttime[MAX_ARRAY_SIZE-1]) << "\n";
		l = l * 2;
	}
}


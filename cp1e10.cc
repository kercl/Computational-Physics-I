#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>

#include "lib/transform.h"
#include "lib/csv.h"
#include "lib/converter.h"

using namespace std;

#define SIZE 64

int main(int argc, char **argv) {	
	vector<complex<float>> data[4], transformed[3];

	srand(time(NULL));
	for(int i = 0; i < SIZE; ++i)
		data[0].push_back(exp(-(i-SIZE/2)*(i-SIZE/2)*0.01) + 0.1*(float)rand() / (float)RAND_MAX - 0.05);
	
	fft ftransform;
	
	for(int i = 0, k = 20; i < 3; ++i, k+=20) {
		ftransform.forward_transform(transformed[i], data[0]);
		
		for(int j = 0; j < k; ++j)
			transformed[i][transformed[i].size() - 1 - j] = 0;
				
		ftransform.backward_transform(data[i + 1], transformed[i]);
	}
	
	cout << "N data r1 r2 r3\n";
	for(int i = 0; i < SIZE; ++i) {
		cout << i << " " << data[0][i].real() << " " << 
		                   (data[1][i].real() + 2) << " " << 
		                   (data[2][i].real() + 4) << " " <<
		                   (data[3][i].real() + 6) << endl;
	}
}


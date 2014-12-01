#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>

#include "lib/transform.h"
#include "lib/csv.h"
#include "lib/converter.h"

using namespace std;

int main(int argc, char **argv) {
	csv fs;
	fs.open("data/cp1e1.csv");
	fs.set_delimiter(" ");
	vector<string> row;
	fs.fetch_next_row(row);
	fs.close();
	
	vector<complex<float>> crow, crow2, transformed;
	converter::complexify_string_vector(crow, row);
	
	cout << "transforming data set:" << endl;
	for(auto &z: crow)
		cout << z << ", ";
	cout << endl << "================================================" << endl;
	
	dft<complex<float>> ftransform;
	
	cout << "the fourier-coefficients are periodic in the fourier-index:" << endl
	     << "setting range to -4 <= k <= 13:" << endl;
	
	ftransform.set_output_range(-4, 13);
	ftransform.forward_transform(transformed, crow);
	
	for(int k = 0; k < transformed.size(); ++k)
		cout << "~f_" << k-4 << ": " << transformed[k] << endl;
	
	cout << endl << "the backward and forward transformation are inverse to each other: " << endl;
	
	ftransform.set_output_range(0, crow.size()-1);
	ftransform.forward_transform(transformed, crow);
	ftransform.backward_transform(crow2, transformed);
	for(int i = 0; i < crow.size(); ++i)
		cout << crow[i] << " --> " << transformed[i] << " --> " << crow2[i] << endl;
	
	cout << endl << "the inverse dft provides a periodic continuation of the original data: " << endl
	     << "setting range to -7 <= k <= 15:" << endl;
	
	ftransform.set_output_range(-7, 15);
	ftransform.backward_transform(crow2, transformed);
	
	for(int k = 0; k < crow2.size(); ++k)
		cout << "f_" << k-7 << ": " << crow2[k] << endl;
}


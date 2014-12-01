#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>

#include "lib/transform.h"
#include "lib/csv.h"
#include "lib/converter.h"
#include "lib/diffgl.h"

using namespace std;

int main(int argc, char **argv) {
	coperator<double> *dopl[] = 
		{
			new diff_operator<double>::dx<0,1>(),
			new diff_operator<double>::dx<-1,0>(),
			new diff_operator<double>::dx<-1,1>(),
			new diff_operator<double>::dx2<-1,1>()
		};
	
	cout << "Resolution, dx<0,1>, dx<-1,0>, dx<-1,1>, dxx<-1,1>" << endl;
	for(int res = 4; res < 500000; res+=1000) {
		double deltax;
		
		deltax = 1.0 / res;
	
		vector<double> sin_01;
		for(int i = -1; i <= res; ++i)
			sin_01.push_back(sin(deltax * i));
		
		double err_dop[] = {0.0, 0.0, 0.0, 0.0};
		for(int i = 1; i < res; ++i) {
			double ref_val[] = {cos(deltax * (i - 1)), cos(deltax * (i - 1)), cos(deltax * (i - 1)), -sin(deltax * (i - 1))};
			
			for(int j = 0; j < 4; ++j)
				if(abs(dopl[j]->eval(sin_01, i, deltax) - ref_val[j]) > err_dop[j])
					err_dop[j] = abs(dopl[j]->eval(sin_01, i, deltax) - ref_val[j]);
		}
		cout << res << ", " << err_dop[0] << ", " << err_dop[1] << ", " << err_dop[2] << ", " << err_dop[3] << endl;
	}
}


#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>

#include "lib/transform.h"
#include "lib/csv.h"
#include "lib/converter.h"
#include "lib/diffgl.h"
#include "lib/snapshot.h"

#define DT 0.00001
#define NX 100
#define DX (1.0/(double)(NX))
#define LIMIT 0.1

using namespace std;

class time_logger : public fixed_number_snapshot {
	protected:
		void do_snapshot() {
			double t = DT*fixed_number_snapshot::get_counter();
			
			for(int i = 0; i <= NX; ++i)
				cout << "\t" << t << "\t" << ((double)i / (double)NX) << "\t" << vstate[i] << endl;
			cout << endl;
		}
		
		vector<double> &vstate;
	public:
		time_logger(int n, vector<double> &v): 
			fixed_number_snapshot(LIMIT / DT + 1, n),
			vstate(v) {
				cout << "splot '-' using 1:2:3 with pm3d" << endl;
			}
};

class accuracy_logger : public fixed_number_snapshot {
	protected:
		void do_snapshot() {
			double diff = 0.0, x, t = DT*fixed_number_snapshot::get_counter(), nd, asol;
			
			for(int i = 0; i <= NX; ++i) {
				x = (double)i/(double)NX;
				
				asol = 0.5*(1-exp(-4*M_PI*M_PI*t)*cos(2*M_PI*x));
				nd = abs(vstate[i] - asol) / asol;
				
				if(nd > diff)
					diff = nd;
			}
			cout << t << "\t" << diff << endl;
		}
		
		vector<double> &vstate;
		ofstream of;
	public:
		accuracy_logger(int n, vector<double> &v): 
			fixed_number_snapshot(LIMIT / DT + 1, n),
			vstate(v) {
			cout << "plot '-' using 1:2 with lines" << endl;
		}
};

class selected_accuracy_logger : public fixed_point_snapshot {
	protected:
		void do_snapshot() {
			double diff = 0.0, x, t = DT*fixed_point_snapshot::get_counter(), nd, asol;
			
			for(int i = 0; i <= NX; ++i) {
				x = (double)i/(double)NX;
				
				asol = 0.5*(1-exp(-4*M_PI*M_PI*t)*cos(2*M_PI*x));
				nd = abs(vstate[i] - asol);
				
				if(nd > diff)
					diff = nd;
			}
			
			cerr << t << ": " << diff << endl;
		}
		
		vector<double> &vstate;
	public:
		selected_accuracy_logger(vector<double> &nv): vstate(nv) {}
};

int main(int argc, char **argv) {
	coperator<double> *dx = new diff_operator<double>::dx_dynamic();
	
	vector<double> u[2];
	int vsel = 0;
	for(int i = 0; i <= NX; ++i) {
		u[0].push_back(0.5 * (1.0 - cos(2.0 * M_PI * (double)i / (double)NX)));
		u[1].push_back(0.0);
	}
	
	double t = 0.0;
	
	selected_accuracy_logger sal(u[0]);
	sal.add_snapshot_point(0.01 / DT);
	sal.add_snapshot_point(0.02 / DT);
	sal.add_snapshot_point(0.05 / DT);
	snapshot::register_logger(&sal);
	
	if(argc == 2 && !strcmp(argv[1], "--data")) {
		time_logger tl(60, u[0]);
		snapshot::register_logger(&tl);
	}else if(argc == 2 && !strcmp(argv[1], "--error")) {
		accuracy_logger al(400, u[0]);
		snapshot::register_logger(&al);
	}
	
	snapshot::evoke();
	
	while(t < LIMIT) {
		for(int i = 0; i <= NX; ++i) {
			u[1][i] = dx->eval(u[0], i, (1.0/(double)NX));
		}
		u[1][0] = 0;
		u[1][NX] = 0;
		for(int i = 0; i <= NX; ++i) {
			u[0][i] = u[0][i] + DT * dx->eval(u[1], i, DX);
		}
		
		snapshot::evoke();
		
		t += DT;
	}
}


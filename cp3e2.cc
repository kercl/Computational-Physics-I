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

#define DT 0.00042
#define NX 100
#define DX (1.0/(double)(NX))
#define LIMIT 0.3

#define INC(x) ((x+1)%3)
#define DEC(x) ((x+2)%3)

using namespace std;

class time_logger : public fixed_number_snapshot {
	protected:
		void do_snapshot() {
			double t = DT*fixed_number_snapshot::get_counter();
			
			for(int i = 0; i <= NX; ++i)
				cout << "\t" << t << "\t" << ((double)i / (double)NX) << "\t" << vstate[selected_state][i] << endl;
			cout << endl;
		}
		
		vector<double> *vstate;
		int &selected_state;
	public:
		time_logger(int n, vector<double> *v, int &sel_state): 
			fixed_number_snapshot(LIMIT / DT + 1, n),
			vstate(v),
			selected_state(sel_state) {
				cout << "splot '-' using 1:2:3 with pm3d" << endl;
			}
};

class accuracy_logger : public fixed_number_snapshot {
	protected:
		void do_snapshot() {
			double diff = 0.0, x, t = DT*fixed_number_snapshot::get_counter(), nd, asol;
			
			for(int i = 0; i <= NX; ++i) {
				x = (double)i/(double)NX;
				
				asol = sin(M_PI*x)*exp(-M_PI*M_PI*t);
				nd = abs(vstate[selected_state][i] - asol);
				
				if(nd > diff)
					diff = nd;
			}
			cout << t << "\t" << diff << endl;
		}
		
		vector<double> *vstate;
		int &selected_state;
		ofstream of;
	public:
		accuracy_logger(int n, vector<double> *v, int &sel_state): 
			fixed_number_snapshot(LIMIT / DT + 1, n),
			vstate(v),
			selected_state(sel_state) {
			cout << "plot '-' using 1:2 with lines" << endl;
		}
};

class selected_accuracy_logger : public fixed_point_snapshot {
	protected:
		void do_snapshot() {
			double diff = 0.0, x, t = DT*fixed_point_snapshot::get_counter(), nd, asol;
			
			for(int i = 0; i <= NX; ++i) {
				x = (double)i/(double)NX;
				
				asol = sin(M_PI*x)*exp(-M_PI*M_PI*t);
				nd = abs(vstate[selected_state][i] - asol);
				
				if(nd > diff)
					diff = nd;
			}
			
			cerr << t << ": " << diff << endl;
		}
		
		vector<double> *vstate;
		int &selected_state;
	public:
		selected_accuracy_logger(vector<double> *nv, int &sel_state): vstate(nv), selected_state(sel_state) {}
};

int main(int argc, char **argv) {
	coperator<double> *dx2 = new diff_operator<double>::dx2<-1,1>();
	
	vector<double> u[3];
	int vsel = 1;
	for(int i = 0; i <= NX; ++i)
		u[1].push_back(sin(M_PI * i * DX));
	
	u[2].push_back(0);
	for(int i = 1; i < NX; ++i)
		u[2].push_back(dx2->eval(u[0], i, DX));
	u[2].push_back(0);
	
	u[0].push_back(0);
	for(int i = 1; i < NX; ++i)
		u[0].push_back(u[1][i] - DT * u[2][i] + 0.5*DT*DT * dx2->eval(u[2], i, DX)); // u(x,0) - DT * u''(x,0) + DT² u''''(x,0) / 2
	u[0].push_back(0);
	
	double t = 0.0;
	
	selected_accuracy_logger sal(u, vsel);
	sal.add_snapshot_point(0.05 / DT);
	sal.add_snapshot_point(0.1 / DT);
	sal.add_snapshot_point(0.25 / DT);
	snapshot::register_logger(&sal);
	
	if(argc == 2 && !strcmp(argv[1], "--data")) {
		time_logger tl(100, u, vsel);
		snapshot::register_logger(&tl);
	}else if(argc == 2 && !strcmp(argv[1], "--error")) {
		accuracy_logger al(100, u, vsel);
		snapshot::register_logger(&al);
	}
	
	snapshot::evoke();
	
	double a = 2.0 * (DT / (DX * DX));
	
	while(t < LIMIT) {
		for(int i = 1; i < NX; ++i)
			u[INC(vsel)][i] = ((1-a)/(1+a))*u[DEC(vsel)][i]+(a/(1+a))*(u[vsel][i+1]+u[vsel][i-1]);
		
		vsel = INC(vsel);
		
		snapshot::evoke();
		
		t += DT;
	}
}


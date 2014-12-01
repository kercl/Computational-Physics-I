#ifndef DIFFGL_H
#define DIFFGL_H

#include <vector>
#include <algorithm>
#include <iostream>

template<class T>
class coperator {
	public:
		virtual double eval(const std::vector<T> &data, int pt, double res) = 0;
};

template<class T>
class diff_operator {
	public:
		template<int ld, int ud>
		class dx : public coperator<T> {
			double eval(const std::vector<T> &data, int pt, double res) {
				if(pt > data.size() || pt < 0)
					return 0.0;
		
				return (data[std::min((long unsigned int)(data.size() - 1), (long unsigned int)(pt + ud))] - 
						data[std::max((long unsigned int)(0), (long unsigned int)(pt + ld))]) / ((ud - ld) * res);
			}
			
			double eval_no_check(const std::vector<T> &data, int pt, double res) {
				if(pt > data.size() || pt < 0)
					return 0.0;
		
				return (data[(long unsigned int)(pt + ud)] - data[(long unsigned int)(pt + ld)]) / ((ud - ld) * res);
			}
		};
		
		class dx_dynamic: public coperator<T> {
				double eval(const std::vector<T> &data, int pt, double res) {
					coperator<double> *gdx;
					if(pt == 0)
						gdx = new diff_operator<T>::dx<0,1>();
					else if(pt == data.size() - 1)
						gdx = new diff_operator<T>::dx<-1,0>();
					else
						gdx = new diff_operator<T>::dx<-1,1>();
					
					return gdx->eval(data, pt, res);
				}
		};
		
		template<int ld, int ud>
		class dx2 : public coperator<T> {
			double eval(const std::vector<T> &data, int pt, double res) {
				using namespace std;
				if(pt > data.size() || pt < 0)
					return 0.0;
				
				return (data[std::min((long unsigned int)(data.size() - 1), (long unsigned int)(pt + ud))] - 
						2.0 * data[pt] + 
						data[std::max((long unsigned int)(0), (long unsigned int)(pt + ld))]) / (res * res);
			}
		};
};

#endif


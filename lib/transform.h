#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include <complex>
#include <iostream>

#include <gsl/gsl_fft_complex.h>

template<class T>
class transformation {
	public:
		virtual void forward_transform(std::vector<T> &out, const std::vector<T> &in) = 0;
		virtual void backward_transform(std::vector<T> &out, const std::vector<T> &in) = 0;
};

template<class T>
class dft: public transformation<T> {
		void  do_transform(std::vector<T> &out, const std::vector<T> &in, float expcoeff, float sumcoeff) {
			out.clear();
			
			int end = (rk2 <= rk1 ? in.size() - 1: rk2);
			
			for(int k = rk1; k <= end; ++k) {
				T fn;
				for(int n = 0; n < in.size(); ++n) {
					fn += in[n] * std::polar(1.0f, expcoeff * k * n / in.size());
				}
				out.push_back(sumcoeff * fn);
			}
		}
		
		int rk1, rk2;
	public:
		dft() {
			rk1 = 0;
			rk2 = -1;
		}
	
		void set_output_range(int k1, int k2) {
			rk1 = std::min(k1, k2);
			rk2 = std::max(k1, k2);
		}
	
		void forward_transform(std::vector<T> &out, const std::vector<T> &in) {
			do_transform(out, in, -2 * M_PI, 1.0);
		}
	
		void backward_transform(std::vector<T> &out, const std::vector<T> &in) {
			do_transform(out, in, 2 * M_PI, 1.0 / in.size());
		}
};

class fft: public transformation<std::complex<float>> {
		void do_transform(std::vector<std::complex<float>> &out, const std::vector<std::complex<float>> &in, gsl_fft_direction dirsgn, float sumcoeff) {
		double *data = new double[2 * in.size()];
			for(int i = 0; i < in.size(); i++) {
				data[i<<1] = in[i].real();
				data[(i<<1)+1] = in[i].imag();
			}
			gsl_fft_complex_radix2_transform(data, 1, in.size(), dirsgn);
			
			out.clear();
			for(int i = 0; i < in.size() * 2; i += 2)
				out.push_back(std::complex<float>(data[i], data[i + 1]) * sumcoeff);
		}
	public:	
		void forward_transform(std::vector<std::complex<float>> &out, const std::vector<std::complex<float>> &in) {
			do_transform(out, in, gsl_fft_forward, 1.0);
		}
	
		void backward_transform(std::vector<std::complex<float>> &out, const std::vector<std::complex<float>> &in) {
			do_transform(out, in, gsl_fft_backward, 1.0f/(float)in.size());
		}
};

#endif


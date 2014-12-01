#ifndef CONVERTER
#define CONVERTER

#include <vector>
#include <string>
#include <complex>

class converter {
	public:
		static bool complexify_string_vector(std::vector<std::complex<float>> &cv, std::vector<std::string> &sv);
		static bool complexify_string(std::complex<float> &c, std::string s);
};

#endif


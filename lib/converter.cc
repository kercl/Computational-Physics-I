#include "converter.h"

#include <algorithm>

using namespace std;

bool converter::complexify_string_vector(std::vector<std::complex<float>> &cv, std::vector<std::string> &sv) {
	cv.clear();
	
	for(auto &s: sv) {
		std::complex<float> c;
		complexify_string(c, s);
		cv.push_back(c);
	}
}

bool converter::complexify_string(std::complex<float> &c, std::string s) {
	/*s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
	s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
	*/
	s.erase( std::remove_if( s.begin(), s.end(), ::isspace ), s.end() );
	
	size_t pos = s.find('+');
	std::string a, b;
	
	if(pos == std::string::npos) {
		a = "0";
		b = "i0";
		
		if(s[0] == 'i' || s[0] == 'j')
			b = s;
		else if(s[s.size() - 1] == 'i' || s[s.size() - 1] == 'j')
			b = s;
		else
			a = s;
	}else {
		a = s.substr(0, pos);
		b = s.substr(pos + 1, s.size() - pos - 1);
	}
	
	if(b[0] == 'i' || b[0] == 'j')
		b[0] = ' ';
	else if(b[b.size() - 1] == 'i' || b[b.size() - 1] == 'j')
		b[b.size() - 1] = ' ';
	else
		return false;
	
	c = std::complex<float>(std::stof(a), std::stof(b));
	return true;
}


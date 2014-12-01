#include "csv.h"

int csv::fetch_row() {
	std::string ln, token;
	size_t pos = 0;
	
	if(fs.good()) {
		current_row.clear();
		
		std::getline(fs, ln);
		while((pos = ln.find(delimiter)) != std::string::npos) {
			if(pos != 0) {
				token = ln.substr(0, pos);
				current_row.push_back(token);
			}
			ln.erase(0, pos + delimiter.length());
		}
		
		if(ln != "")
			current_row.push_back(ln);
		
		it = current_row.begin();
		return new_line;
	}
	
	return nothing_to_read;
}

void csv::open(std::string fn) {
	fs.open(fn.c_str(), std::ifstream::in);
	delimiter = ",";
	it = current_row.end();
}

void csv::set_delimiter(std::string s) {
	delimiter = s;
}

int csv::fetch_next_row(std::vector<std::string> &dest) {
	if(fetch_row() == new_line) {
		dest = current_row;
		it = current_row.end();
		
		return next_row;
	}
	return nothing_to_read;
}

int csv::fetch_next_entry(std::string &dest) {
	if(it == current_row.end()) {
		if(fetch_row() == nothing_to_read)
			return nothing_to_read;
	}
	
	dest = *it;
	it++;
	return next_column;
}
	
void csv::close() {
	fs.close();
}


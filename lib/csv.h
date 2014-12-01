#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>
#include <fstream>

class csv {
		std::ifstream fs;
		
		std::vector<std::string>::iterator it;
		std::vector<std::string> current_row;
		std::string delimiter;
		
		int fetch_row();
	public:
		void open(std::string fn);
		
		void set_delimiter(std::string s);

		int fetch_next_row(std::vector<std::string> &dest);
		int fetch_next_entry(std::string &dest);
	
		void close();
		
		static const int next_column = 0;
		static const int next_row = 1;
		static const int new_line = 2;
		static const int nothing_to_read = 3;
};

#endif


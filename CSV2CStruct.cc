#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ios>

int main(int argc, char* argv[])
{
	if (argc!=2) {
	  std::cerr << "usage: CSV2CStruct csv-file" << std::endl;
	}
	
	std::ifstream ifile(argv[1]);
	
	std::string line;
	size_t pos;
	while (getline(ifile, line)) {
		if (line[0]=='#') continue;
		
		pos = line.rfind('\r');
		if (pos!=std::string::npos) {
			line.erase(pos, 1);
		}
		
		std::cout << "  { " << std::flush;
		
		std::string token;
		pos = line.find(',');
		while (pos!=std::string::npos) {
			token = line.substr(0, pos);
			line.erase(0, pos+1);
			
			std::cout.width(10);
			std::cout << token << ", " << std::flush;
			
			pos = line.find(',');
		}
		token = line.substr(0);
		
		std::cout << token << " }," << std::endl;
	}
}
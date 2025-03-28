#include <iostream>
#include <filesystem>
#include <vector>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>

extern "C" {
#include <unistd.h>
#include <sys/syscall.h>
}

bool is_digit(const std::string& str) {
	for (const auto& s : str) {
		if (!isdigit(s))
			return false;
	}
	return true;
}

uint64_t minElem(const std::vector<uint64_t>& vec) {
	uint64_t min = vec.at(0);

	for (const auto& i : vec) {
		if(min > i)
			min = i;
	}
	return min;
}


void ps() {
	std::cout << '[';
	std::filesystem::path procpath{"/proc"};
	std::string pid;
	bool first {true};
	for (const std::filesystem::directory_entry& dir_entry : std::filesystem::directory_iterator(procpath)){
		if (is_digit(dir_entry.path().filename())) {
			pid = dir_entry.path().filename();

			std::filesystem::path exe{"/proc/" + pid + "/exe"};
			std::filesystem::path cwd{"/proc/" + pid + "/cwd"};
			std::filesystem::path base_address{"/proc/" + pid + "/map_files"};
			
			//bool first {true};
			try {	
			if (std::filesystem::exists(exe) && std::filesystem::exists(cwd)) {
				if(first)
					first = false;
				else
					std::cout << ',';

				std::cout << "\n{" << "\n\"pid\":" << pid << ','
				       	<< "\n\"exe\":" << std::filesystem::read_symlink(exe) << ','
					<< "\n\"cwd\":" << std::filesystem::read_symlink(cwd) << ',';

				std::vector<uint64_t> list_of_addresses;

				for (const std::filesystem::directory_entry& base_dir_entry : std::filesystem::directory_iterator(base_address)) {
					std::string entry_name = base_dir_entry.path().filename();
					std::string dir_address = entry_name.substr(0, entry_name.find('-'));

					uint64_t num;
					std::stringstream str;

					str << std::hex << dir_address;
					str >> num;
					list_of_addresses.push_back(num);
				}	
										
					std::cout << "\n\"base_address\":" << minElem(list_of_addresses) << ',';

					std::ifstream status{"/proc/" + pid + "/status"};

					std::vector<std::string> state;
					for(std::string s; getline(status, s);)
						state.push_back(s);

					char s1{state.at(2).at(7)};

					std::cout << "\n\"state\":" << "\"" << s1 << "\",";

					std::ifstream cmdline{"/proc/" + pid + "/cmdline"};

					std::vector<std::string> cmdline_list;

					for (std::string s; getline(cmdline, s);)
						cmdline_list.push_back(s);

					std::string line{cmdline_list.at(0)};
					std::vector<std::string> cmd_vector;
					std::string cmd;

					for (size_t i{0}; i < line.size(); ++i) {
						if (line.at(i) != '\0')
							cmd += line.at(i);
						else {
							cmd_vector.push_back(cmd);
							cmd = "";
						}
					}

					std::cout << "\n\"cmdline\":[";

					for (size_t i{0}; i < cmd_vector.size(); ++i) {
						std::cout << "\n\"" << cmd_vector.at(i) << "\"";

						if (i != cmd_vector.size()-1)
							std::cout << ',';
					}

					std::cout << "\n]";
					std::cout << "\n}";
				}
			}
		catch (const std::exception& e) {
			continue;
		}
		}
	}
	std::cout << ']';
}

int main() {
	ps();
	return 0;
}

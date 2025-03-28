#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

bool num (const std::string& str) {
	for (const auto& i : str) {
		if (!isdigit(i))
			return false;
	}
	return true;
}

std::string func (std::string str) {
	std::vector<std::string> vec;
	std::ifstream status("/proc/" + str + "/status");
	for (std::string i; getline(status, i); )
		vec.push_back(i);
	std::string s = vec.at(0).substr(6);
	return s;
}

int main(int argc, char* argv[]) {
	if (argc <= 1)
		return 1;

	std::string name = argv[1];
	std::string new_name;
	
	const std::filesystem::path procpath {"/proc"};

	for (const std::filesystem::directory_entry& dir_entry : std::filesystem::directory_iterator(procpath)) {
		if (num(dir_entry.path().filename())== true) {
			new_name = func(dir_entry.path().filename());
			
			if (!(new_name.find(name) == std::string::npos))
				kill(std::stoi(dir_entry.path().filename()), 9);
		}
	}
	return 0;
}

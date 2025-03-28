#include <iostream>
#include <filesystem>
#include <string>
#include <map>
#include <fstream>
#include <vector>
bool is_digit(const std::string str);
bool parent_exists(std::string id);
std::string parent(std::string id);
std::string getName(std::string id);
void parentFormat(std::string id, std::map<std::string, std::vector<std::string>> children);
void childFormat(std::string id, std::map<std::string, std::vector<std::string>> children);

bool is_digit(const std::string str) {
	for (const auto& s : str) {
		if (!isdigit(s))
			return false;
	}
	return true;
}

bool parent_exists(std::string id) {
	std::ifstream stat{"/proc/" + id + "/status"};

	std::vector <std::string> vec;

	for (std::string l; getline(stat, l);)
		vec.push_back(l);

	std::string parent_pid { vec.at(6).substr(6) };

	if (parent_pid != "0")
		return false;

	return true;
}

std::string parent(std::string id) {
	std::ifstream stat  {"/proc/" + id + "/status" };
	std::string parent_pid;
	std::vector<std::string> vec;

	for (std::string l; getline(stat, l);)
		vec.push_back(l);

	parent_pid = vec.at(6).substr(6);

	return parent_pid;
}

void parentFormat(std::string id, std::map<std::string, std::vector<std::string>> children) {
	std::cout << '{' << "\"pid\":" << id << ','
		<< "\"name\":" << "\"" << getName(id) << "\"" << ',';

	childFormat(id, children);
	std::cout << '}';
}

void childFormat(std::string id, std::map<std::string, std::vector<std::string>> children) {

	bool first { true };
	if(children.count(id) == 0) {
		std::cout << "\"children\":[]";
		return;
	}
	
	std::vector<std::string> tmp = children[id];
	std::cout << "\"children\":[";
	for (std::string child : tmp) {
		if(first)
			first = false;
		else
			std::cout << ',';
		parentFormat(child, children);
	}

	std::cout << ']';
}

std::string getName(std::string id) {
	std::ifstream stat { "/proc/" + id + "/status" };
	std::vector<std::string> vec;
	std::string name;

	for (std::string l; getline(stat, l);)
		vec.push_back(l);

	name = vec.at(0).substr(6);

	return name;
}




int main() {
	std::filesystem::path pathproc{"/proc"};
	std::string pid;
	bool first {true};

	std::vector<std::string> parents_vec;
	std::vector<std::string> children_vec;

	std::map<std::string, std::vector<std::string>> children_map;

	for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator{pathproc}) {
		
		if (is_digit(file.path().filename())) {
				pid = file.path().filename();

				if (parent_exists(pid))
					parents_vec.push_back(pid);
				else
					children_map[parent(pid)].push_back(pid);
		}
	}
	
	std::cout << '[';
	for (const auto& i : parents_vec) {
		pid = i;
		if (first)
			first = false;
		else
			std::cout << ',';

		parentFormat(pid, children_map);
	}
	std::cout << "]\n";

	return 0;
}

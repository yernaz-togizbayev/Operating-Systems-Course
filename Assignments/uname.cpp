#include <iostream>
extern "C" {
	#include <errno.h>
	#include <sys/utsname.h>
}

int main() {
	struct utsname info;

	errno = 0;
	if (uname(&info) < errno) {
		perror("uname");
		exit(EXIT_FAILURE);
	}

	std::cout << "OS: " << info.sysname;
	std::cout << "\nHostname: " << info.nodename;
	std::cout << "\nRelease: " << info.release;
	std::cout << "\nVersion: " << info.version;

	return 0;
}

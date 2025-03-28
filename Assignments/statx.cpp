#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

void ownership(std::string& str) {
	const size_t id = STATX_UID + STATX_GID;
	struct statx my_statx;
	statx(AT_FDCWD, str.c_str(), AT_SYMLINK_NOFOLLOW, STATX_MODE, &my_statx);
	
	if (my_statx.stx_mode&S_IRUSR)
		std::cout << 'r';
	else
		std::cout << '-';

	if (my_statx.stx_mode&S_IWUSR)
		std::cout << 'w';
	else
		std::cout << '-';

	if (my_statx.stx_mode&S_IXUSR)
		std::cout << 'x';
	else
		std::cout << '-';
	

	
	if (my_statx.stx_mode&S_IRGRP)
		std::cout << 'r';
	else
		std::cout << '-';

	if (my_statx.stx_mode&S_IWGRP)
		std::cout << 'w';
	else
		std::cout << '-';

	if (my_statx.stx_mode&S_IXGRP)
		std::cout << 'x';
	else
		std::cout << '-';



	if (my_statx.stx_mode&S_IROTH)
		std::cout << 'r';
	else
		std::cout << '-';

	if (my_statx.stx_mode&S_IWOTH)
		std::cout << 'w';
	else
		std::cout << '-';

	if (my_statx.stx_mode&S_IXOTH)
		std::cout << 'x';
	else
		std::cout << '-';

	
	statx(AT_FDCWD, str.c_str(), AT_SYMLINK_NOFOLLOW, id, &my_statx);
	std::cout << "\nUID: " << my_statx.stx_uid << ", GID: " << my_statx.stx_gid << '\n';

	statx(AT_FDCWD, str.c_str(), AT_SYMLINK_NOFOLLOW, STATX_SIZE, &my_statx);
	std::cout << "Size: " << my_statx.stx_size << '\n';
}

int main(int argc, char* argv[1]) {
	if (argc <= 1)
		return 1;
	
	std::string str = argv[1];
	
	ownership(str);
	
	return 0;
}

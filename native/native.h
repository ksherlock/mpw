#ifndef __native_h__
#define __native_h__

#include <string>
#include <cstdint>
#include <ctime>

#include <macos/errors.h>

namespace native {

	using MacOS::macos_error;

	struct file_info {

		enum { none, file, directory } type = none;

		uint16_t attributes = 0;
		uint32_t create_date = 0;
		uint32_t modify_date = 0;
		uint32_t backup_date = 0;

		/* for files */
		uint32_t data_logical_size = 0;
		uint32_t data_physical_size = 0;
		uint32_t resource_logical_size = 0;
		uint32_t resource_physical_size = 0;
		unsigned char finder_info[32] = {};

		/* for directories */
		uint32_t entry_count = 0; // 
	};

	macos_error get_file_info(const std::string &path_name, file_info &fi);
	macos_error set_file_info(const std::string &path_name, const file_info &fi);

	macos_error get_finder_info(const std::string &path_name, void *buffer, bool extended = true);
	macos_error get_finder_info(const std::string &path_name, uint32_t &ftype, uint32_t &ctype);

	macos_error set_finder_info(const std::string &path_name, const void *buffer, bool extended = true);
	macos_error set_finder_info(const std::string &path_name, uint32_t ftype, uint32_t ctype);

	uint32_t unix_to_mac(time_t t);
	time_t mac_to_unix(uint32_t t);


	bool is_text_file(const std::string &path_name);
	bool is_binary_file(const std::string &path_name);



	int open_fork(const std::string &path_name, int fork, int oflag, int perm = 0666);
	int open_resource_fork(const std::string &path_name, int oflag);

}

#endif

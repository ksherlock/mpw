#ifndef __native_internal_h__
#define __native_internal_h__

#include <string>
#include <cstdint>

#include "native.h"

namespace native {

	/* use filename only */
	bool is_text_file_internal(const std::string &path_name);
	bool is_binary_file_internal(const std::string &path_name);

	void prodos_ftype_in(uint8_t *buffer);
	void prodos_ftype_out(uint8_t *buffer);

	void synthesize_finder_info(const std::string &path_name, uint8_t *buffer, bool extended);

}

#endif


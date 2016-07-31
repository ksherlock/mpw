#ifndef __native_internal_h__
#define __native_internal_h__

#include "native.h"

namespace native {

	/* use filename only */
	bool is_text_file_internal(const std::string &path_name);
	bool is_binary_file_internal(const std::string &path_name);

}

#endif


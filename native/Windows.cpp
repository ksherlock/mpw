/*
 * Copyright (c) 2016, Kelvin W Sherlock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 

#include <filesystem>
#include <string>

#include <Windows.h>
#include <Shlobj.h>

namespace fs = std::experimental::filesystem;

namespace native {

 	static fs::path FindRoot() {
		fs::path p;
		std::error_code ec;

		PWSTR tmp = nullptr;
		auto ok = SHGetKnownFolderPath(
			FOLDERID_Profile,
			0,
			nullptr,
			&tmp
		);
		if ((ok == S_OK) && (tmp != nullptr)) {
			p = tmp;
			p /= "mpw";
			if (fs::is_directory(p, ec)) return p;
		}

		p = "c:\\mpw\\";
		if (!fs::is_directory(p, ec)) {
			fprintf(stderr, "Warning: %ls does not exist\n", p.c_str());
		}
		return p;
	}
	fs::path RootPath() {
		// todo -- do it.
		static fs::path root;
		if (root.empty()) {
			root = FindRoot();
		}
		return root;
	}


}
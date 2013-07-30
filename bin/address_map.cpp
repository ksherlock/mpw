/*
 * Copyright (c) 2013, Kelvin W Sherlock
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

#include "address_map.h"

AddressMap::AddressMap()
{
}
AddressMap::~AddressMap()
{
}

void AddressMap::add(uint32_t address)
{
	unsigned page = address >> 12;
	if (page >= pageMap.size()) return;
	pageMap[page]++;

	map.emplace(address, 0);
}

void AddressMap::remove(uint32_t address)
{
	unsigned page = address >> 12;
	if (page >= pageMap.size()) return;

	// erase(key) returns # of elements erased.
	pageMap[page] -= map.erase(address);
}

bool AddressMap::lookup(uint32_t address)
{
	unsigned page = address >> 12;
	if (page >= pageMap.size()) return false;
	if (!pageMap[page]) return false;

	return map.find(address) != map.end();
}

void AddressMap::clear()
{
	pageMap.fill(0);
	map.clear();
}


ToolMap::ToolMap()
{
}

ToolMap::~ToolMap()
{
}

void ToolMap::clear()
{
	map.clear();
}

bool ToolMap::lookup(uint16_t tool)
{
	if ((tool & 0xf000) != 0xa000) return false;
	return map.find(tool) != map.end();
}

void ToolMap::add(uint16_t tool)
{
	if ((tool & 0xf000) != 0xa000) return;

	// return std::pair<iter, bool>
	map.emplace(tool, 0);
}

void ToolMap::remove(uint16_t tool)
{
	if ((tool & 0xf000) != 0xa000) return;

	map.erase(tool);
}




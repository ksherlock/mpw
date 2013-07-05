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




#ifndef __address_map__
#define __address_map__

#include <unordered_map>
#include <array>

class AddressMap
{
public:
	AddressMap();
	~AddressMap();

	void add(uint32_t address);
	void remove(uint32_t address);
	bool lookup(uint32_t address);

	void clear();

	std::unordered_map<uint32_t, unsigned>::iterator begin()
	{
		return map.begin();
	}

	std::unordered_map<uint32_t, unsigned>::iterator end()
	{
		return map.end();
	}

	size_t size()
	{
		return map.size();
	}

private:
	std::array<unsigned, 4096> pageMap;
	std::unordered_map<uint32_t, unsigned> map;
};

class ToolMap
{
public:

	ToolMap();
	~ToolMap();

	void add(uint16_t tool);
	void remove(uint16_t tool);
	bool lookup(uint16_t tool);

	void clear();

	std::unordered_map<uint32_t, unsigned>::iterator begin()
	{
		return map.begin();
	}

	std::unordered_map<uint32_t, unsigned>::iterator end()
	{
		return map.end();
	}
	
	size_t size()
	{
		return map.size();
	}

private:
	std::unordered_map<uint32_t, unsigned> map;

};

#endif
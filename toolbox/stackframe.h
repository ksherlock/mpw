#ifndef __StackFrame_h__
#define __StackFrame_h__

#include <cstdint>
#include <utility>

// 

template<unsigned N>
void ToolReturn(uint32_t sp, uint32_t value)
{
	if (sp == -1) sp = cpuGetAReg(7);
	static_assert(N == 2 || N == 4, "Invalid Return Size");
	if (N == 4)
	{
		memoryWriteLong(value, sp);
	}
	if (N == 2)
	{
		memoryWriteWord(value, sp);
	}
}

// pre-define these templates to prevent instantiation errors.
template<int Bytes, int Offset, typename... Args>
uint32_t StackFrame__(uint32_t sp, uint32_t &x, Args&&... args);
template<int Bytes, int Offset, typename... Args>
uint32_t StackFrame__(uint32_t sp, uint16_t &x, Args&&... args);
template<int Bytes, int Offset, typename... Args>
uint32_t StackFrame__(uint32_t sp, uint8_t &x, Args&&... args);

template<int Bytes, int Offset>
uint32_t StackFrame__(uint32_t sp);


template<int Bytes, int Offset>
uint32_t StackFrame__(uint32_t sp)
{
	static_assert(Offset == 0, "Invalid Stack Size");

	cpuSetAReg(7, sp + Bytes);
	return sp + Bytes;
}

template<int Bytes, int Offset, typename... Args>
uint32_t StackFrame__(uint32_t sp, uint32_t &x, Args&&... args)
{
	x = memoryReadLong(sp + Offset - 4);

	return StackFrame__<Bytes, Offset - 4>(sp, std::forward<Args>(args)...);
}

template<int Bytes, int Offset, typename... Args>
uint32_t StackFrame__(uint32_t sp, uint16_t &x, Args&&... args)
{
	x = memoryReadWord(sp + Offset - 2);

	return StackFrame__<Bytes, Offset - 2>(sp, std::forward<Args>(args)...);
}


template<int Bytes, int Offset, typename... Args>
uint32_t StackFrame__(uint32_t sp, uint8_t &x, Args&&... args)
{
	// byte pushes as 2 bytes with 1 garbage byte
	x = memoryReadByte(sp + Offset - 2);

	return StackFrame__<Bytes, Offset - 2>(sp, std::forward<Args>(args)...);
}

template<int Bytes, typename... Args>
uint32_t StackFrame(Args&&... args)
{
	uint32_t sp = cpuGetAReg(7);

	return StackFrame__<Bytes, Bytes>(sp, std::forward<Args>(args)...);
}



#endif
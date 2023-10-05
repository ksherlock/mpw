#pragma once

static void ADD_D000(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADD_D010(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D018(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D020(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void ADD_D028(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D030(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D038(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D039(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D03A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D03B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D03C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D040(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADD_D048(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADD_D050(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D058(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D060(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void ADD_D068(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D070(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D078(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D079(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D07A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D07B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D07C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D080(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D088(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D090(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D098(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D0A0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D0A8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADD_D0B0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADD_D0B8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADD_D0B9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void ADD_D0BA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADD_D0BB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADD_D0BC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D110(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D118(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D120(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADD_D128(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D130(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADD_D138(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D139(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D150(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D158(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D160(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADD_D168(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D170(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADD_D178(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D179(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D190(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D198(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D1A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADD_D1A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADD_D1B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void ADD_D1B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADD_D1B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDA_D0C0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetDRegWord(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D0C8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetARegWord(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D0D0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADDA_D0D8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADDA_D0E0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADDA_D0E8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D0F0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D0F8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA70());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D0F9(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA71());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADDA_D0FA(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA72());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D0FB(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA73());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D0FC(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetNextWord();
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADDA_D1C0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D1C8(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D1D0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADDA_D1D8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADDA_D1E0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D1E8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D1F0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADDA_D1F8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D1F9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void ADDA_D1FA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D1FB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADDA_D1FC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDI_0600(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDI_0610(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0618(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0620(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDI_0628(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0630(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADDI_0638(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0639(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDI_0640(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDI_0650(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0658(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0660(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDI_0668(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0670(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADDI_0678(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0679(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDI_0680(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void ADDI_0690(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDI_0698(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDI_06A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ADDI_06A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ADDI_06B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void ADDI_06B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ADDI_06B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void ADDQ_5000(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void ADDQ_5010(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5018(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5020(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADDQ_5028(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5030(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDQ_5038(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5039(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_5040(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void ADDQ_5050(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5058(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5060(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADDQ_5068(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5070(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDQ_5078(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5079(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_5080(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDQ_5090(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_5098(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_50A0(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADDQ_50A8(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDQ_50B0(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void ADDQ_50B8(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDQ_50B9(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDQ_5048(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dst = cpuGetAReg(opc_data[0]);
	dst = cpuAddaW(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDQ_5088(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dst = cpuGetAReg(opc_data[0]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void AND_C000(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void AND_C010(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C018(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C020(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void AND_C028(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C030(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C038(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C039(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C03A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C03B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C03C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C040(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void AND_C050(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C058(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C060(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void AND_C068(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C070(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C078(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C079(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C07A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C07B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C07C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C080(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C090(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C098(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C0A0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C0A8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void AND_C0B0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void AND_C0B8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void AND_C0B9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void AND_C0BA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void AND_C0BB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void AND_C0BC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C110(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C118(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C120(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void AND_C128(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C130(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void AND_C138(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C139(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C150(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C158(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C160(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void AND_C168(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C170(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void AND_C178(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C179(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C190(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C198(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C1A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void AND_C1A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void AND_C1B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void AND_C1B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void AND_C1B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ANDI_0200(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ANDI_0210(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0218(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0220(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ANDI_0228(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0230(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ANDI_0238(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0239(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ANDI_0240(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ANDI_0250(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0258(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0260(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ANDI_0268(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0270(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ANDI_0278(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0279(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ANDI_0280(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(14);
}
static void ANDI_0290(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ANDI_0298(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ANDI_02A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ANDI_02A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ANDI_02B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void ANDI_02B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ANDI_02B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void ANDI_023C(uint32_t*opc_data)
{
	cpuAndCcrB();
}
static void ANDI_027C(uint32_t*opc_data)
{
	cpuAndSrW();
}
static void EOR_B100(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuEorB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void EOR_B110(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B118(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B120(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void EOR_B128(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B130(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void EOR_B138(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B139(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EOR_B140(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuEorW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void EOR_B150(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B158(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B160(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void EOR_B168(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B170(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void EOR_B178(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B179(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EOR_B180(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuEorL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void EOR_B190(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EOR_B198(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EOR_B1A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void EOR_B1A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void EOR_B1B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void EOR_B1B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void EOR_B1B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void EORI_0A00(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuEorB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void EORI_0A10(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A18(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A20(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void EORI_0A28(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A30(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void EORI_0A38(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A39(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void EORI_0A40(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuEorW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void EORI_0A50(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A58(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A60(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void EORI_0A68(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A70(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void EORI_0A78(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A79(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void EORI_0A80(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuEorL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void EORI_0A90(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void EORI_0A98(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void EORI_0AA0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void EORI_0AA8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void EORI_0AB0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void EORI_0AB8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void EORI_0AB9(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void EORI_0A3C(uint32_t*opc_data)
{
	cpuEorCcrB();
}
static void EORI_0A7C(uint32_t*opc_data)
{
	cpuEorSrW();
}
static void OR_8000(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void OR_8010(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8018(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8020(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void OR_8028(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8030(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_8038(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8039(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_803A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_803B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_803C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8040(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void OR_8050(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8058(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8060(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void OR_8068(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8070(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_8078(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8079(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_807A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_807B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_807C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8080(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8090(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_8098(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_80A0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_80A8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void OR_80B0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void OR_80B8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void OR_80B9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void OR_80BA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void OR_80BB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void OR_80BC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_8110(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8118(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8120(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void OR_8128(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8130(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void OR_8138(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8139(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_8150(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8158(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8160(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void OR_8168(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8170(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void OR_8178(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8179(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_8190(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_8198(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_81A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void OR_81A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void OR_81B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void OR_81B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void OR_81B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ORI_0000(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ORI_0010(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0018(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0020(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ORI_0028(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0030(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ORI_0038(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0039(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ORI_0040(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ORI_0050(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0058(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0060(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ORI_0068(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0070(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ORI_0078(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0079(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ORI_0080(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void ORI_0090(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ORI_0098(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ORI_00A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ORI_00A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ORI_00B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void ORI_00B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ORI_00B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void ORI_003C(uint32_t*opc_data)
{
	cpuOrCcrB();
}
static void ORI_007C(uint32_t*opc_data)
{
	cpuOrSrW();
}
static void SUB_9000(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUB_9010(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9018(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9020(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void SUB_9028(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9030(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_9038(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9039(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_903A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_903B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_903C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9040(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUB_9048(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUB_9050(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9058(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9060(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void SUB_9068(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9070(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_9078(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9079(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_907A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_907B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_907C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9080(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9088(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9090(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_9098(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_90A0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_90A8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUB_90B0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUB_90B8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUB_90B9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void SUB_90BA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUB_90BB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUB_90BC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_9110(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9118(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9120(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUB_9128(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9130(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUB_9138(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9139(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_9150(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9158(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9160(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUB_9168(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9170(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUB_9178(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9179(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_9190(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_9198(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_91A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUB_91A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUB_91B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void SUB_91B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUB_91B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBA_90C0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetDRegWord(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_90C8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetARegWord(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_90D0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUBA_90D8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUBA_90E0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUBA_90E8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_90F0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_90F8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA70());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_90F9(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA71());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUBA_90FA(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA72());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_90FB(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA73());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_90FC(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetNextWord();
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUBA_91C0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_91C8(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_91D0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUBA_91D8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUBA_91E0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_91E8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_91F0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUBA_91F8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_91F9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void SUBA_91FA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_91FB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUBA_91FC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBI_0400(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBI_0410(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0418(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0420(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBI_0428(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0430(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUBI_0438(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0439(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBI_0440(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBI_0450(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0458(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0460(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBI_0468(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0470(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUBI_0478(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0479(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBI_0480(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void SUBI_0490(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBI_0498(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBI_04A0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void SUBI_04A8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void SUBI_04B0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void SUBI_04B8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void SUBI_04B9(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void SUBQ_5100(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void SUBQ_5110(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5118(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5120(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUBQ_5128(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5130(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBQ_5138(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5139(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)opc_data[1];
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_5140(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void SUBQ_5150(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5158(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5160(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUBQ_5168(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5170(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBQ_5178(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5179(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_5180(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBQ_5190(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_5198(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_51A0(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUBQ_51A8(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBQ_51B0(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void SUBQ_51B8(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBQ_51B9(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBQ_5148(uint32_t*opc_data)
{
	uint16_t src = (uint16_t)opc_data[1];
	uint32_t dst = cpuGetAReg(opc_data[0]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBQ_5188(uint32_t*opc_data)
{
	uint32_t src = opc_data[1];
	uint32_t dst = cpuGetAReg(opc_data[0]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void CHK_4180(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 10);
}
static void CHK_4190(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 14);
}
static void CHK_4198(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 14);
}
static void CHK_41A0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 16);
}
static void CHK_41A8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 18);
}
static void CHK_41B0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 20);
}
static void CHK_41B8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 18);
}
static void CHK_41B9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 22);
}
static void CHK_41BA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 18);
}
static void CHK_41BB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 20);
}
static void CHK_41BC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src, 14);
}
static void CHK_4100(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 10);
}
static void CHK_4110(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 18);
}
static void CHK_4118(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 18);
}
static void CHK_4120(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 20);
}
static void CHK_4128(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 22);
}
static void CHK_4130(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 24);
}
static void CHK_4138(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 22);
}
static void CHK_4139(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 26);
}
static void CHK_413A(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 22);
}
static void CHK_413B(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 24);
}
static void CHK_413C(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src, 18);
}
static void CMP_B000(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(4);
}
static void CMP_B010(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B018(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B020(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(10);
}
static void CMP_B028(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B030(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B038(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B039(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMP_B03A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B03B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B03C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B040(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(4);
}
static void CMP_B048(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(4);
}
static void CMP_B050(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B058(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B060(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(10);
}
static void CMP_B068(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B070(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B078(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B079(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMP_B07A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B07B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B07C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B080(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMP_B088(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMP_B090(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B098(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B0A0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMP_B0A8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMP_B0B0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMP_B0B8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMP_B0B9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(22);
}
static void CMP_B0BA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMP_B0BB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMP_B0BC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0C0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetDRegWord(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B0C8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetARegWord(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B0D0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(10);
}
static void CMPA_B0D8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(10);
}
static void CMPA_B0E0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPA_B0E8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0F0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPA_B0F8(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA70());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0F9(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA71());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B0FA(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA72());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0FB(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)memoryReadWord(cpuEA73());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPA_B0FC(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)(int32_t)(int16_t)cpuGetNextWord();
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(10);
}
static void CMPA_B1C0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B1C8(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B1D0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B1D8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B1E0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPA_B1E8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B1F0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPA_B1F8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B1F9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(22);
}
static void CMPA_B1FA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B1FB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPA_B1FC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C00(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMPI_0C10(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C18(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C20(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C28(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C30(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0C38(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C39(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0C40(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMPI_0C50(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C58(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C60(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C68(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C70(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0C78(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C79(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0C80(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C90(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0C98(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0CA0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(22);
}
static void CMPI_0CA8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(24);
}
static void CMPI_0CB0(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(26);
}
static void CMPI_0CB8(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(24);
}
static void CMPI_0CB9(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(28);
}
static void CMPI_0C3A(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C3B(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	uint8_t dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0C7A(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C7B(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	uint16_t dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0CBA(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA72();
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(24);
}
static void CMPI_0CBB(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA73();
	uint32_t dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(26);
}
static void BCHG_0150(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCHG_0158(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCHG_0160(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BCHG_0168(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCHG_0170(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCHG_0178(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCHG_0179(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCHG_0140(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuBchgL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6 + ((src >= 16) ? 2 : 0));
}
static void BCHG_0850(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCHG_0858(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCHG_0860(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCHG_0868(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCHG_0870(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void BCHG_0878(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCHG_0879(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void BCHG_0840(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)cpuGetNextWord();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuBchgL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(10);
}
static void BCLR_0190(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCLR_0198(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCLR_01A0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BCLR_01A8(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_01B0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCLR_01B8(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_01B9(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCLR_0180(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuBclrL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(8 + ((src >= 16) ? 2 : 0));
}
static void BCLR_0890(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_0898(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_08A0(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCLR_08A8(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCLR_08B0(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void BCLR_08B8(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCLR_08B9(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void BCLR_0880(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)cpuGetNextWord();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuBclrL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(12);
}
static void BSET_01D0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BSET_01D8(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BSET_01E0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BSET_01E8(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BSET_01F0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BSET_01F8(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BSET_01F9(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BSET_01C0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuBsetL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6 + ((src >= 16) ? 2 : 0));
}
static void BSET_08D0(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BSET_08D8(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BSET_08E0(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BSET_08E8(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BSET_08F0(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void BSET_08F8(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BSET_08F9(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void BSET_08C0(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)cpuGetNextWord();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuBsetL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(10);
}
static void BTST_0110(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(8);
}
static void BTST_0118(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(8);
}
static void BTST_0120(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(10);
}
static void BTST_0128(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_0130(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_0138(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_0139(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(16);
}
static void BTST_013A(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA72();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_013B(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint32_t dstea = cpuEA73();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_013C(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[1]);
	uint8_t dst = (uint8_t)cpuGetNextWord();
	cpuBtstB(dst, src);
	cpuSetInstructionTime(8);
}
static void BTST_0100(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[1]);
	uint32_t dst = cpuGetDReg(opc_data[0]);
	cpuBtstL(dst, src);
	cpuSetInstructionTime(6);
}
static void BTST_0810(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_0818(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_0820(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_0828(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(16);
}
static void BTST_0830(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(18);
}
static void BTST_0838(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(16);
}
static void BTST_0839(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(20);
}
static void BTST_083A(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(16);
}
static void BTST_083B(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	uint8_t dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(18);
}
static void BTST_0800(uint32_t*opc_data)
{
	uint32_t src = (uint32_t)cpuGetNextWord();
	uint32_t dst = cpuGetDReg(opc_data[0]);
	cpuBtstL(dst, src);
	cpuSetInstructionTime(10);
}
static void LEA_41D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(4);
}
static void LEA_41E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(8);
}
static void LEA_41F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(12);
}
static void LEA_41F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(8);
}
static void LEA_41F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(12);
}
static void LEA_41FA(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA72();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(8);
}
static void LEA_41FB(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA73();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(12);
}
static void MULS_C1C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 0);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 6);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 12);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMulsW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 0);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 6);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 12);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	uint32_t res = cpuMuluW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void DIVS_81C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVL_4C40(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = cpuGetDReg(opc_data[0]);
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C50(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C58(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C60(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C68(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C70(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C78(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA70());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C79(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA71());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C7A(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA72());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C7B(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA73());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C7C(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = cpuGetNextLong();
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVU_80C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void MOVEM_48A0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	cpuMovemwPre(regs, opc_data[0]);
}
static void MOVEM_48E0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	cpuMovemlPre(regs, opc_data[0]);
}
static void MOVEM_4C98(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	cpuMovemwPost(regs, opc_data[0]);
}
static void MOVEM_4CD8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	cpuMovemlPost(regs, opc_data[0]);
}
static void MOVEM_4890(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48A8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48B0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48B8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48B9(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48D0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48E8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48F0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48F8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48F9(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_4C90(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CA8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CB0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CB8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CB9(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CBA(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CBB(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CD0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CE8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CF0(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CF8(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CF9(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CFA(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CFB(uint32_t*opc_data)
{
	uint16_t regs = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void CLR_4200(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	cpuClr();
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void CLR_4210(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	uint32_t dstea = cpuEA02(opc_data[0]);
	memoryReadByte(dstea);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4218(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	memoryReadByte(dstea);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4220(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	memoryReadByte(dstea);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void CLR_4228(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	uint32_t dstea = cpuEA05(opc_data[0]);
	memoryReadByte(dstea);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4230(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	uint32_t dstea = cpuEA06(opc_data[0]);
	memoryReadByte(dstea);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void CLR_4238(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	uint32_t dstea = cpuEA70();
	memoryReadByte(dstea);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4239(uint32_t*opc_data)
{
	const uint8_t dst = 0;
	uint32_t dstea = cpuEA71();
	memoryReadByte(dstea);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_4240(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	cpuClr();
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void CLR_4250(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	uint32_t dstea = cpuEA02(opc_data[0]);
	memoryReadWord(dstea);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4258(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	memoryReadWord(dstea);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4260(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	memoryReadWord(dstea);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void CLR_4268(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	uint32_t dstea = cpuEA05(opc_data[0]);
	memoryReadWord(dstea);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4270(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	uint32_t dstea = cpuEA06(opc_data[0]);
	memoryReadWord(dstea);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void CLR_4278(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	uint32_t dstea = cpuEA70();
	memoryReadWord(dstea);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4279(uint32_t*opc_data)
{
	const uint16_t dst = 0;
	uint32_t dstea = cpuEA71();
	memoryReadWord(dstea);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_4280(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	cpuClr();
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void CLR_4290(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	uint32_t dstea = cpuEA02(opc_data[0]);
	memoryReadLong(dstea);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_4298(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	memoryReadLong(dstea);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_42A0(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	memoryReadLong(dstea);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void CLR_42A8(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	uint32_t dstea = cpuEA05(opc_data[0]);
	memoryReadLong(dstea);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void CLR_42B0(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	uint32_t dstea = cpuEA06(opc_data[0]);
	memoryReadLong(dstea);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void CLR_42B8(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	uint32_t dstea = cpuEA70();
	memoryReadLong(dstea);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void CLR_42B9(uint32_t*opc_data)
{
	const uint32_t dst = 0;
	uint32_t dstea = cpuEA71();
	memoryReadLong(dstea);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void BFCHG_EAD0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfChgEa(dstea, ext);
}
static void BFCLR_ECD0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfClrEa(dstea, ext);
}
static void BFEXTS_EBD0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBFA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBFB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTU_E9D0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9E8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9F0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9F8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9F9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9FA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9FB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuBfExtuEa(dstea, ext);
}
static void BFFFO_EDD0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDFA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDFB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuBfFfoEa(dstea, ext);
}
static void BFINS_EFD0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfInsEa(dstea, ext);
}
static void BFSET_EED0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfSetEa(dstea, ext);
}
static void BFTST_E8D0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8E8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8F0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8F8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8F9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8FA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8FB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuBfTstEa(dstea, ext);
}
static void BFCHG_EAC0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfChgReg(opc_data[0], ext);
}
static void BFCLR_ECC0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfClrReg(opc_data[0], ext);
}
static void BFEXTS_EBC0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfExtsReg(opc_data[0], ext);
}
static void BFEXTU_E9C0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfExtuReg(opc_data[0], ext);
}
static void BFFFO_EDC0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfFfoReg(opc_data[0], ext);
}
static void BFINS_EFC0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfInsReg(opc_data[0], ext);
}
static void BFSET_EEC0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfSetReg(opc_data[0], ext);
}
static void BFTST_E8C0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	cpuBfTstReg(opc_data[0], ext);
}
static void MULL_4C00(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = cpuGetDReg(opc_data[0]);
	cpuMulL(src, ext);
}
static void MULL_4C10(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuMulL(src, ext);
}
static void MULL_4C18(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuMulL(src, ext);
}
static void MULL_4C20(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuMulL(src, ext);
}
static void MULL_4C28(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuMulL(src, ext);
}
static void MULL_4C30(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuMulL(src, ext);
}
static void MULL_4C38(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA70());
	cpuMulL(src, ext);
}
static void MULL_4C39(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA71());
	cpuMulL(src, ext);
}
static void MULL_4C3A(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA72());
	cpuMulL(src, ext);
}
static void MULL_4C3B(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA73());
	cpuMulL(src, ext);
}
static void MULL_4C3C(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = cpuGetNextLong();
	cpuMulL(src, ext);
}
static void MOVES_0E10(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	cpuMoveSB(src, ext);
}
static void MOVES_0E18(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	cpuMoveSB(src, ext);
}
static void MOVES_0E20(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	cpuMoveSB(src, ext);
}
static void MOVES_0E28(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	cpuMoveSB(src, ext);
}
static void MOVES_0E30(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	cpuMoveSB(src, ext);
}
static void MOVES_0E38(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint8_t src = memoryReadByte(cpuEA70());
	cpuMoveSB(src, ext);
}
static void MOVES_0E39(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint8_t src = memoryReadByte(cpuEA71());
	cpuMoveSB(src, ext);
}
static void MOVES_0E50(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveSW(src, ext);
}
static void MOVES_0E58(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveSW(src, ext);
}
static void MOVES_0E60(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveSW(src, ext);
}
static void MOVES_0E68(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveSW(src, ext);
}
static void MOVES_0E70(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveSW(src, ext);
}
static void MOVES_0E78(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint16_t src = memoryReadWord(cpuEA70());
	cpuMoveSW(src, ext);
}
static void MOVES_0E79(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint16_t src = memoryReadWord(cpuEA71());
	cpuMoveSW(src, ext);
}
static void MOVES_0E90(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuMoveSL(src, ext);
}
static void MOVES_0E98(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuMoveSL(src, ext);
}
static void MOVES_0EA0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuMoveSL(src, ext);
}
static void MOVES_0EA8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuMoveSL(src, ext);
}
static void MOVES_0EB0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuMoveSL(src, ext);
}
static void MOVES_0EB8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA70());
	cpuMoveSL(src, ext);
}
static void MOVES_0EB9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t src = memoryReadLong(cpuEA71());
	cpuMoveSL(src, ext);
}
static void NBCD_4800(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNbcdB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NBCD_4810(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NBCD_4818(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NBCD_4820(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NBCD_4828(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NBCD_4830(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NBCD_4838(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NBCD_4839(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4400(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNegB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEG_4410(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4418(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4420(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEG_4428(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4430(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEG_4438(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4439(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4440(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuNegW(dst);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEG_4450(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4458(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4460(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEG_4468(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4470(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEG_4478(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4479(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4480(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuNegL(dst);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NEG_4490(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4498(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_44A0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void NEG_44A8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEG_44B0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void NEG_44B8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEG_44B9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void NEGX_4000(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNegxB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEGX_4010(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4018(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4020(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEGX_4028(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4030(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEGX_4038(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4039(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_4040(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuNegxW(dst);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEGX_4050(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4058(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4060(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEGX_4068(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4070(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEGX_4078(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4079(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_4080(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuNegxL(dst);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NEGX_4090(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_4098(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_40A0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void NEGX_40A8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEGX_40B0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void NEGX_40B8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEGX_40B9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void NOT_4600(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNotB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NOT_4610(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4618(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4620(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NOT_4628(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4630(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NOT_4638(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4639(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_4640(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuNotW(dst);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NOT_4650(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4658(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4660(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NOT_4668(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4670(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NOT_4678(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4679(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_4680(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuNotL(dst);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NOT_4690(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_4698(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_46A0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void NOT_46A8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NOT_46B0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void NOT_46B8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NOT_46B9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void TAS_4AC0(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuTas(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void TAS_4AD0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void TAS_4AD8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void TAS_4AE0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void TAS_4AE8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void TAS_4AF0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void TAS_4AF8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void TAS_4AF9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void TST_4A00(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	cpuTestB(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A10(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A18(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A20(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(10);
}
static void TST_4A28(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A30(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A38(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A39(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(16);
}
static void TST_4A40(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	cpuTestW(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A50(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A58(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A60(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(10);
}
static void TST_4A68(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A70(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A78(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A79(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(16);
}
static void TST_4A80(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	cpuTestL(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A90(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A98(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(12);
}
static void TST_4AA0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(14);
}
static void TST_4AA8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(16);
}
static void TST_4AB0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(18);
}
static void TST_4AB8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(16);
}
static void TST_4AB9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(20);
}
static void TST_4A3A(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA72();
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A3B(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA73();
	uint8_t dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A3C(uint32_t*opc_data)
{
	uint8_t dst = (uint8_t)cpuGetNextWord();
	cpuTestB(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A48(uint32_t*opc_data)
{
	uint16_t dst = (uint16_t)cpuGetAReg(opc_data[0]);
	cpuTestW(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A7A(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA72();
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A7B(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA73();
	uint16_t dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A7C(uint32_t*opc_data)
{
	uint16_t dst = cpuGetNextWord();
	cpuTestW(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A88(uint32_t*opc_data)
{
	uint32_t dst = cpuGetAReg(opc_data[0]);
	cpuTestL(dst);
	cpuSetInstructionTime(4);
}
static void TST_4ABA(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA72();
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(16);
}
static void TST_4ABB(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA73();
	uint32_t dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(18);
}
static void TST_4ABC(uint32_t*opc_data)
{
	uint32_t dst = cpuGetNextLong();
	cpuTestL(dst);
	cpuSetInstructionTime(12);
}
static void PEA_4850(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuPeaL(dstea);
	cpuSetInstructionTime(12);
}
static void PEA_4868(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuPeaL(dstea);
	cpuSetInstructionTime(16);
}
static void PEA_4870(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuPeaL(dstea);
	cpuSetInstructionTime(20);
}
static void PEA_4878(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	cpuPeaL(dstea);
	cpuSetInstructionTime(16);
}
static void PEA_4879(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	cpuPeaL(dstea);
	cpuSetInstructionTime(20);
}
static void PEA_487A(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA72();
	cpuPeaL(dstea);
	cpuSetInstructionTime(16);
}
static void PEA_487B(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA73();
	cpuPeaL(dstea);
	cpuSetInstructionTime(20);
}
static void JMP_4ED0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuJmp(dstea);
	cpuSetInstructionTime(8);
}
static void JMP_4EE8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuJmp(dstea);
	cpuSetInstructionTime(10);
}
static void JMP_4EF0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuJmp(dstea);
	cpuSetInstructionTime(14);
}
static void JMP_4EF8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	cpuJmp(dstea);
	cpuSetInstructionTime(10);
}
static void JMP_4EF9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	cpuJmp(dstea);
	cpuSetInstructionTime(12);
}
static void JMP_4EFA(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA72();
	cpuJmp(dstea);
	cpuSetInstructionTime(10);
}
static void JMP_4EFB(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA73();
	cpuJmp(dstea);
	cpuSetInstructionTime(14);
}
static void JSR_4E90(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuJsr(dstea);
	cpuSetInstructionTime(16);
}
static void JSR_4EA8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuJsr(dstea);
	cpuSetInstructionTime(18);
}
static void JSR_4EB0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuJsr(dstea);
	cpuSetInstructionTime(22);
}
static void JSR_4EB8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	cpuJsr(dstea);
	cpuSetInstructionTime(18);
}
static void JSR_4EB9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	cpuJsr(dstea);
	cpuSetInstructionTime(20);
}
static void JSR_4EBA(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA72();
	cpuJsr(dstea);
	cpuSetInstructionTime(18);
}
static void JSR_4EBB(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA73();
	cpuJsr(dstea);
	cpuSetInstructionTime(22);
}
static void MOVETOSR_46C0(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	cpuMoveToSr(src);
	cpuSetInstructionTime(12);
}
static void MOVETOSR_46D0(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveToSr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOSR_46D8(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveToSr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOSR_46E0(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveToSr(src);
	cpuSetInstructionTime(18);
}
static void MOVETOSR_46E8(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveToSr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOSR_46F0(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveToSr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOSR_46F8(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA70());
	cpuMoveToSr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOSR_46F9(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA71());
	cpuMoveToSr(src);
	cpuSetInstructionTime(24);
}
static void MOVETOSR_46FA(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA72());
	cpuMoveToSr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOSR_46FB(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = memoryReadWord(cpuEA73());
	cpuMoveToSr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOSR_46FC(uint32_t*opc_data)
{
	if (!cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t src = cpuGetNextWord();
	cpuMoveToSr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOCCR_44C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	cpuMoveToCcr(src);
	cpuSetInstructionTime(12);
}
static void MOVETOCCR_44D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOCCR_44D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOCCR_44E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(18);
}
static void MOVETOCCR_44E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOCCR_44F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOCCR_44F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOCCR_44F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(24);
}
static void MOVETOCCR_44FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOCCR_44FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOCCR_44FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	cpuMoveToCcr(src);
	cpuSetInstructionTime(16);
}
static void SCC_50C0(uint32_t*opc_data)
{
	uint8_t dst = cpuScc(opc_data[1]);
	cpuSetDRegByte(opc_data[0], dst);
	if (dst == 0)
	{
	cpuSetInstructionTime(4);
	}
	else
	{
	cpuSetInstructionTime(6);
	}
}
static void SCC_50D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint8_t dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SCC_50D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	uint8_t dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SCC_50E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	uint8_t dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SCC_50E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint8_t dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SCC_50F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint8_t dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SCC_50F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint8_t dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SCC_50F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint8_t dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void MOVEFROMCCR_42C0(uint32_t*opc_data)
{
	uint16_t dst = cpuMoveFromCcr();
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void MOVEFROMCCR_42D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMCCR_42D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMCCR_42E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void MOVEFROMCCR_42E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMCCR_42F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void MOVEFROMCCR_42F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMCCR_42F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void MOVEFROMSR_40C0(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint16_t dst = cpuMoveFromSr();
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void MOVEFROMSR_40D0(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMSR_40D8(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMSR_40E0(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void MOVEFROMSR_40E8(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMSR_40F0(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void MOVEFROMSR_40F8(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint32_t dstea = cpuEA70();
	uint16_t dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMSR_40F9(uint32_t*opc_data)
{
	if (cpuGetModelMajor() > 0 && !cpuGetFlagSupervisor())
	{
		cpuThrowPrivilegeViolationException();
		return;
	}
	uint32_t dstea = cpuEA71();
	uint16_t dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CAS_0AD0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuCasB(dstea, ext);
}
static void CAS_0AD8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 1);
	cpuCasB(dstea, ext);
}
static void CAS_0AE0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 1);
	cpuCasB(dstea, ext);
}
static void CAS_0AE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuCasB(dstea, ext);
}
static void CAS_0AF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuCasB(dstea, ext);
}
static void CAS_0AF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuCasB(dstea, ext);
}
static void CAS_0AF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuCasB(dstea, ext);
}
static void CAS_0CD0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuCasW(dstea, ext);
}
static void CAS_0CD8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	cpuCasW(dstea, ext);
}
static void CAS_0CE0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	cpuCasW(dstea, ext);
}
static void CAS_0CE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuCasW(dstea, ext);
}
static void CAS_0CF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuCasW(dstea, ext);
}
static void CAS_0CF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuCasW(dstea, ext);
}
static void CAS_0CF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuCasW(dstea, ext);
}
static void CAS_0ED0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuCasL(dstea, ext);
}
static void CAS_0ED8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[0], 4);
	cpuCasL(dstea, ext);
}
static void CAS_0EE0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[0], 4);
	cpuCasL(dstea, ext);
}
static void CAS_0EE8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuCasL(dstea, ext);
}
static void CAS_0EF0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuCasL(dstea, ext);
}
static void CAS_0EF8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuCasL(dstea, ext);
}
static void CAS_0EF9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuCasL(dstea, ext);
}
static void CHKCMP2_00D0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00E8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00F0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00F8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00F9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00FA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00FB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_02D0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02E8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02F0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02F8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02F9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02FA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02FB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_04D0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04E8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04F0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04F8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04F9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04FA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04FB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuChkCmp2L(dstea, ext);
}
static void CALLM_06D0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuCallm(dstea, ext);
}
static void CALLM_06E8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuCallm(dstea, ext);
}
static void CALLM_06F0(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuCallm(dstea, ext);
}
static void CALLM_06F8(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuCallm(dstea, ext);
}
static void CALLM_06F9(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuCallm(dstea, ext);
}
static void CALLM_06FA(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA72();
	cpuCallm(dstea, ext);
}
static void CALLM_06FB(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA73();
	cpuCallm(dstea, ext);
}
static void PFLUSH030_F010(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[0]);
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F028(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[0]);
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F030(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[0]);
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F038(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F039(uint32_t*opc_data)
{
	uint16_t ext = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuPflush030(dstea, ext);
}
static void MOVEQ_7000(uint32_t*opc_data)
{
	cpuSetDReg(opc_data[0], opc_data[1]);
	cpuSetFlagsAbs((uint16_t)opc_data[2]);
	cpuSetInstructionTime(4);
}
static void MOVE_1000(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_1010(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_1018(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_1020(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(10);
}
static void MOVE_1028(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_1030(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_1038(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_1039(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_103A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_103B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_103C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_1080(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_1090(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1098(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10A0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_10A8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10B0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10B8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10B9(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_10BA(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10BB(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10BC(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10C0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_10D0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10D8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10E0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_10E8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10F0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10F8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10F9(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_10FA(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10FB(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10FC(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1100(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_1110(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1118(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1120(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_1128(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1130(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_1138(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1139(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_113A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_113B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_113C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1140(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1150(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1158(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1160(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_1168(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_1170(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_1178(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_1179(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_117A(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_117B(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_117C(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1180(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_1190(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_1198(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_11A0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11A8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11B0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_11B8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11B9(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_11BA(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11BB(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_11BC(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_11C0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_11D0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_11D8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_11E0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_11E8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11F0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11F8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11F9(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_11FA(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11FB(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11FC(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_13C0(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_13D0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_13D8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA03(opc_data[0],1));
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_13E0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_13E8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_13F0(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_13F8(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA70());
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_13F9(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA71());
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_13FA(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA72());
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_13FB(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA73());
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_13FC(uint32_t*opc_data)
{
	uint8_t src = (uint8_t)cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_3000(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_3008(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_3010(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_3018(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_3020(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(10);
}
static void MOVE_3028(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_3030(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_3038(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_3039(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_303A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_303B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_303C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_3080(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3088(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3090(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3098(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30A0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_30A8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30B0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30B8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30B9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_30BA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30BB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30BC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_30C8(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_30D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_30E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_30FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3100(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3108(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3110(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3118(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3120(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_3128(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3130(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_3138(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3139(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_313A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_313B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_313C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3140(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3148(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3150(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3158(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3160(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_3168(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_3170(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_3178(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_3179(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_317A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_317B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_317C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3180(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_3188(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_3190(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_3198(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_31A0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31A8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31B0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_31B8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31B9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_31BA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31BB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_31BC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_31C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_31C8(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_31D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_31D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_31E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_31E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_31FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_33C0(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_33C8(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_33D0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_33D8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_33E0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_33E8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_33F0(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_33F8(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_33F9(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_33FA(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_33FB(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_33FC(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	uint32_t dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2000(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_2008(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_2010(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_2018(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_2020(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_2028(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_2030(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVE_2038(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_2039(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(20);
}
static void MOVE_203A(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_203B(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVE_203C(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_2080(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2088(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2090(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2098(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20A0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_20A8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20B0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20B8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20B9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_20BA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20BB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20BC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20C0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_20C8(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_20D0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20D8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20E0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_20E8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20F0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20F8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20F9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_20FA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20FB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20FC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2100(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2108(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2110(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2118(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2120(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_2128(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2130(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_2138(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2139(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_213A(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_213B(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_213C(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2140(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_2148(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_2150(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2158(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2160(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_2168(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_2170(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_2178(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_2179(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_217A(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_217B(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_217C(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2180(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_2188(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_2190(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_2198(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_21A0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21A8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21B0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_21B8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21B9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(34);
}
static void MOVE_21BA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21BB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_21BC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_21C0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_21C8(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_21D0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_21D8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_21E0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_21E8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21F0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21F8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21F9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_21FA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21FB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21FC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_23C0(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_23C8(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_23D0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_23D8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_23E0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_23E8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_23F0(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(34);
}
static void MOVE_23F8(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_23F9(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(36);
}
static void MOVE_23FA(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_23FB(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(34);
}
static void MOVE_23FC(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	uint32_t dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVEA_3040(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(4);
}
static void MOVEA_3048(uint32_t*opc_data)
{
	uint16_t src = cpuGetARegWord(opc_data[0]);
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(4);
}
static void MOVEA_3050(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(8);
}
static void MOVEA_3058(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(8);
}
static void MOVEA_3060(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(10);
}
static void MOVEA_3068(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(12);
}
static void MOVEA_3070(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(14);
}
static void MOVEA_3078(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA70());
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(12);
}
static void MOVEA_3079(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA71());
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(16);
}
static void MOVEA_307A(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA72());
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(12);
}
static void MOVEA_307B(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA73());
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(14);
}
static void MOVEA_307C(uint32_t*opc_data)
{
	uint16_t src = cpuGetNextWord();
	cpuSetAReg(opc_data[1], (uint32_t)(int32_t)(int16_t)src);
	cpuSetInstructionTime(8);
}
static void MOVEA_2040(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVEA_2048(uint32_t*opc_data)
{
	uint32_t src = cpuGetAReg(opc_data[0]);
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVEA_2050(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVEA_2058(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVEA_2060(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVEA_2068(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVEA_2070(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVEA_2078(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA70());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVEA_2079(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA71());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(20);
}
static void MOVEA_207A(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA72());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVEA_207B(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA73());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVEA_207C(uint32_t*opc_data)
{
	uint32_t src = cpuGetNextLong();
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void BCCB_6200(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode2(), opc_data[1]);
}
static void BCCB_6300(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode3(), opc_data[1]);
}
static void BCCB_6400(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode4(), opc_data[1]);
}
static void BCCB_6500(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode5(), opc_data[1]);
}
static void BCCB_6600(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode6(), opc_data[1]);
}
static void BCCB_6700(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode7(), opc_data[1]);
}
static void BCCB_6800(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode8(), opc_data[1]);
}
static void BCCB_6900(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode9(), opc_data[1]);
}
static void BCCB_6A00(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode10(), opc_data[1]);
}
static void BCCB_6B00(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode11(), opc_data[1]);
}
static void BCCB_6C00(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode12(), opc_data[1]);
}
static void BCCB_6D00(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode13(), opc_data[1]);
}
static void BCCB_6E00(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode14(), opc_data[1]);
}
static void BCCB_6F00(uint32_t*opc_data)
{
	cpuBccB(cpuCalculateConditionCode15(), opc_data[1]);
}
static void BCCW_6200(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode2());
}
static void BCCW_6300(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode3());
}
static void BCCW_6400(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode4());
}
static void BCCW_6500(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode5());
}
static void BCCW_6600(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode6());
}
static void BCCW_6700(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode7());
}
static void BCCW_6800(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode8());
}
static void BCCW_6900(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode9());
}
static void BCCW_6A00(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode10());
}
static void BCCW_6B00(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode11());
}
static void BCCW_6C00(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode12());
}
static void BCCW_6D00(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode13());
}
static void BCCW_6E00(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode14());
}
static void BCCW_6F00(uint32_t*opc_data)
{
	cpuBccW(cpuCalculateConditionCode15());
}
static void BCCL_62FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode2());
}
static void BCCL_63FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode3());
}
static void BCCL_64FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode4());
}
static void BCCL_65FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode5());
}
static void BCCL_66FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode6());
}
static void BCCL_67FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode7());
}
static void BCCL_68FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode8());
}
static void BCCL_69FF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode9());
}
static void BCCL_6AFF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode10());
}
static void BCCL_6BFF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode11());
}
static void BCCL_6CFF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode12());
}
static void BCCL_6DFF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode13());
}
static void BCCL_6EFF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode14());
}
static void BCCL_6FFF(uint32_t*opc_data)
{
	cpuBccL(cpuCalculateConditionCode15());
}
static void BKPT_4848(uint32_t*opc_data)
{
	cpuBkpt(opc_data[0]);
}
static void EXG_C140(uint32_t*opc_data)
{
	cpuExgDD(opc_data[0], opc_data[1]);
}
static void EXG_C148(uint32_t*opc_data)
{
	cpuExgAA(opc_data[0], opc_data[1]);
}
static void EXG_C188(uint32_t*opc_data)
{
	cpuExgDA(opc_data[0], opc_data[1]);
}
static void EXT_4880(uint32_t*opc_data)
{
	cpuExtW(opc_data[0]);
}
static void EXT_48C0(uint32_t*opc_data)
{
	cpuExtL(opc_data[0]);
}
static void EXT_49C0(uint32_t*opc_data)
{
	cpuExtBL(opc_data[0]);
}
static void SWAP_4840(uint32_t*opc_data)
{
	cpuSwap(opc_data[0]);
}
static void LINK_4E50(uint32_t*opc_data)
{
	cpuLinkW(opc_data[0]);
}
static void LINK_4808(uint32_t*opc_data)
{
	cpuLinkL(opc_data[0]);
}
static void UNLK_4E58(uint32_t*opc_data)
{
	cpuUnlk(opc_data[0]);
}
static void BRAB_6000(uint32_t*opc_data)
{
	cpuBraB(opc_data[1]);
}
static void BRAW_6000(uint32_t*opc_data)
{
	cpuBraW();
}
static void BRAL_60FF(uint32_t*opc_data)
{
	cpuBraL();
}
static void BSRB_6100(uint32_t*opc_data)
{
	cpuBsrB(opc_data[1]);
}
static void BSRW_6100(uint32_t*opc_data)
{
	cpuBsrW();
}
static void BSRL_61FF(uint32_t*opc_data)
{
	cpuBsrL();
}
static void DBCC_50C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode0(), opc_data[1]);
}
static void DBCC_51C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode1(), opc_data[1]);
}
static void DBCC_52C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode2(), opc_data[1]);
}
static void DBCC_53C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode3(), opc_data[1]);
}
static void DBCC_54C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode4(), opc_data[1]);
}
static void DBCC_55C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode5(), opc_data[1]);
}
static void DBCC_56C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode6(), opc_data[1]);
}
static void DBCC_57C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode7(), opc_data[1]);
}
static void DBCC_58C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode8(), opc_data[1]);
}
static void DBCC_59C8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode9(), opc_data[1]);
}
static void DBCC_5AC8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode10(), opc_data[1]);
}
static void DBCC_5BC8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode11(), opc_data[1]);
}
static void DBCC_5CC8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode12(), opc_data[1]);
}
static void DBCC_5DC8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode13(), opc_data[1]);
}
static void DBCC_5EC8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode14(), opc_data[1]);
}
static void DBCC_5FC8(uint32_t*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode15(), opc_data[1]);
}
static void TRAPCC_50FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode0());
}
static void TRAPCC_51FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode1());
}
static void TRAPCC_52FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode2());
}
static void TRAPCC_53FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode3());
}
static void TRAPCC_54FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode4());
}
static void TRAPCC_55FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode5());
}
static void TRAPCC_56FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode6());
}
static void TRAPCC_57FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode7());
}
static void TRAPCC_58FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode8());
}
static void TRAPCC_59FC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode9());
}
static void TRAPCC_5AFC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode10());
}
static void TRAPCC_5BFC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode11());
}
static void TRAPCC_5CFC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode12());
}
static void TRAPCC_5DFC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode13());
}
static void TRAPCC_5EFC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode14());
}
static void TRAPCC_5FFC(uint32_t*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode15());
}
static void TRAPCC_50FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode0());
}
static void TRAPCC_51FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode1());
}
static void TRAPCC_52FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode2());
}
static void TRAPCC_53FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode3());
}
static void TRAPCC_54FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode4());
}
static void TRAPCC_55FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode5());
}
static void TRAPCC_56FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode6());
}
static void TRAPCC_57FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode7());
}
static void TRAPCC_58FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode8());
}
static void TRAPCC_59FA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode9());
}
static void TRAPCC_5AFA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode10());
}
static void TRAPCC_5BFA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode11());
}
static void TRAPCC_5CFA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode12());
}
static void TRAPCC_5DFA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode13());
}
static void TRAPCC_5EFA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode14());
}
static void TRAPCC_5FFA(uint32_t*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode15());
}
static void TRAPCC_50FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode0());
}
static void TRAPCC_51FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode1());
}
static void TRAPCC_52FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode2());
}
static void TRAPCC_53FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode3());
}
static void TRAPCC_54FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode4());
}
static void TRAPCC_55FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode5());
}
static void TRAPCC_56FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode6());
}
static void TRAPCC_57FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode7());
}
static void TRAPCC_58FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode8());
}
static void TRAPCC_59FB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode9());
}
static void TRAPCC_5AFB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode10());
}
static void TRAPCC_5BFB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode11());
}
static void TRAPCC_5CFB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode12());
}
static void TRAPCC_5DFB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode13());
}
static void TRAPCC_5EFB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode14());
}
static void TRAPCC_5FFB(uint32_t*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode15());
}
static void RTD_4E74(uint32_t*opc_data)
{
	cpuRtd();
}
static void RTE_4E73(uint32_t*opc_data)
{
	cpuRte();
}
static void RTS_4E75(uint32_t*opc_data)
{
	cpuRts();
}
static void RTR_4E77(uint32_t*opc_data)
{
	cpuRtr();
}
static void NOP_4E71(uint32_t*opc_data)
{
	cpuNop();
}
static void MOVEC_4E7A(uint32_t*opc_data)
{
	cpuMoveCFrom();
}
static void MOVEC_4E7B(uint32_t*opc_data)
{
	cpuMoveCTo();
}
static void CAS2_0CFC(uint32_t*opc_data)
{
	cpuCas2W();
}
static void CAS2_0EFC(uint32_t*opc_data)
{
	cpuCas2L();
}
static void TRAP_4E40(uint32_t*opc_data)
{
	cpuTrap(opc_data[0]);
}
static void TRAPV_4E76(uint32_t*opc_data)
{
	cpuTrapv();
}
static void STOP_4E72(uint32_t*opc_data)
{
	cpuStop(cpuGetNextWord());
}
static void RESET_4E70(uint32_t*opc_data)
{
	cpuReset();
}
static void MOVEUSP_4E60(uint32_t*opc_data)
{
	cpuMoveToUsp(opc_data[0]);
}
static void MOVEUSP_4E68(uint32_t*opc_data)
{
	cpuMoveFromUsp(opc_data[0]);
}
static void CMPM_B108(uint32_t*opc_data)
{
	cpuCmpMB(opc_data[0], opc_data[1]);
}
static void CMPM_B148(uint32_t*opc_data)
{
	cpuCmpMW(opc_data[0], opc_data[1]);
}
static void CMPM_B188(uint32_t*opc_data)
{
	cpuCmpML(opc_data[0], opc_data[1]);
}
static void RTM_06C0(uint32_t*opc_data)
{
	cpuRtm(opc_data[0], opc_data[1]);
}
static void PFLUSH040_F500(uint32_t*opc_data)
{
	cpuPflush040(opc_data[0], opc_data[1]);
}
static void PTEST040_F548(uint32_t*opc_data)
{
	cpuPtest040(opc_data[0], opc_data[1]);
}
static void ADDX_D100(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddXB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADDX_D140(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddXW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADDX_D180(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddXL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDX_D108(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAddXB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDX_D148(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAddXW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDX_D188(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuAddXL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void SUBX_9100(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubXB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUBX_9140(uint32_t*opc_data)
{
	uint16_t src = cpuGetDRegWord(opc_data[0]);
	uint16_t dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubXW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUBX_9180(uint32_t*opc_data)
{
	uint32_t src = cpuGetDReg(opc_data[0]);
	uint32_t dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubXL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBX_9108(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSubXB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBX_9148(uint32_t*opc_data)
{
	uint16_t src = memoryReadWord(cpuEA04(opc_data[0],2));
	uint32_t dstea = cpuEA04(opc_data[1], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuSubXW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBX_9188(uint32_t*opc_data)
{
	uint32_t src = memoryReadLong(cpuEA04(opc_data[0],4));
	uint32_t dstea = cpuEA04(opc_data[1], 4);
	uint32_t dst = memoryReadLong(dstea);
	dst = cpuSubXL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ABCD_C100(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAbcdB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(6);
}
static void ABCD_C108(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuAbcdB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SBCD_8100(uint32_t*opc_data)
{
	uint8_t src = cpuGetDRegByte(opc_data[0]);
	uint8_t dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSbcdB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(6);
}
static void SBCD_8108(uint32_t*opc_data)
{
	uint8_t src = memoryReadByte(cpuEA04(opc_data[0],1));
	uint32_t dstea = cpuEA04(opc_data[1], 1);
	uint8_t dst = memoryReadByte(dstea);
	dst = cpuSbcdB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void LSL_E108(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLslB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSL_E148(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLslW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSL_E188(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuLslL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSL_E128(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLslB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSL_E168(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLslW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSL_E1A8(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuLslL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSL_E3D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E008(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLsrB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSR_E048(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLsrW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSR_E088(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuLsrL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSR_E028(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLsrB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSR_E068(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLsrW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSR_E0A8(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuLsrL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSR_E2D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E100(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAslB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASL_E140(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAslW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASL_E180(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuAslL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASL_E120(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAslB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASL_E160(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAslW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASL_E1A0(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuAslL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASL_E1D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E000(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAsrB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASR_E040(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAsrW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASR_E080(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuAsrL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASR_E020(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAsrB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASR_E060(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAsrW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASR_E0A0(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuAsrL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASR_E0D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E118(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRolB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROL_E158(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRolW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROL_E198(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRolL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROL_E138(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRolB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROL_E178(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRolW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROL_E1B8(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRolL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROL_E7D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E018(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRorB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROR_E058(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRorW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROR_E098(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRorL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROR_E038(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRorB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROR_E078(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRorW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROR_E0B8(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRorL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROR_E6D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E110(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxlB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXL_E150(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxlW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXL_E190(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxlL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXL_E130(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxlB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXL_E170(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxlW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXL_E1B0(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxlL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXL_E5D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E010(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxrB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXR_E050(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxrW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXR_E090(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxrL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXR_E030(uint32_t*opc_data)
{
	uint8_t dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxrB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXR_E070(uint32_t*opc_data)
{
	uint16_t dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxrW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXR_E0B0(uint32_t*opc_data)
{
	uint32_t dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxrL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXR_E4D0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA02(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4D8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA03(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4E0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA04(opc_data[0], 2);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4E8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA05(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4F0(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA06(opc_data[0]);
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4F8(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA70();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4F9(uint32_t*opc_data)
{
	uint32_t dstea = cpuEA71();
	uint16_t dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void MOVEP_0188(uint32_t*opc_data)
{
	cpuMovepWEa(opc_data[1], opc_data[0]);
}
static void MOVEP_01C8(uint32_t*opc_data)
{
	cpuMovepLEa(opc_data[1], opc_data[0]);
}
static void MOVEP_0108(uint32_t*opc_data)
{
	cpuMovepWReg(opc_data[1], opc_data[0]);
}
static void MOVEP_0148(uint32_t*opc_data)
{
	cpuMovepLReg(opc_data[1], opc_data[0]);
}
static void PACK_8140(uint32_t*opc_data)
{
	cpuPackReg(opc_data[1], opc_data[0]);
}
static void PACK_8148(uint32_t*opc_data)
{
	cpuPackEa(opc_data[1], opc_data[0]);
}
static void UNPK_8180(uint32_t*opc_data)
{
	cpuUnpkReg(opc_data[1], opc_data[0]);
}
static void UNPK_8188(uint32_t*opc_data)
{
	cpuUnpkEa(opc_data[1], opc_data[0]);
}

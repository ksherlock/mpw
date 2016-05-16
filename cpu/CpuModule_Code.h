#ifndef CPUMODULE_CODE_H
#define CPUMODULE_CODE_H

static void ADD_D000(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADD_D010(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D018(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D020(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void ADD_D028(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D030(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D038(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D039(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D03A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D03B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D03C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D040(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADD_D048(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADD_D050(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D058(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D060(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void ADD_D068(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D070(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D078(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D079(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D07A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADD_D07B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D07C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D080(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D088(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADD_D090(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D098(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADD_D0A0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D0A8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADD_D0B0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADD_D0B8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADD_D0B9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void ADD_D0BA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADD_D0BB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADD_D0BC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADD_D110(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D118(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D120(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADD_D128(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D130(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADD_D138(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D139(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D150(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D158(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADD_D160(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADD_D168(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D170(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADD_D178(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADD_D179(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D190(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D198(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADD_D1A0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADD_D1A8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADD_D1B0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void ADD_D1B8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADD_D1B9(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDA_D0C0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetDRegWord(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D0C8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetARegWord(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D0D0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA02(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADDA_D0D8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADDA_D0E0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADDA_D0E8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA05(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D0F0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA06(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D0F8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA70());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D0F9(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA71());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADDA_D0FA(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA72());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D0FB(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA73());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D0FC(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetNextWord();
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void ADDA_D1C0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D1C8(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDA_D1D0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADDA_D1D8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void ADDA_D1E0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDA_D1E8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D1F0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADDA_D1F8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D1F9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void ADDA_D1FA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void ADDA_D1FB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void ADDA_D1FC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void ADDI_0600(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDI_0610(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0618(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0620(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDI_0628(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0630(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADDI_0638(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0639(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDI_0640(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDI_0650(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0658(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDI_0660(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDI_0668(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0670(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADDI_0678(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDI_0679(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDI_0680(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void ADDI_0690(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDI_0698(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDI_06A0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ADDI_06A8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ADDI_06B0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void ADDI_06B8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ADDI_06B9(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void ADDQ_5000(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAddB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void ADDQ_5010(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5018(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5020(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADDQ_5028(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5030(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDQ_5038(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5039(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_5040(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAddW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void ADDQ_5050(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5058(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void ADDQ_5060(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void ADDQ_5068(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5070(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDQ_5078(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ADDQ_5079(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_5080(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuAddL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDQ_5090(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_5098(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ADDQ_50A0(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ADDQ_50A8(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDQ_50B0(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void ADDQ_50B8(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ADDQ_50B9(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ADDQ_5048(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dst = cpuGetAReg(opc_data[0]);
	dst = cpuAddaW(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ADDQ_5088(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dst = cpuGetAReg(opc_data[0]);
	dst = cpuAddaL(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void AND_C000(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void AND_C010(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C018(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C020(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void AND_C028(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C030(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C038(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C039(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C03A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C03B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C03C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C040(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void AND_C050(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C058(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C060(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void AND_C068(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C070(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C078(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C079(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C07A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void AND_C07B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C07C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C080(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void AND_C090(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C098(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void AND_C0A0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C0A8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void AND_C0B0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void AND_C0B8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void AND_C0B9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void AND_C0BA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void AND_C0BB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void AND_C0BC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void AND_C110(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C118(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C120(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void AND_C128(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C130(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void AND_C138(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C139(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C150(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C158(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void AND_C160(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void AND_C168(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C170(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void AND_C178(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void AND_C179(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C190(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C198(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void AND_C1A0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void AND_C1A8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void AND_C1B0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void AND_C1B8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void AND_C1B9(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ANDI_0200(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAndB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ANDI_0210(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0218(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0220(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ANDI_0228(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0230(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ANDI_0238(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0239(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuAndB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ANDI_0240(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAndW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ANDI_0250(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0258(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ANDI_0260(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ANDI_0268(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0270(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ANDI_0278(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ANDI_0279(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAndW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ANDI_0280(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuAndL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(14);
}
static void ANDI_0290(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ANDI_0298(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ANDI_02A0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ANDI_02A8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ANDI_02B0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void ANDI_02B8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ANDI_02B9(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuAndL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void ANDI_023C(ULO*opc_data)
{
	cpuAndCcrB();
	cpuSetInstructionTime(20);
}
static void ANDI_027C(ULO*opc_data)
{
	cpuAndSrW();
	cpuSetInstructionTime(20);
}
static void EOR_B100(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuEorB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void EOR_B110(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(8);
}
static void EOR_B118(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(8);
}
static void EOR_B120(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(10);
}
static void EOR_B128(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B130(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void EOR_B138(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B139(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B140(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuEorW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void EOR_B150(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(8);
}
static void EOR_B158(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(8);
}
static void EOR_B160(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(10);
}
static void EOR_B168(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B170(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void EOR_B178(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void EOR_B179(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B180(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuEorL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void EOR_B190(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B198(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EOR_B1A0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(18);
}
static void EOR_B1A8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EOR_B1B0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void EOR_B1B8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EOR_B1B9(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void EORI_0A00(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuEorB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void EORI_0A10(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A18(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A20(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void EORI_0A28(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A30(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void EORI_0A38(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A39(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuEorB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void EORI_0A40(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuEorW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void EORI_0A50(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A58(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void EORI_0A60(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void EORI_0A68(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A70(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void EORI_0A78(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void EORI_0A79(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuEorW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void EORI_0A80(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuEorL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void EORI_0A90(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void EORI_0A98(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void EORI_0AA0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void EORI_0AA8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void EORI_0AB0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void EORI_0AB8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void EORI_0AB9(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuEorL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void EORI_0A3C(ULO*opc_data)
{
	cpuEorCcrB();
	cpuSetInstructionTime(20);
}
static void EORI_0A7C(ULO*opc_data)
{
	cpuEorSrW();
	cpuSetInstructionTime(20);
}
static void OR_8000(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void OR_8010(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8018(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8020(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void OR_8028(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8030(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_8038(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8039(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_803A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_803B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_803C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8040(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void OR_8050(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8058(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8060(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void OR_8068(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8070(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_8078(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_8079(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_807A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void OR_807B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_807C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8080(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void OR_8090(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_8098(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void OR_80A0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_80A8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void OR_80B0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void OR_80B8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void OR_80B9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void OR_80BA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void OR_80BB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void OR_80BC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void OR_8110(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8118(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8120(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void OR_8128(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8130(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void OR_8138(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8139(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_8150(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8158(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void OR_8160(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void OR_8168(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8170(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void OR_8178(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void OR_8179(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_8190(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_8198(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void OR_81A0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void OR_81A8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void OR_81B0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void OR_81B8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void OR_81B9(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ORI_0000(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuOrB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ORI_0010(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0018(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0020(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ORI_0028(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0030(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ORI_0038(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0039(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuOrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ORI_0040(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuOrW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void ORI_0050(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0058(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void ORI_0060(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ORI_0068(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0070(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void ORI_0078(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void ORI_0079(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuOrW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void ORI_0080(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuOrL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void ORI_0090(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ORI_0098(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void ORI_00A0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ORI_00A8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ORI_00B0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void ORI_00B8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void ORI_00B9(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuOrL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void ORI_003C(ULO*opc_data)
{
	cpuOrCcrB();
	cpuSetInstructionTime(20);
}
static void ORI_007C(ULO*opc_data)
{
	cpuOrSrW();
	cpuSetInstructionTime(20);
}
static void SUB_9000(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUB_9010(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9018(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9020(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void SUB_9028(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9030(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_9038(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9039(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_903A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_903B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_903C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9040(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUB_9048(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUB_9050(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9058(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9060(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(10);
}
static void SUB_9068(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9070(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_9078(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_9079(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_907A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUB_907B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_907C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9080(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9088(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUB_9090(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_9098(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUB_90A0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_90A8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUB_90B0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUB_90B8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUB_90B9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void SUB_90BA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUB_90BB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUB_90BC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUB_9110(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9118(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9120(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUB_9128(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9130(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUB_9138(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9139(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_9150(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9158(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUB_9160(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUB_9168(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9170(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUB_9178(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUB_9179(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[1]);
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_9190(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_9198(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUB_91A0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUB_91A8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUB_91B0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void SUB_91B8(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUB_91B9(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBA_90C0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetDRegWord(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_90C8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetARegWord(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_90D0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA02(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUBA_90D8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUBA_90E0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUBA_90E8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA05(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_90F0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA06(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_90F8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA70());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_90F9(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA71());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUBA_90FA(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA72());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_90FB(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA73());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_90FC(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetNextWord();
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(12);
}
static void SUBA_91C0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_91C8(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBA_91D0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUBA_91D8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(14);
}
static void SUBA_91E0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBA_91E8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_91F0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUBA_91F8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_91F9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(22);
}
static void SUBA_91FA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(18);
}
static void SUBA_91FB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(20);
}
static void SUBA_91FC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetAReg(opc_data[1]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[1], dst);
	cpuSetInstructionTime(16);
}
static void SUBI_0400(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBI_0410(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0418(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0420(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBI_0428(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0430(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUBI_0438(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0439(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBI_0440(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBI_0450(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0458(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBI_0460(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBI_0468(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0470(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUBI_0478(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBI_0479(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBI_0480(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(16);
}
static void SUBI_0490(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBI_0498(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBI_04A0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void SUBI_04A8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void SUBI_04B0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(34);
}
static void SUBI_04B8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(32);
}
static void SUBI_04B9(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(36);
}
static void SUBQ_5100(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuSubB(dst, src);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void SUBQ_5110(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5118(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5120(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUBQ_5128(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5130(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBQ_5138(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5139(ULO*opc_data)
{
	UBY src = (UBY)opc_data[1];
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_5140(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuSubW(dst, src);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void SUBQ_5150(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5158(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SUBQ_5160(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SUBQ_5168(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5170(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBQ_5178(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SUBQ_5179(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_5180(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuSubL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBQ_5190(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_5198(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void SUBQ_51A0(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void SUBQ_51A8(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBQ_51B0(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void SUBQ_51B8(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void SUBQ_51B9(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void SUBQ_5148(ULO*opc_data)
{
	UWO src = (UWO)opc_data[1];
	ULO dst = cpuGetAReg(opc_data[0]);
	dst = cpuSubaW(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void SUBQ_5188(ULO*opc_data)
{
	ULO src = opc_data[1];
	ULO dst = cpuGetAReg(opc_data[0]);
	dst = cpuSubaL(dst, src);
	cpuSetAReg(opc_data[0], dst);
	cpuSetInstructionTime(8);
}
static void CHK_4180(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(10);
}
static void CHK_4190(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(14);
}
static void CHK_4198(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(14);
}
static void CHK_41A0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(16);
}
static void CHK_41A8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(18);
}
static void CHK_41B0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(20);
}
static void CHK_41B8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(18);
}
static void CHK_41B9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(22);
}
static void CHK_41BA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(18);
}
static void CHK_41BB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(20);
}
static void CHK_41BC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuChkW(dst, src);
	cpuSetInstructionTime(14);
}
static void CHK_4100(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(10);
}
static void CHK_4110(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(18);
}
static void CHK_4118(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(18);
}
static void CHK_4120(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(20);
}
static void CHK_4128(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(22);
}
static void CHK_4130(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(24);
}
static void CHK_4138(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(22);
}
static void CHK_4139(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(26);
}
static void CHK_413A(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(22);
}
static void CHK_413B(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(24);
}
static void CHK_413C(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuChkL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMP_B000(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(4);
}
static void CMP_B010(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B018(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B020(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(10);
}
static void CMP_B028(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B030(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B038(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B039(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMP_B03A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B03B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B03C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[1]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B040(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(4);
}
static void CMP_B048(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(4);
}
static void CMP_B050(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B058(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B060(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(10);
}
static void CMP_B068(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B070(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B078(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B079(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMP_B07A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMP_B07B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B07C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMP_B080(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMP_B088(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMP_B090(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B098(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMP_B0A0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMP_B0A8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMP_B0B0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMP_B0B8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMP_B0B9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(22);
}
static void CMP_B0BA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMP_B0BB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMP_B0BC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0C0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetDRegWord(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B0C8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetARegWord(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B0D0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA02(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(10);
}
static void CMPA_B0D8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(10);
}
static void CMPA_B0E0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPA_B0E8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA05(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0F0(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA06(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPA_B0F8(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA70());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0F9(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA71());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B0FA(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA72());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B0FB(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)memoryReadWord(cpuEA73());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPA_B0FC(ULO*opc_data)
{
	ULO src = (ULO)(LON)(WOR)cpuGetNextWord();
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(10);
}
static void CMPA_B1C0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B1C8(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(6);
}
static void CMPA_B1D0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B1D8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPA_B1E0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPA_B1E8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B1F0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPA_B1F8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B1F9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(22);
}
static void CMPA_B1FA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPA_B1FB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPA_B1FC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetAReg(opc_data[1]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C00(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	UBY dst = cpuGetDRegByte(opc_data[0]);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(8);
}
static void CMPI_0C10(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C18(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C20(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C28(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C30(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0C38(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C39(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0C40(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[0]);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(8);
}
static void CMPI_0C50(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C58(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(12);
}
static void CMPI_0C60(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C68(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C70(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0C78(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C79(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0C80(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dst = cpuGetDReg(opc_data[0]);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(14);
}
static void CMPI_0C90(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0C98(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(20);
}
static void CMPI_0CA0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(22);
}
static void CMPI_0CA8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(24);
}
static void CMPI_0CB0(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(26);
}
static void CMPI_0CB8(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(24);
}
static void CMPI_0CB9(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(28);
}
static void CMPI_0C3A(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA72();
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C3B(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA73();
	UBY dst = memoryReadByte(dstea);
	cpuCmpB(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0C7A(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA72();
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(16);
}
static void CMPI_0C7B(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA73();
	UWO dst = memoryReadWord(dstea);
	cpuCmpW(dst, src);
	cpuSetInstructionTime(18);
}
static void CMPI_0CBA(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA72();
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(24);
}
static void CMPI_0CBB(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA73();
	ULO dst = memoryReadLong(dstea);
	cpuCmpL(dst, src);
	cpuSetInstructionTime(26);
}
static void BCHG_0150(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(10);
}
static void BCHG_0158(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(10);
}
static void BCHG_0160(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCHG_0168(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BCHG_0170(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCHG_0178(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BCHG_0179(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCHG_0140(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuBchgL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(10 + ((src >= 16) ? 2 : 0));
}
static void BCHG_0850(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCHG_0858(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCHG_0860(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BCHG_0868(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCHG_0870(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCHG_0878(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCHG_0879(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBchgB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCHG_0840(ULO*opc_data)
{
	ULO src = (ULO)cpuGetNextWord();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuBchgL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(12);
}
static void BCLR_0190(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCLR_0198(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCLR_01A0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BCLR_01A8(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_01B0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCLR_01B8(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_01B9(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCLR_0180(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuBclrL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(12 + ((src >= 16) ? 2 : 0));
}
static void BCLR_0890(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCLR_0898(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BCLR_08A0(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BCLR_08A8(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_08B0(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BCLR_08B8(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BCLR_08B9(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBclrB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BCLR_0880(ULO*opc_data)
{
	ULO src = (ULO)cpuGetNextWord();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuBclrL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(12);
}
static void BSET_01D0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(10);
}
static void BSET_01D8(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(10);
}
static void BSET_01E0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BSET_01E8(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BSET_01F0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BSET_01F8(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BSET_01F9(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BSET_01C0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuBsetL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(10 + ((src >= 16) ? 2 : 0));
}
static void BSET_08D0(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BSET_08D8(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void BSET_08E0(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void BSET_08E8(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BSET_08F0(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void BSET_08F8(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void BSET_08F9(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuBsetB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void BSET_08C0(ULO*opc_data)
{
	ULO src = (ULO)cpuGetNextWord();
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuBsetL(dst, src);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(12);
}
static void BTST_0110(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(10);
}
static void BTST_0118(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(10);
}
static void BTST_0120(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_0128(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_0130(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(16);
}
static void BTST_0138(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_0139(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(18);
}
static void BTST_013A(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA72();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_013B(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	ULO dstea = cpuEA73();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(16);
}
static void BTST_013C(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[1]);
	UBY dst = (UBY)cpuGetNextWord();
	cpuBtstB(dst, src);
	cpuSetInstructionTime(10);
}
static void BTST_0100(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[1]);
	ULO dst = cpuGetDReg(opc_data[0]);
	cpuBtstL(dst, src);
	cpuSetInstructionTime(10);
}
static void BTST_0810(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(8);
}
static void BTST_0818(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(8);
}
static void BTST_0820(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(10);
}
static void BTST_0828(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_0830(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_0838(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_0839(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(16);
}
static void BTST_083A(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA72();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(12);
}
static void BTST_083B(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA73();
	UBY dst = memoryReadByte(dstea);
	cpuBtstB(dst, src);
	cpuSetInstructionTime(14);
}
static void BTST_0800(ULO*opc_data)
{
	ULO src = (ULO)cpuGetNextWord();
	ULO dst = cpuGetDReg(opc_data[0]);
	cpuBtstL(dst, src);
	cpuSetInstructionTime(8);
}
static void LEA_41D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(4);
}
static void LEA_41E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(8);
}
static void LEA_41F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(12);
}
static void LEA_41F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(8);
}
static void LEA_41F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(12);
}
static void LEA_41FA(ULO*opc_data)
{
	ULO dstea = cpuEA72();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(8);
}
static void LEA_41FB(ULO*opc_data)
{
	ULO dstea = cpuEA73();
	cpuSetAReg(opc_data[1], dstea);
	cpuSetInstructionTime(12);
}
static void MULS_C1C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 0);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 6);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 12);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULS_C1FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMulsW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 0);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 6);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 12);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 8);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 10);
	cpuSetDReg(opc_data[1], res);
}
static void MULU_C0FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	UWO dst = cpuGetDRegWord(opc_data[1]);
	ULO res = cpuMuluW(dst, src, 4);
	cpuSetDReg(opc_data[1], res);
}
static void DIVS_81C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVS_81FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivsW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVL_4C40(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = cpuGetDReg(opc_data[0]);
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C50(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C58(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C60(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C68(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C70(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C78(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA70());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C79(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA71());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C7A(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA72());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C7B(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA73());
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVL_4C7C(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = cpuGetNextLong();
	cpuDivL(src, ext, opc_data[2]);
}
static void DIVU_80C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void DIVU_80FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dst = cpuGetDReg(opc_data[1]);
	cpuDivuW(dst, src, opc_data[1], opc_data[2]);
}
static void MOVEM_48A0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	cpuMovemwPre(regs, opc_data[0]);
}
static void MOVEM_48E0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	cpuMovemlPre(regs, opc_data[0]);
}
static void MOVEM_4C98(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	cpuMovemwPost(regs, opc_data[0]);
}
static void MOVEM_4CD8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	cpuMovemlPost(regs, opc_data[0]);
}
static void MOVEM_4890(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48A8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48B0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48B8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48B9(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuMovemwR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48D0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48E8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48F0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48F8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_48F9(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuMovemlR2Ea(regs, dstea, opc_data[1]);
}
static void MOVEM_4C90(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CA8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CB0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CB8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CB9(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CBA(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CBB(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuMovemwEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CD0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CE8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CF0(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CF8(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CF9(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CFA(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void MOVEM_4CFB(ULO*opc_data)
{
	UWO regs = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuMovemlEa2R(regs, dstea, opc_data[1]);
}
static void CLR_4200(ULO*opc_data)
{
	UBY dst = 0;
	cpuClr();
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void CLR_4210(ULO*opc_data)
{
	UBY dst = 0;
	ULO dstea = cpuEA02(opc_data[0]);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4218(ULO*opc_data)
{
	UBY dst = 0;
	ULO dstea = cpuEA03(opc_data[0], 1);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4220(ULO*opc_data)
{
	UBY dst = 0;
	ULO dstea = cpuEA04(opc_data[0], 1);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void CLR_4228(ULO*opc_data)
{
	UBY dst = 0;
	ULO dstea = cpuEA05(opc_data[0]);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4230(ULO*opc_data)
{
	UBY dst = 0;
	ULO dstea = cpuEA06(opc_data[0]);
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void CLR_4238(ULO*opc_data)
{
	UBY dst = 0;
	ULO dstea = cpuEA70();
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4239(ULO*opc_data)
{
	UBY dst = 0;
	ULO dstea = cpuEA71();
	cpuClr();
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_4240(ULO*opc_data)
{
	UWO dst = 0;
	cpuClr();
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void CLR_4250(ULO*opc_data)
{
	UWO dst = 0;
	ULO dstea = cpuEA02(opc_data[0]);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4258(ULO*opc_data)
{
	UWO dst = 0;
	ULO dstea = cpuEA03(opc_data[0], 2);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void CLR_4260(ULO*opc_data)
{
	UWO dst = 0;
	ULO dstea = cpuEA04(opc_data[0], 2);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void CLR_4268(ULO*opc_data)
{
	UWO dst = 0;
	ULO dstea = cpuEA05(opc_data[0]);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4270(ULO*opc_data)
{
	UWO dst = 0;
	ULO dstea = cpuEA06(opc_data[0]);
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void CLR_4278(ULO*opc_data)
{
	UWO dst = 0;
	ULO dstea = cpuEA70();
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void CLR_4279(ULO*opc_data)
{
	UWO dst = 0;
	ULO dstea = cpuEA71();
	cpuClr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_4280(ULO*opc_data)
{
	ULO dst = 0;
	cpuClr();
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void CLR_4290(ULO*opc_data)
{
	ULO dst = 0;
	ULO dstea = cpuEA02(opc_data[0]);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_4298(ULO*opc_data)
{
	ULO dst = 0;
	ULO dstea = cpuEA03(opc_data[0], 4);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CLR_42A0(ULO*opc_data)
{
	ULO dst = 0;
	ULO dstea = cpuEA04(opc_data[0], 4);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void CLR_42A8(ULO*opc_data)
{
	ULO dst = 0;
	ULO dstea = cpuEA05(opc_data[0]);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void CLR_42B0(ULO*opc_data)
{
	ULO dst = 0;
	ULO dstea = cpuEA06(opc_data[0]);
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void CLR_42B8(ULO*opc_data)
{
	ULO dst = 0;
	ULO dstea = cpuEA70();
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void CLR_42B9(ULO*opc_data)
{
	ULO dst = 0;
	ULO dstea = cpuEA71();
	cpuClr();
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void BFCHG_EAD0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfChgEa(dstea, ext);
}
static void BFCHG_EAF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfChgEa(dstea, ext);
}
static void BFCLR_ECD0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfClrEa(dstea, ext);
}
static void BFCLR_ECF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfClrEa(dstea, ext);
}
static void BFEXTS_EBD0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBFA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTS_EBFB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuBfExtsEa(dstea, ext);
}
static void BFEXTU_E9D0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9E8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9F0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9F8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9F9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9FA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuBfExtuEa(dstea, ext);
}
static void BFEXTU_E9FB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuBfExtuEa(dstea, ext);
}
static void BFFFO_EDD0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDFA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuBfFfoEa(dstea, ext);
}
static void BFFFO_EDFB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuBfFfoEa(dstea, ext);
}
static void BFINS_EFD0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfInsEa(dstea, ext);
}
static void BFINS_EFF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfInsEa(dstea, ext);
}
static void BFSET_EED0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfSetEa(dstea, ext);
}
static void BFSET_EEF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfSetEa(dstea, ext);
}
static void BFTST_E8D0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8E8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8F0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8F8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8F9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8FA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuBfTstEa(dstea, ext);
}
static void BFTST_E8FB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuBfTstEa(dstea, ext);
}
static void BFCHG_EAC0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfChgReg(opc_data[0], ext);
}
static void BFCLR_ECC0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfClrReg(opc_data[0], ext);
}
static void BFEXTS_EBC0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfExtsReg(opc_data[0], ext);
}
static void BFEXTU_E9C0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfExtuReg(opc_data[0], ext);
}
static void BFFFO_EDC0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfFfoReg(opc_data[0], ext);
}
static void BFINS_EFC0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfInsReg(opc_data[0], ext);
}
static void BFSET_EEC0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfSetReg(opc_data[0], ext);
}
static void BFTST_E8C0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	cpuBfTstReg(opc_data[0], ext);
}
static void MULL_4C00(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = cpuGetDReg(opc_data[0]);
	cpuMulL(src, ext);
}
static void MULL_4C10(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuMulL(src, ext);
}
static void MULL_4C18(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuMulL(src, ext);
}
static void MULL_4C20(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuMulL(src, ext);
}
static void MULL_4C28(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuMulL(src, ext);
}
static void MULL_4C30(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuMulL(src, ext);
}
static void MULL_4C38(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA70());
	cpuMulL(src, ext);
}
static void MULL_4C39(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA71());
	cpuMulL(src, ext);
}
static void MULL_4C3A(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA72());
	cpuMulL(src, ext);
}
static void MULL_4C3B(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA73());
	cpuMulL(src, ext);
}
static void MULL_4C3C(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = cpuGetNextLong();
	cpuMulL(src, ext);
}
static void MOVES_0E10(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	cpuMoveSB(src, ext);
}
static void MOVES_0E18(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	cpuMoveSB(src, ext);
}
static void MOVES_0E20(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	cpuMoveSB(src, ext);
}
static void MOVES_0E28(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	cpuMoveSB(src, ext);
}
static void MOVES_0E30(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	cpuMoveSB(src, ext);
}
static void MOVES_0E38(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UBY src = memoryReadByte(cpuEA70());
	cpuMoveSB(src, ext);
}
static void MOVES_0E39(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UBY src = memoryReadByte(cpuEA71());
	cpuMoveSB(src, ext);
}
static void MOVES_0E50(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveSW(src, ext);
}
static void MOVES_0E58(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveSW(src, ext);
}
static void MOVES_0E60(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveSW(src, ext);
}
static void MOVES_0E68(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveSW(src, ext);
}
static void MOVES_0E70(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveSW(src, ext);
}
static void MOVES_0E78(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UWO src = memoryReadWord(cpuEA70());
	cpuMoveSW(src, ext);
}
static void MOVES_0E79(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	UWO src = memoryReadWord(cpuEA71());
	cpuMoveSW(src, ext);
}
static void MOVES_0E90(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuMoveSL(src, ext);
}
static void MOVES_0E98(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuMoveSL(src, ext);
}
static void MOVES_0EA0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuMoveSL(src, ext);
}
static void MOVES_0EA8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuMoveSL(src, ext);
}
static void MOVES_0EB0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuMoveSL(src, ext);
}
static void MOVES_0EB8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA70());
	cpuMoveSL(src, ext);
}
static void MOVES_0EB9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO src = memoryReadLong(cpuEA71());
	cpuMoveSL(src, ext);
}
static void NBCD_4800(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNbcdB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NBCD_4810(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NBCD_4818(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NBCD_4820(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NBCD_4828(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NBCD_4830(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NBCD_4838(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NBCD_4839(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNbcdB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4400(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNegB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEG_4410(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4418(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4420(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEG_4428(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4430(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEG_4438(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4439(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4440(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuNegW(dst);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEG_4450(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4458(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEG_4460(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEG_4468(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4470(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEG_4478(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEG_4479(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4480(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuNegL(dst);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NEG_4490(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_4498(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEG_44A0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void NEG_44A8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEG_44B0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void NEG_44B8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEG_44B9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void NEGX_4000(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNegxB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEGX_4010(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4018(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4020(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEGX_4028(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4030(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEGX_4038(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4039(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNegxB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_4040(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuNegxW(dst);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NEGX_4050(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4058(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NEGX_4060(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NEGX_4068(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4070(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NEGX_4078(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NEGX_4079(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuNegxW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_4080(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuNegxL(dst);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NEGX_4090(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_4098(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NEGX_40A0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void NEGX_40A8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEGX_40B0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void NEGX_40B8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NEGX_40B9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuNegxL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void NOT_4600(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuNotB(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NOT_4610(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4618(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4620(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NOT_4628(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4630(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NOT_4638(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4639(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuNotB(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_4640(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuNotW(dst);
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void NOT_4650(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4658(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void NOT_4660(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void NOT_4668(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4670(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void NOT_4678(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void NOT_4679(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuNotW(dst);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_4680(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuNotL(dst);
	cpuSetDReg(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void NOT_4690(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_4698(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(20);
}
static void NOT_46A0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(22);
}
static void NOT_46A8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NOT_46B0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(26);
}
static void NOT_46B8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(24);
}
static void NOT_46B9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	dst = cpuNotL(dst);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(28);
}
static void TAS_4AC0(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuTas(dst);
	cpuSetDRegByte(opc_data[0], dst);
	cpuSetInstructionTime(4);
}
static void TAS_4AD0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void TAS_4AD8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void TAS_4AE0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void TAS_4AE8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void TAS_4AF0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void TAS_4AF8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void TAS_4AF9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	dst = cpuTas(dst);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(22);
}
static void TST_4A00(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	cpuTestB(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A10(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A18(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A20(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(10);
}
static void TST_4A28(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A30(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A38(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A39(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(16);
}
static void TST_4A40(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	cpuTestW(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A50(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A58(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A60(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(10);
}
static void TST_4A68(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A70(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A78(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A79(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(16);
}
static void TST_4A80(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	cpuTestL(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A90(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A98(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(12);
}
static void TST_4AA0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 4);
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(14);
}
static void TST_4AA8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(16);
}
static void TST_4AB0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(18);
}
static void TST_4AB8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(16);
}
static void TST_4AB9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(20);
}
static void TST_4A3A(ULO*opc_data)
{
	ULO dstea = cpuEA72();
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A3B(ULO*opc_data)
{
	ULO dstea = cpuEA73();
	UBY dst = memoryReadByte(dstea);
	cpuTestB(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A3C(ULO*opc_data)
{
	UBY dst = (UBY)cpuGetNextWord();
	cpuTestB(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A48(ULO*opc_data)
{
	UWO dst = (UWO)cpuGetAReg(opc_data[0]);
	cpuTestW(dst);
	cpuSetInstructionTime(4);
}
static void TST_4A7A(ULO*opc_data)
{
	ULO dstea = cpuEA72();
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(12);
}
static void TST_4A7B(ULO*opc_data)
{
	ULO dstea = cpuEA73();
	UWO dst = memoryReadWord(dstea);
	cpuTestW(dst);
	cpuSetInstructionTime(14);
}
static void TST_4A7C(ULO*opc_data)
{
	UWO dst = cpuGetNextWord();
	cpuTestW(dst);
	cpuSetInstructionTime(8);
}
static void TST_4A88(ULO*opc_data)
{
	ULO dst = cpuGetAReg(opc_data[0]);
	cpuTestL(dst);
	cpuSetInstructionTime(4);
}
static void TST_4ABA(ULO*opc_data)
{
	ULO dstea = cpuEA72();
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(16);
}
static void TST_4ABB(ULO*opc_data)
{
	ULO dstea = cpuEA73();
	ULO dst = memoryReadLong(dstea);
	cpuTestL(dst);
	cpuSetInstructionTime(18);
}
static void TST_4ABC(ULO*opc_data)
{
	ULO dst = cpuGetNextLong();
	cpuTestL(dst);
	cpuSetInstructionTime(12);
}
static void PEA_4850(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	cpuPeaL(dstea);
	cpuSetInstructionTime(12);
}
static void PEA_4868(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	cpuPeaL(dstea);
	cpuSetInstructionTime(16);
}
static void PEA_4870(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	cpuPeaL(dstea);
	cpuSetInstructionTime(20);
}
static void PEA_4878(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	cpuPeaL(dstea);
	cpuSetInstructionTime(16);
}
static void PEA_4879(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	cpuPeaL(dstea);
	cpuSetInstructionTime(20);
}
static void PEA_487A(ULO*opc_data)
{
	ULO dstea = cpuEA72();
	cpuPeaL(dstea);
	cpuSetInstructionTime(16);
}
static void PEA_487B(ULO*opc_data)
{
	ULO dstea = cpuEA73();
	cpuPeaL(dstea);
	cpuSetInstructionTime(20);
}
static void JMP_4ED0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	cpuJmp(dstea);
	cpuSetInstructionTime(8);
}
static void JMP_4EE8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	cpuJmp(dstea);
	cpuSetInstructionTime(10);
}
static void JMP_4EF0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	cpuJmp(dstea);
	cpuSetInstructionTime(14);
}
static void JMP_4EF8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	cpuJmp(dstea);
	cpuSetInstructionTime(10);
}
static void JMP_4EF9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	cpuJmp(dstea);
	cpuSetInstructionTime(12);
}
static void JMP_4EFA(ULO*opc_data)
{
	ULO dstea = cpuEA72();
	cpuJmp(dstea);
	cpuSetInstructionTime(10);
}
static void JMP_4EFB(ULO*opc_data)
{
	ULO dstea = cpuEA73();
	cpuJmp(dstea);
	cpuSetInstructionTime(14);
}
static void JSR_4E90(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	cpuJsr(dstea);
	cpuSetInstructionTime(16);
}
static void JSR_4EA8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	cpuJsr(dstea);
	cpuSetInstructionTime(18);
}
static void JSR_4EB0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	cpuJsr(dstea);
	cpuSetInstructionTime(22);
}
static void JSR_4EB8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	cpuJsr(dstea);
	cpuSetInstructionTime(18);
}
static void JSR_4EB9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	cpuJsr(dstea);
	cpuSetInstructionTime(20);
}
static void JSR_4EBA(ULO*opc_data)
{
	ULO dstea = cpuEA72();
	cpuJsr(dstea);
	cpuSetInstructionTime(18);
}
static void JSR_4EBB(ULO*opc_data)
{
	ULO dstea = cpuEA73();
	cpuJsr(dstea);
	cpuSetInstructionTime(22);
}
static void MOVETOSR_46C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	cpuMoveToSr(src);
	cpuSetInstructionTime(12);
}
static void MOVETOSR_46D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveToSr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOSR_46D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveToSr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOSR_46E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveToSr(src);
	cpuSetInstructionTime(18);
}
static void MOVETOSR_46E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveToSr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOSR_46F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveToSr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOSR_46F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	cpuMoveToSr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOSR_46F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	cpuMoveToSr(src);
	cpuSetInstructionTime(24);
}
static void MOVETOSR_46FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	cpuMoveToSr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOSR_46FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	cpuMoveToSr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOSR_46FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	cpuMoveToSr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOCCR_44C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	cpuMoveToCcr(src);
	cpuSetInstructionTime(12);
}
static void MOVETOCCR_44D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOCCR_44D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(16);
}
static void MOVETOCCR_44E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(18);
}
static void MOVETOCCR_44E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOCCR_44F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveToCcr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOCCR_44F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOCCR_44F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(24);
}
static void MOVETOCCR_44FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(20);
}
static void MOVETOCCR_44FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	cpuMoveToCcr(src);
	cpuSetInstructionTime(22);
}
static void MOVETOCCR_44FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	cpuMoveToCcr(src);
	cpuSetInstructionTime(16);
}
static void SCC_50C0(ULO*opc_data)
{
	UBY dst = cpuScc(opc_data[1]);
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
static void SCC_50D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UBY dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SCC_50D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 1);
	UBY dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(12);
}
static void SCC_50E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 1);
	UBY dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(14);
}
static void SCC_50E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UBY dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SCC_50F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UBY dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SCC_50F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UBY dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(16);
}
static void SCC_50F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UBY dst = cpuScc(opc_data[1]);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(20);
}
static void MOVEFROMCCR_42C0(ULO*opc_data)
{
	UWO dst = cpuMoveFromCcr();
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void MOVEFROMCCR_42D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMCCR_42D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMCCR_42E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void MOVEFROMCCR_42E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMCCR_42F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void MOVEFROMCCR_42F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMCCR_42F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = cpuMoveFromCcr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void MOVEFROMSR_40C0(ULO*opc_data)
{
	UWO dst = cpuMoveFromSr();
	cpuSetDRegWord(opc_data[0], dst);
	cpuSetInstructionTime(6);
}
static void MOVEFROMSR_40D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMSR_40D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(12);
}
static void MOVEFROMSR_40E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(14);
}
static void MOVEFROMSR_40E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMSR_40F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void MOVEFROMSR_40F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(16);
}
static void MOVEFROMSR_40F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = cpuMoveFromSr();
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(20);
}
static void CAS_0AD0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuCasB(dstea, ext);
}
static void CAS_0AD8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 1);
	cpuCasB(dstea, ext);
}
static void CAS_0AE0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 1);
	cpuCasB(dstea, ext);
}
static void CAS_0AE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuCasB(dstea, ext);
}
static void CAS_0AF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuCasB(dstea, ext);
}
static void CAS_0AF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuCasB(dstea, ext);
}
static void CAS_0AF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuCasB(dstea, ext);
}
static void CAS_0CD0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuCasW(dstea, ext);
}
static void CAS_0CD8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 2);
	cpuCasW(dstea, ext);
}
static void CAS_0CE0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 2);
	cpuCasW(dstea, ext);
}
static void CAS_0CE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuCasW(dstea, ext);
}
static void CAS_0CF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuCasW(dstea, ext);
}
static void CAS_0CF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuCasW(dstea, ext);
}
static void CAS_0CF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuCasW(dstea, ext);
}
static void CAS_0ED0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuCasL(dstea, ext);
}
static void CAS_0ED8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[0], 4);
	cpuCasL(dstea, ext);
}
static void CAS_0EE0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[0], 4);
	cpuCasL(dstea, ext);
}
static void CAS_0EE8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuCasL(dstea, ext);
}
static void CAS_0EF0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuCasL(dstea, ext);
}
static void CAS_0EF8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuCasL(dstea, ext);
}
static void CAS_0EF9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuCasL(dstea, ext);
}
static void CHKCMP2_00D0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00E8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00F0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00F8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00F9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00FA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_00FB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuChkCmp2B(dstea, ext);
}
static void CHKCMP2_02D0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02E8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02F0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02F8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02F9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02FA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_02FB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuChkCmp2W(dstea, ext);
}
static void CHKCMP2_04D0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04E8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04F0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04F8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04F9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04FA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuChkCmp2L(dstea, ext);
}
static void CHKCMP2_04FB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuChkCmp2L(dstea, ext);
}
static void CALLM_06D0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuCallm(dstea, ext);
}
static void CALLM_06E8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuCallm(dstea, ext);
}
static void CALLM_06F0(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuCallm(dstea, ext);
}
static void CALLM_06F8(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuCallm(dstea, ext);
}
static void CALLM_06F9(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuCallm(dstea, ext);
}
static void CALLM_06FA(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA72();
	cpuCallm(dstea, ext);
}
static void CALLM_06FB(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA73();
	cpuCallm(dstea, ext);
}
static void PFLUSH030_F010(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[0]);
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F028(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[0]);
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F030(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[0]);
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F038(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuPflush030(dstea, ext);
}
static void PFLUSH030_F039(ULO*opc_data)
{
	UWO ext = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuPflush030(dstea, ext);
}
static void MOVEQ_7000(ULO*opc_data)
{
	cpuSetDReg(opc_data[0], opc_data[1]);
	cpuSetFlagsAbs((UWO)opc_data[2]);
	cpuSetInstructionTime(4);
}
static void MOVE_1000(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_1008(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_1010(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_1018(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_1020(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(10);
}
static void MOVE_1028(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_1030(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_1038(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_1039(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_103A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_103B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_103C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	cpuMoveB(src);
	cpuSetDRegByte(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_1080(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_1088(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_1090(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1098(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10A0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_10A8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10B0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10B8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10B9(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_10BA(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10BB(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10BC(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10C0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_10C8(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_10D0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10D8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_10E0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_10E8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10F0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10F8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10F9(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_10FA(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_10FB(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_10FC(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1100(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_1108(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_1110(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1118(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1120(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_1128(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1130(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_1138(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1139(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_113A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_113B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_113C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[1], 1);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1140(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1148(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_1150(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1158(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1160(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_1168(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_1170(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_1178(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_1179(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_117A(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_117B(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_117C(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_1180(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_1188(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_1190(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_1198(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_11A0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11A8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11B0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_11B8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11B9(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_11BA(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11BB(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_11BC(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_11C0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_11C8(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_11D0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_11D8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_11E0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_11E8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11F0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11F8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11F9(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_11FA(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_11FB(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_11FC(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_13C0(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_13C8(ULO*opc_data)
{
	UBY src = cpuGetARegByte(opc_data[0]);
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_13D0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_13D8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA03(opc_data[0],1));
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_13E0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_13E8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_13F0(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_13F8(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA70());
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_13F9(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA71());
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_13FA(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA72());
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_13FB(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA73());
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_13FC(ULO*opc_data)
{
	UBY src = (UBY)cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuMoveB(src);
	memoryWriteByte(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_3000(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_3008(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_3010(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_3018(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_3020(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(10);
}
static void MOVE_3028(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_3030(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_3038(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_3039(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_303A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_303B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_303C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	cpuMoveW(src);
	cpuSetDRegWord(opc_data[1], src);
	cpuSetInstructionTime(8);
}
static void MOVE_3080(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3088(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3090(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3098(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30A0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_30A8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30B0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30B8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30B9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_30BA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30BB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30BC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_30C8(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_30D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_30E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_30E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_30FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_30FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_30FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA03(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3100(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3108(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(8);
}
static void MOVE_3110(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3118(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3120(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_3128(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3130(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_3138(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3139(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_313A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_313B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_313C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA04(opc_data[1], 2);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3140(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3148(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_3150(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3158(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3160(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_3168(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_3170(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_3178(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_3179(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_317A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_317B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_317C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_3180(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_3188(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(14);
}
static void MOVE_3190(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_3198(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_31A0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31A8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31B0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_31B8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31B9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_31BA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31BB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_31BC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_31C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_31C8(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_31D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_31D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_31E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_31E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_31FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_31FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_31FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA70();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_33C0(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_33C8(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_33D0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_33D8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_33E0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_33E8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_33F0(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_33F8(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_33F9(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_33FA(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_33FB(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_33FC(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	ULO dstea = cpuEA71();
	cpuMoveW(src);
	memoryWriteWord(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2000(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_2008(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVE_2010(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_2018(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_2020(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVE_2028(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_2030(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVE_2038(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_2039(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(20);
}
static void MOVE_203A(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVE_203B(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVE_203C(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	cpuMoveL(src);
	cpuSetDReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVE_2080(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2088(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2090(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2098(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20A0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_20A8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20B0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20B8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20B9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_20BA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20BB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20BC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA02(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20C0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_20C8(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_20D0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20D8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_20E0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_20E8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20F0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20F8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20F9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_20FA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_20FB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_20FC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA03(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2100(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2108(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(12);
}
static void MOVE_2110(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2118(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2120(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(22);
}
static void MOVE_2128(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2130(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_2138(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2139(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_213A(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_213B(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_213C(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA04(opc_data[1], 4);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_2140(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_2148(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_2150(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2158(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2160(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_2168(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_2170(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_2178(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_2179(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_217A(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_217B(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_217C(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA05(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_2180(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_2188(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(18);
}
static void MOVE_2190(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_2198(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_21A0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21A8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21B0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_21B8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21B9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(34);
}
static void MOVE_21BA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21BB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_21BC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA06(opc_data[1]);
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_21C0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_21C8(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(16);
}
static void MOVE_21D0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_21D8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_21E0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(26);
}
static void MOVE_21E8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21F0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21F8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21F9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_21FA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_21FB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_21FC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA70();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(24);
}
static void MOVE_23C0(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_23C8(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(20);
}
static void MOVE_23D0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_23D8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVE_23E0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(30);
}
static void MOVE_23E8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_23F0(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(34);
}
static void MOVE_23F8(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_23F9(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(36);
}
static void MOVE_23FA(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(32);
}
static void MOVE_23FB(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(34);
}
static void MOVE_23FC(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	ULO dstea = cpuEA71();
	cpuMoveL(src);
	memoryWriteLong(src, dstea);
	cpuSetInstructionTime(28);
}
static void MOVEA_3040(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(4);
}
static void MOVEA_3048(ULO*opc_data)
{
	UWO src = cpuGetARegWord(opc_data[0]);
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(4);
}
static void MOVEA_3050(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA02(opc_data[0]));
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(8);
}
static void MOVEA_3058(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA03(opc_data[0],2));
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(8);
}
static void MOVEA_3060(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(10);
}
static void MOVEA_3068(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA05(opc_data[0]));
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(12);
}
static void MOVEA_3070(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA06(opc_data[0]));
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(14);
}
static void MOVEA_3078(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA70());
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(12);
}
static void MOVEA_3079(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA71());
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(16);
}
static void MOVEA_307A(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA72());
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(12);
}
static void MOVEA_307B(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA73());
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(14);
}
static void MOVEA_307C(ULO*opc_data)
{
	UWO src = cpuGetNextWord();
	cpuSetAReg(opc_data[1], (ULO)(LON)(WOR)src);
	cpuSetInstructionTime(8);
}
static void MOVEA_2040(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVEA_2048(ULO*opc_data)
{
	ULO src = cpuGetAReg(opc_data[0]);
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(4);
}
static void MOVEA_2050(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA02(opc_data[0]));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVEA_2058(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA03(opc_data[0],4));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void MOVEA_2060(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(14);
}
static void MOVEA_2068(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA05(opc_data[0]));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVEA_2070(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA06(opc_data[0]));
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVEA_2078(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA70());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVEA_2079(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA71());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(20);
}
static void MOVEA_207A(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA72());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(16);
}
static void MOVEA_207B(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA73());
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(18);
}
static void MOVEA_207C(ULO*opc_data)
{
	ULO src = cpuGetNextLong();
	cpuSetAReg(opc_data[1], src);
	cpuSetInstructionTime(12);
}
static void BCCB_6200(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode2(), opc_data[1]);
}
static void BCCB_6300(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode3(), opc_data[1]);
}
static void BCCB_6400(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode4(), opc_data[1]);
}
static void BCCB_6500(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode5(), opc_data[1]);
}
static void BCCB_6600(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode6(), opc_data[1]);
}
static void BCCB_6700(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode7(), opc_data[1]);
}
static void BCCB_6800(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode8(), opc_data[1]);
}
static void BCCB_6900(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode9(), opc_data[1]);
}
static void BCCB_6A00(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode10(), opc_data[1]);
}
static void BCCB_6B00(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode11(), opc_data[1]);
}
static void BCCB_6C00(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode12(), opc_data[1]);
}
static void BCCB_6D00(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode13(), opc_data[1]);
}
static void BCCB_6E00(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode14(), opc_data[1]);
}
static void BCCB_6F00(ULO*opc_data)
{
	cpuBccB(cpuCalculateConditionCode15(), opc_data[1]);
}
static void BCCW_6200(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode2());
}
static void BCCW_6300(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode3());
}
static void BCCW_6400(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode4());
}
static void BCCW_6500(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode5());
}
static void BCCW_6600(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode6());
}
static void BCCW_6700(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode7());
}
static void BCCW_6800(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode8());
}
static void BCCW_6900(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode9());
}
static void BCCW_6A00(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode10());
}
static void BCCW_6B00(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode11());
}
static void BCCW_6C00(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode12());
}
static void BCCW_6D00(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode13());
}
static void BCCW_6E00(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode14());
}
static void BCCW_6F00(ULO*opc_data)
{
	cpuBccW(cpuCalculateConditionCode15());
}
static void BCCL_62FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode2());
}
static void BCCL_63FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode3());
}
static void BCCL_64FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode4());
}
static void BCCL_65FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode5());
}
static void BCCL_66FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode6());
}
static void BCCL_67FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode7());
}
static void BCCL_68FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode8());
}
static void BCCL_69FF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode9());
}
static void BCCL_6AFF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode10());
}
static void BCCL_6BFF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode11());
}
static void BCCL_6CFF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode12());
}
static void BCCL_6DFF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode13());
}
static void BCCL_6EFF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode14());
}
static void BCCL_6FFF(ULO*opc_data)
{
	cpuBccL(cpuCalculateConditionCode15());
}
static void BKPT_4848(ULO*opc_data)
{
	cpuBkpt(opc_data[0]);
}
static void EXG_C140(ULO*opc_data)
{
	cpuExgDD(opc_data[0], opc_data[1]);
}
static void EXG_C148(ULO*opc_data)
{
	cpuExgAA(opc_data[0], opc_data[1]);
}
static void EXG_C188(ULO*opc_data)
{
	cpuExgDA(opc_data[0], opc_data[1]);
}
static void EXT_4880(ULO*opc_data)
{
	cpuExtW(opc_data[0]);
}
static void EXT_48C0(ULO*opc_data)
{
	cpuExtL(opc_data[0]);
}
static void EXT_49C0(ULO*opc_data)
{
	cpuExtBL(opc_data[0]);
}
static void SWAP_4840(ULO*opc_data)
{
	cpuSwap(opc_data[0]);
}
static void LINK_4E50(ULO*opc_data)
{
	cpuLinkW(opc_data[0]);
}
static void LINK_4808(ULO*opc_data)
{
	cpuLinkL(opc_data[0]);
}
static void UNLK_4E58(ULO*opc_data)
{
	cpuUnlk(opc_data[0]);
}
static void BRAB_6000(ULO*opc_data)
{
	cpuBraB(opc_data[1]);
}
static void BRAW_6000(ULO*opc_data)
{
	cpuBraW();
}
static void BRAL_60FF(ULO*opc_data)
{
	cpuBraL();
}
static void BSRB_6100(ULO*opc_data)
{
	cpuBsrB(opc_data[1]);
}
static void BSRW_6100(ULO*opc_data)
{
	cpuBsrW();
}
static void BSRL_61FF(ULO*opc_data)
{
	cpuBsrL();
}
static void DBCC_50C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode0(), opc_data[1]);
}
static void DBCC_51C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode1(), opc_data[1]);
}
static void DBCC_52C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode2(), opc_data[1]);
}
static void DBCC_53C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode3(), opc_data[1]);
}
static void DBCC_54C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode4(), opc_data[1]);
}
static void DBCC_55C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode5(), opc_data[1]);
}
static void DBCC_56C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode6(), opc_data[1]);
}
static void DBCC_57C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode7(), opc_data[1]);
}
static void DBCC_58C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode8(), opc_data[1]);
}
static void DBCC_59C8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode9(), opc_data[1]);
}
static void DBCC_5AC8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode10(), opc_data[1]);
}
static void DBCC_5BC8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode11(), opc_data[1]);
}
static void DBCC_5CC8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode12(), opc_data[1]);
}
static void DBCC_5DC8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode13(), opc_data[1]);
}
static void DBCC_5EC8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode14(), opc_data[1]);
}
static void DBCC_5FC8(ULO*opc_data)
{
	cpuDbcc(cpuCalculateConditionCode15(), opc_data[1]);
}
static void TRAPCC_50FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode0());
}
static void TRAPCC_51FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode1());
}
static void TRAPCC_52FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode2());
}
static void TRAPCC_53FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode3());
}
static void TRAPCC_54FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode4());
}
static void TRAPCC_55FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode5());
}
static void TRAPCC_56FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode6());
}
static void TRAPCC_57FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode7());
}
static void TRAPCC_58FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode8());
}
static void TRAPCC_59FC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode9());
}
static void TRAPCC_5AFC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode10());
}
static void TRAPCC_5BFC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode11());
}
static void TRAPCC_5CFC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode12());
}
static void TRAPCC_5DFC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode13());
}
static void TRAPCC_5EFC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode14());
}
static void TRAPCC_5FFC(ULO*opc_data)
{
	cpuTrapcc(cpuCalculateConditionCode15());
}
static void TRAPCC_50FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode0());
}
static void TRAPCC_51FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode1());
}
static void TRAPCC_52FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode2());
}
static void TRAPCC_53FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode3());
}
static void TRAPCC_54FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode4());
}
static void TRAPCC_55FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode5());
}
static void TRAPCC_56FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode6());
}
static void TRAPCC_57FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode7());
}
static void TRAPCC_58FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode8());
}
static void TRAPCC_59FA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode9());
}
static void TRAPCC_5AFA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode10());
}
static void TRAPCC_5BFA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode11());
}
static void TRAPCC_5CFA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode12());
}
static void TRAPCC_5DFA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode13());
}
static void TRAPCC_5EFA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode14());
}
static void TRAPCC_5FFA(ULO*opc_data)
{
	cpuTrapccW(cpuCalculateConditionCode15());
}
static void TRAPCC_50FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode0());
}
static void TRAPCC_51FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode1());
}
static void TRAPCC_52FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode2());
}
static void TRAPCC_53FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode3());
}
static void TRAPCC_54FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode4());
}
static void TRAPCC_55FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode5());
}
static void TRAPCC_56FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode6());
}
static void TRAPCC_57FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode7());
}
static void TRAPCC_58FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode8());
}
static void TRAPCC_59FB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode9());
}
static void TRAPCC_5AFB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode10());
}
static void TRAPCC_5BFB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode11());
}
static void TRAPCC_5CFB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode12());
}
static void TRAPCC_5DFB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode13());
}
static void TRAPCC_5EFB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode14());
}
static void TRAPCC_5FFB(ULO*opc_data)
{
	cpuTrapccL(cpuCalculateConditionCode15());
}
static void RTD_4E74(ULO*opc_data)
{
	cpuRtd();
}
static void RTE_4E73(ULO*opc_data)
{
	cpuRte();
}
static void RTS_4E75(ULO*opc_data)
{
	cpuRts();
}
static void RTR_4E77(ULO*opc_data)
{
	cpuRtr();
}
static void NOP_4E71(ULO*opc_data)
{
	cpuNop();
}
static void MOVEC_4E7A(ULO*opc_data)
{
	cpuMoveCFrom();
}
static void MOVEC_4E7B(ULO*opc_data)
{
	cpuMoveCTo();
}
static void CAS2_0CFC(ULO*opc_data)
{
	cpuCas2W();
}
static void CAS2_0EFC(ULO*opc_data)
{
	cpuCas2L();
}
static void TRAP_4E40(ULO*opc_data)
{
	cpuTrap(opc_data[0]);
}
static void TRAPV_4E76(ULO*opc_data)
{
	cpuTrapv();
}
static void STOP_4E72(ULO*opc_data)
{
	cpuStop(cpuGetNextWord());
}
static void RESET_4E70(ULO*opc_data)
{
	cpuReset();
}
static void MOVEUSP_4E60(ULO*opc_data)
{
	cpuMoveToUsp(opc_data[0]);
}
static void MOVEUSP_4E68(ULO*opc_data)
{
	cpuMoveFromUsp(opc_data[0]);
}
static void CMPM_B108(ULO*opc_data)
{
	cpuCmpMB(opc_data[0], opc_data[1]);
}
static void CMPM_B148(ULO*opc_data)
{
	cpuCmpMW(opc_data[0], opc_data[1]);
}
static void CMPM_B188(ULO*opc_data)
{
	cpuCmpML(opc_data[0], opc_data[1]);
}
static void RTM_06C0(ULO*opc_data)
{
	cpuRtm(opc_data[0], opc_data[1]);
}
static void PFLUSH040_F500(ULO*opc_data)
{
	cpuPflush040(opc_data[0], opc_data[1]);
}
static void PTEST040_F548(ULO*opc_data)
{
	cpuPtest040(opc_data[0], opc_data[1]);
}
static void ADDX_D100(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAddXB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADDX_D140(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuAddXW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void ADDX_D180(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuAddXL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void ADDX_D108(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA04(opc_data[1], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAddXB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDX_D148(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA04(opc_data[1], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAddXW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void ADDX_D188(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA04(opc_data[1], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuAddXL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void SUBX_9100(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSubXB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUBX_9140(ULO*opc_data)
{
	UWO src = cpuGetDRegWord(opc_data[0]);
	UWO dst = cpuGetDRegWord(opc_data[1]);
	dst = cpuSubXW(dst, src);
	cpuSetDRegWord(opc_data[1], dst);
	cpuSetInstructionTime(4);
}
static void SUBX_9180(ULO*opc_data)
{
	ULO src = cpuGetDReg(opc_data[0]);
	ULO dst = cpuGetDReg(opc_data[1]);
	dst = cpuSubXL(dst, src);
	cpuSetDReg(opc_data[1], dst);
	cpuSetInstructionTime(8);
}
static void SUBX_9108(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA04(opc_data[1], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSubXB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBX_9148(ULO*opc_data)
{
	UWO src = memoryReadWord(cpuEA04(opc_data[0],2));
	ULO dstea = cpuEA04(opc_data[1], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuSubXW(dst, src);
	memoryWriteWord(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SUBX_9188(ULO*opc_data)
{
	ULO src = memoryReadLong(cpuEA04(opc_data[0],4));
	ULO dstea = cpuEA04(opc_data[1], 4);
	ULO dst = memoryReadLong(dstea);
	dst = cpuSubXL(dst, src);
	memoryWriteLong(dst, dstea);
	cpuSetInstructionTime(30);
}
static void ABCD_C100(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuAbcdB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(6);
}
static void ABCD_C108(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA04(opc_data[1], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuAbcdB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void SBCD_8100(ULO*opc_data)
{
	UBY src = cpuGetDRegByte(opc_data[0]);
	UBY dst = cpuGetDRegByte(opc_data[1]);
	dst = cpuSbcdB(dst, src);
	cpuSetDRegByte(opc_data[1], dst);
	cpuSetInstructionTime(6);
}
static void SBCD_8108(ULO*opc_data)
{
	UBY src = memoryReadByte(cpuEA04(opc_data[0],1));
	ULO dstea = cpuEA04(opc_data[1], 1);
	UBY dst = memoryReadByte(dstea);
	dst = cpuSbcdB(dst, src);
	memoryWriteByte(dst, dstea);
	cpuSetInstructionTime(18);
}
static void LSL_E108(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLslB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSL_E148(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLslW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSL_E188(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuLslL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSL_E128(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLslB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSL_E168(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLslW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSL_E1A8(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuLslL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSL_E3D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSL_E3F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuLslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E008(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLsrB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSR_E048(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLsrW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSR_E088(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuLsrL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSR_E028(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuLsrB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void LSR_E068(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuLsrW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void LSR_E0A8(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuLsrL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void LSR_E2D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void LSR_E2F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuLsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E100(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAslB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASL_E140(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAslW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASL_E180(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuAslL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASL_E120(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAslB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASL_E160(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAslW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASL_E1A0(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuAslL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASL_E1D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASL_E1F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAslW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E000(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAsrB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASR_E040(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAsrW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASR_E080(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuAsrL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASR_E020(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuAsrB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ASR_E060(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuAsrW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ASR_E0A0(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuAsrL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ASR_E0D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ASR_E0F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuAsrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E118(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRolB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROL_E158(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRolW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROL_E198(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRolL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROL_E138(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRolB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROL_E178(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRolW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROL_E1B8(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRolL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROL_E7D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROL_E7F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRolW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E018(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRorB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROR_E058(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRorW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROR_E098(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRorL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROR_E038(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRorB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROR_E078(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRorW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROR_E0B8(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRorL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROR_E6D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROR_E6F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRorW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E110(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxlB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXL_E150(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxlW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXL_E190(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxlL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXL_E130(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxlB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXL_E170(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxlW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXL_E1B0(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxlL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXL_E5D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXL_E5F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxlW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E010(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxrB(dst, opc_data[1], opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXR_E050(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxrW(dst, opc_data[1], opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXR_E090(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxrL(dst, opc_data[1], opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXR_E030(ULO*opc_data)
{
	UBY dst = cpuGetDRegByte(opc_data[0]);
	dst = cpuRoxrB(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegByte(opc_data[0], dst);
}
static void ROXR_E070(ULO*opc_data)
{
	UWO dst = cpuGetDRegWord(opc_data[0]);
	dst = cpuRoxrW(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDRegWord(opc_data[0], dst);
}
static void ROXR_E0B0(ULO*opc_data)
{
	ULO dst = cpuGetDReg(opc_data[0]);
	dst = cpuRoxrL(dst, cpuGetDReg(opc_data[1]), opc_data[2]);
	cpuSetDReg(opc_data[0], dst);
}
static void ROXR_E4D0(ULO*opc_data)
{
	ULO dstea = cpuEA02(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4D8(ULO*opc_data)
{
	ULO dstea = cpuEA03(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4E0(ULO*opc_data)
{
	ULO dstea = cpuEA04(opc_data[0], 2);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4E8(ULO*opc_data)
{
	ULO dstea = cpuEA05(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4F0(ULO*opc_data)
{
	ULO dstea = cpuEA06(opc_data[0]);
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4F8(ULO*opc_data)
{
	ULO dstea = cpuEA70();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void ROXR_E4F9(ULO*opc_data)
{
	ULO dstea = cpuEA71();
	UWO dst = memoryReadWord(dstea);
	dst = cpuRoxrW(dst, 1, opc_data[2]);
	memoryWriteWord(dst, dstea);
}
static void MOVEP_0188(ULO*opc_data)
{
	cpuMovepWEa(opc_data[1], opc_data[0]);
}
static void MOVEP_01C8(ULO*opc_data)
{
	cpuMovepLEa(opc_data[1], opc_data[0]);
}
static void MOVEP_0108(ULO*opc_data)
{
	cpuMovepWReg(opc_data[1], opc_data[0]);
}
static void MOVEP_0148(ULO*opc_data)
{
	cpuMovepLReg(opc_data[1], opc_data[0]);
}
static void PACK_8140(ULO*opc_data)
{
	cpuPackReg(opc_data[1], opc_data[0]);
}
static void PACK_8148(ULO*opc_data)
{
	cpuPackEa(opc_data[1], opc_data[0]);
}
static void UNPK_8180(ULO*opc_data)
{
	cpuUnpkReg(opc_data[1], opc_data[0]);
}
static void UNPK_8188(ULO*opc_data)
{
	cpuUnpkEa(opc_data[1], opc_data[0]);
}
#endif

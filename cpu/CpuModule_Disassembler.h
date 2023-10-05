#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t cpuDisOpcode(uint32_t disasm_pc, char *saddress, char *sdata, char *sinstruction, char *soperands);

extern uint32_t cpuDisIllegal(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAbcd(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAdd(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAdda(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAddi(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAddq(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAddx(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAnd(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAndi(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisAsx(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisBcc(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisBt(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisChk(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisClr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisCmp(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisCmpa(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisCmpi(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisCmpm(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisDBcc(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisDivs(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisDivu(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisEor(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisEori(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisExg(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisExt(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisJmp(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisJsr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisLea(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisLink(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisLsx(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMove(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMoveToCcr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMoveToSr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMoveFromSr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMoveUsp(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMovea(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMovem(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMovep(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMoveq(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMuls(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMulu(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisNbcd(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisNeg(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisNegx(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisNop(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisNot(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisOr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisOri(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisPea(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisReset(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisRox(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisRoxx(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisRte(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisRtr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisRts(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisSbcd(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisScc(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisStop(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisSub(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisSuba(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisSubi(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisSubq(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisSubx(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisSwap(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisTas(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisTrap(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisTrapv(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisTst(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisUnlk(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisBkpt(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisBf(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisCas(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisChkl(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisChk2(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisDivl(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisExtb(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisLinkl(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMoveFromCcr(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMovec(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMoves(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisMull(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisPack(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisPflush030(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisPflush040(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisPtest040(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisRtd(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisTrapcc(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisUnpk(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisCallm(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);
extern uint32_t cpuDisRtm(uint32_t prc, uint32_t opc, char *sdata, char *sinstruction, char *soperands);

#ifdef __cplusplus
}
#endif


# ignore built-in rules, don't delete intermediate .o files.

MAKEFLAGS += --no-builtin-rules
.SUFFIXES:
.SECONDARY:

MPWFLAGS = -DMPWVersion=3.2
MPW ?= mpw 

LIBS = \
	{Libraries}Stubs.o \
	{CLibraries}StdCLib.o \
	{Libraries}Interface.o \
	{Libraries}Runtime.o \
	{Libraries}ToolLibs.o

PPC_LIBS = {SharedLibraries}InterfaceLib \
	{SharedLibraries}StdCLib \
	{PPCLibraries}StdCRuntime.o \
	{PPCLibraries}PPCCRuntime.o

LDFLAGS = -w -c 'MPS ' -t MPST \
	-sn STDIO=Main -sn INTENV=Main -sn %A5Init=Main

SCFLAGS = -p

TARGETS = test_new_handle test_new_handle_2 test_new_pointer test_volumes \
	test_createresfile test_hwpriv test_sane

all : $(TARGETS)

clean : 
	rm -f $(TARGETS) o/*

# test_new_handle : o/test_new_handle.o
# 	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 

# test_new_handle_2 : o/test_new_handle_2.o
# 	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 

# test_new_pointer : o/test_new_pointer.o
# 	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 

# test_volumes : o/test_volumes.o
# 	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 

test_sane: o/nan.o o/test_sane.o 
	$(MPW) $(MPWFLAGS) Link $(LDFLAGS) -o $@ $^ $(LIBS) {CLibraries}CSANELib.o

% : o/%.o
	$(MPW) $(MPWFLAGS) Link $(LDFLAGS) -o $@ $^ $(LIBS) 

o/%.o : %.c
	$(MPW) $(MPWFLAGS) SC $(SCFLAGS) $< -o $@

o/%.o : %.asm
	$(MPW) $(MPWFLAGS) Asm $(ASMFLAGS) $< -o $@



# makefile

Libraries=~/mpw/Libraries/Libraries
CLibraries=~/mpw/Libraries/CLibraries

LIBS = 	$(Libraries)/Stubs.o \
	$(CLibraries)/StdCLib.o \
	$(Libraries)/Interface.o \
	$(Libraries)/Runtime.o \
	$(Libraries)/ToolLibs.o

LDFLAGS = -w -c 'MPS ' -t MPST \
	-sn STDIO=Main -sn INTENV=Main -sn %A5Init=Main

all: Help GetEnv

GetEnv: GetEnv.c.o
	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 

Help: Help.c.o
	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 


%.c.o : %.c
	mpw SC -p $< -o $@

# GetEnv.c.o : GetEnv.c
# 	mpw SC -p GetEnv.c -o $@

# Help.c.o : Help.c
# 	mpw SC -p Help.c -o $@
# makefile

Libraries=~/mpw/Libraries/Libraries
CLibraries=~/mpw/Libraries/CLibraries

# MPW 3.2
LIBS = \
	$(Libraries)/Stubs.o \
	$(CLibraries)/StdCLib.o \
	$(Libraries)/Interface.o \
	$(Libraries)/Runtime.o \
	$(Libraries)/ToolLibs.o

LDFLAGS = -w -c 'MPS ' -t MPST \
	-sn STDIO=Main -sn INTENV=Main -sn %A5Init=Main

# MPW 3.5

# LIBS = \
# 	$(CLibraries)/StdCLib.o \
# 	$(Libraries)/Stubs.o \
# 	$(Libraries)/IntEnv.o \
# 	$(Libraries)/MacRuntime.o \
# 	$(Libraries)/Interface.o \
# 	$(Libraries)/ToolLibs.o

# LDFLAGS = -d -c 'MPS ' -t MPST

all: Help GetEnv Duplicate SetFile

clean:
	rm -f *.c.o
	rm -f Help GetEnv Duplicate SetFile

GetEnv: GetEnv.c.o
	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 

Help: Help.c.o
	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 


Duplicate: Duplicate.c.o
	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 


SetFile: SetFile.c.o SetFile-flags.c.o
	mpw Link $(LDFLAGS) -o $@ $^ $(LIBS) 


#SetFile.c : SetFile.rl
#	ragel -G2 -p -m -o $@ $<


%.c.o : %.c
	mpw SC -p $< -o $@

# GetEnv.c.o : GetEnv.c
# 	mpw SC -p GetEnv.c -o $@

# Help.c.o : Help.c
# 	mpw SC -p Help.c -o $@
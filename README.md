MPW Emulator
------------

by Kelvin W Sherlock, _et alia_

Please check the [wiki](https://github.com/ksherlock/mpw/wiki/_pages) for useful information.

## System compatibility

Currently, only OS X 10.8 with case-insensitive HFS+ is supported.

## License

The 680x0 CPU code is from [WinFellow](http://fellow.sourceforge.net) and is 
licensed under GPL v2 or later.  Consequently, the rest of the code is licensed 
under the GPL v2 as well.

The memory allocator (NewHandle/NewPointer) code is from [mempoolite](https://github.com/jeftyneg/mempoolite), 
which is a fork of the SQLite zero-alloc allocator by Jefty Negapatan and D. Richard Hipp.  It, as is SQLite, 
is in the public domain.

## Building

Compiling requires cmake, ragel, lemon, and a recent version of clang++ with 
c++11 support.  It has only been built and tested with OS X 10.8.

mkdir build
cd build
cmake ..
make

This will generate `bin/mpw` and `bin/disasm`.

## Installation

Certain configuration and execution files are generally useful.  They are
stored in an mpw directory, which may be located:

    $MPW (shell variable)
    ~/mpw/ (your home directory)
    /usr/local/share/mpw/
    /usr/share/mpw/

The layout is reminiscent of actual MPW installations.

    mpw/Environment.text
    mpw/Tools/...
    mpw/Interfaces/...
    mpw/Libraries/...
    mpw/Help/...

## Environment file

The Environment.text file is new; it contains MPW environment variables (many
of them set the library and include file locations).  The format is fairly 
simple.

    # this is a comment
    
    #this sets a variable
    name = value
    
    # this sets a variable if it is undefined.
    name ?= value
    
    # values may refer to other variables
    Libraries=$MPW:Libraries:Libraries:
    Libraries=${MPW}:Libraries:Libraries:
    



## Usage

`mpw [mpw flags] command-name [command arguments]`

you may also create shell aliases:

`alias AsmIIgs='mpw AsmIIgs'`

or create a shell script (in `/usr/local/bin`, etc)

`/usr/local/bin/AsmIIgs`:

    #!/usr/bin/sh
    
    exec mpw AsmIIgs $@


mpw looks in the current directory and then in the `$MPW:Tools:` directory
for the command to run.  The MPW `$Commands` variable is not yet supported.


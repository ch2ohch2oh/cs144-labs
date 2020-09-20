- [x] Lab 0: networking warmup (~2 hours)

- [x] Lab 1: stitching substrings into a byte stream (~5 hours)

- [ ] Lab 2: the TCP receiver

- [ ] Lab 3: the TCP sender

- [ ] Lab 4: the TCP connection

- [ ] Lab 5: the network interface

- [ ] Lab 6: the IP router 

For build prereqs, see [the VM setup instructions](https://web.stanford.edu/class/cs144/vm_howto).

## Course materials

### Handouts, assignments and labs

The handouts for fall 2019 can be found here
[here](https://github.com/ch2ohch2oh/cs144.github.io/tree/96579e7f63b6ea67164e3dca0735de83a523a240). 
It is a good idea to
make a fork of the official course repo as no one knows when
the instructors decide to take it down. Since the repo now 
is public, your fork will still be there even if the original
repo is deleted or made private. 

### Video lectures

The video lectures can be found on 
[bilibili](https://www.bilibili.com/video/BV137411Z7LR)
and youtube.

## Sponge quickstart

On my local machine (ubuntu 20.04), I had to install the following 
packages.

    $ apt install cmake doxygen graphviz libpcap-dev clang-format


To set up your build directory:

	$ mkdir -p <path/to/sponge>/build
	$ cd <path/to/sponge>/build
	$ cmake ..

**Note:** all further commands listed below should be run from the `build` dir.

To build:

    $ make

You can use the `-j` switch to build in parallel, e.g.,

    $ make -j$(nproc)

To test (after building; make sure you've got the [build prereqs](https://web.stanford.edu/class/cs144/vm_howto) installed!)

    $ make check

The first time you run `make check`, it will run `sudo` to configure two
[TUN](https://www.kernel.org/doc/Documentation/networking/tuntap.txt) devices for use during
testing.

### build options

You can specify a different compiler when you run cmake:

    $ CC=clang CXX=clang++ cmake ..

You can also specify `CLANG_TIDY=` or `CLANG_FORMAT=` (see "other useful targets", below).

Sponge's build system supports several different build targets. By default, cmake chooses the `Release`
target, which enables the usual optimizations. The `Debug` target enables debugging and reduces the
level of optimization. To choose the `Debug` target:

    $ cmake .. -DCMAKE_BUILD_TYPE=Debug

The following targets are supported:

- `Release` - optimizations
- `Debug` - debug symbols and `-Og`
- `RelASan` - release build with [ASan](https://en.wikipedia.org/wiki/AddressSanitizer) and
  [UBSan](https://developers.redhat.com/blog/2014/10/16/gcc-undefined-behavior-sanitizer-ubsan/)
- `RelTSan` - release build with
  [ThreadSan](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/Thread_Sanitizer)
- `DebugASan` - debug build with ASan and UBSan
- `DebugTSan` - debug build with ThreadSan

Of course, you can combine all of the above, e.g.,

    $ CLANG_TIDY=clang-tidy-6.0 CXX=clang++-6.0 .. -DCMAKE_BUILD_TYPE=Debug

**Note:** if you want to change `CC`, `CXX`, `CLANG_TIDY`, or `CLANG_FORMAT`, you need to remove
`build/CMakeCache.txt` and re-run cmake. (This isn't necessary for `CMAKE_BUILD_TYPE`.)

### other useful targets

To generate documentation (you'll need `doxygen`; output will be in `build/doc/`):

    $ make doc

To view the generated docs:

    $ python3 -m http.server --directory doc/html

To lint (you'll need `clang-tidy`):

    $ make -j$(nproc) tidy

To run cppcheck (you'll need `cppcheck`):

    $ make cppcheck

To format (you'll need `clang-format`):

    $ make format

To see all available targets,

    $ make help

Lab 0 Writeup
=============

The main difficulty is to set up the envinronment and make the first successful build.

My machine has ubuntu 18 installed. Some of the issues I encountered:
- `gcc7` is intalled on my machine so I need to install `gcc8` and use
  `update-alternatives` to select the correct `gcc` version.
- `clang` gives compile errors but `gcc` does not. No idea why.
- Install `doxygen` and `graphviz` library.
- Install `libpcap-dev`, which is a network lib.

The `webget` app is easy to implement. Just make sure you print what you 
received as-is and do not add extra `\n`.

The `lab0` is slightly harder. The spec is pretty slim and I have to make 
some guesses about the program should do under corner cases. Good news is 
there are plenty of test cases. Make sure to run `make` before `make check_lab0`.


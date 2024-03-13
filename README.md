# chestnut

For Windows you need to install [Microsoft Visual C++
Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe).

## Quick Start

Clone:

    git clone https://github.com/andreymlv/chestnut.git --recurse-submodule
    cd chestnut

Build on Linux:

    cmake --preset=Unix-Release
    cd build
    make -j$(nproc)

Build on Windows:

    cmake --preset=MSVC-Release
    cd build
    ninja


## Discover server

Discover server is a simple Rust program that ignore all input from socket and
just return all socket addresses separated by comma (`,`).

## TODO

- [ ] Android build
- [ ] Opus encoding and decoding
- [ ] On client side show bit-rate


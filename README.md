# APEX OS
The new bleeding-edge OS designed to surpass Windows, Linux, _and_ MacOS. Just kidding. This is just a project I work on in my spare time.

## General Info
This is a project I'm working on because it is my hobby, as such I'm currently maintaining exclusive control over the code base.

You are, however, encouraged to go through the source code and take a look around, whether or not it's just out of curiosity, figuring out how something *could* be implemented, or otherwise. Just know that the implementation of the STL is very sparse (maybe look at [libstdc++](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3))

Please use this code only as a single example of how a give concept _could_ be implemented. Your platform ~probably~ will differ. You have been warned.

Good Luck!

## Project layout
`/`
- `disk/` Files that need to be written to the boot disk in addition to source code (currently just config files for grub)
- `src/` The source code for the OS
  - `libapex++/` The STL implementation for the OS
  - `libapex/` A low-level helper library for the OS
  - `libscreen/` The VGA terminal screen manager for the OS
  - `*` The kernel source files

## Build info

Currently I'm developing this project on a custom Arch Linux (what install of Arch isn't?) USB.
This should build perfectly however, given that a version of GCC built for i386-elf is present in your path.

In order to use the VM targets, QEMU is necessary, and in order to use the debug targets, GDB is also needed.

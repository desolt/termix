# Termix
**OpenGL powered terminal emulator for OSX and Linux**

Actively developed on https://discord.gg/CXJjcrX.

Requirements:
* OpenGL 2.1 compatible video card
* Mac OSX (10.7 Lion and later) or Linux

## Compiling

First, install `glfw` and `freetype` using your favourite package manager:
```sh
brew install glfw freetype                          # Homebrew - OSX
sudo apt-get install libglfw3-dev libfreetype6-dev  # APT - Debian
sudo pacman -S glfw-x11 freetype2                   # Pacman - Arch
sudo emerge media-libs/glfw media-libs/freetype     # Portage - Gentoo
```
Then run `make` to compile. It produces the executable at `build/termix`. Running `make run` compiles and runs the executable in one step.

## Contributing

Please adhere to the style used in the rest of the repository:
* C99 (`--std=c99`)
* Allman style braces
* Pointer stars separated by a space on each side

You should run `make format` before sending a pull request (requires [`astyle`](http://astyle.sourceforge.net/)). See `.astyle` for more details. You can ensure that all your code is formatted nicely by adding a git pre-commit hook, placing the following in `.git/hooks/pre-commit`:
```sh
#!/bin/sh
make format
exit $?
```
You will then need to run `chmod +x .git/hooks/pre-commit` to give the file execute permission (otherwise git won't try and run it).

If you have any questions, feel free to ask us [here on Discord](https://discord.gg/CXJjcrX) or open an issue.

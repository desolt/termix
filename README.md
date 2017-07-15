# Termix
**OpenGL powered terminal emulator for OSX and Linux**

Actively developed on https://discord.gg/CXJjcrX.

Requirements:
* OpenGL 2.1 compatible video card
* Linux or Mac OSX (10.7 Lion and later)
* Dependencies: `glfw`, `freetype`

## Compiling

First, install `glfw` and `freetype` using your favourite package manager:
```sh
# Linux
apt-get install libglfw3-dev libfreetype6-dev  # APT - (Debian, Ubuntu)
pacman -S glfw-x11 freetype2                   # Pacman - Arch
emerge media-libs/glfw media-libs/freetype     # Portage - Gentoo

# OS X
brew install glfw freetype                      # Homebrew
```
Then run `make` to compile. It produces the executable at `build/termix`. Running `make run` compiles and runs the executable in one step.

## Contributing

For contribution information please read [CONTRIBUTING.md](https://github.com/Bfgeshka/termix/blob/master/CONTRIBUTING.md)

---

If you have any questions, feel free to ask us [here on Discord](https://discord.gg/CXJjcrX) or open an issue.

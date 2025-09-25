![](assets/images/logobg.svg)

**FORMA** is a digital emulation of a transistor combo organ manufactured in the GDR during the 1980s. It focuses on a specific single-manual model, though voicings from other models may be added in the future.

Implemented features:

* High-quality bandlimited SIMD oscillators.

* Polysynth-style voice allocation with oscillators across voices synced so they won't cancel each other out.

* (Mostly) Accurate *FLUTES II* frequency response voicing from the **Formation 1** model. 5⅓′and 1⅗′ stops are voiced to be a little bit duller. Slightly nonlinear amplifying stage is included.

* Realistic janky keyboard action in which stops are not activated simultaneously. Velocity-sensitive.

* Noise floor and hum (gated).

Planned features (high to low priority):

* Noisy potentiometers.

* *SOLO* stops from the **Formation 1**: *Clarinet, Oboe, and Nasal*. Including the non-linear behaviour of the formant filters.

* Voicings from the **Formation 2** model.

* Voicings from the **ET 6-1** model.

* Percussion section.

* Phaser section.

* Spring Reverb.

# Building from source

Tested on Linux Mint and Windows 10 with VSCode.

```
git clone https://github.com/ath-r/forma.git
git submodule update --init --recursive
```

## Linux
```
sudo apt update
sudo apt install build-essential gdb
sudo apt install pkg-config
sudo apt install clang-tools-14 lld-14 llvm-dev
```
Then configure cmake and compile with Clang.

## Windows

On Windows, you will need to use [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) to get MSVC and Windows SDK (installed with "Desktop Development with C++") **and also** MSBuild support for LLVM (clang-cl) toolset **and** C++ Clang Compiler for Windows (can be installed from "Individual Components"). Then compile with Clang (MSVC CLI) amd64.
![](assets/images/buildtoolsinstallation.png)
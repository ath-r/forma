![](assets/images/logobg.svg)

# Table of Contents <!-- omit from toc -->
- [Description](#description)
  - [Implemented features](#implemented-features)
  - [Signal Flow](#signal-flow)
- [Описание](#описание)
- [Building from source](#building-from-source)
  - [Linux](#linux)
  - [Windows](#windows)

# Description

**FORMA** is a digital emulation of a transistor combo organ manufactured in the GDR during the 1980s. It focuses on a specific single-manual **Formation 1** model, though voicings from other models may be added in the future.

## Implemented features

* High-quality bandlimited oscillators. Four low octaves are computed using original Sinc Integral algorithm, and the rest is computed with an optimized additive algorithm. Deviation $(\pm0.5¢)$ from 12EDO tuning is preserved based on data from MM5555/MM5556 Chromatic Frequency Generators datasheet. 

* Organ-style voice allocation. Like in the real thing, $12 \times 9$ oscillators are active at all times, then their signals are routed to keyswitches.

* (Mostly) Accurate *FLUTES II* frequency response voicing from the **Formation 1** model with nonlinear behaviour per filter bank.

* Janky keyboard action like in the original organ: six switches under each key close non-simultaneously. Velocity-sensitive.

## Signal Flow

![](media/signalflow.drawio.png)

# Описание
**FORMA** $-$ цифровая имитация транзисторного органа, который производился в ГДР в 80х. За основу взята модель **Formation 1** с одним мануалом.
* Качественные тон-генераторы без цифровых артефактов. Сохранены отклонения микросхем хроматических генераторов от стандартного строя.
* Как в настоящем органе, все генераторы и делители работают непрерывно, и их сигналы распределяются по контактным группам.
* Имитация механики оригинального органа: шесть контактов под каждой клавишей могли замыкаться не одновременно, из-за чего шесть регистров вступали вразнобой.
* Фильтры флейтовых регистров настроены как в оригинале и сохраняют нелинейность транзисторной схемы.

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
Then configure cmake and compile with Clang. Use Clangd for code completion.

## Windows

On Windows, you will need to use [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) to get MSVC and Windows SDK (installed with "Desktop Development with C++") **and also** MSBuild support for LLVM (clang-cl) toolset **and** C++ Clang Compiler for Windows (can be installed from "Individual Components").

![](media/buildtoolsinstallation.png)

 Then compile with Clang (MSVC CLI) amd64. Use Intellisense for code completion.
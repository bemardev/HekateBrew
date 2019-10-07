# HekateBrew

## Features

HekateBrew is a simple Nintendo Switch Homebrew for 
- Launching Hekate configs described in [hekate](https://github.com/CTCaer/hekate) config file and ini directory.

*(config logos are shown if they are defined as `icon=` in ini file)*
- Launching payloads from [hekate](https://github.com/CTCaer/hekate) payloads directory.

*(payload logos are shown if they exist in payload folder with the same name and `.bmp` or `.png` extension)*
- Launching payloads from [Argon-NX](https://github.com/Guillem96/argon-nx) payloads directory.

*(payload logos are shown if they exist in Argon-NX logos directory)*

- Launching payloads from `/payloads/` and any custom directory defined

## Configuration

Extract HekateBrew zip release to the sd card root. (A settings file is automatically created on launch if not found)

*(Or, place `HekateBrew.nro` in switch directory and last Hekate payload, renamed `payload.bin`, inside `switch\HekateBrew` directory in your sd card)*

## Preview

### *HekateBrew main menu - Dark theme*
![mainPage_dark](https://user-images.githubusercontent.com/55793274/66199590-3e3ec380-e69f-11e9-9469-4a68bd7c4eaa.jpg)

### *HekateBrew config page - Dark theme*
![configPage_dark](https://user-images.githubusercontent.com/55793274/66199599-4139b400-e69f-11e9-9972-3f25446e2f57.jpg)

### *HekateBrew payload page - Dark theme*
![payloadPage_dark](https://user-images.githubusercontent.com/55793274/66199606-439c0e00-e69f-11e9-8e15-f75d7ecff96f.jpg)

### *HekateBrew settings page - Dark theme*
![optionsPage1_dark](https://user-images.githubusercontent.com/55793274/66199612-45fe6800-e69f-11e9-944e-e48fe7f32f73.jpg)

### *HekateBrew settings page - Dark theme*
![optionsPage3_dark](https://user-images.githubusercontent.com/55793274/66199613-4860c200-e69f-11e9-95a0-d9089fa7d9ee.jpg)

## Credits

* __switchbrew__ for the [libnx](https://github.com/switchbrew/libnx) project and switch-examples
* __devkitPro__ for the [devkitA64](https://devkitpro.org/) toolchain and libnx support
* __CTCaer__ for the continued [hekate](https://github.com/CTCaer/hekate) and storage in payload binary
* __XorTroll__ for [Plutonium](https://github.com/XorTroll/Plutonium)
* __SciresM__ for [Atmosphere](https://github.com/Atmosphere-NX/Atmosphere)

![Version](https://img.shields.io/badge/Version-v0.0.4-blue)
![Compiler](https://img.shields.io/badge/Compiler-gcc-green)
![License](https://img.shields.io/badge/License-GPLv3-blue)
![Language](https://img.shields.io/badge/Language-C-yellow)

# kernel.f
(Kernel-dot-F) Multitask kernel project

# Build

Right after cloning the repository, if the bin and obj directories do not exist, the following command must be executed:

```sh
./init.sh
```

The following command compiles the kernel and generates the kernel8.img file in the bin directory:

```sh
python3 build.py
```

The following command starts the emulation:

```sh
./qemurpi3.sh
```

# Project Owner
* Filipe Chagas [https://github.com/filipechagasdev, filipe.ferraz0@gmail.com]

## Features
* [x] UART I/O (done at v0.0.1)
* [x] Physical memory pages alloc-free (done at v0.0.2)
* [x] PageTable create-delete-map-unmap (done at v0.0.3)
* [x] PageTable enable-switch (to do at v0.0.4)
* [ ] Kernel malloc-free
* [ ] Other features... 

## Credits

* Leon de Boer [https://github.com/LdB-ECM, ldeboer@gateway.net.au]

Some code used here was taken and adapted from the Raspi3-kernel project (https://github.com/LdB-ECM/Raspi3-Kernel).

* Rene Stange [https://github.com/rsta2, rsta2@o2online.de]

Some code used here was taken and adapted from the Circle project (https://github.com/rsta2/circle).

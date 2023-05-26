<details>

<summary>Linux
</summary>

Firstly you need to install **gcc**, **make**. Any version, which supports c++-17, but we recommend to use gcc-8 and above.

```bash
apt-get update && apt-get install python3 python3-distutils make git gcc-8 g++-8 libz-dev wget unzip libtinfo-dev libssl-dev -y
ln -sf /usr/bin/gcc-8 /usr/bin/gcc && ln -sf /usr/bin/g++-8 /usr/bin/g++
ln -sf /usr/bin/gcc-8 /usr/bin/cc && ln -sf /usr/bin/g++-8 /usr/bin/c++
```

Install **cmake**, at least version 3.25.0.

```bash
wget https://github.com/Kitware/CMake/archive/refs/tags/v3.25.0.zip
unzip v3.25.0.zip && cd v3.25.0.zip
mkdir build && cd build
../configure
make -j <your_num_of_cores>
make install
```

</details>




<details>

<summary>Windows
</summary>

* Install [mingw](https://www.mingw-w64.org/downloads/#msys2) with support of c++-17. Add <your_mingw_root_installation_dir>\mingw64\bin to your environmental variable PATH, if not presented already.
* Install [cmake](https://cmake.org/download/) at least version 3.25.1. Add <your_cmake_root_installation_dir>\bin to your environmental variable PATH, if not presented already.

</details>

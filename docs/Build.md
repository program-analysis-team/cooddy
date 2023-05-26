## Setup environment

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

## Build 3rd party dependencies

Cooddy uses standalone libraries of **clang** and **z3**, so manual build of specified versions is required. Now Cooddy is based on LLVM 14.0 and z3 4.8.9. As far as backward compatibility of LLVM project is not guaranteed we recommend to use these versions.#### Build z3

More info about z3 build: [here](https://github.com/Z3Prover/z3).



<details>

<summary>Windows (MINGW)
</summary>

```bash
wget https://github.com/Z3Prover/z3/archive/refs/tags/z3-4.8.9.zip
unzip z3-4.8.9.zip && cd z3-4.8.9
mkdir build && cd build
cmake -DZ3_BUILD_LIBZ3_SHARED=TRUE -G "CodeBlocks - MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=<path_to_z3_installation> -DCMAKE_BUILD_TYPE=Release ../
make -j <your_cores_num>
make install
export Z3_HOME=<path_to_z3_installation>
```

</details>



<details>

<summary>Linux
</summary>

```bash
wget https://github.com/Z3Prover/z3/releases/download/z3-4.8.9/z3-4.8.9-x64-ubuntu-16.04.zip
unzip z3-4.8.9-x64-ubuntu-16.04.zip
export Z3_HOME=<path_to_z3_installation>
```

</details>

#### Build Clang

More info about clang build: [here](https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm).

```bash
wget https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-14.0.0.zip
unzip llvmorg-14.0.0.zip && cd llvmorg-14.0.0
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_TERMINFO=OFF -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=<path_to_clang_installation> -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF ../llvm
make -j <your_num_of_cores> && make install
export LLVM_CMAKE=<path_to_clang_installation>lib/cmake
```

After that you can start build a Cooddy project.

### Prepare and build release version of Cooddy

> ⚠️ Make sure that environmental variables Z3_HOME and LLVM_CMAKE are set




<details>

<summary>Linux
</summary>


```bash
git clone https://github.com/program-analysis-team/cooddy.git && cd cooddy
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -G 'CodeBlocks - Unix Makefiles' ../
make -j <your_cores_num>
```

</details>




<details>

<summary>Windows
</summary>

```bash
git clone https://github.com/program-analysis-team/cooddy.git && cd cooddy
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - MinGW Makefiles" ../
make -j <your_cores_num>
```

</details>


If some change of project is required, you can manage creation of CMake project by using following options:




<details>

<summary>List of CMake options
</summary>

| Option        | Description                                                                                                           | Usage example                                                                                                          |
|---------------|-----------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|
| Z3_HOME       | path to Z3 installation, required if environmental variable was not set                                               | cmake -DZ3_HOME=<path_to_your_installation> -DCMAKE_BUILD_TYPE=Release -G 'CodeBlocks - Unix Makefiles' .              |
| LLVM_CMAKE    | path to cmake installation, required if environmental variable was not set                                            | cmake -DLLVM_CMAKE=<path_to_your_installation>/lib/cmake -DCMAKE_BUILD_TYPE=Release -G 'CodeBlocks - Unix Makefiles' . |
| USE_SANITIZER | Enable Asan sanitizer. Use **only** in debug build, since it can greatly slow down analyses time. Disabled by default | cmake -DCMAKE_BUILD_TYPE=Debug -G 'CodeBlocks - Unix Makefiles' -DUSE_SANITIZER=ON .                                   |

</details>

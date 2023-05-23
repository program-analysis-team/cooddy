Cooddy uses standalone libraries of **clang** and **z3**, so manual build of specified versions is required. Now Cooddy is based on LLVM 14.0 and z3 4.8.9. As far as backward compatibility of LLVM project is not guaranteed we recommend to use these versions.

#### 
 Build z3


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


```
wget https://github.com/Z3Prover/z3/releases/download/z3-4.8.9/z3-4.8.9-x64-ubuntu-16.04.zip
unzip z3-4.8.9-x64-ubuntu-16.04.zip
export Z3_HOME=<path_to_z3_installation>
```


</details>

#### 
 Build Clang


More info about clang build: [here](https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm).




```bash
wget https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-14.0.0.zip
unzip llvmorg-14.0.0.zip && cd llvmorg-14.0.0
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_TERMINFO=OFF -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=<path_to_clang_installation> -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF ../llvm
make -j <your_num_of_cores> && make install
export LLVM_CMAKE=<path_to_clang_installation>lib/cmake
```

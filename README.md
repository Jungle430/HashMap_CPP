# HashMap CPP

![C++](https://img.shields.io/badge/HashMap-C++-informational?style=flat-square&logo=cplusplus&logoColor=white&color=2bbc8a)


```shell
 _   _              _      __  __                 __ _  __ __     ____    _    ____               
| | | |  __ _  ___ | |__  |  \/  |  __ _  _ __   / /| |/ / \ \   / /\ \  | |  / ___|   _      _   
| |_| | / _` |/ __|| '_ \ | |\/| | / _` || '_ \ / / | ' /   \ \ / /  \ \ | | | |     _| |_  _| |_ 
|  _  || (_| |\__ \| | | || |  | || (_| || |_) |\ \ | . \    \ V /   / / | | | |___ |_   _||_   _|
|_| |_| \__,_||___/|_| |_||_|  |_| \__,_|| .__/  \_\|_|\_\    \_/   /_/  | |  \____|  |_|    |_|  
                                         |_|                             |_|                      
```

This C++ library provides an efficient HashMap implementation designed to offer higher efficiency than the standard library std::unordered_map when dealing with large datasets, while being on par with the efficiency of std::map. Additionally, the library can be easily included by simply importing the header file, without the need for complex installation or configuration processes.

## Features

- **High Performance:** The HashMap implementation is optimized to maintain high efficiency when processing large-scale data.
- **Compatibility with Standard Library:** Compatible with the interface of std::unordered_map, making it easy to replace existing code.
- **Simple Inclusion:** Just include the header file, no need for cumbersome setup or configuration.

## BenckMark Test

```shell
git clone https://github.com/Jungle430/HashMap_CPP.git
git submodule init
git submodule update
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
HASHMAP_CPP
```

Test effect

```md
Run on (16 X 3293.84 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 512 KiB (x8)
  L3 Unified 16384 KiB (x1)
Load Average: 2.40, 1.76, 1.38
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
CustomHashMapBenchmark         3882636393 ns   3879073300 ns            1
CustomStdUnorderedMapBenchmark 7883063037 ns   7883028200 ns            1
CustomStdMapBenchmark          4078177020 ns   4078161000 ns            1
```

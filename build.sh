mkdir build
cd build
cmake .. -GNinja -DCMAKE_CXX_COMPILER=clang++-22 -DCMAKE_CXX_FLAGS="-stdlib=libc++"
cmake --build .
./modulesUI
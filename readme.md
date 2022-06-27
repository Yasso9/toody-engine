# Toody Engine

Game engine for 2D games made with C++ and SFML. <br>
Projet d'un moteur 2D ecrit en C++ en utlisant la SFML. <br>

- Format all files
clang-format -i -style=file sources/**

## Compilation

### Prerequisite for building:
Windows : install gcc 12.1.0 or clang 14.0.4 and have it in PATH. <br>
Linux : Have clang++ and clang available (14 verified). <br>
MacOs : Not taken in charge.

### Libraries

1. SFML-2.6.x
    1. Windows (64 bits)
        > Libraries already present in the package
        > If not - Build SFML
        mkdir build
        cd build
        CC=gcc CXX=g++ cmake -G"Unix Makefiles" -DWARNINGS_AS_ERRORS=OFF ..
        make all
    2. Linux
        > sudo apt-get install libsfml-dev

2. Assimp (5.2.4)
    1. Windows (64 bits)
        > Libraries already present in the package
        > If not - Build Assimp
        mkdir build
        cd build
        CC=gcc CXX=g++ cmake -G"Unix Makefiles" ..
        cmake --build .

3. Glad and Sqlite <br>
    Makefile already run them

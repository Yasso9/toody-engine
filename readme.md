# Toody Engine

Game engine for 2D games made with C++ and SFML. <br>
Projet d'un moteur 2D ecrit en C++ en utlisant la SFML. <br>

- Format all files
clang-format -i --verbose -style=file:.clang-format $(find . -regex './sources/.*\.\(cpp\|hpp\|cc\|cxx\)')

make 2> iwyu_file.txt
sed -i '/.inl/d' ./iwyu_file.txt
python3 fix_includes.py --comments --update_comments < iwyu_file.txt

cppclean --verbose --include-path=sources --include-path=external/includes sources/**

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/glm.hpp>

#include <GLM/gtc/type_ptr.hpp>

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

3. Glad and Sqlite
    Makefile already run them

4. Moreover installation on linux
    libminizip-dev
    freeglut3-dev

# todo

Test the code with valgrind

Test the code with cpp checker

Réarenger les fichier dans la racine du projet pour qu'on en ai le moins possible

Add Google Test to the project

git submodule update --init --recursive

- library format
- utiliser CMake
- Revoir input.cpp
- descriptor de settings dispo dans tt le programme
- avoir des subdirectories pour assimp, sfml, glm, etc ...


git submodule add https://github.com/fmtlib/fmt.git submodules/fmt

git submodule update --init --recursive

cd deps/tinyxml2
git checkout 6.2.0
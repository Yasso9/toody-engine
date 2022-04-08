# Toody Engine

Game engine for 2D games made with C++ and SFML. <br>
Projet d'un moteur 2D ecrit en C++ en utlisant la SFML. <br>

cmake -D ASSIMP_BUILD_ASSIMP_TOOLS=OFF ./assimp-5.2.3 ./assimp-build

- Have the script available for compilation in Linux/MaxOs
chmod u+x ./tools/compile_libraries.sh

- Faire un format de tout les fichiers
clang-format -i -style=file sources/*/*.cpp sources/*/*.hpp sources/*.cpp

## Compilation

### Prerequisite for building:
Windows : install MinGW 11.2 or LLVM 13 and have clang in PATH. <br>
Linux : Have clang++ and clang available (14 verified). <br>
MacOs : Not taken in charge.

### Libraries

1. SFML (2.5.1)
    1. Windows
        Déjà compris dans le package
    2. Linux
        > sudo apt-get install libsfml-dev

2. Assimp (5.1.5)
Source code (All OS) : cmake and make

3. Glad and Sqlite <br>
    To compile the run : <br>
    > python3 ./tools/compile_libraries.py <path/to/object_libraries> <path/to/glad> <path/to/sqlite> <br>
    For exemple in Ubuntu : <br>
    > python3 ./tools/compile_libraries.py ./external/object /home/ineo/Developpement/CPP_LIBRAIRIES/GLAD /home/ineo/Developpement/CPP_LIBRAIRIES/SQLITE <br>
    Or in windows : <br>
    > python ./tools/compile_libraries.py ./external/object C:/Users/Turki/GoogleDrive/Programs/Programming/CPP_LIBRAIRIES/GLAD C:/Users/Turki/GoogleDrive/Programs/Programming/CPP_LIBRAIRIES/SQLITE <br>

4. Sqlite
GLAD sources (All OS) : https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on
SQLITE sources and precompiled binaries (All OS) : https://www.sqlite.org/download.html


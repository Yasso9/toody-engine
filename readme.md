Toody Engine

Game engine for 2D games made with C++ and SFML

Projet d'un moteur 2D ecrit en C++ en utlisant la SFML

cmake -D ASSIMP_BUILD_ASSIMP_TOOLS=OFF ./assimp-5.2.3 ./assimp-build

- Have the script available for compilation in Linux/MaxOs
chmod u+x ./tools/compile_libraries.sh

- Faire un format de tout les fichiers
clang-format -i -style=file sources/*/*.cpp sources/*/*.hpp sources/*.cpp
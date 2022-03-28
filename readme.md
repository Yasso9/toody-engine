Toody Engine

Game engine for 2D games made with C++ and SFML

Projet d'un moteur 2D ecrit en C++ en utlisant la SFML

powershell

cd "C:/Users/Turki/GoogleDrive/Programs/Windows/Programming/C and C++/Tools/cppclean"

py cppclean --include-path=entity --include-path=graphics --include-path=input --include-path=main --include-path=output_info --include-path=ressources_handling--include-path=settings --include-path=states --include-path=tilemap --include-path=tools C:/Users/Turki/GoogleDrive/Computing/Projects/ToodyEngine/sources/project

cd "C:/Developpement/CPP_LIBRAIRIES/GLAD"
clang -c glad/glad.c -o glad.o -I"./"
cp glad.o "C:/Users/Turki/GoogleDrive/Computing/Projects/ToodyEngine/object_libraries"
cd "C:/Developpement/CPP_LIBRAIRIES/SQLITE/sources/SQLITE"
clang -c sqlite3.c -o sqlite3.o
cp sqlite3.o "C:/Users/Turki/GoogleDrive/Computing/Projects/ToodyEngine/object_libraries"
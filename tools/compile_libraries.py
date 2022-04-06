import os
import pathlib

# Project Directory is in the parent folder of this file
projectDirectoryPath = str(pathlib.Path().absolute().parents[0])
librariesObjectPath = projectDirectoryPath + "/external/object"


def compile_glad(gladDirectory):
    # Filename we need to generate
    gladObjectFileName = "glad.o"

    # Compile Glad
    os.system(
        f'clang -c {gladDirectory}/src/glad.c -o {gladObjectFileName} -I"{gladDirectory}/include"'
    )

    # Delete object file if he already exist
    oldGladObjectPath = librariesObjectPath + "/" + gladObjectFileName
    if os.path.isfile(oldGladObjectPath):
        os.remove(oldGladObjectPath)

    # # Copy object file to oppropriate directory
    os.system(f"mv {gladObjectFileName} {librariesObjectPath}")


def compile_sqlite(sqliteDirectory):
    # Filename we need to generate
    sqliteObjectFileName = "sqlite.o"

    # Compile Glad
    os.system(f"clang -c {sqliteDirectory}/sqlite3.c -o {sqliteObjectFileName}")

    # Delete object file if he already exist
    oldSqliteObjectPath = librariesObjectPath + "/" + sqliteObjectFileName
    if os.path.isfile(oldSqliteObjectPath):
        os.remove(oldSqliteObjectPath)

    # # Copy object file to oppropriate directory
    os.system(f"mv {sqliteObjectFileName} {librariesObjectPath}")


# gladDirectory = "/home/ineo/Developpement/CPP_LIBRAIRIES/GLAD"
# sqliteDirectory = "/home/ineo/Developpement/CPP_LIBRAIRIES/SQLITE"

gladDirectory = "C:/Users/Turki/GoogleDrive/Programs/Programming/CPP_LIBRAIRIES/GLAD"
sqliteDirectory = "C:/Users/Turki/GoogleDrive/Programs/Programming/CPP_LIBRAIRIES/SQLITE"

compile_glad(gladDirectory)
compile_sqlite(sqliteDirectory)


# gladPath = "C:/Developpement/CPP_LIBRAIRIES/GLAD"
# sqlitePath = "C:/Developpement/CPP_LIBRAIRIES/SQLITE"


# cd "C:/Developpement/CPP_LIBRAIRIES/GLAD"
# clang -c glad/glad.c -o glad.o -I"./"
# cp glad.o "C:/Users/Turki/GoogleDrive/Computing/Projects/ToodyEngine/object_libraries"
# cd "C:/Developpement/CPP_LIBRAIRIES/SQLITE/sources/SQLITE"
# clang -c sqlite3.c -o sqlite3.o
# cp sqlite3.o "C:/Users/Turki/GoogleDrive/Computing/Projects/ToodyEngine/object_libraries"

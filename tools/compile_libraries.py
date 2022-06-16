import os
import pathlib
import sys
from dataclasses import dataclass


@dataclass
class Paths:
    objectCreated: str
    gladLibrary: str
    sqliteLibrary: str


def launch_error(errorMessage: str):
    print("ERROR :" + errorMessage, file=sys.stderr)
    sys.exit(1)


def extract_arguments():
    argumentList = sys.argv[1:]

    if len(argumentList) != 3:
        launch_error("This script should have 3 args.")

    for argument in argumentList:
        if not os.path.isdir(argument):
            launch_error(f"All the args should be directories that exist : '{argument}'")

    return Paths(argumentList[0], argumentList[1], argumentList[2])


def compile_glad(objectCreatedDirectory, gladDirectory):
    # Filename we need to generate
    gladObjectFileName = "glad.o"

    # Compile Glad
    os.system(
        f'clang -c {gladDirectory}/src/glad.c -o {gladObjectFileName} -I"{gladDirectory}/include"'
    )

    # Delete object file if he already exist
    oldGladObjectPath = objectCreatedDirectory + "/" + gladObjectFileName
    if os.path.isfile(oldGladObjectPath):
        os.remove(oldGladObjectPath)

    # # Copy object file to oppropriate directory
    os.system(f"mv {gladObjectFileName} {objectCreatedDirectory}")


def compile_sqlite(objectCreatedDirectory, sqliteDirectory):
    # Filename we need to generate
    sqliteObjectFileName = "sqlite.o"

    # Compile Glad
    os.system(f"clang -c {sqliteDirectory}/sqlite3.c -o {sqliteObjectFileName}")

    # Delete object file if he already exist
    oldSqliteObjectPath = objectCreatedDirectory + "/" + sqliteObjectFileName
    if os.path.isfile(oldSqliteObjectPath):
        os.remove(oldSqliteObjectPath)

    # # Copy object file to oppropriate directory
    os.system(f"mv {sqliteObjectFileName} {objectCreatedDirectory}")

def compile_imgui():
    imGuiPath = "./external/includes/IMGUI"
    objectPath = "./external/object"
    os.system(f"make -C {imGuiPath}")
    os.system(f"mv {imGuiPath}/*.o {objectPath}")

if __name__ == "__main__":
    paths = extract_arguments()

    compile_glad(paths.objectCreated, paths.gladLibrary)
    compile_sqlite(paths.objectCreated, paths.sqliteLibrary)

    compile_imgui()

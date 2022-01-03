############################## Global Variable ##############################

# TYPO regarder le makefile du projet de groupe automate pour voir les petits détatils qu'on a rajouté

# g++
# CXX_COMMAND := g++
# CXX_FLAGS := $(CXX_COMMAND) -std=c++2a

# clang
CXX_COMMAND := clang++
CXX_FLAGS := -std=c++20 -MD -O0 -g
# -MD => Create .d files for dependencies

WARNINGS := -pedantic -Wpedantic -pedantic-errors \
-Wall -Wextra \
-Wcast-align \
-Wcast-qual \
-Wctor-dtor-privacy \
-Wdisabled-optimization \
-Wformat=2 \
-Winit-self \
-Wmissing-declarations \
-Wmissing-include-dirs \
-Wold-style-cast \
-Woverloaded-virtual \
-Wredundant-decls \
-Wshadow \
-Wsign-conversion \
-Wsign-promo \
-Wstrict-overflow=5 \
-Wswitch-default \
-Wundef \
-Wno-unused \
-Wconversion \
-Wfloat-equal \
-Wformat-nonliteral \
-Wundef \
-Wformat=2 \
-Wformat-security  \
-Wformat-y2k \
-Wimport \
-Winline \
-Winvalid-pch \
-Wmissing-field-initializers \
-Wmissing-format-attribute   \
-Wmissing-noreturn \
-Wpacked \
-Wpointer-arith \
-Wstack-protector \
-Wstrict-aliasing=2  \
-Wunreachable-code \
-Wunused \
-Wvariadic-macros \
-Wwrite-strings \
-Weffc++ \
-Werror \
-Wunused-parameter \
-Wlong-long \
-fno-common

# Warnings that works only on clang :
# adding 'no' after '-W' remove the warning

# Warnings that works only on gcc : \
-Wlogical-op \
-Wnoexcept \
-Wstrict-null-sentinel \
-Wformat-truncation \

# .cpp and .hpp files
FILES_DIRECTORY := ./sources/project
# .o files
OBJECT_DIRECTORY := ./object
# .exe and dll's
BUILD_DIRECTORY := ./build

# Name of the exe file
EXECUTABLE := $(BUILD_DIRECTORY)/application



############################## knowing files and object location ##############################

# Produce list in the form "./sources/project/sub_directory/filename.hpp"
HEADERS_FILES := $(wildcard $(FILES_DIRECTORY)/*/*.hpp)
# Erase files directory => "sub_directory/filename.hpp"
HEADERS_FILES := $(subst $(FILES_DIRECTORY)/,,$(HEADERS_FILES))
# Same for cpp files
SOURCES_FILES := $(wildcard $(FILES_DIRECTORY)/*/*.cpp)
SOURCES_FILES := $(subst $(FILES_DIRECTORY)/,,$(SOURCES_FILES))

# Object files of the all cpp hpp pair
# sub_directory/filename.hpp => sub_directory/filename.o
OBJECT_PAIR := $(patsubst %.hpp,%.o,$(HEADERS_FILES))
# => ./object/sub_directory/filename.o
OBJECT_PAIR := $(addprefix $(OBJECT_DIRECTORY)/, $(OBJECT_PAIR))

# All the object (needed for all prerequesites)
OBJECT_ALL := $(patsubst %.cpp,%.o,$(SOURCES_FILES))
OBJECT_ALL := $(addprefix $(OBJECT_DIRECTORY)/, $(OBJECT_ALL))

# Object files of cpp without hpp combinaison
# Removes all OBJECT_PAIR of OBJECT_ALL
OBJECT_SOLO := $(filter-out $(OBJECT_PAIR), $(OBJECT_ALL))

# Dependencies (.d files) will be on the same directories than object files
DEPENDENCIES := $(patsubst %.cpp,%.d,$(SOURCES_FILES))
DEPENDENCIES := $(addprefix $(OBJECT_DIRECTORY)/, $(DEPENDENCIES))


SYSTEM_NAME := Unkown
ifdef OS
	SYSTEM_NAME := Windows
else
	SYSTEM_NAME := Unix
endif

############################## call action ##############################

# These commands do not represent physical files
.PHONY: all run clean debug remake


all : $(OBJECT_ALL) $(EXECUTABLE)

run :
#	./build/application.exe
	$(EXECUTABLE).exe

info:
	echo $(SYSTEM_NAME)

clean :
	rm -rf $(OBJECT_DIRECTORY)
	rm -rf $(EXECUTABLE)


debug :
	gdb -quiet $(EXECUTABLE)

remake: cleaner all



############################## project path ##############################

PROJECT_DIRECTORY_PATH := $(CURDIR)
LIBRARIES_PATH := $(PROJECT_DIRECTORY_PATH)/libraries
SFML_PATH := $(LIBRARIES_PATH)/SFML
SQLITE_PATH :=  $(LIBRARIES_PATH)/Sqlite
JSON_NLOHMANN_PATH :=  $(LIBRARIES_PATH)/cpp-json-nlohmann
INCLUDES := -I"$(SFML_PATH)/include" -I"$(SQLITE_PATH)/sources" -I"$(JSON_NLOHMANN_PATH)/include" -I"$(PROJECT_DIRECTORY_PATH)/sources"
LIBRARIES :=  "$(SQLITE_PATH)/object/sqlite3.o" -L"$(SFML_PATH)/lib" -lsfml-graphics -lsfml-system -lsfml-window



############################## making project compilation ##############################

# Create the executable by Linking all the object files and the SFML together
$(EXECUTABLE) : $(OBJECT_ALL)
#	g++ object/sub_directory_A/filename_A.o object/sub_directory_B/filename_B.o etc... -o executable -L"C:/Informatique/SFML-2.5.1/lib" -lsfml-graphics -lsfml-system -lsfml-window
	$(CXX_COMMAND) $^ -o $@ $(LIBRARIES)

# set up the dependencies
-include $(DEPENDENCIES)

# Creating the object file of OBJECT_PAIR with gather the dependencies of a .cpp .hpp pair
$(OBJECT_PAIR) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/%.cpp $(FILES_DIRECTORY)/%.hpp
#	removes object folder if exist
#	rm -rf -- $@
#	create the object directory if not exist
	mkdir -p $(dir $@)
#	g++ -std=c++2a -Wall -Wextra -c -o object/sub_directory/filename.o sources/project/sub_directory/filename.cpp -I"C:/Users/Turki/GoogleDrive/Informatique/Projets/Moteur-2D/sources" -I"C:/Informatique/SFML-2.5.1/include"
	$(CXX_COMMAND) $(WARNINGS) $(CXX_FLAGS) -c $< -o $@ $(INCLUDES)

# Creating the object file of OBJECT_SOLO with gather the dependencies of the .cpp's only
# commands are the same than above
$(OBJECT_SOLO) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/%.cpp
#	rm -rf -- $@
	mkdir -p $(dir $@)
	$(CXX_COMMAND) $(WARNINGS) $(CXX_FLAGS) -c $< -o $@ $(INCLUDES)
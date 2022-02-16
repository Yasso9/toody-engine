############################## Global Variable ##############################

# TYPO regarder le makefile du projet de groupe automate pour voir les petits détatils qu'on a rajouté

# g++
# CXX_COMMAND := g++
# CXX_FLAGS := $(CXX_COMMAND) -std=c++2a

# clang
CXX_COMMAND := clang++
CXX_FLAGS := -std=c++20 -MD -O0 -g
# -MD => Create .d files for dependencies
# -g => Generate debug information
# -O0 => No optmization, faster compilation time, better for debugging builds

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
-Wformat-truncation

# Warning that must not be used in clang \
-Wfloat-equal # The equality between floats works, it's the addition that is wrong

# .cpp and .hpp files
FILES_DIRECTORY := ./sources/project
# .o files
OBJECT_DIRECTORY := ./object
# .exe and dll's
BUILD_DIRECTORY := ./build

# Name of the exe file
EXECUTABLE := $(BUILD_DIRECTORY)/application




############################## knowing files and object location ##############################

# Produce list in the form "./sources/project/filename.cpp"
SOURCES_FILES := $(wildcard $(FILES_DIRECTORY)/*.cpp)
# Add also all the subfolders "./sources/project/sub_directory/filename.cpp"
SOURCES_FILES := $(SOURCES_FILES) $(wildcard $(FILES_DIRECTORY)/*/*.cpp)
# Erase files directory => "sub_directory/filename.cpp"
SOURCES_FILES := $(subst $(FILES_DIRECTORY)/,,$(SOURCES_FILES))

# Object files of the all cpp files of the project
# sub_directory/filename.cpp => sub_directory/filename.o
OBJECT_PROJECT := $(patsubst %.cpp,%.o,$(SOURCES_FILES))
# => ./object/sub_directory/filename.o
OBJECT_PROJECT := $(addprefix $(OBJECT_DIRECTORY)/,$(subst /,-,$(OBJECT_PROJECT)))


# List of the library object that needs to be linked
OBJECT_GLAD := $(OBJECT_DIRECTORY)/glad.o
OBJECT_SQLITE := $(OBJECT_DIRECTORY)/sqlite3.o
# All object needed for the project to compile (the project files objects + the libraries objects)
OBJECT_ALL := $(OBJECT_PROJECT) $(OBJECT_GLAD) $(OBJECT_SQLITE)

# Dependencies (.d files) will be on the same directories
# and have the same name than object files
DEPENDENCIES := $(patsubst %.o,%.d,$(OBJECT_PROJECT))


SYSTEM_NAME := Unkown
ifdef OS
	SYSTEM_NAME := Windows
else
	SYSTEM_NAME := Unix
endif




############################## project path ##############################

PROJECT_ROOT_PATH := $(CURDIR)
LIBRARIES_PATH := $(PROJECT_ROOT_PATH)/libraries
SQLITE_PATH :=  $(LIBRARIES_PATH)/Sqlite
JSON_NLOHMANN_PATH :=  $(LIBRARIES_PATH)/Json/include
GLAD_PATH :=  $(LIBRARIES_PATH)/Glad# For OpenGL
PROJECT_DIRECTORY_PATH := $(PROJECT_ROOT_PATH)/sources/project/
ifeq ($(SYSTEM_NAME),Windows)
	SFML_PATH := $(LIBRARIES_PATH)/Sfml_Windows
else
	ifeq ($(SYSTEM_NAME),Unix)
		SFML_PATH := $(LIBRARIES_PATH)/Sfml_Linux
	endif
endif

INCLUDES := -I"$(SFML_PATH)/include" \
			-I"$(SQLITE_PATH)" \
			-I"$(JSON_NLOHMANN_PATH)" \
			-I"$(GLAD_PATH)" \
			-I"$(PROJECT_DIRECTORY_PATH)"
LIBRARIES := -L"$(SFML_PATH)/lib" -lsfml-graphics -lsfml-system -lsfml-window




############################## call action ##############################

# These commands do not represent physical files
.PHONY: all run libraries create_object_directory clean_executable clean debug remake

# info :
# 	echo $(SYSTEM_NAME)

all : clean_executable create_object_directory $(OBJECT_ALL) $(EXECUTABLE)

run :
#	./build/application.exe
	$(EXECUTABLE).exe

create_object_directory:
	mkdir -p $(OBJECT_DIRECTORY)

clean_executable :
	rm -rf $(EXECUTABLE)

clean : clean_executable
	rm -rf $(OBJECT_DIRECTORY)

debug :
	gdb -quiet $(EXECUTABLE)

remake: cleaner all




############################## making project compilation ##############################

# set up the dependencies
-include $(DEPENDENCIES)

# Creating all the object files needed
.SECONDEXPANSION:
$(OBJECT_PROJECT) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/$$(subst -,/,%).cpp
#	Print the current file that is compiled
	$(info $<)
#	compilator++ -WarningFlags -cpp_standard -compilerOptions -c sources/project/sub_directory/filename.cpp -o sub_directory_filename.o -I"/Path/To/Includes"
#   -c => Doesn't create WinMain error if there is no WinMain in the file
#   -o => Create custom object
	$(CXX_COMMAND) $(WARNINGS) $(CXX_FLAGS) -c $< -o $@ $(INCLUDES)

# Create the executable by Linking all the object files and the SFML together
$(EXECUTABLE) : $(OBJECT_ALL)
#	compilator++ sub_directory_A_filename_A.o sub_directory_B_filename_B.o etc... -o executable -L"/Path/To/Library" -library_flags
#   -o => choose custom object
	$(CXX_COMMAND) $^ -o $@ $(LIBRARIES)

$(OBJECT_GLAD) :
	gcc -c $(GLAD_PATH)/glad/glad.c -o $@ -I"$(GLAD_PATH)"

$(OBJECT_SQLITE) :
	gcc -c $(SQLITE_PATH)/SQLITE/sqlite3.c -o $@ -I"$(SQLITE_PATH)"

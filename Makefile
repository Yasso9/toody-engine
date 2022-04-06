############################## OS detection ##############################

# Get the OS information
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
	DETECTED_OS := $(shell uname)
endif

ifneq ($(DETECTED_OS),Windows)
	ifneq ($(DETECTED_OS),Linux)
		$(error ERROR : OS UNDETECTED)
	endif
endif




############################## Warnings ##############################

WARNINGS := \
-pedantic -Wpedantic -pedantic-errors \
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




############################## Global Informations ##############################

# g++
# CXX_COMMAND := g++
# CXX_FLAGS := $(CXX_COMMAND) -std=c++2a

# clang
CXX_COMMAND := clang++
CXX_FLAGS := -std=c++20 -MD -O0 -g
# -MD => Create .d files for dependencies
# -g => Generate debug information
# -O0 => No optmization, faster compilation time, better for debugging builds

# .cpp and .hpp files
FILES_DIRECTORY := ./sources
# .exe and dll's
BUILD_DIRECTORY := ./build
# .o files
OBJECT_DIRECTORY := $(BUILD_DIRECTORY)/object

EXTERNAL_DIRECTORY := ./external
# Libraries Header
LIBRARIES_INCLUDE_PATH := $(EXTERNAL_DIRECTORY)/includes
# .o files of libraries
LIBRARIES_OBJECT_DIRECTORY := $(EXTERNAL_DIRECTORY)/object
# .a files of librairies
ifeq ($(DETECTED_OS),Windows)
	LIBRARIES_PATH := $(EXTERNAL_DIRECTORY)/libraries/Windows
else
	LIBRARIES_PATH := $(EXTERNAL_DIRECTORY)/libraries/Linux
endif
# .dll files needed for executable to work on windows
DLLS_PATH := $(EXTERNAL_DIRECTORY)/DLLs

# Path of the exe file
EXECUTABLE_DIRECTORY := $(BUILD_DIRECTORY)/application
EXECUTABLE := $(EXECUTABLE_DIRECTORY)/application




############################## files and object location ##############################

# Produce list in the form "./sources/filename.cpp"
SOURCES_FILES := $(wildcard $(FILES_DIRECTORY)/*.cpp)
# Add also all the subfolders "./sources/sub_directory/filename.cpp"
SOURCES_FILES := $(SOURCES_FILES) $(wildcard $(FILES_DIRECTORY)/*/*.cpp)
# Erase files directory => "sub_directory/filename.cpp"
SOURCES_FILES := $(subst $(FILES_DIRECTORY)/,,$(SOURCES_FILES))

# Object files of the all cpp files of the project
# sub_directory/filename.cpp => sub_directory/filename.o
OBJECT_PROJECT := $(patsubst %.cpp,%.o,$(SOURCES_FILES))
# => ./object/sub_directory-filename.o
OBJECT_PROJECT := $(addprefix $(OBJECT_DIRECTORY)/,$(subst /,-,$(OBJECT_PROJECT)))


# List of the library object that needs to be linked
# In the form "./object_library/objectA.o" "./object_library/objectB.o"
OBJECT_LIBRARIES := $(wildcard $(LIBRARIES_OBJECT_DIRECTORY)/*.o)

# All object needed for the project to compile
# (the project files objects + the libraries objects)
OBJECT_ALL := $(OBJECT_PROJECT) $(OBJECT_LIBRARIES)

# Dependencies (.d files) will be on the same directories
# and have the same name than object files
DEPENDENCIES := $(patsubst %.o,%.d,$(OBJECT_PROJECT))




############################## call action ##############################

# These commands do not represent physical files
.PHONY: buildrun build run initialize_build \
		clean_executable clean debug remake

buildrun : build run

build : clean_executable initialize_build $(OBJECT_ALL) $(EXECUTABLE)

run :
	$(EXECUTABLE)

initialize_build:
	mkdir -p $(BUILD_DIRECTORY)
	mkdir -p $(EXECUTABLE_DIRECTORY)
# Use the DLL's only on windows
ifeq ($(DETECTED_OS),Windows)
	cp $(DLLS_PATH)/* $(EXECUTABLE_DIRECTORY)
endif
	mkdir -p $(OBJECT_DIRECTORY)

clean_executable:
	rm -rf $(EXECUTABLE)

clean : clean_executable
	rm -rf $(OBJECT_DIRECTORY)
	rm -rf $(EXECUTABLE_DIRECTORY)
	rm -rf $(BUILD_DIRECTORY)

debug :
	gdb -quiet $(EXECUTABLE)

remake: clean buildrun




############################## include and libraries ##############################

INCLUDES := -I"$(FILES_DIRECTORY)" -I"$(LIBRARIES_INCLUDE_PATH)"

LIB_FLAG_SFML := -lsfml-graphics -lsfml-system -lsfml-window
LIB_FLAG_ASSIMP := -lassimp
LIB_FLAG_SQLITE := -lpthread -ldl
LIBRARIES_FLAG := $(LIB_FLAG_SFML) $(LIB_FLAG_ASSIMP) $(LIB_FLAG_SQLITE)
ifeq ($(DETECTED_OS),Windows)
	LIBRARIES := -L"$(LIBRARIES_PATH)"
endif
LIBRARIES := $(LIBRARIES) $(LIBRARIES_FLAG)




############################## making project compilation ##############################

# set up the dependencies
-include $(DEPENDENCIES)

# Creating all the object files needed
.SECONDEXPANSION:
$(OBJECT_PROJECT) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/$$(subst -,/,%).cpp
#	compilatorCommand -WarningFlags -compilerOptions -c sources/sub_directory/filename.cpp -o sub_directory_filename.o -I"/Path/To/Includes"
#   -c => Doesn't create WinMain error if there is no main in the file
#   -o => Create custom object
	@$(CXX_COMMAND) $(WARNINGS) $(CXX_FLAGS) -c $< -o $@ $(INCLUDES)
#	Nicer way to print the current file compiled
	@echo "Compiling $(subst sources/,,$<)"

# Create the executable by Linking all the object files and the SFML together
$(EXECUTABLE) : $(OBJECT_ALL)
#	compilator++ sub_directory_A_filename_A.o sub_directory_B_filename_B.o etc... -o executable -L"/Path/To/Library" -libraries_flags
#   -o => choose custom object
	@$(CXX_COMMAND) $^ -o $@ $(LIBRARIES)
	@echo "Building $@"
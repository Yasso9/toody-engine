############################## USER CUSTOM ##############################

# Specify the compiler command and version
COMPILER_COMMAND := gcc
COMPILER_VERSION :=

# Put @ if we should not show the command, put nothing otherwise
SHOW := @



############################## OS detection ##############################

# Know in what operating system we are
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



############################## COMPILER SETUP ##############################

ifneq ($(COMPILER_VERSION),)
COMPILER_VERSION := -$(COMPILER_VERSION)
endif
GCC_COMPILER := gcc$(COMPILER_VERSION)
G++_COMPILER := g++$(COMPILER_VERSION)
CLANG_COMPILER := clang$(COMPILER_VERSION)
CLANG++_COMPILER := clang++$(COMPILER_VERSION)

C_COMMAND := $(COMPILER_COMMAND)$(COMPILER_VERSION)
ifeq ($(C_COMMAND),$(GCC_COMPILER))
CXX_COMMAND := $(G++_COMPILER)
else ifeq ($(C_COMMAND),$(CLANG_COMPILER))
CXX_COMMAND := $(CLANG++_COMPILER)
else
$(error ERROR : UNKNOWN COMPILER)
endif
# DEPENDENCY_FLAGS := -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
DEPENDENCY_FLAGS := -MMD -MP
# -MMD => Create .d files for dependencies of users files only (not system files)
# -MP => Handle renamed or missing files for dependency
COMPILING_FLAGS := -std=c++20 -O0 -g
# -g => Generate debug information
# -O0 => No optmization, faster compilation time, better for debugging builds
LINKING_FLAGS :=


############################## Warnings ##############################

GENERAL_WARNINGS := \
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
-Wvirtual-inheritance  \
-Wredundant-decls \
-Wshadow \
-Wsign-conversion \
-Wsign-promo \
-Wundef \
-Wno-unused \
-Wconversion \
-Wformat-nonliteral \
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
-Wsuggest-override \
-fno-common

# Warning I don't understand
# -Wstrict-overflow=5

# Warning not used be can be useful in certain scenario
# -Wswitch-default # Warn if there is not a default in a switch statement

# Warnings that works only on clang :
# adding 'no' after '-W' remove the warning

# Warnings that works only on gcc
GCC_WARNINGS_ENABLE := \
	-Wlogical-op \
	-Wnoexcept \
	-Wstrict-null-sentinel \
	-Wformat-truncation

GCC_WARNINGS_REMOVE := \
	-Wno-long-long \
	-Wno-undef \

GCC_WARNINGS := $(GENERAL_WARNINGS) $(GCC_WARNINGS_ENABLE) $(GCC_WARNINGS_REMOVE)
CLANG_WARNINGS := $(GENERAL_WARNINGS)

ifeq ($(C_COMMAND),$(GCC_COMPILER))
FINAL_WARNINGS := $(GCC_WARNINGS)
else ifeq ($(C_COMMAND),$(CLANG_COMPILER))
FINAL_WARNINGS := $(CLANG_WARNINGS)
else
$(error ERROR : UNKNOWN COMPILER)
endif
# Warning that must not be used in clang \
-Wfloat-equal # The equality between floats works, it's the addition that is wrong




############################## Global Informations ##############################

# .cpp and .hpp files
PURE_FILES_DIRECTORY := sources
FILES_DIRECTORY := ./$(PURE_FILES_DIRECTORY)
# .exe and dll's
BUILD_DIRECTORY := ./build
# .o files
OBJECT_DIRECTORY := $(BUILD_DIRECTORY)/object
# .d files
DEPS_DIRECTORY := $(OBJECT_DIRECTORY)

EXTERNAL_DIRECTORY := ./external
# Libraries Header
LIBRARIES_INCLUDE_PATH := $(EXTERNAL_DIRECTORY)/includes
# .o files of libraries
LIBRARIES_OBJECT_DIRECTORY := $(BUILD_DIRECTORY)/object_libraries
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

# Produce list of the current cpp files in the form "./sources/filename.cpp"
SOURCES_FILES := $(wildcard $(FILES_DIRECTORY)/*.cpp)
# Add also all the subfolders "./sources/sub_directory/filename.cpp"
SOURCES_FILES := $(SOURCES_FILES) $(wildcard $(FILES_DIRECTORY)/**/*.cpp)
# Erase files directory => "sub_directory/filename.cpp"
SOURCES_FILES := $(subst $(FILES_DIRECTORY)/,,$(SOURCES_FILES))


# Object files of the all cpp files of the project that we need to generate (or are already generated)
# sub_directory/filename.cpp => sub_directory/filename.o
OBJECT_PROJECT := $(patsubst %.cpp,%.o,$(SOURCES_FILES))
# => ./object/sub_directory-filename.o
OBJECT_PROJECT := $(addprefix $(OBJECT_DIRECTORY)/,$(subst /,-,$(OBJECT_PROJECT)))

# List of the c++ library object that needs to be linked
CPP_SOURCES_LIBRARIES := $(wildcard $(LIBRARIES_INCLUDE_PATH)/*/*.cpp)
CPP_SOURCES_LIBRARIES := $(subst $(LIBRARIES_INCLUDE_PATH)/,,$(CPP_SOURCES_LIBRARIES))
CPP_OBJECT_LIBRARIES := $(patsubst %.cpp,%.o,$(CPP_SOURCES_LIBRARIES))
CPP_OBJECT_LIBRARIES := $(addprefix $(LIBRARIES_OBJECT_DIRECTORY)/,$(subst /,~,$(CPP_OBJECT_LIBRARIES)))
# List of the c library object that needs to be linked
C_SOURCES_LIBRARIES := $(wildcard $(LIBRARIES_INCLUDE_PATH)/*/*.c)
C_SOURCES_LIBRARIES := $(subst $(LIBRARIES_INCLUDE_PATH)/,,$(C_SOURCES_LIBRARIES))
C_OBJECT_LIBRARIES := $(patsubst %.c,%.o,$(C_SOURCES_LIBRARIES))
C_OBJECT_LIBRARIES := $(addprefix $(LIBRARIES_OBJECT_DIRECTORY)/,$(subst /,~,$(C_OBJECT_LIBRARIES)))

# All object needed for the project to compile
# (the project files objects + the libraries objects)
OBJECT_ALL := $(OBJECT_PROJECT) $(CPP_OBJECT_LIBRARIES) $(C_OBJECT_LIBRARIES)



# Dependencies (.d files) will be on the same directories
# and have the same name than object files
# Je ne sais toujours pas quoi est le mieux entre ces deux lignes ?
DEPENDENCIES := $(patsubst %.o,%.d,$(OBJECT_PROJECT))
# DEPENDENCIES := $(wildcard $(DEPS_DIRECTORY)/*.d)




############################## call action ##############################

# Compile only one file :
# make build/object/maths-vector2.o
# make build/object/<directory>-<filename>.o

# These commands do not represent physical files
.PHONY: buildrun build run initialize_build \
		clean_executable clean_project clean_libraries \
		clean debug remake nothing valgrind release

buildrun : build run

build : initialize_build $(OBJECT_ALL) $(EXECUTABLE)

run :
ifeq ($(DETECTED_OS),Linux)
	export LD_LIBRARY_PATH="$(LIBRARIES_PATH)" && $(EXECUTABLE)
else
	$(EXECUTABLE)
endif

initialize_build: clean_executable
	$(SHOW)echo "Create Build Directories"
	$(SHOW)mkdir -p $(BUILD_DIRECTORY)
	$(SHOW)mkdir -p $(EXECUTABLE_DIRECTORY)
	$(SHOW)mkdir -p $(OBJECT_DIRECTORY)
	$(SHOW)mkdir -p $(DEPS_DIRECTORY)
	$(SHOW)mkdir -p $(LIBRARIES_OBJECT_DIRECTORY)
# Use the DLL's only on windows
ifeq ($(DETECTED_OS),Windows)
	$(SHOW)echo "Copy Dll's for Executable"
	$(SHOW)cp $(DLLS_PATH)/* $(EXECUTABLE_DIRECTORY)
endif

clean_executable:
	$(SHOW)echo "Clean Executable"
	$(SHOW)rm -rf $(EXECUTABLE)

clean_project: clean_executable
	rm -rf $(OBJECT_DIRECTORY)
	rm -rf $(DEPS_DIRECTORY)

clean_libraries:
	rm -rf $(LIBRARIES_OBJECT_DIRECTORY)

clean : clean_project clean_libraries
	rm -rf $(BUILD_DIRECTORY)

debug :
ifeq ($(DETECTED_OS),Linux)
	export LD_LIBRARY_PATH="$(LIBRARIES_PATH)" && gdb -quiet $(EXECUTABLE)
else
	gdb -quiet $(EXECUTABLE)
endif

valgrind : build
ifeq ($(DETECTED_OS),Linux)
	export LD_LIBRARY_PATH="$(LIBRARIES_PATH)" && valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--trace-children=yes --track-fds=yes \
		--num-callers=50 \
		--log-file=valgrind-out.txt \
		$(EXECUTABLE)
else
	$(error ERROR : Valgrind not usable on Windows)
endif


remake: clean buildrun

RELEASE_DIRECTORY := Game
release:
	rm -rf $(RELEASE_DIRECTORY)
	mkdir $(RELEASE_DIRECTORY)
	cp $(EXECUTABLE) $(RELEASE_DIRECTORY)
ifeq ($(DETECTED_OS),Windows)
	cp $(DLLS_PATH)/* $(RELEASE_DIRECTORY)
endif
	cp -r data $(RELEASE_DIRECTORY)

nothing:




############################## include and libraries ##############################

INCLUDES := -I"$(FILES_DIRECTORY)" -isystem"$(LIBRARIES_INCLUDE_PATH)"

ifeq ($(DETECTED_OS),Windows)
LIB_FLAG_SFML := -lsfml-main
endif
LIB_FLAG_SFML := $(LIB_FLAG_SFML) -lsfml-graphics -lsfml-system -lsfml-window

ifeq ($(DETECTED_OS),Windows)
LIB_FLAG_IMGUI := -lopengl32
else # Linux
LIB_FLAG_IMGUI := -lGL -ldl
endif

LIB_FLAG_ASSIMP := -lassimp
LIB_FLAG_SQLITE := -lpthread -ldl

LIBRARIES_FLAG := $(LIB_FLAG_SFML) $(LIB_FLAG_IMGUI) \
				  $(LIB_FLAG_ASSIMP) $(LIB_FLAG_SQLITE)

LIBRARIES := -L"$(LIBRARIES_PATH)" $(LIBRARIES_FLAG)




############################## making project compilation ##############################

# Creating object files of the cpp libraries
.SECONDEXPANSION:
$(CPP_OBJECT_LIBRARIES) : $(LIBRARIES_OBJECT_DIRECTORY)/%.o : $(LIBRARIES_INCLUDE_PATH)/$$(subst ~,/,%).cpp
	$(SHOW)echo "C++ Library Compile $(subst external/includes/,,$<)"
#	compilatorCommand -c filename.cpp -o filename.o -I"/Path/To/Includes"
#   -c => Doesn't create WinMain error if there is no main in the file
#   -o => Create custom object
	$(SHOW)$(CXX_COMMAND) -c $< -o $@ $(INCLUDES)

# Creating object files of the c libraries
.SECONDEXPANSION:
$(C_OBJECT_LIBRARIES) : $(LIBRARIES_OBJECT_DIRECTORY)/%.o : $(LIBRARIES_INCLUDE_PATH)/$$(subst ~,/,%).c
	$(SHOW)echo "C Library Compile $(subst external/includes/,,$<)"
#	compilatorCommand -c filename.cpp -o filename.o -I"/Path/To/Includes"
#   -c => Doesn't create WinMain error if there is no main in the file
#   -o => Create custom object
	$(SHOW)$(C_COMMAND) -c $< -o $@ $(INCLUDES)

# set up the dependencies
-include $(DEPENDENCIES)

# Creating the object files of the project
.SECONDEXPANSION:
$(OBJECT_PROJECT) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/$$(subst -,/,%).cpp # $(DEPS_DIRECTORY)/%.d
#	Nicer way to print the current file compiled
	$(SHOW)echo "Project Compile $(subst sources/,,$<)"
#	compilatorCommand -WarningFlags -compilerOptions -c sources/sub_directory/filename.cpp -o sub_directory_filename.o -I"/Path/To/Includes"
#   -c => Doesn't create WinMain error if there is no main in the file
#   -o => Create custom object
	$(SHOW)$(CXX_COMMAND) $(FINAL_WARNINGS) $(COMPILING_FLAGS) $(DEPENDENCY_FLAGS) -c $< -o $@ $(INCLUDES)

# $(DEPS_DIRECTORY)/%.d : ;

# Create the executable by Linking all the object files and the libraries together
$(EXECUTABLE) : $(OBJECT_ALL)
	$(SHOW)echo "Building $@"
#	compilator++ -linkingOptions sub_directory_A_filename_A.o sub_directory_B_filename_B.o etc... -o executable -L"/Path/To/Library" -libraries_flags
#   -o => choose custom object
	$(SHOW)$(CXX_COMMAND) $(LINKING_FLAGS) $^ -o $@ $(LIBRARIES)
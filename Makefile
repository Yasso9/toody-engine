############################## USER CUSTOM ##############################

# Put @ if we should not show the command, put nothing otherwise
SHOW := @

# Function to get all files in subdirectories
define GET_FILES
	$(shell find $(1) -type f -name '$(2)')
endef



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
C_COMMAND := $(CC)
CXX_COMMAND := $(CXX)

# Default compiler
ifeq ($(C_COMMAND),)
C_COMMAND := gcc
endif
ifeq ($(CXX_COMMAND),)
CXX_COMMAND := g++
endif

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
-fno-common \

ifeq ($(CXX_COMMAND),g++)
WARNINGS := $(WARNINGS) \
			-Wlogical-op \
			-Wnoexcept \
			-Wstrict-null-sentinel \
			-Wformat-truncation \
			-Wno-long-long \
			-Wno-undef
endif

CPP_FLAGS := $(WARNINGS) -std=c++20 -O0 -g -MMD -MP
# -g => Generate debug information
# -O0 => No optmization, faster compilation time, better for debugging builds
# -MMD => Create .d files for dependencies of users files only (not system files)
# -MP => Handle renamed or missing files for dependency

LINKING_FLAGS :=



############################## Global Informations ##############################

WORKSPACE_PATH := .

# directory of .cpp and .hpp files
FILES_DIRECTORY := $(WORKSPACE_PATH)/sources
# directory of files needed to build executable and the executable itself
BUILD_DIRECTORY := $(WORKSPACE_PATH)/build
# directory of .o (object) files of project
OBJECT_DIRECTORY := $(BUILD_DIRECTORY)/object
# .so files directory
SHARED_OBJECT_DIR := $(BUILD_DIRECTORY)/shared_object
# directory of .d (dependency) files of project
DEPS_DIRECTORY := $(OBJECT_DIRECTORY)

# directory of libraries that the project depends
LIBRARIES_INCLUDE_PATH := $(WORKSPACE_PATH)/libHeaders
# directory of .o (object) files of libraries
LIBRARIES_OBJECT_DIRECTORY := $(BUILD_DIRECTORY)/libraries_object

# Exectuable path and name
EXECUTABLE := $(WORKSPACE_PATH)/application



############################## files and object location ##############################

# Get all cpp files inside sources => "./sources/sub_directory/filename.cpp"
SOURCES_FILES := $(shell find . -wholename "$(FILES_DIRECTORY)/*.cpp")
# Erase files directory => "sub_directory/filename.cpp"
SOURCES_FILES := $(subst $(FILES_DIRECTORY)/,,$(SOURCES_FILES))


# Object files of the all cpp files of the project that we need to generate (or are already generated)
# sub_directory/filename.cpp => sub_directory/filename.o
PROJECT_OBJECT := $(patsubst %.cpp,%.o,$(SOURCES_FILES))
# => ./object/sub_directory-filename.o
PROJECT_OBJECT := $(addprefix $(OBJECT_DIRECTORY)/,$(subst /,-,$(PROJECT_OBJECT)))

# List of the c++ library object that needs to be linked
CPP_SOURCES_LIBRARIES := $(wildcard $(LIBRARIES_INCLUDE_PATH)/*/*.cpp)
CPP_SOURCES_LIBRARIES := $(subst $(LIBRARIES_INCLUDE_PATH)/,,$(CPP_SOURCES_LIBRARIES))
CPP_LIBRARIES_OBJECT := $(patsubst %.cpp,%.o,$(CPP_SOURCES_LIBRARIES))
CPP_LIBRARIES_OBJECT := $(addprefix $(LIBRARIES_OBJECT_DIRECTORY)/,$(subst /,~,$(CPP_LIBRARIES_OBJECT)))
# List of the c library object that needs to be linked
C_SOURCES_LIBRARIES := $(wildcard $(LIBRARIES_INCLUDE_PATH)/*/*.c)
C_SOURCES_LIBRARIES := $(subst $(LIBRARIES_INCLUDE_PATH)/,,$(C_SOURCES_LIBRARIES))
C_LIBRARIES_OBJECT := $(patsubst %.c,%.o,$(C_SOURCES_LIBRARIES))
C_LIBRARIES_OBJECT := $(addprefix $(LIBRARIES_OBJECT_DIRECTORY)/,$(subst /,~,$(C_LIBRARIES_OBJECT)))

LIBRARIES_OBJECT := $(C_LIBRARIES_OBJECT) $(CPP_LIBRARIES_OBJECT)

# All object needed for the project to compile
# (the project files objects + the libraries objects)
OBJECT_ALL := $(PROJECT_OBJECT) $(LIBRARIES_OBJECT)

# Dependencies (.d files) will be on the same directories
# and have the same name than object files
# I still don't know what is better between these two lines
DEPENDENCIES := $(patsubst %.o,%.d,$(PROJECT_OBJECT))
# DEPENDENCIES := $(wildcard $(DEPS_DIRECTORY)/*.d)



############################## call action ##############################

# Compile only one file :
# make build/object/maths-vector2.o
# make build/object/<directory>-<filename>.o

# These commands do not represent physical files
# .PHONY: all buildrun build compile link run initialize_build \
# 		clean_executable clean_project clean_libraries \
# 		clean debug remake nothing valgrind release iwyu \
# 		format cppclean compile_commands

all : build

buildrun : build run

remake: clean buildrun

build : initialize_build compile_libraries compile link

compile : $(PROJECT_OBJECT)

TMP_BUILD_DIR:=$(BUILD_DIRECTORY)/tmp
ASSIMP_BUILD_DIR:=$(TMP_BUILD_DIR)/assimp-5.2.5
ASSIMP_REQUIREMENTS:= \
$(SHARED_OBJECT_DIR)/libassimp.so \
$(LIBRARIES_INCLUDE_PATH)/assimp
SFML_BUILD_DIR:=$(TMP_BUILD_DIR)/SFML-2.5.1
# List of all the .so files of SFML and the SFML include directory
SFML_REQUIREMENTS := \
$(SHARED_OBJECT_DIR)/libsfml-audio.so \
$(SHARED_OBJECT_DIR)/libsfml-graphics.so \
$(SHARED_OBJECT_DIR)/libsfml-system.so \
$(SHARED_OBJECT_DIR)/libsfml-window.so \
$(LIBRARIES_INCLUDE_PATH)/SFML

compile_libraries : $(SFML_REQUIREMENTS) $(ASSIMP_REQUIREMENTS) $(LIBRARIES_OBJECT)

# Building SFML
$(SFML_REQUIREMENTS) :
	$(SHOW)echo "Building SFML"
	$(SHOW)mkdir -p $(SHARED_OBJECT_DIR)
	$(SHOW)mkdir -p $(TMP_BUILD_DIR)
# Get version 2.5.1 of SFML
	$(SHOW)wget --quiet -P $(TMP_BUILD_DIR) https://github.com/SFML/SFML/archive/refs/tags/2.5.1.tar.gz
	$(SHOW)tar -xzf $(TMP_BUILD_DIR)/2.5.1.tar.gz -C $(TMP_BUILD_DIR)
# Build SFML
	$(SHOW)cmake -S $(SFML_BUILD_DIR) -B $(SFML_BUILD_DIR)/build -Wno-dev \
	-D CMAKE_C_COMPILER=$(C_COMMAND)
	-D CMAKE_CXX_COMPILER=$(CXX_COMMAND)
	-D BUILD_SHARED_LIBS=1 \
	-D SFML_BUILD_AUDIO=1 \
	-D SFML_BUILD_DOC=0 \
	-D SFML_BUILD_EXAMPLES=0 \
	-D SFML_BUILD_GRAPHICS=1 \
	-D SFML_BUILD_NETWORK=1 \
	-D SFML_BUILD_WINDOW=1 \
	-D SFML_INSTALL_PKGCONFIG_FILES=0 \
	-D SFML_OPENGL_ES=0 \
	-D SFML_USE_SYSTEM_DEPS=0
	$(SHOW)cmake --build $(SFML_BUILD_DIR)/build/
# Copy the shared object to the shared object directory
	$(SHOW)cp -r $(SFML_BUILD_DIR)/build/lib/libsfml*.so* $(SHARED_OBJECT_DIR)
# Copy the include files of SFML to the include directory
	$(SHOW)cp -r $(SFML_BUILD_DIR)/include/SFML $(LIBRARIES_INCLUDE_PATH)
# Remove the downloaded files
	$(SHOW)rm -r $(TMP_BUILD_DIR)

# Building Assimp
$(ASSIMP_REQUIREMENTS) :
	$(SHOW)echo "Building Assimp"
	$(SHOW)mkdir -p $(SHARED_OBJECT_DIR)
	$(SHOW)mkdir -p $(TMP_BUILD_DIR)
# Get version 5.2.5 of Assimp
	$(SHOW)wget --quiet -P $(TMP_BUILD_DIR) https://github.com/assimp/assimp/archive/refs/tags/v5.2.5.tar.gz
	$(SHOW)tar -xzf $(TMP_BUILD_DIR)/v5.2.5.tar.gz -C $(TMP_BUILD_DIR)
# Build Assimp
	$(SHOW)cmake -S $(ASSIMP_BUILD_DIR) -B $(ASSIMP_BUILD_DIR)/build -Wno-dev \
	-D CMAKE_C_COMPILER=$(C_COMMAND)
	-D CMAKE_CXX_COMPILER=$(CXX_COMMAND)
	-D ASSIMP_BUILD_TESTS=0 \
	-D ASSIMP_INSTALL=0 \
	-D ASSIMP_WARNINGS_AS_ERRORS=0 \
	-D BUILD_SHARED_LIBS=1
	$(SHOW)cmake --build $(ASSIMP_BUILD_DIR)/build/
# Copy the shared object to the shared object directory
	$(SHOW)cp -r $(ASSIMP_BUILD_DIR)/build/bin/libassimp.so* $(SHARED_OBJECT_DIR)
# Copy the include files of Assimp to the include directory
	$(SHOW)cp -r $(ASSIMP_BUILD_DIR)/include/assimp $(LIBRARIES_INCLUDE_PATH)
	$(SHOW)cp -r $(ASSIMP_BUILD_DIR)/build/include/assimp $(LIBRARIES_INCLUDE_PATH)
# Remove the downloaded files
	$(SHOW)rm -r $(TMP_BUILD_DIR)

link : $(EXECUTABLE)

# Run exectutable
run :
	$(SHOW)echo "Running $(EXECUTABLE)"
ifeq ($(DETECTED_OS),Linux)
	$(SHOW)LD_LIBRARY_PATH="$(SHARED_OBJECT_DIR)" $(EXECUTABLE)
else
	$(SHOW)$(EXECUTABLE)
endif

compile_commands : clean initialize_build
	bear -- $(MAKE) compile

iwyu : clean
	CXX="include-what-you-use" \
	CPP_FLAGS="-Xiwyu --update_comments -Xiwyu --mapping_file=iwyu_mapping.imp -std=c++20" \
	$(MAKE) compile 2>iwyu_file.txt

	sed -i '/.inl/d' ./iwyu_file.txt

	python3 fix_includes.py --comments --update_comments --nosafe_headers < iwyu_file.txt
	$(MAKE) format

# Format all files of the project
format:
	$(SHOW)clang-format -i --verbose -style=file:.clang-format \
	$(call GET_FILES,$(FILES_DIRECTORY),*.cpp) \
	$(call GET_FILES,$(FILES_DIRECTORY),*.hpp) \
	$(call GET_FILES,$(FILES_DIRECTORY),*.tpp)

cppclean:
	cppclean --verbose --include-path=sources --include-path=external/includes sources/**

initialize_build: clean_executable
	$(SHOW)echo "Create Build Directories"
	$(SHOW)mkdir -p $(BUILD_DIRECTORY)
	$(SHOW)mkdir -p $(OBJECT_DIRECTORY)
	$(SHOW)mkdir -p $(SHARED_OBJECT_DIR)
	$(SHOW)mkdir -p $(DEPS_DIRECTORY)
	$(SHOW)mkdir -p $(LIBRARIES_OBJECT_DIRECTORY)

clean : clean_project clean_libraries
	$(SHOW)rm -rf $(BUILD_DIRECTORY)

clean_project: clean_executable
	$(SHOW)echo "Clean Project"
	$(SHOW)rm -rf $(OBJECT_DIRECTORY)
	$(SHOW)rm -rf $(SHARED_OBJECT_DIR)
	$(SHOW)rm -rf $(DEPS_DIRECTORY)

clean_libraries:
	$(SHOW)echo "Clean Libraries"
	$(SHOW)rm -rf $(LIBRARIES_OBJECT_DIRECTORY)

clean_executable:
	$(SHOW)echo "Clean Executable"
	$(SHOW)rm -rf $(EXECUTABLE)

debug :
ifeq ($(DETECTED_OS),Linux)
	LD_LIBRARY_PATH="$(SHARED_OBJECT_DIR)" gdb -quiet $(EXECUTABLE)
else
	gdb -quiet $(EXECUTABLE)
endif

nothing:



############################## include and libraries ##############################

INCLUDES := -I"$(FILES_DIRECTORY)" -isystem"$(LIBRARIES_INCLUDE_PATH)"
LIBRARIES := -L"$(SHARED_OBJECT_DIR)" \
	-lsfml-graphics -lsfml-system -lsfml-window \
	-lGL -ldl -lassimp



############################## making project compilation ##############################

# Creating object files of the cpp libraries
.SECONDEXPANSION:
$(CPP_LIBRARIES_OBJECT) : $(LIBRARIES_OBJECT_DIRECTORY)/%.o : $(LIBRARIES_INCLUDE_PATH)/$$(subst ~,/,%).cpp
	$(SHOW)echo "Compile C++ Library $(subst external/includes/,,$<)"
	$(SHOW)$(CXX_COMMAND) -c $< -o $@ $(INCLUDES)
#	g++ -c filename.cpp -o filename.o
#   -c => Doesn't create WinMain error if there is no main in the file
#   -o => Create custom object

# Creating object files of the c libraries
.SECONDEXPANSION:
$(C_LIBRARIES_OBJECT) : $(LIBRARIES_OBJECT_DIRECTORY)/%.o : $(LIBRARIES_INCLUDE_PATH)/$$(subst ~,/,%).c
	$(SHOW)echo "Compile C Library $(subst libraries/,,$<)"
	$(SHOW)$(C_COMMAND) -c $< -o $@ $(INCLUDES)
#	gcc -c filename.cpp -o filename.o
#   -c => Doesn't create WinMain error if there is no main in the file
#   -o => Create custom object

# set up the dependencies
-include $(DEPENDENCIES)

# Creating the object files of the project
.SECONDEXPANSION:
$(PROJECT_OBJECT) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/$$(subst -,/,%).cpp
#	Nicer way to print the current file compiled
	$(SHOW)echo "Compile Project $(subst sources/,,$<)"
	$(SHOW)$(CXX_COMMAND) $(CPP_FLAGS) -c $< -o $@ $(INCLUDES)
#	g++ -WarningFlags -compilerOptions -c sources/sub_directory/filename.cpp -o sub_directory_filename.o -I"/Path/To/Includes"
#   -c => Doesn't create WinMain error if there is no main in the file
#   -o => Create custom object

# Create the executable by Linking all the object files and the libraries together
$(EXECUTABLE) : $(OBJECT_ALL)
	$(SHOW)echo "Building and Linking $@"
	$(SHOW)$(CXX_COMMAND) $(LINKING_FLAGS) $^ -o $@ $(LIBRARIES)
#	gcc -linkingOptions sub_directory_A_filename_A.o sub_directory_B_filename_B.o etc... -o executable -L"/Path/To/Library" -libraries_flags
#   -o => Choose custom object

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



############################## Global Informations ##############################

# Current working directory
CWD := $(shell pwd)
# directory of .cpp and .hpp files
SOURCES_DIR := $(CWD)/sources
# directory of files needed to build executable and the executable itself
BUILD_DIR := $(CWD)/build
# directory of .o (object) files of project
OBJ_DIR := $(BUILD_DIR)/object
# .so files directory
OBJ_LIBS_DIR := $(BUILD_DIR)/libs
# directory of .d (dependency) files of project
DEPS_DIR := $(OBJ_DIR)
# Submodules directory
SUBMODULES:=$(CWD)/submodules
# directory of libraries that the project depends
LIBS_INC := $(SUBMODULES)/includes
# Exectuable path and name
EXEC := $(CWD)/application



############################## files and object location ##############################

# Get all cpp files inside sources => "./sources/sub_directory/filename.cpp"
SOURCES_FILES := $(call GET_FILES,$(SOURCES_DIR),*.cpp)
# Erase files directory => "sub_directory/filename.cpp"
SOURCES_FILES := $(subst $(SOURCES_DIR)/,,$(SOURCES_FILES))

# Object files of the all cpp files of the project that we need to generate (or are already generated)
# sub_directory/filename.cpp => sub_directory/filename.o
OBJ_PROJECT := $(patsubst %.cpp,%.o,$(SOURCES_FILES))
# => ./object/sub_directory-filename.o
OBJ_PROJECT := $(addprefix $(OBJ_DIR)/,$(subst /,-,$(OBJ_PROJECT)))

# All object needed for the project to compile
# (the project files objects + the libraries objects)
OBJ := $(OBJ_PROJECT)

# Dependencies (.d files) have same name than object files
DEPENDENCIES := $(patsubst %.o,%.d,$(OBJ_PROJECT))



############################## libraries ##############################

ASSIMP_SRC:=$(SUBMODULES)/assimp
ASSIMP_REQUIREMENTS:= \
$(OBJ_LIBS_DIR)/libassimp.a \
$(LIBS_INC)/assimp/config.h

SFML_SRC:=$(SUBMODULES)/SFML
SFML_REQUIREMENTS := \
$(OBJ_LIBS_DIR)/libsfml-graphics-s.a \
$(OBJ_LIBS_DIR)/libsfml-system-s.a \
$(OBJ_LIBS_DIR)/libsfml-window-s.a

FMT_SRC:=$(SUBMODULES)/fmt
FMT_REQUIREMENTS:=$(OBJ_LIBS_DIR)/libfmt.a

IMGUI_SRC:=$(SUBMODULES)/imgui-sfml
IMGUI_REQUIREMENTS:=$(OBJ_LIBS_DIR)/libImGui-SFML.a



############################## COMPILER SETUP ##############################

# Default compiler if not specified
ifeq ($(CC),)
CC := gcc
endif
ifeq ($(CXX),)
CXX := g++
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

ifeq ($(CXX),g++)
WARNINGS:=$(WARNINGS) \
			-Wlogical-op \
			-Wnoexcept \
			-Wstrict-null-sentinel \
			-Wformat-truncation \
			-Wno-long-long \
			-Wno-undef
endif

COMPILE_FLAGS_LIBS := -std=c++20 -O0 -g -c
COMPILE_FLAGS := $(WARNINGS) $(COMPILE_FLAGS_LIBS) -MMD -MP
LINK_FLAGS :=
# -g => Generate debug information
# -c => Doesn't create WinMain error if there is no main in the file
# -O0 => No optmization, faster compilation time, better for debugging builds
# -MMD => Create .d files for dependencies of users files only (not system files)
# -MP => Handle renamed or missing files for dependency



############################## call action ##############################

# Compile only one file :
# make build/object/maths-vector2.o
# make build/object/<directory>-<filename>.o

all : build

buildrun : build run

remake: clean buildrun

build : initialize_build compile_libs compile link

compile : $(OBJ_PROJECT)

compile_libs : \
	$(SFML_REQUIREMENTS) \
	$(ASSIMP_REQUIREMENTS) \
	$(FMT_REQUIREMENTS) \
	$(IMGUI_REQUIREMENTS) \
	$(OBJ_LIBS_DIR)/glad.o

# Building SFML
$(SFML_REQUIREMENTS) :
	$(SHOW)echo "Building SFML"

	$(SHOW)cmake -S $(SFML_SRC) -B $(SFML_SRC)/build -Wno-dev \
	-D CMAKE_C_COMPILER=$(CC) \
	-D CMAKE_CXX_COMPILER=$(CXX) \
	-D CMAKE_BUILD_TYPE=Release \
	-D BUILD_SHARED_LIBS=1 \
	-D SFML_BUILD_AUDIO=0 \
	-D SFML_BUILD_DOC=0 \
	-D SFML_BUILD_EXAMPLES=0 \
	-D SFML_BUILD_GRAPHICS=1 \
	-D SFML_BUILD_NETWORK=0 \
	-D SFML_BUILD_WINDOW=1 \
	-D SFML_INSTALL_PKGCONFIG_FILES=0 \
	-D SFML_OPENGL_ES=0 \
	-D SFML_USE_SYSTEM_DEPS=0
	$(SHOW)cmake --build $(SFML_SRC)/build/

	$(SHOW)mkdir -p $(OBJ_LIBS_DIR)
	$(SHOW)ln -sfr $(SFML_SRC)/build/**/*.so* $(OBJ_LIBS_DIR)

# Building Assimp
$(ASSIMP_REQUIREMENTS) :
	$(SHOW)echo "Building Assimp"

	$(SHOW)cmake -S $(ASSIMP_SRC) -B $(ASSIMP_SRC)/build -Wno-dev \
	-D CMAKE_C_COMPILER=$(CC) \
	-D CMAKE_CXX_COMPILER=$(CXX) \
	-D CMAKE_BUILD_TYPE=Release \
	-D BUILD_SHARED_LIBS=1 \
	-D ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=1 \
	-D ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=1 \
	-D ASSIMP_BUILD_ASSIMP_TOOLS=1 \
	-D ASSIMP_BUILD_TESTS=0 \
	-D ASSIMP_INSTALL=0 \
	-D ASSIMP_WARNINGS_AS_ERRORS=0
	$(SHOW)cmake --build $(ASSIMP_SRC)/build/

	$(SHOW)mkdir -p $(OBJ_LIBS_DIR)
	$(SHOW)ln -sfr $(ASSIMP_SRC)/build/**/*.so* $(OBJ_LIBS_DIR)
	$(SHOW)ln -sfr $(ASSIMP_SRC)/build/include/assimp/* $(LIBS_INC)/assimp


# Building FMT
$(FMT_REQUIREMENTS) :
	$(SHOW)echo "Building FMT"

	$(SHOW)cmake -S $(FMT_SRC) -B $(FMT_SRC)/build -Wno-dev \
	-D CMAKE_CXX_COMPILER=$(CXX) \
	-D CMAKE_BUILD_TYPE=Release \
	-D FMT_DOC=0 \
	-D FMT_TEST=0
	$(SHOW)cmake --build $(FMT_SRC)/build/

	$(SHOW)mkdir -p $(OBJ_LIBS_DIR)
	$(SHOW)ln -sfr $(FMT_SRC)/build/*.a $(OBJ_LIBS_DIR)

$(IMGUI_REQUIREMENTS) :
	$(SHOW)echo "Building ImGui"

	$(SHOW)cmake -S $(IMGUI_SRC) -B $(IMGUI_SRC)/build -Wno-dev \
	-D CMAKE_CXX_COMPILER=$(CXX) \
	-D CMAKE_BUILD_TYPE=Release \
	-D BUILD_SHARED_LIBS=1 \
	-D IMGUI_SFML_FIND_SFML=1 \
	-D IMGUI_SFML_IMGUI_DEMO=1 \
	-D IMGUI_SFML_USE_DEFAULT_CONFIG=1 \
	-D SFML_DIR=$(SUBMODULES)/SFML/build \
	-D IMGUI_DIR=$(SUBMODULES)/imgui \
	-D IMGUI_INCLUDE_DIR=$(SUBMODULES)/imgui
	$(SHOW)cmake --build $(IMGUI_SRC)/build/

	$(SHOW)mkdir -p $(OBJ_LIBS_DIR)
	$(SHOW)ln -sfr $(IMGUI_SRC)/build/*.so* $(OBJ_LIBS_DIR)

link : $(EXEC)

# Run exectutable
run :
	$(SHOW)echo "Running $(EXEC)"
ifeq ($(DETECTED_OS),Linux)
	$(SHOW)LD_LIBRARY_PATH="$(OBJ_LIBS_DIR)" $(EXEC)
else
	$(SHOW)$(EXEC)
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

# Format all files of the project with clang format
format:
	$(SHOW)clang-format -i --verbose -style=file:.clang-format \
	$(call GET_FILES,$(SOURCES_DIR),*.cpp) \
	$(call GET_FILES,$(SOURCES_DIR),*.hpp) \
	$(call GET_FILES,$(SOURCES_DIR),*.tpp)

initialize_build:
	$(SHOW)echo "Create Build Directories"
	$(SHOW)mkdir -p $(BUILD_DIR)
	$(SHOW)mkdir -p $(OBJ_DIR)
	$(SHOW)mkdir -p $(OBJ_LIBS_DIR)
	$(SHOW)mkdir -p $(DEPS_DIR)

cleanall : clean clean_executable clean_submodules

clean : clean_project
	$(SHOW)rm -rf $(BUILD_DIR)

clean_project: clean_executable
	$(SHOW)echo "Clean Project"
	$(SHOW)rm -rf $(OBJ_DIR)
	$(SHOW)rm -rf $(OBJ_LIBS_DIR)
	$(SHOW)rm -rf $(DEPS_DIR)

clean_executable:
	$(SHOW)echo "Clean Executable"
	$(SHOW)rm -rf $(EXEC)

clean_submodules:
	$(SHOW)echo "Clean Submodules"
	$(SHOW)rm -rf $(SUBMODULES)/**/build

debug :
ifeq ($(DETECTED_OS),Linux)
	LD_LIBRARY_PATH="$(OBJ_LIBS_DIR)" gdb -quiet $(EXEC)
else
	gdb -quiet $(EXEC)
endif

nothing:



############################## project compilation ##############################

INCLUDES := -I"$(SOURCES_DIR)" -isystem"$(LIBS_INC)"
LIBRARIES := -L"$(OBJ_LIBS_DIR)" $(OBJ_LIBS_DIR)/glad.o \
	-lz -lX11 -lXrandr -lXinerama -lXcursor -lpthread -lGL -ldl \
	-lfreetype -lm -ludev \
	-lsfml-graphics -lsfml-window -lsfml-system \
	-lassimp -lfmt -lOpenGL -lImGui-SFML

$(OBJ_LIBS_DIR)/glad.o : $(SUBMODULES)/glad/glad.c
	$(SHOW)echo "Compile $(subst $(CWD)/,,$<)"
	$(SHOW)$(CXX) $(COMPILE_FLAGS_LIBS) $< -o $@

# set up the dependencies
-include $(DEPENDENCIES)

# Creating the object files of the project
.SECONDEXPANSION:
$(OBJ_PROJECT) : $(OBJ_DIR)/%.o : $(SOURCES_DIR)/$$(subst -,/,%).cpp
#	Nicer way to print the current file compiled
	$(SHOW)echo "Compile $(subst $(SOURCES_DIR)/,,$<)"
	$(SHOW)$(CXX) $(COMPILE_FLAGS) $< -o $@ $(INCLUDES)

# Create the executable by Linking all the object files and the libraries together
$(EXEC) : $(OBJ)
	$(SHOW)echo "Building and Linking $@"
	$(SHOW)$(CXX) $(LINK_FLAGS) $^ -o $@ $(LIBRARIES)

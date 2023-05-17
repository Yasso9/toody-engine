# Put @ to show the command, put nothing otherwise
SHOW := @


################### functions ###################

# Function to get all files in subdirectories
define GET_FILES
	$(shell find -L $(1) -type f -name '$(2)')
endef

# $(1) : lib name
# $(2) : lib directory (source)
# $(3) : cmake options
# $(4) : lib object pattern (*.a, *.so*, *.o)
define COMPILE_LIB
	$(eval ARG_1 := $(strip $(1)))
	$(eval ARG_2 := $(strip $(2)))
	$(eval ARG_3 := $(strip $(3)))
	$(eval ARG_4 := $(strip $(4)))

	$(SHOW)echo "Building $(ARG_1)"

	$(SHOW)cmake -S $(ARG_2) -B $(ARG_2)/build -Wno-dev $(ARG_3)
	$(SHOW)cmake --build $(ARG_2)/build/

	$(SHOW)mkdir -p $(LIBS_OBJ_DIR)
	$(SHOW)ln -sfr $(ARG_2)/build/$(ARG_4) $(LIBS_OBJ_DIR)
endef


################### directories ###################

# Current working directory
CWD:=$(shell pwd)
# directory of .cpp and .hpp source files
SRC_DIR := $(CWD)/sources
# build directory
BUILD_DIR := $(CWD)/build
# project objects directory (contains .o files)
OBJ_DIR := $(BUILD_DIR)/object
# libraries objects directory (contains .so, .a, .o, etc...)
LIBS_OBJ_DIR := $(BUILD_DIR)/libs
# project dependencies directory (contains .d files)
DEPS_DIR := $(OBJ_DIR)
# libraries that the project depends on
LIBS_DIR := $(CWD)/submodules
# libraries include directory
LIBS_INC_DIR := $(LIBS_DIR)/includes
# libraries sources to build directory
LIBS_SRC_DIR := $(LIBS_DIR)/sources
# Exectuable file
EXEC := $(CWD)/app.out


################### project files ###################

# Get all cpp files inside sources directory
SRC := $(call GET_FILES,$(SRC_DIR),*.cpp)
# Erase the path of the files
SRC := $(subst $(SRC_DIR)/,,$(SRC))

# Project object files have same name than source files
OBJ := $(patsubst %.cpp,%.o,$(SRC))
# Add the object directory to the object files
OBJ := $(addprefix $(OBJ_DIR)/,$(subst /,-,$(OBJ)))
# Dependencies files have same name than object files
DEPS := $(patsubst %.o,%.d,$(OBJ))


################### compiler ###################

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

# Library cpp flags for compilation
COMPILE_FLAGS_LIBS := -std=c++20 -O0 -g -c
# -O0 => No optmization, faster compilation time, better for debugging builds
# -g => Generate debug information
# -c => Doesn't create WinMain error if there is no main in the file
# Project cpp flags for compilation
COMPILE_FLAGS := $(WARNINGS) $(COMPILE_FLAGS_LIBS) -MMD -MP
# -MMD => Create .d files for dependencies of users files only (not system files)
# -MP => Handle renamed or missing files for dependency


################### rules ###################

all : build
buildrun : build run
remake: clean buildrun

build : init_build compile_libs compile link
compile : $(OBJ)
link : $(EXEC)

run :
	$(SHOW)echo "Running $(EXEC)"
	$(SHOW)LD_LIBRARY_PATH="$(LIBS_OBJ_DIR)" $(EXEC)
debug :
	$(SHOW)echo "Debugging $(EXEC)"
	$(SHOW)gdb -quiet $(EXEC)

init_build:
	$(SHOW)echo "Create Build Directories"
	$(SHOW)mkdir -p $(BUILD_DIR)
	$(SHOW)mkdir -p $(OBJ_DIR)
	$(SHOW)mkdir -p $(LIBS_OBJ_DIR)
	$(SHOW)mkdir -p $(DEPS_DIR)

cleanall : clean clean_libs
clean : clean_exec
	$(SHOW)echo "Clean Project"
	$(SHOW)rm -rf $(BUILD_DIR)
	$(SHOW)rm -rf $(OBJ_DIR)
	$(SHOW)rm -rf $(DEPS_DIR)
clean_exec:
	$(SHOW)echo "Clean Executable"
	$(SHOW)rm -rf $(EXEC)
clean_libs:
	$(SHOW)echo "Clean Libraries Build"
	$(SHOW)rm -rf $(LIBS_DIR)/**/build
	$(SHOW)rm -rf $(LIBS_OBJ_DIR)

format:
	$(SHOW)clang-format -i --verbose -style=file:.clang-format \
	$(call GET_FILES,$(SRC_DIR),*.cpp) \
	$(call GET_FILES,$(SRC_DIR),*.hpp) \
	$(call GET_FILES,$(SRC_DIR),*.tpp)


################### libraries rules ###################

FMT_REQ:=$(LIBS_OBJ_DIR)/libfmt.a
SFML_REQ:=$(LIBS_OBJ_DIR)/libsfml-graphics.so \
			$(LIBS_OBJ_DIR)/libsfml-window.so \
			$(LIBS_OBJ_DIR)/libsfml-system.so
ASSIMP_REQ:=$(LIBS_OBJ_DIR)/libassimp.so $(LIBS_INC_DIR)/assimp/config.h
IMGUI_REQ:=$(LIBS_OBJ_DIR)/libImGui-SFML.so
GLAD_REQ:=$(LIBS_OBJ_DIR)/glad.o

compile_libs : $(FMT_REQ) $(SFML_REQ) $(ASSIMP_REQ) $(IMGUI_REQ) $(GLAD_REQ)

$(FMT_REQ) :
	$(call COMPILE_LIB, FMT \
		,$(LIBS_DIR)/fmt\
		, -D CMAKE_CXX_COMPILER=$(CXX) \
		-D CMAKE_BUILD_TYPE=Release \
		-D FMT_DOC=0 \
		-D FMT_TEST=0 \
		, *.a \
	)
$(SFML_REQ) :
	$(call COMPILE_LIB, SFML \
		,$(LIBS_DIR)/SFML \
		, -D CMAKE_C_COMPILER=$(CC) \
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
		-D SFML_USE_SYSTEM_DEPS=0 \
		, **/*.so* \
	)
$(ASSIMP_REQ) :
	$(call COMPILE_LIB, ASSIMP \
		,$(LIBS_DIR)/assimp \
		, -D CMAKE_C_COMPILER=$(CC) \
		-D CMAKE_CXX_COMPILER=$(CXX) \
		-D CMAKE_BUILD_TYPE=Release \
		-D BUILD_SHARED_LIBS=1 \
		-D ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=1 \
		-D ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=1 \
		-D ASSIMP_BUILD_ASSIMP_TOOLS=1 \
		-D ASSIMP_BUILD_TESTS=0 \
		-D ASSIMP_INSTALL=0 \
		-D ASSIMP_WARNINGS_AS_ERRORS=0 \
		, **/*.so* \
	)
	$(SHOW)ln -sfr $(LIBS_DIR)/assimp/build/include/assimp/* $(LIBS_INC_DIR)/assimp
$(IMGUI_REQ) :
	$(call COMPILE_LIB, IMGUI-SFML \
		,$(LIBS_DIR)/imgui-sfml \
		, -D CMAKE_CXX_COMPILER=$(CXX) \
		-D CMAKE_BUILD_TYPE=Release \
		-D BUILD_SHARED_LIBS=1 \
		-D IMGUI_SFML_FIND_SFML=1 \
		-D IMGUI_SFML_IMGUI_DEMO=1 \
		-D IMGUI_SFML_USE_DEFAULT_CONFIG=1 \
		-D SFML_DIR=$(LIBS_DIR)/SFML/build \
		-D IMGUI_DIR=$(LIBS_DIR)/imgui \
		-D IMGUI_INCLUDE_DIR=$(LIBS_DIR)/imgui \
		, *.so* \
	)

$(GLAD_REQ) : $(LIBS_DIR)/glad/glad.c
	$(SHOW)echo "Compile $(subst $(CWD)/,,$<)"
	$(SHOW)$(CXX) $(COMPILE_FLAGS_LIBS) $< -o $@


################### compile rules ###################

INCLUDES:=-I"$(SRC_DIR)" -isystem"$(LIBS_INC_DIR)"
LIBRARIES:=-L"$(LIBS_OBJ_DIR)" $(LIBS_OBJ_DIR)/*.o \
	-lz -lX11 -lXrandr -lXinerama -lXcursor -lpthread -lGL -ldl \
	-lfreetype -lm -ludev \
	-lsfml-graphics -lsfml-window -lsfml-system \
	-lassimp -lfmt -lOpenGL -lImGui-SFML

# set up the dependencies
-include $(DEPS)

# Creating the object files of the project
.SECONDEXPANSION:
$(OBJ) : $(OBJ_DIR)/%.o : $(SRC_DIR)/$$(subst -,/,%).cpp
	$(SHOW)echo "Compile $(subst $(SRC_DIR)/,,$<)"
	$(SHOW)$(CXX) $(COMPILE_FLAGS) $< -o $@ $(INCLUDES)

# Create the executable by Linking all the object files and the libraries together
$(EXEC) : $(OBJ)
	$(SHOW)echo "Building and Linking $@"
	$(SHOW)$(CXX) $^ -o $@ $(LIBRARIES)

nothing:
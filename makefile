############################## Global Variable ##############################

CPP := g++
CPP20 := g++ -std=c++2a
WARNING_FLAG := -pedantic -pedantic-errors \
-Wall -Wextra \
-Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization \
-Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs \
-Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 \
-Wswitch-default -Wundef -Werror -Wno-unused \
-Wconversion -Wfloat-equal \
-Wformat-nonliteral -Wformat-security  \
-Wformat-y2k \
-Wimport \
-Winline \
-Winvalid-pch \
-Wmissing-field-initializers -Wmissing-format-attribute   \
-Wmissing-noreturn \
-Wpacked -Wpointer-arith \
-Wstack-protector \
-Wstrict-aliasing=2  \
-Wunreachable-code -Wunused \
-Wunused-parameter \
-Wvariadic-macros \
-Wwrite-strings \
-Weffc++ \
-Wlong-long

COMPILER_FLAGS := -O0 -g $(WARNING_FLAG) -c -o
LINKER_FLAGS :=

# .cpp and .hpp files
FILES_DIRECTORY := ./sources/project
# .o files
OBJECT_DIRECTORY := ./object
# .exe and dll's
BUILD_DIRECTORY := ./build

# Name of the exe file
EXECUTABLE := $(BUILD_DIRECTORY)/application



############################## knowing files and object location ##############################

# Porduce list in the form "./sources/project/sub_directory/filename.hpp"
HEADERS_FILES := $(wildcard $(FILES_DIRECTORY)/*/*.hpp)
# Erase files directory "sub_directory/filename.hpp"
HEADERS_FILES := $(subst $(FILES_DIRECTORY)/,,$(HEADERS_FILES))
# Same for cpp files
SOURCES_FILES := $(wildcard $(FILES_DIRECTORY)/*/*.cpp)
SOURCES_FILES := $(subst $(FILES_DIRECTORY)/,,$(SOURCES_FILES))

# Object files of the all cpp hpp pair
OBJECT_PAIR := $(basename $(HEADERS_FILES)) # sub_directory/filename.hpp :=> sub_directory/filename
OBJECT_PAIR := $(addprefix $(OBJECT_DIRECTORY)/, $(OBJECT_PAIR)) # :=> ./object/sub_directory/filename
OBJECT_PAIR := $(addsuffix .o,$(OBJECT_PAIR)) # :=> ./object/sub_directory/filename.c

# All the object (needed for all prerequesites)
OBJECT_ALL := $(basename $(SOURCES_FILES))
OBJECT_ALL := $(addprefix $(OBJECT_DIRECTORY)/, $(OBJECT_ALL))
OBJECT_ALL := $(addsuffix .o, $(OBJECT_ALL))

# Object files of cpp without hpp combinaison
# Removes all OBJECT_PAIR of OBJECT_ALL
OBJECT_SOLO := $(filter-out $(OBJECT_PAIR), $(OBJECT_ALL))



############################## call action ##############################

all : $(OBJECT_ALL) $(EXECUTABLE)

run :
#	./build/application.exe
	$(EXECUTABLE).exe

clean :
	rm -rf -- $(OBJECT_DIRECTORY)/*.o
	rm -rf -- $(OBJECT_DIRECTORY)/*
	rm -rf -- $(OBJECT_DIRECTORY)
	rm -rf -- $(EXECUTABLE)

debug :
	gdb -quiet $(EXECUTABLE)

remake: cleaner all

info : # for debuging
	echo $(HEADERS_FILES)



############################## project path ##############################

# PS : If using vscode, you also have to define includes in c_cpp_properties.json project config
PROJECT_DIRECTORY := C:/Users/Turki/Mon Drive/Computing/Projects/ToodyEngine/sources
SFML_PATH := C:/Users/Turki/Mon Drive/Programs/Windows/SFML-2.5.1
INCLUDE := -I"$(SFML_PATH)/include" -I"$(PROJECT_DIRECTORY)"
LIBRARIES := -L"$(SFML_PATH)/lib" -lsfml-graphics -lsfml-system -lsfml-window



############################## making project compilation ##############################

# Creating the object file of OBJECT_PAIR with gather the dependencies of a .cpp .hpp pair
$(OBJECT_PAIR) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/%.cpp $(FILES_DIRECTORY)/%.hpp
#	removes object folder if exist
	rm -rf -- $@
#	create the object directory if not exist
	mkdir -p $(dir $@)
#	g++ -std=c++2a -Wall -Wextra -c -o object/sub_directory/filename.o sources/project/sub_directory/filename.cpp -I"C:/Users/Turki/Mon Drive/Informatique/Projets/Moteur-2D/sources" -I"C:/Informatique/SFML-2.5.1/include"
	$(CPP20) $(COMPILER_FLAGS) $@ $< $(INCLUDE)

# Creating the object file of OBJECT_SOLO with gather the dependencies of the .cpp's only
# commands are the same than above
$(OBJECT_SOLO) : $(OBJECT_DIRECTORY)/%.o : $(FILES_DIRECTORY)/%.cpp
	rm -rf -- $@
	mkdir -p $(dir $@)
	$(CPP20) $(COMPILER_FLAGS) $@ $< $(INCLUDE)

# Create the executable by Linking all the object files and the SFML together
$(EXECUTABLE) : $(OBJECT_ALL)
#	g++ object/sub_directory_A/filename_A.o object/sub_directory_B/filename_B.o etc... -o executable -L"C:/Informatique/SFML-2.5.1/lib" -lsfml-graphics -lsfml-system -lsfml-window
	$(CPP) $^ -o $@ $(LINKER_FLAGS) $(LIBRARIES)
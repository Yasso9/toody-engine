# Setting the minimum version of CMake
cmake_minimum_required(VERSION 3.7...3.26)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ToodyEngine VERSION 0.1
                    DESCRIPTION "C++ Game Engine"
                    LANGUAGES CXX)

option(MY_OPTION "This is settable from the command line" OFF)

set(SOURCES
    src/Hello.cpp
    src/main.cpp
)

# Set SOURCES recursivly to cpp files inside sources folder without using GLOB_RECURSE
# file(GLOB_RECURSE SOURCES "sources/*.cpp")


add_library(glad STATIC
    ${PROJECT_SOURCE_DIR}/libHeaders/GLAD/glad.c
)
target_include_directories(glad
    PUBLIC
        ${PROJECT_SOURCE_DIR}/libHeaders/GLAD/
)


# add_library(application STATIC two.cpp three.h)
# add_library(calclib STATIC src/calclib.cpp include/calc/lib.hpp)
# target_include_directories(calclib PUBLIC include)
# target_compile_features(calclib PUBLIC cxx_std_11)

add_executable(application $(SOURCES))

target_link_libraries(application
    PRIVATE
        ${PROJECT_SOURCE_DIR}/sources
        glad)
# set(CMAKE_CXX_STANDARD 20)

# set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)

# message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")

# # Add all cpp files located in the "src" directory and its subdirectories
# file(GLOB_RECURSE SOURCES "src/*.cpp")

# message(DEBUG "Sources: ${SOURCES}")

# # Find the SFML library
# # find_package(SFML 2.5.1 REQUIRED)
# include(ExternalProject)
#     ExternalProject_Add(SFML
#         GIT_REPOSITORY    https://github.com/SFML/SFML.git
#         GIT_TAG           master
#         CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/SFML-install
#                             -DSFML_BUILD_EXAMPLES=OFF
#                             -DSFML_BUILD_TESTS=OFF
#                             -DSFML_BUILD_DOC=OFF
#         PREFIX            ${BUILD_DIR}/SFML-prefix
#         SOURCE_DIR        ${BUILD_DIR}/SFML-src
#         BINARY_DIR        ${BUILD_DIR}/SFML-build
#         INSTALL_DIR       ${BUILD_DIR}/SFML-install
# )
# include_directories(${BUILD_DIR}/SFML-install/include)
# link_directories(${BUILD_DIR}/SFML-install/lib)
# set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)


# # Add the executable target
# add_executable(toody-engine ${SOURCES})

# # Link the SFML library to the target
# target_link_libraries(toody-engine ${SFML_LIBRARIES})

# cmake -S . -B build
# # cmake --build build
cmake_minimum_required(VERSION 3.10...3.26)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()



##################### Project Setup #####################

project(ToodyEngine DESCRIPTION "C++ Game Engine"
                    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SUBMODULES_DIR "${PROJECT_SOURCE_DIR}/submodules")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/sources")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.hpp")



##################### Submodules #####################

find_package(Boost REQUIRED)
add_subdirectory(${SUBMODULES_DIR}/fmt)
add_subdirectory(${SUBMODULES_DIR}/SFML)
add_subdirectory(${SUBMODULES_DIR}/assimp)
add_subdirectory(${SUBMODULES_DIR}/glm)
# add_subdirectory(${SUBMODULES_DIR}/imgui-sfml)

add_library(glad STATIC ${SUBMODULES_DIR}/glad/glad.c)
target_include_directories(glad PUBLIC ${SUBMODULES_DIR}/includes/glad)
add_library(imgui STATIC
	${SUBMODULES_DIR}/imgui/imgui.cpp
	${SUBMODULES_DIR}/imgui/imgui.h
	${SUBMODULES_DIR}/imgui/imstb_rectpack.h
	${SUBMODULES_DIR}/imgui/imstb_textedit.h
	${SUBMODULES_DIR}/imgui/imstb_truetype.h
    ${SUBMODULES_DIR}/imgui/imgui_demo.cpp
    ${SUBMODULES_DIR}/imgui/imgui_draw.cpp
    ${SUBMODULES_DIR}/imgui/imgui_internal.h
    ${SUBMODULES_DIR}/imgui/imgui_tables.cpp
    ${SUBMODULES_DIR}/imgui/imgui_widgets.cpp
    ${SUBMODULES_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${SUBMODULES_DIR}/imgui/misc/cpp/imgui_stdlib.h
    ${SUBMODULES_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
)
target_include_directories(imgui PUBLIC
    ${SUBMODULES_DIR}/includes/imgui/
)



##################### target #####################

add_executable(ToodyEngine ${SOURCES} ${TPP_FILES})

target_compile_options(ToodyEngine PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(ToodyEngine PRIVATE fmt SFML assimp glm glad imgui-sfml imgui)

target_include_directories(ToodyEngine PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${SUBMODULES_DIR}/includes
    ${PROJECT_SOURCE_DIR}/sources
)
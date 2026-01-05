cmake_minimum_required(VERSION 3.16)
project(renderer)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_CXX_STANDARD "20")

set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(renderer
        src/main.cpp
        src/obj.cpp
        src/point.cpp
)

# Link to the actual SDL3 library.
target_link_libraries(renderer PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
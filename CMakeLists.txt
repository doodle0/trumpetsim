cmake_minimum_required(VERSION 3.27)
project(trumpetsim)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Export compile commands for clangd support

# Emscripten settings
set(EM_LDFLAGS "-s WASM=1 -s USE_WEBGL2=1 -s USE_SDL=2 -s LEGACY_GL_EMULATION")

# Executables
add_executable(trumpetsim
    src/main.cpp
    src/draw.h
    src/draw.cpp
)
# add_executable(hello sandbox/hello.cpp)
# add_executable(test sandbox/test.cpp)

# Emscripten compilation
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Explicitly export emscripten flags
execute_process(COMMAND em++ --cflags OUTPUT_VARIABLE EM_CFLAGS)

# Target properties
set_target_properties(trumpetsim PROPERTIES
    COMPILE_FLAGS "${EM_CFLAGS}"
    LINK_FLAGS "${EM_LDFLAGS}"
)
# set_target_properties(hello PROPERTIES
#     COMPILE_FLAGS "${EM_CFLAGS}"
#     LINK_FLAGS "${EM_LDFLAGS}"
# )
# set_target_properties(test PROPERTIES
#     COMPILE_FLAGS "${EM_CFLAGS}"
#     LINK_FLAGS "${EM_LDFLAGS}"
# )

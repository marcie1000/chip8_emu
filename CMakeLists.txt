cmake_minimum_required (VERSION 3.10.0)

project (chip8_emu)

# Add cmake directory to the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Add project files
set (HEADERS
    opcodes.h
    emulator.h
    chip8_graphics.h
    chip8_cpu.h
)

set (SOURCES
    chip8_cpu.c
    chip8_graphics.c
    emulator.c
    main.c
    opcodes.c
)

find_package (SDL2 REQUIRED)
find_package (SDL2_mixer REQUIRED)

# ============= SDL2 ============
if (NOT SDL2_FOUND)
    message (FATAL_ERROR "Error: SDL2 not found.")
else ()
    message (STATUS "SDL2 include directory : ${SDL2_INCLUDE_DIRS}")
    message (STATUS "SDL2 library directory : ${SDL2_LIBRARIES}")
    include_directories(${SDL2_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARIES})
endif (NOT SDL2_FOUND)

# =========== SDL2_mixer ============
if (NOT SDL2_mixer_FOUND)
    message (FATAL_ERROR "Error: SDL2_mixer not found.")
else ()
    message (STATUS "SDL2_mixer include directory : ${SDL2_MIXER_INCLUDE_DIRS}")
    message (STATUS "SDL2_mixer library directory : ${SDL2_MIXER_LIBRARIES}")
    include_directories(${SDL2_MIXER_INCLUDE_DIRS})
    link_directories(${SDL2_MIXER_LIBRARIES})
endif (NOT SDL2_mixer_FOUND)

set (CMAKE_C_STANDARD 11)
add_executable (chip8_emu ${SOURCES} ${HEADERS})
target_link_libraries (chip8_emu ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})





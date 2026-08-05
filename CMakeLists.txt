cmake_minimum_required(VERSION 3.25)
project(cstat)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCE_FILES 
    CONFIGURE_DEPENDS
    src/*.c
)

add_executable(cstat
    ${SOURCE_FILES}
)

target_include_directories(cstat PUBLIC 
    src/
)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-O2 -Wall -Wextra)
endif()

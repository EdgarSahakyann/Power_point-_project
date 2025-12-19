cmake_minimum_required(VERSION 3.10)
project(SlideEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(include external)

# Find nlohmann/json (header-only, so just include path)
find_path(NLOHMANN_JSON_INCLUDE_DIR
    NAMES nlohmann/json.hpp
    PATHS ${CMAKE_SOURCE_DIR}/external
    NO_DEFAULT_PATH
)

if(NOT NLOHMANN_JSON_INCLUDE_DIR)
    message(FATAL_ERROR "nlohmann/json.hpp not found in external/nlohmann/")
endif()

set(SOURCES
    src/core/Shape.cpp
    src/core/Slide.cpp
    src/core/SlideFactory.cpp
    src/core/SlideRepository.cpp
    src/commands/Commands.cpp
    src/serialization/JsonSerializer.cpp
    src/parser/Lexer.cpp
    src/parser/Parser.cpp
    src/parser/Tokenizer.cpp
    src/CommandFactory.cpp
    src/main.cpp
)

add_executable(SlideEditor ${SOURCES})

# Link libraries (none needed for header-only json)
target_include_directories(SlideEditor PRIVATE 
    ${CMAKE_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/external
    ${NLOHMANN_JSON_INCLUDE_DIR}
)

# Compiler warnings
if(MSVC)
    target_compile_options(SlideEditor PRIVATE /W4 /WX)
else()
    target_compile_options(SlideEditor PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Debug configuration
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

# Custom target for running tests
add_custom_target(run_tests
    COMMAND ./bin/SlideEditor < ${CMAKE_SOURCE_DIR}/test_commands.txt
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running SlideEditor tests"
    VERBATIM
)

# Install target
install(TARGETS SlideEditor DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(FILES external/nlohmann/json.hpp DESTINATION include/nlohmann)

# CPack configuration (optional)
include(CPack)
cmake_minimum_required(VERSION 3.20)

project(LuminLanguage
    VERSION 0.0.1
    DESCRIPTION "Lumin Language Implementation"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler warnings and optimization flags
include(CheckCXXCompilerFlag)
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Configure build types
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3 -funroll-loops -fomit-frame-pointer -march=native")

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the output directory for static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Define directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Subdirectories in the unified include directory
set(COMMON_INCLUDE_DIR ${INCLUDE_DIR}/common)
set(COMPILER_INCLUDE_DIR ${INCLUDE_DIR}/compiler)
set(VM_INCLUDE_DIR ${INCLUDE_DIR}/vm)
set(DEBUGGER_INCLUDE_DIR ${INCLUDE_DIR}/debugger)

# Glob common sources
file(GLOB_RECURSE COMMON_SOURCES ${SRC_DIR}/common/*.cpp)
file(GLOB_RECURSE COMMON_HEADERS ${COMMON_INCLUDE_DIR}/*.hpp)

# Add common library (lumincommon)
add_library(lumincommon STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})
target_include_directories(lumincommon PUBLIC ${COMMON_INCLUDE_DIR} ${INCLUDE_DIR})
set_target_properties(lumincommon PROPERTIES OUTPUT_NAME lumincommon LINKER_LANGUAGE CXX)

# Compiler executable (luminc)
file(GLOB_RECURSE COMPILER_SOURCES ${SRC_DIR}/compiler/*.cpp)
file(GLOB_RECURSE COMPILER_HEADERS ${COMPILER_INCLUDE_DIR}/*.hpp)
add_executable(luminc ${COMPILER_SOURCES})
target_include_directories(luminc PRIVATE ${COMPILER_INCLUDE_DIR} ${INCLUDE_DIR})
target_link_libraries(luminc PRIVATE lumincommon)
set_target_properties(luminc PROPERTIES OUTPUT_NAME luminc)

# VM executable (lumin)
file(GLOB_RECURSE VM_SOURCES ${SRC_DIR}/vm/*.cpp)
file(GLOB_RECURSE VM_HEADERS ${VM_INCLUDE_DIR}/*.hpp)
add_executable(lumin ${VM_SOURCES})
target_include_directories(lumin PRIVATE ${VM_INCLUDE_DIR} ${INCLUDE_DIR})
target_link_libraries(lumin PRIVATE lumincommon)
set_target_properties(lumin PROPERTIES OUTPUT_NAME lumin)

# Debugger executable (lmdb)
file(GLOB_RECURSE DEBUGGER_SOURCES ${SRC_DIR}/debugger/*.cpp)
file(GLOB_RECURSE DEBUGGER_HEADERS ${DEBUGGER_INCLUDE_DIR}/*.hpp)
add_executable(lmdb ${DEBUGGER_SOURCES})
target_include_directories(lmdb PRIVATE ${DEBUGGER_INCLUDE_DIR} ${INCLUDE_DIR})
target_link_libraries(lmdb PRIVATE lumincommon)
set_target_properties(lmdb PROPERTIES OUTPUT_NAME lmdb)

# Installation
install(TARGETS luminc lumin lmdb RUNTIME DESTINATION bin)
install(TARGETS lumincommon ARCHIVE DESTINATION lib)
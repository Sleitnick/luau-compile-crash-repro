cmake_minimum_required(VERSION 3.16)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(CMAKE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(LUAU_BUILD_CLI OFF)
set(LUAU_BUILD_TESTS OFF)

set(BUILD_SHARED_LIBS OFF)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeBuild")

project(
	LuauIrExample
	VERSION 0.1.0
	LANGUAGES CXX C
)

include(FetchContent)

FetchContent_Declare(
    fetch_luau
    GIT_REPOSITORY https://github.com/luau-lang/luau
    GIT_TAG 0.699
)
FetchContent_MakeAvailable(fetch_luau)

set(EXECUTABLE_NAME ${PROJECT_NAME})
set(BUILD_OPTIONS)

if (MSVC)
	list(APPEND BUILD_OPTIONS "/we4018")
	list(APPEND BUILD_OPTIONS "/we4388")
else()
	list(APPEND BUILD_OPTIONS -Wall)
	list(APPEND BUILD_OPTIONS -Wimplicit-fallthrough)
	list(APPEND BUILD_OPTIONS -Wsign-compare)
	list(APPEND BUILD_OPTIONS -Werror)
endif()

add_executable(${EXECUTABLE_NAME})

target_sources(${EXECUTABLE_NAME} PRIVATE
	main.cpp
)

target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
	Luau.Common
	Luau.VM
	Luau.Compiler
	Luau.CodeGen
)

target_compile_options(${EXECUTABLE_NAME} PRIVATE ${BUILD_OPTIONS})

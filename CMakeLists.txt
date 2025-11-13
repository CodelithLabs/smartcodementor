# ========================================================================
# 🌟 SmartCodeMentor - Cross-Platform C++20 Build System
# ------------------------------------------------------------------------
# Author: Prasanta Ray & Teamcodelith
# Version: 1.2.0 (Professional Refactor)
# ========================================================================

cmake_minimum_required(VERSION 3.20)
project(SmartCodeMentor VERSION 1.2.0 LANGUAGES CXX)

# ------------------------------------------------------------------------
# 🧩 Global Configurations
# ------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# ------------------------------------------------------------------------
# 📂 Directory Paths
# ------------------------------------------------------------------------
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")
set(DOCS_DIR "${CMAKE_SOURCE_DIR}/docs")

# ------------------------------------------------------------------------
# ⚙️ Options
# ------------------------------------------------------------------------
option(ENABLE_TESTS "Enable unit tests" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_SANITIZERS "Enable Address/UB Sanitizers" OFF)
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(ENABLE_STATIC_ANALYSIS "Enable static analysis (cppcheck, clang-tidy)" ON)
option(ENABLE_DOXYGEN "Enable Doxygen documentation" ON)

# ------------------------------------------------------------------------
# 🧱 Core Library & Application (Professional Structure)
# ------------------------------------------------------------------------
message(STATUS "📚 Defining core library and application...")

# This is the robust, professional way. No more fragile GLOB.
add_library(quiz_lib
    src/Question.cpp
    src/Quiz.cpp
    src/QuizAttempt.cpp
)
# We use PUBLIC includes so any target linking quiz_lib also gets the headers
target_include_directories(quiz_lib PUBLIC ${INCLUDE_DIR})

# Create the main EXECUTABLE
add_executable(quiz_app src/main.cpp)
# Link the app against the core logic library
target_link_libraries(quiz_app PRIVATE quiz_lib)

# ------------------------------------------------------------------------
# 🧰 Compiler Configuration Function (OPTIMIZATION)
# ------------------------------------------------------------------------
# This function applies all our settings to ANY target (lib, app, or test)
# ------------------------------------------------------------------------

# Find static analysis tools
if(ENABLE_STATIC_ANALYSIS)
    find_program(CPPCHECK_EXECUTABLE cppcheck)
    find_program(CLANG_TIDY_EXECUTABLE clang-tidy)
endif()

function(apply_common_compile_options target)
    message(STATUS "Applying common compile options to ${target}")

    # Set include directories
    target_include_directories(${target} PRIVATE ${INCLUDE_DIR})

    # Compiler warning flags
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow
        )
        if(ENABLE_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()

    # Build-type specific optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Removed -march=native. It's not portable for CI builds.
        target_compile_options(${target} PRIVATE -O3)
    else()
        # Default to Debug-friendly flags
        target_compile_options(${target} PRIVATE -O0 -g3 -fno-omit-frame-pointer)
    endif()

    # Sanitizers
    if(ENABLE_SANITIZERS)
        message(STATUS "🧩 Sanitizers Enabled for ${target}")
        target_compile_options(${target} PRIVATE -fsanitize=address,undefined)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()

    # Coverage
    if(ENABLE_COVERAGE)
        message(STATUS "📈 Code Coverage Enabled for ${target}")
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_libraries(${target} PRIVATE gcov)
    endif()

    # Integrate static analysis directly into the build
    if(ENABLE_STATIC_ANALYSIS)
        if(CPPCHECK_EXECUTABLE)
            message(STATUS "🔍 Enabling cppcheck for ${target}")
            # FIX 1: Corrected command from 'target_set_property' to 'set_target_properties'
            # FIX 3: Wrapped all cppcheck args in ONE string.
            set_target_properties(${target} PROPERTIES CMAKE_CXX_CPPCHECK
                "${CPPCHECK_EXECUTABLE} --enable=all --inconclusive --std=c++20")
        endif()
        if(CLANG_TIDY_EXECUTABLE)
            message(STATUS "🔬 Enabling clang-tidy for ${target}")
            # You can add a .clang-tidy file to your root to configure checks
            # FIX 1: Corrected command from 'target_set_property' to 'set_target_properties'
            set_target_properties(${target} PROPERTIES CMAKE_CXX_CLANG_TIDY
                "${CLANG_TIDY_EXECUTABLE}")
        endif()
    endif()
endfunction()

# --- Apply settings to our targets ---
apply_common_compile_options(quiz_lib)
apply_common_compile_options(quiz_app)

# ------------------------------------------------------------------------
# 🧪 Unit Tests (GoogleTest)
# ------------------------------------------------------------------------
if(ENABLE_TESTS)
    message(STATUS "🧪 Configuring Unit Tests...")
    include(CTest)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.13.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Find all test files in the test directory
    file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cpp")
    add_executable(run_tests ${TEST_SOURCES})

    # Apply our common settings to the test executable as well
    apply_common_compile_options(run_tests)

    # Link tests against the LIBRARY (quiz_lib)
    target_link_libraries(run_tests PRIVATE gtest_main quiz_lib)

    add_test(NAME SmartCodeMentor_Tests COMMAND run_tests)
endif()

# ------------------------------------------------------------------------
# 📚 Documentation (Doxygen)
# ------------------------------------------------------------------------
if(ENABLE_DOXYGEN)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        message(STATUS "📚 Doxygen found, configuring documentation...")
        
        # Define the output directory path
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/html)
        
        # ----------------------------------------------------------------
        # ✅ REQUIRED FIX: 
        # Explicitly create the output directory before Doxygen runs.
        # This solves the "does not exist and cannot be created" error.
        # ----------------------------------------------------------------
        file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                       ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
                        
        add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            COMMENT "📘 Generating API Documentation (output: ${DOXYGEN_OUTPUT_DIRECTORY})"
        )
    else()
        message(WARNING "Doxygen not found. Skipping documentation build.")
    endif()
endif()

# ------------------------------------------------------------------------
# 🧰 Post-Build Summary
# ------------------------------------------------------------------------
# FIX 2: Replaced all non-standard whitespace with regular spaces
add_custom_command(
    TARGET quiz_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "✅ Build complete for SmartCodeMentor"
    COMMAND ${CMAKE_COMMAND} -E echo "Output: $<TARGET_FILE:quiz_app>"
    COMMAND ${CMAKE_COMMAND} -E echo "Run tests from build dir: ctest"
)

# ------------------------------------------------------------------------
# 📦 Packaging
# ------------------------------------------------------------------------
include(CPack)
set(CPACK_PACKAGE_NAME "SmartCodeMentor")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "ZIP")

message(STATUS "🏁 CMake Configuration Complete 🚀")
# ========================================================================
# 🌟 SmartCodeMentor - Cross-Platform C++20 Build System
# ------------------------------------------------------------------------
# Author: Prasanta Ray & GPT-5
# Version: 1.1.0 (Refactored for Library-First Design)
# ========================================================================

cmake_minimum_required(VERSION 3.20)
project(SmartCodeMentor VERSION 1.1.0 LANGUAGES CXX)

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
option(ENABLE_STATIC_ANALYSIS "Enable static analysis tools" ON)
option(ENABLE_DOXYGEN "Enable Doxygen documentation" ON)

# ------------------------------------------------------------------------
# 🧱 Core Library & Application (THE PRIMARY FIX)
# ------------------------------------------------------------------------
message(STATUS "📚 Defining core library and application...")

# 1. Find all source files in the 'src' directory
file(GLOB_RECURSE ALL_SOURCE_FILES
     "${SRC_DIR}/*.cpp"
     "${SRC_DIR}/*.cc"
     "${SRC_DIR}/*.cxx")

# 2. Separate the main executable file (e.g., main.cpp) from the library files
#    We assume your main function is in 'main.cpp' inside 'src/'.
#    If it's named differently (e.g., quiz_app.cpp), change the name here.
set(MAIN_APP_FILE "${SRC_DIR}/main.cpp")

# 3. Create a list of all *other* source files for the library
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX "${MAIN_APP_FILE}")
set(LIB_SOURCE_FILES ${ALL_SOURCE_FILES})

# 4. Create the CORE LIBRARY (this is the key fix)
#    This library holds all your application logic.
add_library(quiz_lib ${LIB_SOURCE_FILES})
#    We use PUBLIC includes so any target linking quiz_lib also gets the headers
target_include_directories(quiz_lib PUBLIC ${INCLUDE_DIR})

# 5. Create the main EXECUTABLE
add_executable(quiz_app ${MAIN_APP_FILE})
#    Link the app against the core logic library
target_link_libraries(quiz_app PRIVATE quiz_lib)

# ------------------------------------------------------------------------
# 🧰 Compiler Configuration Function (OPTIMIZATION)
# ------------------------------------------------------------------------
# This function applies all our settings to ANY target (lib, app, or test)
# This is far more robust and avoids repeating code.
# ------------------------------------------------------------------------
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
        target_compile_options(${target} PRIVATE -O3 -march=native)
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
    # This prevents gtest from being built/installed at the top level
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cpp")
    add_executable(run_tests ${TEST_SOURCES})

    # Apply our common settings to the test executable as well
    apply_common_compile_options(run_tests)

    # <--- THE FIX --->
    # Link tests against the LIBRARY (quiz_lib), not the executable (quiz_app)
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
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                       ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            COMMENT "📘 Generating API Documentation with Doxygen"
        )
    else()
        message(WARNING "Doxygen not found. Skipping documentation build.")
    endif()
endif()

# ------------------------------------------------------------------------
# 🔍 Static Analysis Integration
# ------------------------------------------------------------------------
if(ENABLE_STATIC_ANALYSIS)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        message(STATUS "🔍 Cppcheck found, adding static_analysis target...")
        add_custom_target(static_analysis
            COMMAND ${CPPCHECK} --enable=all --inconclusive --std=c++20 ${SRC_DIR}
            COMMENT "🔍 Running cppcheck"
        )
    else()
        message(WARNING "cppcheck not found. Skipping static_analysis target.")
    endif()
endif()

# ------------------------------------------------------------------------
# 🧰 Post-Build Summary
# ------------------------------------------------------------------------
add_custom_command(
    TARGET quiz_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "✅ Build complete for SmartCodeMentor"
    COMMAND ${CMAKE_COMMAND} -E echo "Output: $<TARGET_FILE:quiz_app>"
    COMMAND ${CMAKE_COMMAND} -E echo "Run tests (from build dir): ctest"
)

# ------------------------------------------------------------------------
# 📦 Packaging
# ------------------------------------------------------------------------
include(CPack)
set(CPACK_PACKAGE_NAME "SmartCodeMentor")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "ZIP")

message(STATUS "🏁 CMake Configuration Complete 🚀")
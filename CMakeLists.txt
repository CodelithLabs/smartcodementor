# ========================================================================
# 🌟 SmartCodeMentor - Cross-Platform C++20 Build System
# ------------------------------------------------------------------------
# Author: Prasanta Ray & GPT-5
# Version: 1.0.0
# ========================================================================

cmake_minimum_required(VERSION 3.20)
project(SmartCodeMentor VERSION 1.0.0 LANGUAGES CXX)

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
# 🧱 Source Files
# ------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.cc" "${SRC_DIR}/*.cxx")
add_executable(quiz_app ${SOURCE_FILES})
target_include_directories(quiz_app PRIVATE ${INCLUDE_DIR})

# ------------------------------------------------------------------------
# 🧰 Compiler Flags
# ------------------------------------------------------------------------
if(MSVC)
    target_compile_options(quiz_app PRIVATE /W4 /permissive-)
else()
    target_compile_options(quiz_app PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow
    )
    if(ENABLE_WARNINGS_AS_ERRORS)
        target_compile_options(quiz_app PRIVATE -Werror)
    endif()
endif()

# ------------------------------------------------------------------------
# 🚀 Optimizations
# ------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(quiz_app PRIVATE -O3 -march=native)
else()
    target_compile_options(quiz_app PRIVATE -O0 -g3 -fno-omit-frame-pointer)
endif()

# ------------------------------------------------------------------------
# 🧠 Sanitizers
# ------------------------------------------------------------------------
if(ENABLE_SANITIZERS)
    message(STATUS "🧩 Sanitizers Enabled")
    target_compile_options(quiz_app PRIVATE -fsanitize=address,undefined)
    target_link_options(quiz_app PRIVATE -fsanitize=address,undefined)
endif()

# ------------------------------------------------------------------------
# 📈 Coverage
# ------------------------------------------------------------------------
if(ENABLE_COVERAGE)
    message(STATUS "📈 Code Coverage Enabled")
    target_compile_options(quiz_app PRIVATE --coverage -O0 -g)
    target_link_libraries(quiz_app PRIVATE gcov)
endif()

# ------------------------------------------------------------------------
# 🧪 Unit Tests (GoogleTest)
# ------------------------------------------------------------------------
if(ENABLE_TESTS)
    include(CTest)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.13.0
    )
    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cpp")
    add_executable(run_tests ${TEST_SOURCES})
    target_link_libraries(run_tests PRIVATE gtest_main quiz_app)
    add_test(NAME SmartCodeMentor_Tests COMMAND run_tests)
endif()

# ------------------------------------------------------------------------
# 📚 Documentation (Doxygen)
# ------------------------------------------------------------------------
if(ENABLE_DOXYGEN)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                       ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            COMMENT "📘 Generating API Documentation with Doxygen"
        )
    endif()
endif()

# ------------------------------------------------------------------------
# 🔍 Static Analysis Integration
# ------------------------------------------------------------------------
if(ENABLE_STATIC_ANALYSIS)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        add_custom_target(static_analysis
            COMMAND ${CPPCHECK} --enable=all --inconclusive --std=c++20 ${SRC_DIR}
            COMMENT "🔍 Running cppcheck"
        )
    endif()
endif()

# ------------------------------------------------------------------------
# 🧰 Post-Build Summary
# ------------------------------------------------------------------------
add_custom_command(
    TARGET quiz_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "✅ Build complete for SmartCodeMentor"
    COMMAND ${CMAKE_COMMAND} -E echo "Output: ${CMAKE_BINARY_DIR}/quiz_app"
    COMMAND ${CMAKE_COMMAND} -E echo "Run tests: ctest"
)

# ------------------------------------------------------------------------
# 📦 Packaging
# ------------------------------------------------------------------------
include(CPack)
set(CPACK_PACKAGE_NAME "SmartCodeMentor")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "ZIP")

message(STATUS "🏁 CMake Configuration Complete 🚀")

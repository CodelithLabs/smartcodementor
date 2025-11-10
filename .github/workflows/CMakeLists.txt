# ========================================================================
# 🌟 SmartCodeMentor - Ultimate CMake Configuration
# ------------------------------------------------------------------------
# Author: Prasanta Ray & GPT-5
# Version: 0.1.0
# Description:
#   This CMake script sets up a full-featured modern C++20 environment
#   for your SmartCodeMentor project with:
#     ✅ Modular project structure
#     ✅ Cross-platform builds
#     ✅ Static analysis tools
#     ✅ Code coverage
#     ✅ Unit testing
#     ✅ Doxygen documentation
#     ✅ Code formatting
#     ✅ Sanitizers (ASan, UBSan)
#     ✅ Fun, educational comments
# ========================================================================

cmake_minimum_required(VERSION 3.20)

# ------------------------------------------------------------------------
# 🧠 Project Configuration
# ------------------------------------------------------------------------
project(
    SmartCodeMentor
    VERSION 0.1.0
    DESCRIPTION "An intelligent C++ quiz and code learning system"
    LANGUAGES CXX
)

# ------------------------------------------------------------------------
# 🧩 Global Settings
# ------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable verbose build output (for education)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Default build type (Release or Debug)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "🚀 Building SmartCodeMentor in ${CMAKE_BUILD_TYPE} mode")

# ------------------------------------------------------------------------
# 📂 Directory Layout
# ------------------------------------------------------------------------
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")
set(DOCS_DIR "${CMAKE_SOURCE_DIR}/docs")
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
set(BUILD_DIR "${CMAKE_BINARY_DIR}")

# ------------------------------------------------------------------------
# ⚙️ Build Options
# ------------------------------------------------------------------------
option(ENABLE_TESTS "Enable unit tests" ON)
option(ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(ENABLE_SANITIZERS "Enable sanitizers for debugging" OFF)
option(ENABLE_DOXYGEN "Generate documentation using Doxygen" ON)
option(ENABLE_STATIC_ANALYSIS "Run static analysis tools" ON)
option(ENABLE_AUTO_FORMAT "Enable clang-format integration" ON)

# ------------------------------------------------------------------------
# 🧱 Source Files
# ------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.cxx" "${SRC_DIR}/*.cc")
add_executable(quiz_app ${SOURCE_FILES})

target_include_directories(quiz_app PRIVATE ${INCLUDE_DIR})

# ------------------------------------------------------------------------
# 🧰 Compiler Flags & Optimizations
# ------------------------------------------------------------------------
if(MSVC)
    target_compile_options(quiz_app PRIVATE /W4 /permissive-)
else()
    target_compile_options(quiz_app PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Werror=$<BOOL:${ENABLE_WARNINGS_AS_ERRORS}>
    )
endif()

# ------------------------------------------------------------------------
# 🔥 Optimization Flags
# ------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(quiz_app PRIVATE -O3 -march=native)
else()
    target_compile_options(quiz_app PRIVATE -O0 -g3 -fno-omit-frame-pointer)
endif()

# ------------------------------------------------------------------------
# 🧪 Code Coverage
# ------------------------------------------------------------------------
if(ENABLE_COVERAGE)
    message(STATUS "📈 Code coverage enabled")
    target_compile_options(quiz_app PRIVATE --coverage -O0 -g)
    target_link_libraries(quiz_app PRIVATE gcov)
endif()

# ------------------------------------------------------------------------
# 🧹 Clang-Format (Code Formatter)
# ------------------------------------------------------------------------
if(ENABLE_AUTO_FORMAT)
    find_program(CLANG_FORMAT "clang-format")
    if(CLANG_FORMAT)
        add_custom_target(
            format
            COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES}
            COMMENT "✨ Running clang-format on all source files"
        )
    endif()
endif()

# ------------------------------------------------------------------------
# 🔍 Static Analysis (cppcheck & clang-tidy)
# ------------------------------------------------------------------------
if(ENABLE_STATIC_ANALYSIS)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        add_custom_target(
            static_analysis
            COMMAND ${CPPCHECK} --enable=all --inconclusive --std=c++20 --quiet ${SRC_DIR}
            COMMENT "🧠 Running cppcheck analysis"
        )
    endif()

    find_program(CLANG_TIDY clang-tidy)
    if(CLANG_TIDY)
        set_target_properties(quiz_app PROPERTIES CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*")
        message(STATUS "🤖 clang-tidy enabled for quiz_app target")
    endif()
endif()

# ------------------------------------------------------------------------
# 🧠 Address/Undefined Sanitizers (for Debugging)
# ------------------------------------------------------------------------
if(ENABLE_SANITIZERS)
    message(STATUS "🧩 Sanitizers enabled")
    target_compile_options(quiz_app PRIVATE -fsanitize=address,undefined)
    target_link_options(quiz_app PRIVATE -fsanitize=address,undefined)
endif()

# ------------------------------------------------------------------------
# 🧪 Unit Testing Setup
# ------------------------------------------------------------------------
if(ENABLE_TESTS)
    include(CTest)
    enable_testing()

    # Fetch GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.13.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cpp")

    add_executable(run_tests ${TEST_SOURCES})
    target_link_libraries(run_tests PRIVATE gtest_main quiz_app)
    target_include_directories(run_tests PRIVATE ${INCLUDE_DIR})

    add_test(NAME SmartCodeMentor_Tests COMMAND run_tests)

    message(STATUS "🧪 GoogleTest integrated successfully")
endif()

# ------------------------------------------------------------------------
# 📚 Doxygen Documentation
# ------------------------------------------------------------------------
if(ENABLE_DOXYGEN)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target(
            doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "📘 Generating API documentation with Doxygen"
        )
    else()
        message(WARNING "⚠️ Doxygen not found; skipping docs generation")
    endif()
endif()

# ------------------------------------------------------------------------
# 🧠 Post-Build Summary
# ------------------------------------------------------------------------
add_custom_command(
    TARGET quiz_app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "✅ Build complete for SmartCodeMentor"
    COMMAND ${CMAKE_COMMAND} -E echo "🧩 Build Type: ${CMAKE_BUILD_TYPE}"
    COMMAND ${CMAKE_COMMAND} -E echo "📦 Output: ${CMAKE_BINARY_DIR}/quiz_app"
    COMMAND ${CMAKE_COMMAND} -E echo "💡 Tip: Run 'ctest' to execute tests."
)

# ------------------------------------------------------------------------
# 🧰 Packaging Setup
# ------------------------------------------------------------------------
include(CPack)
set(CPACK_PACKAGE_NAME "SmartCodeMentor")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "contact.prasanta.ray@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An interactive quiz and code learning app")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_GENERATOR "ZIP")

# ------------------------------------------------------------------------
# 🎯 Advanced Developer Utilities
# ------------------------------------------------------------------------
# Command: make run
add_custom_target(run
    COMMAND quiz_app
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "🚀 Running SmartCodeMentor..."
)

# Command: make clean_all
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
    COMMENT "🧹 Cleaning entire build directory"
)

# ------------------------------------------------------------------------
# 🧩 Version Info Header Auto-Generation
# ------------------------------------------------------------------------
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/version.hpp.in
    ${CMAKE_BINARY_DIR}/generated/version.hpp
    @ONLY
)
target_include_directories(quiz_app PRIVATE ${CMAKE_BINARY_DIR}/generated)

# ------------------------------------------------------------------------
# 🧮 Summary Display
# ------------------------------------------------------------------------
message(STATUS "")
message(STATUS "================== 💡 SmartCodeMentor Build Summary ==================")
message(STATUS "Project Name:        ${PROJECT_NAME}")
message(STATUS "Version:             ${PROJECT_VERSION}")
message(STATUS "C++ Standard:        ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:          ${CMAKE_BUILD_TYPE}")
message(STATUS "Enable Tests:        ${ENABLE_TESTS}")
message(STATUS "Enable Coverage:     ${ENABLE_COVERAGE}")
message(STATUS "Enable Docs:         ${ENABLE_DOXYGEN}")
message(STATUS "Enable Analysis:     ${ENABLE_STATIC_ANALYSIS}")
message(STATUS "Enable Format:       ${ENABLE_AUTO_FORMAT}")
message(STATUS "=======================================================================")
message(STATUS "")

# ------------------------------------------------------------------------
# 🎉 End of File
# ------------------------------------------------------------------------
message(STATUS "🏁 CMake configuration completed successfully! Happy coding 🚀")

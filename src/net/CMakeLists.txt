cmake_minimum_required(VERSION 3.10)
project(cmake_test VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -g -Wextra -std=c++17  -Wno-unused-parameter -pthread")

#设置输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# #需要在根目录下添加，表示开启项目的测试功能
# enable_testing()
# find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)

set(PINE_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(PINE_CLANG_SEARCH_PATH  "${CMAKE_SOURCE_DIR}/thirdfile/bin")
# clang-format
if (NOT DEFINED CLANG_FORMAT_BIN)
    # attempt to find the binary if user did not specify
    find_program(CLANG_FORMAT_BIN
            NAMES clang-format clang-format-8
            HINTS ${PINE_CLANG_SEARCH_PATH})
endif ()
if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(WARNING "Pine/main couldn't find clang-format.")
else ()
    message(STATUS "Pine/main found clang-format at ${CLANG_FORMAT_BIN}")
endif ()
# # clang-tidy
if (NOT DEFINED CLANG_TIDY_BIN)
    # attempt to find the binary if user did not specify
    find_program(CLANG_TIDY_BIN
            NAMES clang-tidy clang-fidy-8
            HINTS ${PINE_CLANG_SEARCH_PATH})
endif ()
if ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(WARNING "Pine/main couldn't find clang-tidy.")
else ()
    # Output compile_commands.json
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
    message(STATUS "Pine/main found clang-fidy at ${CLANG_TIDY_BIN}")
endif ()

# cpplint
find_program(CPPLINT_BIN
        NAMES cpplint cpplint.py
        HINTS ${PINE_BUILD_SUPPORT_DIR})
if ("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
    message(WARNING "Pine/main couldn't find cpplint.")
else ()
    message(STATUS "Pine/main found cpplint at ${CPPLINT_BIN}")
endif ()
######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# "make format"
# "make check-format"
##########################################
string(CONCAT PINE_FORMAT_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src,"
        )

# runs clang format and updates files in place.
add_custom_target(format ${PINE_BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${PINE_BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${PINE_FORMAT_DIRS}
        --fix
        --quiet
        )

# runs clang format and exits with a non-zero exit code if any files need to be reformatted
add_custom_target(check-format ${PINE_BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${PINE_BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${PINE_FORMAT_DIRS}
        --quiet
        )

##########################################
# "make cpplint"
##########################################

file(GLOB_RECURSE PINE_LINT_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
        )

# Balancing act: cpplint.py takes a non-trivial time to launch,
# so process 12 files per invocation, while still ensuring parallelism
add_custom_target(cpplint echo '${PINE_LINT_FILES}' | xargs -n12 -P8
        ${CPPLINT_BIN}
        --verbose=2 --quiet
        --linelength=120
        --filter=-legal/copyright,-build/include_subdir,-readability/casting
        )

###########################################################
# "make clang-tidy" target
###########################################################
# runs clang-tidy and exits with a non-zero exit code if any errors are found.
# note that clang-tidy automatically looks for a .clang-tidy file in parent directories
add_custom_target(clang-tidy
        ${PINE_BUILD_SUPPORT_DIR}/run_clang_tidy.py                     # run LLVM's clang-tidy script
        -clang-tidy-binary ${CLANG_TIDY_BIN}                              # using our clang-tidy binary
        -p ${CMAKE_BINARY_DIR}                                            # using cmake's generated compile commands
        )
# add_dependencies(check-clang-tidy pine_shared)                    # needs gtest headers, compile_commands.json


message( "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Removing all generated files.")


add_subdirectory(test)
add_subdirectory(src)


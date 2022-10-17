cmake_minimum_required(VERSION 3.15)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name.
set(PROJECT_NAME history)
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too much problem for students.

##########################################################
# Project files, packages, libraries and so on
##########################################################
#! add subdirectories for external commands

#! Project main executable source compilation
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories(/usr/include/readline)

add_executable(${PROJECT_NAME} history_main.cpp)

#! Put path to your project headers
target_include_directories(${PROJECT_NAME} PRIVATE options_parser)

#! Add external packages
# options_parser requires boost::program_options library
find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
find_package(Readline)

target_include_directories(${PROJECT_NAME} PRIVATE ${Readline_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Readline_LIBRARY})

##########################################################
# Fixed CMakeLists.txt part
##########################################################

INSTALL(PROGRAMS
                $<TARGET_FILE:${PROJECT_NAME}>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
                DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.8)
project(nt_wrapper VERSION 3.2 LANGUAGES CXX)

include(CTest)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(NT_WRAPPER_TARGET_NAME         ${PROJECT_NAME})
set(NT_WRAPPER_INCLUDE_INSTALL_DIR "include")
set(NT_WRAPPER_INCLUDE_BUILD_DIR   "${PROJECT_SOURCE_DIR}/include/")

add_library(${NT_WRAPPER_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${NT_WRAPPER_TARGET_NAME} ALIAS ${NT_WRAPPER_TARGET_NAME})
target_compile_features(${NT_WRAPPER_TARGET_NAME} INTERFACE cxx_std_20)

find_path(PHNT_INCLUDE_DIR phnt.h)
include_directories(${PHNT_INCLUDE_DIR})

target_include_directories(
    ${NT_WRAPPER_TARGET_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${NT_WRAPPER_INCLUDE_BUILD_DIR}>
    $<INSTALL_INTERFACE:include>
)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

install(
    DIRECTORY ${NT_WRAPPER_INCLUDE_BUILD_DIR}
    DESTINATION ${NT_WRAPPER_INCLUDE_INSTALL_DIR}
)

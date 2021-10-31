# Determine if doctest is built as a subproject (using add_subdirectory) or if it is the main project.
set(MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

if(MAIN_PROJECT)
    message(STATUS "PUM UnitTest settings begin")
    cmake_minimum_required(VERSION 3.14)    

    project(pum VERSION 0.3.0 LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 17)#C++17    

    # test
    include(CTest)
    include(FetchContent)
    FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/onqtam/doctest GIT_TAG 2.4.5)
    FetchContent_MakeAvailable(doctest)
    enable_testing()
    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)        

    add_executable(tests test/test.cpp)        

    doctest_discover_tests(tests)

    # Include directories
    target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test)
    
    target_link_libraries(tests PRIVATE doctest::doctest)
    message(STATUS "PUM UnitTest settings end")
else()
    message(STATUS "PUM Pico USB-MIDI library")
    add_library(pum INTERFACE)    
    target_include_directories(pum INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
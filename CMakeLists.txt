# Determine if doctest is built as a subproject (using add_subdirectory) or if it is the main project.
set(MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

if(MAIN_PROJECT)
    message("main project")
    cmake_minimum_required(VERSION 3.14)    

    project(pum VERSION 0.2.4 LANGUAGES CXX C ASM)
    set(CMAKE_CXX_STANDARD 17)#C++17    


    # test
    include(CTest)
    include(FetchContent)
    FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/onqtam/doctest GIT_TAG 2.4.5)
    FetchContent_MakeAvailable(doctest)
    enable_testing()
    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)    

    # tinyUSB
    FetchContent_Populate(tinyusb GIT_REPOSITORY https://github.com/hathach/tinyusb GIT_TAG 0.12.0)    

    add_executable(tests 
        test/test.cpp
        test/usb_descriptors.c
        ${tinyusb_SOURCE_DIR}/src/class/midi/midi_device.c
        ${tinyusb_SOURCE_DIR}/src/class/audio/audio_device.c
        ${tinyusb_SOURCE_DIR}/src/common/tusb_fifo.c)

    doctest_discover_tests(tests)

    # Include directories
    target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test
        PUBLIC ${tinyusb_SOURCE_DIR}/src
        PUBLIC ${tinyusb_SOURCE_DIR}/src/class/midi
        PUBLIC ${tinyusb_SOURCE_DIR}/src/class/audio
        PUBLIC ${tinyusb_SOURCE_DIR}/src/common
    )
    
    target_link_libraries(tests PRIVATE doctest::doctest)
    target_compile_definitions(tests PRIVATE CFG_TUSB_MCU=OPT_MCU_RP2040)
else()
    message("NOT main project")
    message("PROJECT_NAME: ${PROJECT_NAME}")
    add_library(pum INTERFACE)
    # add_library(pum::pum ALIAS pum)
    # add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
    # target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
    target_include_directories(pum INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
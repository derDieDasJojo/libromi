cmake_minimum_required(VERSION 3.10)
project(libromi-project)

############################################################

set(CMAKE_MODULES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)
set(CMAKE_THIRDPARTY_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

############################################################

include(${CMAKE_MODULES_DIRECTORY}/RomiFunctions.cmake)
set(PI_BUILD FALSE)
set_romi_build_defaults(PI_BUILD)
message("${PROJECT_NAME} PI_BUILD = ${PI_BUILD}")
output_compile_flags()

############################################################

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)

############################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

############################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(COMMON_COMPILATION_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${COMMON_COMPILATION_FLAGS}")
set(CMAKE_C_FLAGS "${COMMON_COMPILATION_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

############################################################

# Options.
# Build tests by default. We want to run tests whether we are developing or building a release. I mean... why wouldn't you? :)
option(BUILD_TESTS "Build all tests." ON)
option(BUILD_COVERAGE "Build coverage." ON)
option(BUILD_ARDUINO "Build arduino projects." ON)

if(BUILD_TESTS)

    include(CTest)
    enable_testing()

    if (NOT DEFINED googletest_SOURCE_DIR)
        if (CMAKE_VERSION VERSION_LESS 3.2)
            set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
        else()
            set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
        endif()

        include(${CMAKE_MODULES_DIRECTORY}/DownloadProject.cmake)
        download_project(
                PROJ                googletest
                GIT_REPOSITORY      https://github.com/google/googletest.git
                GIT_TAG             v1.10.x
                PREFIX              ${CMAKE_THIRDPARTY_DIRECTORY}/googletest
                ${UPDATE_DISCONNECTED_IF_AVAILABLE}
                )

        # Prevent GoogleTest from overriding our compiler/linker options
        # when building with Visual Studio
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

endif() # BUILD_TESTS

# Can't build for coverage without the test.
if (BUILD_COVERAGE AND BUILD_TESTS)
    if (NOT DEFINED lcov_SOURCE_DIR)
        include(${CMAKE_MODULES_DIRECTORY}/CodeCoverage.cmake)

        # lcov doesn't build with CMake so we download, set lcov as an external target, then call make.
        # we then manually add the bin path so that we can call it later.
        download_project(   PROJ              lcov
                URL               https://github.com/linux-test-project/lcov/releases/download/v1.15/lcov-1.15.tar.gz
                PREFIX            ${CMAKE_THIRDPARTY_DIRECTORY}/lcov
                TIMEOUT           180
                ${UPDATE_DISCONNECTED_IF_AVAILABLE}
                )

        set(LCOV_PATH "${lcov_SOURCE_DIR}/bin/lcov")
        set(GENHTML_PATH "${lcov_SOURCE_DIR}/bin/genhtml")
    endif()

    set( COVERAGE_EXCLUDES
            "/usr/*"
            "${CMAKE_SOURCE_DIR}/thirdparty/*"
            "${CMAKE_SOURCE_DIR}/librcom/*"
            "*test/*")
    append_coverage_compiler_flags()

endif()

######################################

if (${BUILD_ARDUINO})
    SET(ROMISERIAL_ARDUINO_LIB ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/RomiSerial.zip)
    include(${CMAKE_MODULES_DIRECTORY}/ArduinoCliFunctions.cmake)

    SET(ARDUINOCLI_DOWNLOAD_URL, "")
    build_arduino_cli_download_url(ARDUINOCLI_DOWNLOAD_URL)
    MESSAGE("Download URL ${ARDUINOCLI_DOWNLOAD_URL}")

    if(NOT ${ARDUINOCLI_DOWNLOAD_URL} STREQUAL "")

        if (NOT DEFINED arduinocli_SOURCE_DIR)
            # arduinocli doesn't build with CMake so we download, set arduinocli as an external target, then call make.
            # we then manually add the bin path so that we can call it later.
            download_project(
                    PROJ              arduinocli
                    URL               ${ARDUINOCLI_DOWNLOAD_URL}
                    PREFIX            ${CMAKE_THIRDPARTY_DIRECTORY}/arduinocli
                    TIMEOUT           180
                    ${UPDATE_DISCONNECTED_IF_AVAILABLE}
                    )
        endif()
        set(ARDUINOCLI_EXE "${arduinocli_SOURCE_DIR}/arduino-cli")

        arduino_cli_function ("config" "init")
    #    We need to set this config entry to true to install RomiSerial from .zip
        arduino_cli_function ("config" "set" "library.enable_unsafe_install" "true")
        arduino_cli_function ("core" "update-index")
        arduino_cli_function ("core" "install" "arduino:avr")
#        arduino_cli_function ("core" "install" "arduino:sam")

    #    Add any other Adruino hosted library dependencies here.
        arduino_cli_function ("lib" "install" "PID")
        arduino_cli_function ("lib" "install" "LiquidCrystal")
    else()
            MESSAGE(WARNING "unsupported setting BUILD_ARDUINO OFF")
            set(BUILD_ARDUINO  OFF)
    endif()
endif()

######################################

if (NOT DEFINED librcom_SOURCE_DIR)
    set(LIBRCOM_DIR "${CMAKE_SOURCE_DIR}/librcom")
    find_path(LIBRCOM_DIRECTORY
            NAMES "CMakeLists.txt"
            PATHS ${LIBRCOM_DIR})
    #    Save the current Build test settings so we can restore them after including librcom. We don't want to build libr tests.
    SET(SAVED_TEST_OPTION BUILD_TESTS)
    SET(BUILD_TESTS OFF)

    if (NOT LIBRCOM_DIRECTORY)
        MESSAGE("${PROJECT_NAME} - ${LIBRCOM_DIR} does not exist. Downloading dependency.")
        MESSAGE("${PROJECT_NAME} - WARNING")
        MESSAGE("${PROJECT_NAME} - The dependency is cloned to support libromi development only.")
        MESSAGE("${PROJECT_NAME} - To work on rcom please see: https://github.com/romi/romi-rover-build-and-test")

        download_project(
                PROJ              librcom
                GIT_REPOSITORY    https://github.com/romi/librcom.git
                GIT_TAG           remove_libr
                TIMEOUT           180
                SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/librcom
                BINARY_DIR        ${CMAKE_CURRENT_BINARY_DIR}/librcom
                ${UPDATE_DISCONNECTED_IF_AVAILABLE})
        add_subdirectory(${LIBRCOM_DIR})
    else()
        MESSAGE("${PROJECT_NAME} - ${LIBRCOM_DIRECTORY} exists. Not downloading dependency.")
        add_subdirectory(librcom)
    endif()
    # Restore build tests option.
    SET(BUILD_TESTS OFF SAVED_TEST_OPTION)
endif()

######################################

if (NOT DEFINED eigen3_SOURCE_DIR)
    set (EIGEN_VERSION "3.3.9" CACHE STRING "Select Eigen Version.")
    download_project(
            PROJ              eigen3
            URL               https://gitlab.com/libeigen/eigen/-/archive/${EIGEN_VERSION}/eigen-${EIGEN_VERSION}.tar.bz2
            PREFIX            ${CMAKE_THIRDPARTY_DIRECTORY}/eigen3
            TIMEOUT           180
            ${UPDATE_DISCONNECTED_IF_AVAILABLE}
    )
    include_directories ("${eigen3_SOURCE_DIR}")
endif()

######################################

include(${CMAKE_MODULES_DIRECTORY}/RomiProjectBuildOptions.cmake)

######################################
add_subdirectory(binortools)
add_subdirectory(arduino_libraries)
#add_subdirectory(firmware)
add_subdirectory(libromi)


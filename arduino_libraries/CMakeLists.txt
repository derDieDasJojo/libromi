
cmake_minimum_required(VERSION 3.10)
project(romiserial)

set(romiserial_VERSION_MAJOR 0)
set(romiserial_VERSION_MINOR 1)

set(SOURCES
  RomiSerial/IInputStream.h
  RomiSerial/IOutputStream.h
  RomiSerial/CRC8.h
  RomiSerial/CRC8.cpp
  RomiSerial/EnvelopeParser.h
  RomiSerial/EnvelopeParser.cpp
  RomiSerial/MessageParser.h
  RomiSerial/MessageParser.cpp
  RomiSerial/IRomiSerialClient.h
  RomiSerial/RomiSerialClient.h
  RomiSerial/RomiSerialClient.cpp
  RomiSerial/RomiSerial.h
  RomiSerial/RomiSerial.cpp
  RomiSerial/RSerial.h
  RomiSerial/RSerial.cpp
  RomiSerial/Printer.h
  RomiSerial/Printer.cpp
  RomiSerial/Reader.h
  RomiSerial/Reader.cpp
  RomiSerial/RomiSerialUtil.h
  RomiSerial/RomiSerialUtil.cpp
)

add_library(romiserial SHARED ${SOURCES})

target_include_directories(romiserial
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/RomiSerial"
        )

target_link_libraries(romiserial rpp)

if (${BUILD_ARDUINO})
    add_custom_command(OUTPUT ${ROMISERIAL_ARDUINO_LIB} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E tar "cvf" "${ROMISERIAL_ARDUINO_LIB}" --format=zip -- ${CMAKE_CURRENT_SOURCE_DIR}/RomiSerial/
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS
                romiserial
            COMMENT
                "Creating ${ROMISERIAL_ARDUINO_LIB}"
            )

    SET(LIBNSTALL_ROMISERIAL_PARAMS "lib" "install" "--zip-path" "${ROMISERIAL_ARDUINO_LIB}")
    MESSAGE( "LIBNSTALL_ROMISERIAL_PARAMS = ${LIBNSTALL_ROMISERIAL_PARAMS}")
    add_custom_target(install_romiserial_lib ALL
            COMMAND "${ARDUINOCLI_EXE}" ${LIBNSTALL_ROMISERIAL_PARAMS}
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS
                ${ROMISERIAL_ARDUINO_LIB}
            COMMENT
                "Installing ${ROMISERIAL_ARDUINO_LIB}"
            )
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

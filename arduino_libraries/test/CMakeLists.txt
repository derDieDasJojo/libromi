
cmake_minimum_required(VERSION 3.10)

set(SRCS_ENVELOPE_PARSER
  src/tests_main.cpp
  src/test_envelope_parser.cpp)

add_executable(libromi_romiserial_envelope_parser ${SRCS_ENVELOPE_PARSER})

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../RomiSerial" )

target_link_libraries(libromi_romiserial_envelope_parser romiserial gtest gmock)

add_test(
    NAME libromi_romiserial_envelope_parser
    COMMAND libromi_romiserial_envelope_parser
)

set(SRCS_MESSAGE_PARSER
  src/tests_main.cpp
  src/test_message_parser.cpp)

add_executable(libromi_romiserial_message_parser ${SRCS_MESSAGE_PARSER})

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../RomiSerial" )

target_link_libraries(libromi_romiserial_message_parser romiserial gtest gmock)

add_test(
    NAME libromi_romiserial_message_parser
    COMMAND libromi_romiserial_message_parser
)



set(SRCS_ROMISERIAL
  src/tests_main.cpp
  src/test_romiserial.cpp
  mock/mock_inputstream.h
  mock/mock_outputstream.h)

add_executable(libromi_romiserial ${SRCS_ROMISERIAL})

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../RomiSerial" )

target_link_libraries(libromi_romiserial romiserial gtest gmock)

add_test(
    NAME libromi_romiserial
    COMMAND libromi_romiserial
)



set(SRCS_ROMISERIALCLIENT
  src/tests_main.cpp
  src/test_romiserialclient.cpp
  mock/mock_inputstream.h
  mock/mock_outputstream.h)

add_executable(libromi_romiserial_client ${SRCS_ROMISERIALCLIENT})

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../RomiSerial" )

target_link_libraries(libromi_romiserial_client romiserial gtest gmock)

add_test(
    NAME libromi_romiserial_client
    COMMAND libromi_romiserial_client
)



set(SRCS_ROMISERIAL_ARDUINO
  src/tests_main.cpp
  src/test_romiserial_arduino.cpp)

add_executable(libromi_romiserial_arduino ${SRCS_ROMISERIAL_ARDUINO})

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../RomiSerial" )

target_link_libraries(libromi_romiserial_arduino romiserial gtest gmock)


if(BUILD_COVERAGE)
    SETUP_TARGET_FOR_COVERAGE_LCOV(
            NAME libromi_romiserial_coverage
            EXECUTABLE ctest -V ${n_cores}
            EXCLUDE '*wrapper*' "src/Linux.cpp" "RomiSerial/*.h" "include/*" '${CMAKE_BINARY_DIR}/libr/*'
            DEPENDENCIES
                libromi_romiserial_envelope_parser
                libromi_romiserial_message_parser
                libromi_romiserial
                libromi_romiserial_client)
endif()

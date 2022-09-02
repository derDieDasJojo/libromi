############################################################

function (arduino_cli_function)
    MESSAGE( "arduini cli call:")
    execute_process (COMMAND "${ARDUINOCLI_EXE}" ${ARGV}
            COMMAND_ECHO
                STDOUT
            RESULT_VARIABLE
                CMD_ERROR
            OUTPUT_VARIABLE
                CMD_OUTPUT)
#    MESSAGE( "ERROR:" ${CMD_ERROR})
    MESSAGE("${CMD_OUTPUT}")

endfunction()


function (build_arduino_cli_download_url ARDUINO_CLI_DOWNLOAD_URL )
    cmake_host_system_information (RESULT 64BIT QUERY IS_64BIT)
#    We only support linux32/64 or PI32/64
    Message("Pi Build = ${PI_BUILD}")
    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        message (FATAL_ERROR "${CMAKE_SYSTEM_NAME} not supported. We only support linux32/64 or PI32/64 at this time." PARENT_SCOPE)
    else()
        if(CMAKE_SYSTEM_PROCESSOR MATCHES ".*aarch64")
            set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_ARM64.tar.gz" PARENT_SCOPE)
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES ".*armv7")
            set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_ARMv7.tar.gz" PARENT_SCOPE)
        elseif (CMAKE_SYSTEM_PROCESSOR MATCHES ".*x86_64")
            set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_64bit.tar.gz" PARENT_SCOPE)
        elseif (CMAKE_SYSTEM_PROCESSOR MATCHES ".*x86_32")
            set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_32bit.tar.gz" PARENT_SCOPE)
        else()
            #message (FATAL_ERROR "Architecture ${CMAKE_SYSTEM_PROCESSOR} on ${CMAKE_SYSTEM_NAME} not supported. We only support linux32/64 arm/x64 at this time." PARENT_SCOPE)
            Message("Architecture ${CMAKE_SYSTEM_PROCESSOR} on ${CMAKE_SYSTEM_NAME} not supported. We only support linux32/64 arm/x64 at this time.")
        endif()
    endif()
endfunction()
############################################################

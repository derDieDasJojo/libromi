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
    if (${PI_BUILD})
        if (64BIT)
            set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_ARM64.tar.gz" PARENT_SCOPE)
        else()
            set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_Linux_ARMv7.tar.gz" PARENT_SCOPE)
        endif()
    else()
        if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
            message (FATAL_ERROR "${CMAKE_SYSTEM_NAME} not supported. We only support linux32/64 or PI32/64 at this time." PARENT_SCOPE)
            else()
                if (64BIT)
                    set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_64bit.tar.gz" PARENT_SCOPE)
                else()
                    set(${ARDUINO_CLI_DOWNLOAD_URL} "https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_32bit.tar.gz" PARENT_SCOPE)
                endif()
        endif ()
    endif()
endfunction()
############################################################

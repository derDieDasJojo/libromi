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

############################################################

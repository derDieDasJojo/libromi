############################################################

function (arduino_cli_function)
    execute_process (COMMAND "${ARDUINOCLI_EXE}" ${ARGV}
            RESULT_VARIABLE
                CMD_ERROR
            OUTPUT_FILE
                CMD_OUTPUT)
    MESSAGE( "arduino_cli_function parameters ${ARGV}")
    MESSAGE( "arduini_cli_function ERROR:" ${CMD_ERROR})
    MESSAGE( "arduini_cli_function OUTPUT:" ${CMD_OUTPUT})
endfunction()

############################################################

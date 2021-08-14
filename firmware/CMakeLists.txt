cmake_minimum_required(VERSION 3.10)

# We add the tests whether we are building the binaries or not.
add_subdirectory(Azhoo/tests)

if (${BUILD_ARDUINO})

    MESSAGE( "Arduino Firmware output = SourceDir/build")
    add_custom_target(Azhoo ALL
            COMMAND "${ARDUINOCLI_EXE}" "compile" "--clean" "--export-binaries" "--fqbn" "arduino:avr:uno" "${CMAKE_CURRENT_SOURCE_DIR}/Azhoo"
            COMMENT
                "Building Arduino Azhoo"
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_SOURCE_DIR}/Azhoo
            DEPENDS
                install_romiserial_lib
            )

    add_custom_target(CrystalDisplay ALL
            COMMAND "${ARDUINOCLI_EXE}" "compile" "--clean" "--export-binaries" "--fqbn" "arduino:avr:uno" "${CMAKE_CURRENT_SOURCE_DIR}/CrystalDisplay"
            COMMENT
            "Building Arduino CrystalDisplay"
            WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/CrystalDisplay
            DEPENDS
            install_romiserial_lib
            )

    add_custom_target(Oquam ALL
            COMMAND "${ARDUINOCLI_EXE}" "compile" "--clean" "--export-binaries" "--fqbn" "arduino:avr:uno" "${CMAKE_CURRENT_SOURCE_DIR}/Oquam"
            COMMENT
            "Building Arduino Oquam"
            WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/Oquam
            DEPENDS
            install_romiserial_lib
            )

    MESSAGE(WARNING "BLDC, BrushMotorController, SabertoothController, ToolCarrierJrkG2 do not compile. Need updating.")

#    add_custom_target(BLDC ALL
#            COMMAND "${ARDUINOCLI_EXE}" "compile" "--clean" "--export-binaries" "--fqbn" "arduino:avr:uno" "${CMAKE_CURRENT_SOURCE_DIR}/BLDC"
#            COMMENT
#            "Building Arduino BLDC"
#            WORKING_DIRECTORY
#            ${CMAKE_CURRENT_SOURCE_DIR}
#            DEPENDS
#            install_romiserial_lib
#            )
#
#    add_custom_target(BrushMotorController ALL
#            COMMAND "${ARDUINOCLI_EXE}" "compile" "--clean" "--export-binaries" "--fqbn" "arduino:avr:uno" "${CMAKE_CURRENT_SOURCE_DIR}/BrushMotorController"
#            COMMENT
#            "Building Arduino BrushMotorController"
#            WORKING_DIRECTORY
#            ${CMAKE_CURRENT_SOURCE_DIR}
#            DEPENDS
#            install_romiserial_lib
#            )
#
#    add_custom_target(SabertoothController ALL
#            COMMAND "${ARDUINOCLI_EXE}" "compile" "--clean" "--export-binaries" "--fqbn" "arduino:avr:uno" "${CMAKE_CURRENT_SOURCE_DIR}/SabertoothController"
#            COMMENT
#            "Building Arduino SabertoothController"
#            WORKING_DIRECTORY
#            ${CMAKE_CURRENT_SOURCE_DIR}
#            DEPENDS
#            install_romiserial_lib
#            )
#
#    add_custom_target(ToolCarrierJrkG2 ALL
#            COMMAND "${ARDUINOCLI_EXE}" "compile" "--clean" "--export-binaries" "--fqbn" "arduino:avr:uno" "${CMAKE_CURRENT_SOURCE_DIR}/ToolCarrierJrkG2"
#            COMMENT
#            "Building Arduino ToolCarrierJrkG2"
#            WORKING_DIRECTORY
#            ${CMAKE_CURRENT_SOURCE_DIR}
#            DEPENDS
#            install_romiserial_lib
#            )

endif()


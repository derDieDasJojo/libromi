cmake_minimum_required(VERSION 3.10)
project(libromi)

############################################################

output_compile_flags()

############################################################

set(romi_VERSION_MAJOR 0)
set(romi_VERSION_MINOR 1)

############################################################
if (${PI_BUILD})
        set( PICAMERA_LIB_PATH_ROOT "/opt/vc")

        find_path(PICAMERA_INCLUDE_DIR
                NAMES bcm_host.h
                PATHS ${PICAMERA_LIB_PATH_ROOT}/include)
                
        find_library(PICAMERA_SHARED_LIB
                NAMES mmal
                PATHS ${PICAMERA_LIB_PATH_ROOT}/lib)

        add_library(mmal SHARED IMPORTED GLOBAL)   
        set_property(TARGET mmal 
                        PROPERTY
                        IMPORTED_LOCATION ${PICAMERA_SHARED_LIB})    

        set_property(TARGET mmal 
                        PROPERTY
                        INTERFACE_INCLUDE_DIRECTORIES "${PICAMERA_INCLUDE_DIR}"
                )
        set(PI_CAMERA_LIB mmal)
else()
        set(PI_CAMERA_LIB "")
endif()
############################################################

if(${PI_BUILD})
    set(PICAMERA_SOURCES
      include/picamera/BaseCamera.h
      include/picamera/CameraComponent.h
      include/picamera/Component.h
      include/picamera/Connection.h
      include/picamera/EncoderComponent.h
      include/picamera/ImageEncoder.h
      include/picamera/MMAL.h
      include/picamera/PiCamera.h
      include/picamera/PiCameraSettings.h
      include/picamera/Semaphore.h
      include/picamera/StillCamera.h
      include/picamera/VideoCamera.h
      include/picamera/VideoEncoder.h
      src/picamera/BaseCamera.cpp
      src/picamera/CameraComponent.cpp
      src/picamera/Component.cpp
      src/picamera/Connection.cpp
      src/picamera/EncoderComponent.cpp
      src/picamera/ImageEncoder.cpp
      src/picamera/MMAL.cpp
      src/picamera/PiCamera.cpp
      src/picamera/PiCameraSettings.cpp
      src/picamera/Semaphore.cpp
      src/picamera/StillCamera.cpp
      src/picamera/VideoCamera.cpp
      src/picamera/VideoEncoder.cpp
      include/cablebot/Cablebot.h
      src/cablebot/Cablebot.cpp)

    set(ROVER_SOURCES
      )
else()
   set(PICAMERA_SOURCES)
   set(ROVER_SOURCES
        include/api/INavigation.h
     
        include/cv/connected.h
        include/cv/ConnectedComponentsImage.h
        include/cv/ImageCropper.h
        include/cv/IImageCropper.h
        include/cv/IPrincipalComponentAnalysis.h
        
        include/oquam/ATDC.h
        include/oquam/ICNCController.h
        include/oquam/FakeCNCController.h
        include/oquam/Helix.h
        include/oquam/is_valid.h
        include/oquam/Oquam.h
        include/oquam/OquamSettings.h
        include/oquam/plotter.h
        include/oquam/print.h
        include/oquam/SmoothPath.h
        include/oquam/Section.h
        include/oquam/StepperController.h
        include/oquam/StepperSettings.h

        include/rover/DifferentialSteering.h
        include/rover/DoubleSteering.h
        include/rover/EventsAndStates.h
        include/rover/IDistanceMeasure.h
        include/rover/IMUTrackFollower.h
        include/rover/ISteering.h
        include/rover/ISteeringController.h
        include/rover/ITrackFollower.h
        include/rover/INavigationController.h
        include/rover/L1NavigationController.h
        include/rover/LocationTracker.h
        include/rover/ManualTrackFollower.h
        include/rover/Rover.h
        include/rover/RoverInterface.h
        include/rover/RoverNotifications.h
        include/rover/RoverOptions.h
        include/rover/RoverScriptEngine.h
        include/rover/RoverStateMachine.h
        include/rover/Navigation.h
        include/rover/PythonTrackFollower.h
        include/rover/SpeedController.h
        include/rover/SteeringData.h
        include/rover/NavigationSettings.h
        include/rover/ISpeedController.h
        include/rover/SpeedConverter.h
        include/rover/SteeringController.h
        include/rover/StepperSteering.h
        include/rover/WheelOdometry.h
        include/rover/ZeroNavigationController.h
        
        include/ui/CrystalDisplay.h
        include/ui/JoystickInputDevice.h
        include/ui/IJoystickEventMapper.h
        include/ui/LinuxJoystick.h
        include/ui/Menu.h
        include/ui/RemoteStateInputDevice.h
        include/ui/IScriptEngine.h
        include/ui/ScriptMenu.h
        include/ui/UIEventMapper.h

        src/cv/ConnectedComponentsImage.cpp
        src/cv/ImageCropper.cpp
        src/cv/PrincipalComponentAnalysis.cpp
        src/cv/Superpixels.cpp

        src/oquam/ATDC.cpp
        src/oquam/Helix.cpp
        src/oquam/is_valid.cpp
        src/oquam/Oquam.cpp
        src/oquam/OquamSettings.cpp
        src/oquam/plotter.cpp
        src/oquam/print.cpp
        src/oquam/SmoothPath.cpp
        src/oquam/Section.cpp
        src/oquam/StepperSettings.cpp
        src/oquam/StepperController.cpp

        src/rover/DifferentialSteering.cpp
        src/rover/DoubleSteering.cpp
        src/rover/EventsAndStates.cpp
        src/rover/IMUTrackFollower.cpp
        src/rover/L1NavigationController.cpp
        src/rover/LocationTracker.cpp
        src/rover/ManualTrackFollower.cpp
        src/rover/NavigationSettings.cpp
        src/rover/PythonTrackFollower.cpp
        src/rover/Rover.cpp
        src/rover/RoverInterface.cpp
        src/rover/RoverOptions.cpp
        src/rover/RoverScriptEngine.cpp
        src/rover/RoverStateMachine.cpp
        src/rover/Navigation.cpp
        src/rover/SpeedController.cpp
        src/rover/SpeedConverter.cpp
        src/rover/SteeringController.cpp
        src/rover/StepperSteering.cpp
        src/rover/WheelOdometry.cpp

        src/ui/CrystalDisplay.cpp
        src/ui/JoystickInputDevice.cpp
        src/ui/LinuxJoystick.cpp
        src/ui/UIEventMapper.cpp
        src/ui/RemoteStateInputDevice.cpp
        src/ui/ScriptMenu.cpp

      )
endif()
        
set(SOURCES

        include/IFileCabinet.h
        include/IFolder.h
        include/v3.h

        include/api/ActivityHelper.h
        include/api/EventTimer.h
        include/api/IEventTimer.h
        include/api/DataLog.h
        include/api/DataLogAccessor.h
        include/api/IActivity.h
        include/api/ICamera.h
        include/api/ICNC.h
        include/api/IDataLog.h
        include/api/IDisplay.h
        include/api/IEventSource.h
        include/api/IGimbal.h
        include/api/IImager.h
        include/api/IInputDevice.h
        include/api/IMotorDriver.h
        include/api/INotifications.h
        include/api/IPowerDevice.h
        include/api/IWeeder.h
        include/api/IJoystick.h
        include/api/Path.h
        include/api/StateMachine.h

        include/battery_monitor/IBatteryMonitor.h
        include/battery_monitor/BatteryMonitor.h

        include/cablebot/CablebotBase.h
        
        #include/camera/Camera360.h
        include/camera/CameraInfo.h
        include/camera/CameraInfoIO.h
        include/camera/CameraIntrinsics.h
        include/camera/CameraSettings.h
        include/camera/ICameraDistortion.h
        include/camera/ICameraInfo.h
        include/camera/ICameraInfoIO.h
        include/camera/ICameraIntrinsics.h
        include/camera/ICameraIntrinsics.h
        include/camera/ICameraSettings.h
        include/camera/Imager.h
        include/camera/FileCamera.h
        include/camera/SimpleRadialDistortion.h        
        include/camera/USBCamera.h

        include/configuration/ConfigurationProvider.h
        include/configuration/IOptions.h
        include/configuration/GetOpt.h

        include/cv/cv.h
        include/cv/Image.h
        include/cv/ImageIO.h
        include/cv/slic.h

        include/fake/FakeCNC.h
        include/fake/FakeDisplay.h
        include/fake/FakeInputDevice.h
        include/fake/FakeMotorDriver.h
        include/fake/FakeNavigation.h
        include/fake/FakeNotifications.h
        include/fake/FakeScriptEngine.h
        include/fake/FakeWeeder.h

        include/hal/BldcGimbal.h
        include/hal/BrushMotorDriver.h
        include/hal/FakeGimbal.h
        include/hal/ImagingDevice.h

        include/notifications/FluidSoundNotifications.h


        include/rpc/CameraAdaptor.h
        include/rpc/CNCAdaptor.h
        include/rpc/GimbalAdaptor.h
        include/rpc/IRPCClient.h
        include/rpc/IRPCHandler.h
        include/rpc/IRPCServer.h
        include/rpc/MethodsActivity.h
        include/rpc/MethodsCamera.h
        include/rpc/MethodsCNC.h
        include/rpc/MethodsGimbal.h
        include/rpc/MethodsNavigation.h
        include/rpc/MethodsRover.h
        include/rpc/MethodsWeeder.h
        include/rpc/NavigationAdaptor.h
        include/rpc/RcomClient.h
        include/rpc/RcomMessageHandler.h
        include/rpc/RcomServer.h
        include/rpc/RemoteCamera.h
        include/rpc/RemoteCNC.h
        include/rpc/RemoteGimbal.h
        include/rpc/RemoteNavigation.h
        include/rpc/RemoteStub.h
        include/rpc/RemoteWeeder.h
        include/rpc/IScriptHub.h
        include/rpc/ScriptHub.h
        include/rpc/WeederAdaptor.h
        include/rpc/RemoteFunctionCallNames.h

        include/ui/ScriptList.h

        src/api/CNCRange.cpp
        src/api/DataLog.cpp
        src/api/DataLogAccessor.cpp
        src/api/Path.cpp
        src/api/ActivityHelper.cpp
        src/api/EventTimer.cpp

        src/battery_monitor/BatteryMonitor.cpp

        src/cablebot/CablebotBase.cpp

        src/hal/BldcGimbal.cpp
        src/hal/BrushMotorDriver.cpp
        src/hal/FakeGimbal.cpp
        src/hal/ImagingDevice.cpp

        src/configuration/ConfigurationProvider.cpp
        src/configuration/GetOpt.cpp
        
        #src/camera/Camera360.cpp
        src/camera/CameraInfo.cpp
        src/camera/CameraInfoIO.cpp
        src/camera/CameraIntrinsics.cpp
        src/camera/CameraSettings.cpp
        src/camera/SimpleRadialDistortion.cpp
        src/camera/FileCamera.cpp
        src/camera/USBCamera.cpp
        src/camera/camera_v4l.h
        src/camera/camera_v4l.c
        src/camera/Imager.cpp

        src/cv/cv.cpp
        src/cv/Image.cpp
        src/cv/ImageIO.cpp
        src/cv/slic.cpp
        
        src/fake/FakeScriptEngine.cpp

        src/notifications/FluidSoundNotifications.cpp

        src/rpc/CameraAdaptor.cpp
        src/rpc/CNCAdaptor.cpp
        src/rpc/GimbalAdaptor.cpp
        src/rpc/NavigationAdaptor.cpp
        src/rpc/RcomClient.cpp
        src/rpc/RcomMessageHandler.cpp
        src/rpc/RcomServer.cpp
        src/rpc/RemoteCamera.cpp
        src/rpc/RemoteCNC.cpp
        src/rpc/RemoteGimbal.cpp
        src/rpc/RemoteNavigation.cpp
        src/rpc/RemoteStub.cpp
        src/rpc/RemoteWeeder.cpp
        src/rpc/ScriptHub.cpp
        src/rpc/ScriptHubListener.cpp
        src/rpc/WeederAdaptor.cpp

        src/ui/ScriptList.cpp
        
        src/v3.cpp
        )

set(DATA_PROVIDER_SOURCES
        include/data_provider/CNCLocationProvider.h
        include/data_provider/Gps.h
        include/data_provider/GpsLocationProvider.h
        include/data_provider/IGPS.h
        include/data_provider/IIdentityProvider.h
        include/data_provider/ILocationProvider.h
        include/data_provider/IOrientationProvider.h
        include/data_provider/ISoftwareVersion.h
        include/data_provider/JsonFieldNames.h
        include/data_provider/RomiDeviceData.h
        include/data_provider/RoverIdentityProvider.h
        include/data_provider/SoftwareVersion.h
        
        src/data_provider/CNCLocationProvider.cpp
        src/data_provider/Gps.cpp
        src/data_provider/GpsLocationProvider.cpp        
        src/data_provider/RoverIdentityProvider.cpp
        src/data_provider/SoftwareVersion.cpp
        src/data_provider/RomiDeviceData.cpp
        
        include/session/IMetaFolder.h
        include/session/ISession.h
        include/session/MetaFolder.h
        include/session/Session.h
        
        src/session/MetaFolder.cpp
        src/session/Session.cpp
        
        include/debug_tools/debug_data_dumper.h
        src/debug_tools/debug_data_dumper.cpp)

add_library(romi SHARED ${SOURCES} ${DATA_PROVIDER_SOURCES} ${PICAMERA_SOURCES} ${ROVER_SOURCES})

target_include_directories(romi
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

target_link_libraries(romi
        romiserial
        r
        rpp
        rcom
        fluidsynth
        ortools
        ${PI_CAMERA_LIB})

add_subdirectory(test/mock)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()


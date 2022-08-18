#ifndef _ROMI_PI_CAMERA_SETTINGS_H_
#define _ROMI_PI_CAMERA_SETTINGS_H_

#include <stdint.h>
#include "PiCameraIncludes.h"

namespace romi {
        
        static const size_t kV2FullWidth = 3280;
        static const size_t kV2FullHeight = 2464;
        static const size_t kV2HalfWidth = 1640;
        static const size_t kV2HalfHeight = 1232;
        static const size_t kHQFullWidth = 4056;
        static const size_t kHQFullHeight = 3040;
        static const size_t kHQHalfWidth = 2028;
        static const size_t kHQHalfHeight = 1520;

        // This is a decent default bitrate for 1080p
        static const uint32_t kDefaultBitRate =  17000000;
        static const uint32_t kMaxBitrateMJPEG = 25000000; // 25Mbits/s
        
        static const int32_t kVariableFrameRate = 0;
        
        struct ColourEffects
        {
                bool enable;
                uint32_t u;
                uint32_t v;
        };

        struct FloatRectangle
        {
                float x;
                float y;
                float w;
                float h;
        };

        enum CameraMode
        {
                kStillMode,
                kVideoMode
        };

        struct PiCameraSettings
        {
                CameraMode mode_;
                int32_t framerate_;
                uint32_t bitrate_;
                size_t max_width_;
                size_t max_height_;
                size_t width_;
                size_t height_;
                size_t preview_width_;
                size_t preview_height_;
                int32_t sharpness_;       // -100 to 100
                int32_t contrast_;        // -100 to 100
                int32_t brightness_;      //  0 to 100
                int32_t saturation_;      //  -100 to 100
                uint32_t iso_;        //  TODO : what range?
                bool video_stabilisation_;  
                int32_t exposure_compensation_;  // -10 to +10 ?
                MMAL_PARAM_EXPOSUREMODE_T exposure_mode_;
                MMAL_PARAM_EXPOSUREMETERINGMODE_T exposure_meter_mode_;
                MMAL_PARAM_AWBMODE_T awb_mode_;
                float awb_gains_r_;   // AWB red gain
                float awb_gains_b_;   // AWB blue gain
                MMAL_PARAM_IMAGEFX_T image_effect_;
                ColourEffects colour_effects_;
                MMAL_PARAM_FLICKERAVOID_T flicker_avoid_mode_;
                int32_t rotation_;    // 0-359
                bool hflip_;
                bool vflip_;
                FloatRectangle roi_;  // region of interest to use on the sensor. Normalised [0,1] values in the rect
                MMAL_PARAMETER_DRC_STRENGTH_T drc_level_;  // Strength of Dynamic Range compression to apply
                bool stats_pass_;     // Stills capture statistics pass on/off
                bool focus_window_;
                float analog_gain_;   // Analog gain [1, 12]
                float digital_gain_;  // Digital gain [1, 64]
                uint32_t shutter_speed_;   // 0 = auto, otherwise the shutter speed in ms


                PiCameraSettings();
                virtual ~PiCameraSettings() = default;
                
                bool is_valid();

                static bool is_resolution_valid(size_t width, size_t height);
                static bool is_preview_valid(size_t width, size_t height);
                static bool is_saturation_valid(int32_t saturation);
                static bool is_sharpness_valid(int32_t sharpness);
                static bool is_contrast_valid(int32_t contrast);
                static bool is_brightness_valid(int32_t brightness);
                static bool is_iso_valid(uint32_t iso);
                static bool is_exposure_compensation_valid(int32_t value);
                static bool is_roi_valid(float x, float y, float w, float h);
                static bool is_shutter_speed_valid(uint32_t speed);
                static bool are_awb_gains_valid(float red, float blue);
                static bool is_analog_gain_valid(float value);
                static bool is_digital_gain_valid(float value);
                static bool is_colour_effects_valid(uint32_t u,  uint32_t v);
        };

        struct V2StillCameraSettings : public PiCameraSettings {
                V2StillCameraSettings(size_t width, size_t height);
                ~V2StillCameraSettings() override = default;
        };

        struct HQStillCameraSettings : public PiCameraSettings {
                HQStillCameraSettings(size_t width, size_t height);
                ~HQStillCameraSettings() override = default;
        };

        struct V2VideoCameraSettings : public PiCameraSettings {
                V2VideoCameraSettings(size_t width, size_t height,
                                      int32_t framerate);
                ~V2VideoCameraSettings() override = default;
        };

        struct HQVideoCameraSettings : public PiCameraSettings {
                HQVideoCameraSettings(size_t width, size_t height,
                                      int32_t framerate);
                ~HQVideoCameraSettings() override = default;
        };
}

#endif // _ROMI_PI_CAMERA_SETTINGS_H_




        


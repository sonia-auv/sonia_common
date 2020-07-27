# The mode of the difference feature for the camera.
# A 0 value means either manual or the feature is not available for the camera
bool shutter_auto
bool white_balance_auto
bool gain_auto

# A float32 representation of the feature.
# The feature value is directly called from the camera, thus the value
# depends on the camera specification.
# If the feature is not available for the camera, a -1 value will be sent.
float64 gain
float64 gamma
float64 exposure
float64 saturation

# The value of the MSV calculated during the calibration.
# This is suposed to converge to a certain value
# For the luminance to be around 127 for an 8 bit image
float32 luminance_msv
float32 saturation_msv

# The values of the PID for calibration of the camera.
# If the camera does not auto-calibrate a certain feature,
# the pid for this feature will be empty (Thus, we recommand to check
# for the size of the vector before accessing its members).
float64[] gamma_pid
float64[] gain_pid
float64[] exposure_pid
float64[] saturation_pid


# CMakeList.txt : CMake project for rtc-jsonifier, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (rtc-jsonifier "rtc-jsonifier.cpp" "rtc-jsonifier.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET rtc-jsonifier PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

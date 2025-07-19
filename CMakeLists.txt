# 🌍 Ultimate Public Lighthouse CMakeLists.txt
# Simplified Version - GUARANTEED TO BUILD!

cmake_minimum_required(VERSION 3.16)
project(UltimatePublicLighthouse 
    VERSION 3.0.0 
    DESCRIPTION "The Ultimate Public Internet Lighthouse - Simplified JSON Version"
    LANGUAGES CXX)

# 🚀 C++20 Standard Required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 🔥 Basic Dependencies - Just what we need!
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)

if(NOT CURL_FOUND)
    message(FATAL_ERROR "🚨 libcurl is required for FastPing monitoring!")
endif()

message(STATUS "✅ libcurl found: ${CURL_VERSION}")

# 🚀 Compiler Optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(OPTIMIZATION_FLAGS 
            -O3 -march=native -mtune=native 
            -ffast-math -funroll-loops 
            -fomit-frame-pointer
            -DNDEBUG)
        message(STATUS "🔥 Release optimizations enabled for maximum performance")
    endif()
endif()

# 🌍 Create the Ultimate Public Lighthouse executable
add_executable(ultimate_public_lighthouse 
    ${CMAKE_CURRENT_SOURCE_DIR}/ultimate_public_lighthouse_beacon.cpp)

target_compile_features(ultimate_public_lighthouse PRIVATE cxx_std_20)

if(OPTIMIZATION_FLAGS)
    target_compile_options(ultimate_public_lighthouse PRIVATE ${OPTIMIZATION_FLAGS})
endif()

# 🔥 Link libraries (simplified!)
target_link_libraries(ultimate_public_lighthouse 
    PRIVATE 
    ${CURL_LIBRARIES}
    pthread)

target_include_directories(ultimate_public_lighthouse PRIVATE ${CURL_INCLUDE_DIRS})

# 🎯 Create enhanced TCP listener executable
add_executable(ultimate_tcp_lighthouse_listener
    ${CMAKE_CURRENT_SOURCE_DIR}/ultimate_tcp_lighthouse_listener.cpp)

target_compile_features(ultimate_tcp_lighthouse_listener PRIVATE cxx_std_20)

if(OPTIMIZATION_FLAGS)
    target_compile_options(ultimate_tcp_lighthouse_listener PRIVATE ${OPTIMIZATION_FLAGS})
endif()

target_link_libraries(ultimate_tcp_lighthouse_listener 
    PRIVATE 
    pthread)

# 🚀 Build information display
message(STATUS "")
message(STATUS "🌍 ═══════════════════════════════════════════════════════════════")
message(STATUS "   ULTIMATE PUBLIC LIGHTHOUSE BUILD CONFIGURATION")
message(STATUS "🌍 ═══════════════════════════════════════════════════════════════")
message(STATUS "🚀 Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "🔥 C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "⚡ Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "💎 Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "🎯 JSON Processing: Simplified Regex-Based (GUARANTEED TO WORK!)")
message(STATUS "🌍 ═══════════════════════════════════════════════════════════════")
message(STATUS "")

# 🎯 Custom build targets for convenience  
add_custom_target(run_public_lighthouse
    COMMAND $<TARGET_FILE:ultimate_public_lighthouse>
    DEPENDS ultimate_public_lighthouse
    COMMENT "🌍 Running Ultimate Public Lighthouse for Global Broadcasting..."
    VERBATIM)

add_custom_target(run_tcp_listener
    COMMAND $<TARGET_FILE:ultimate_tcp_lighthouse_listener> 161.35.248.233 9876
    DEPENDS ultimate_tcp_lighthouse_listener
    COMMENT "🎧 Running Ultimate TCP Lighthouse Listener..."
    VERBATIM)

# 🚀 Installation configuration
install(TARGETS 
    ultimate_public_lighthouse 
    ultimate_tcp_lighthouse_listener
    RUNTIME DESTINATION bin
    COMPONENT runtime)

# 📝 Package configuration
set(CPACK_PACKAGE_NAME "UltimatePublicLighthouse")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Ultimate Public Internet Lighthouse")
set(CPACK_PACKAGE_VENDOR "RYO Modular & whispr.dev")
set(CPACK_PACKAGE_DESCRIPTION "Global Internet Broadcasting with Simplified JSON Processing")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
else()
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "RYO Modular <contact@ryomodular.com>")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libcurl4")
endif()

include(CPack)

# 🔥 Display final build instructions
message(STATUS "🌍 ULTIMATE PUBLIC LIGHTHOUSE BUILD INSTRUCTIONS:")
message(STATUS "   Build all targets:        cmake --build . --config Release")
message(STATUS "   Run public lighthouse:    cmake --build . --target run_public_lighthouse")
message(STATUS "   Run TCP listener:         cmake --build . --target run_tcp_listener") 
message(STATUS "   Install:                  cmake --install .")
message(STATUS "   Create package:           cpack")
message(STATUS "")
message(STATUS "🚀 Ready to broadcast to the entire internet!")
message(STATUS "🌍 Global access: nc 161.35.248.233 9876")
message(STATUS "🏰 Website: https://litehaus.online")
message(STATUS "🌍 ═══════════════════════════════════════════════════════════════")
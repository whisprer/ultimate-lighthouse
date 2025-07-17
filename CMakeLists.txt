# 🌍 Ultimate Public Lighthouse CMakeLists.txt
# Powered by RTC's Jsonifier for Global Broadcasting Excellence

cmake_minimum_required(VERSION 3.20)
project(UltimatePublicLighthouse 
    VERSION 3.0.0 
    DESCRIPTION "The Ultimate Public Internet Lighthouse with RTC Jsonifier"
    LANGUAGES CXX)

# 🚀 C++20 Standard Required for Maximum Performance
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 🔥 CPU Architecture Detection and Optimization
include(CheckCXXSourceCompiles)
include(ProcessorCount)
ProcessorCount(CPU_COUNT)

# Detect CPU capabilities for maximum SIMD optimization
function(detect_cpu_features)
    message(STATUS "🔍 Detecting CPU capabilities for ultimate public performance...")
    
    # Test for AVX-512
    check_cxx_source_compiles("
        #include <immintrin.h>
        int main() { 
            __m512i vec = _mm512_setzero_si512(); 
            return 0; 
        }" 
        HAS_AVX512)
    
    # Test for AVX2
    check_cxx_source_compiles("
        #include <immintrin.h>
        int main() { 
            __m256i vec = _mm256_setzero_si256(); 
            return 0; 
        }" 
        HAS_AVX2)
    
    # Test for AVX
    check_cxx_source_compiles("
        #include <immintrin.h>
        int main() { 
            __m128i vec = _mm_setzero_si128(); 
            return 0; 
        }" 
        HAS_AVX)
    
    # Calculate CPU instruction level for RTC Jsonifier
    set(CPU_INSTRUCTION_LEVEL 0)
    if(HAS_AVX)
        math(EXPR CPU_INSTRUCTION_LEVEL "${CPU_INSTRUCTION_LEVEL} + 32")
        message(STATUS "✅ AVX support detected")
    endif()
    if(HAS_AVX2)
        math(EXPR CPU_INSTRUCTION_LEVEL "${CPU_INSTRUCTION_LEVEL} + 64")
        message(STATUS "✅ AVX2 support detected")
    endif()
    if(HAS_AVX512)
        math(EXPR CPU_INSTRUCTION_LEVEL "${CPU_INSTRUCTION_LEVEL} + 128")
        message(STATUS "🚀 AVX-512 support detected - MAXIMUM POWER!")
    endif()
    
    set(JSONIFIER_CPU_INSTRUCTIONS ${CPU_INSTRUCTION_LEVEL} PARENT_SCOPE)
    message(STATUS "🔥 CPU Instruction Level: ${CPU_INSTRUCTION_LEVEL}")
endfunction()

detect_cpu_features()

# 🚀 Fetch RTC's Jsonifier - The Ultimate JSON Library
include(FetchContent)

message(STATUS "🚀 Fetching RTC's Jsonifier for ultimate public performance...")
FetchContent_Declare(
    Jsonifier
    GIT_REPOSITORY https://github.com/RealTimeChris/Jsonifier.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

# Configure Jsonifier with our detected CPU capabilities
set(JSONIFIER_CPU_INSTRUCTIONS ${JSONIFIER_CPU_INSTRUCTIONS} CACHE STRING "CPU instruction set level")
FetchContent_MakeAvailable(Jsonifier)

# 🔥 Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)

if(NOT CURL_FOUND)
    message(FATAL_ERROR "🚨 libcurl is required for FastPing monitoring!")
endif()

message(STATUS "✅ libcurl found: ${CURL_VERSION}")

# 🚀 Compiler-Specific Optimizations for Public Performance
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(OPTIMIZATION_FLAGS 
        -O3 -march=native -mtune=native 
        -ffast-math -funroll-loops 
        -fomit-frame-pointer -flto
        -DNDEBUG)
    if(HAS_AVX512)
        list(APPEND OPTIMIZATION_FLAGS -mavx512f -mavx512dq -mavx512cd -mavx512bw -mavx512vl)
    elseif(HAS_AVX2)
        list(APPEND OPTIMIZATION_FLAGS -mavx2 -mbmi2)
    elseif(HAS_AVX)
        list(APPEND OPTIMIZATION_FLAGS -mavx)
    endif()
    message(STATUS "🔥 GCC optimization flags configured for ultimate performance")
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(OPTIMIZATION_FLAGS 
        -O3 -march=native -mtune=native 
        -ffast-math -funroll-loops 
        -fomit-frame-pointer -flto
        -DNDEBUG)
    if(HAS_AVX512)
        list(APPEND OPTIMIZATION_FLAGS -mavx512f -mavx512dq -mavx512cd -mavx512bw -mavx512vl)
    elseif(HAS_AVX2)
        list(APPEND OPTIMIZATION_FLAGS -mavx2 -mbmi2)
    elseif(HAS_AVX)
        list(APPEND OPTIMIZATION_FLAGS -mavx)
    endif()
    message(STATUS "🔥 Clang optimization flags configured for ultimate performance")
endif()

# 🌍 Create the Ultimate Public Lighthouse executable
add_executable(ultimate_public_lighthouse 
    ${CMAKE_CURRENT_SOURCE_DIR}/ultimate_public_lighthouse_beacon.cpp)

# 🚀 Configure target properties
target_compile_features(ultimate_public_lighthouse PRIVATE cxx_std_20)
target_compile_options(ultimate_public_lighthouse PRIVATE ${OPTIMIZATION_FLAGS})

# 🔥 Link libraries
target_link_libraries(ultimate_public_lighthouse 
    PRIVATE 
    jsonifier::Jsonifier
    ${CURL_LIBRARIES}
    pthread)

# Include directories
target_include_directories(ultimate_public_lighthouse PRIVATE ${CURL_INCLUDE_DIRS})

# 🎯 Create enhanced TCP listener executable
add_executable(ultimate_tcp_lighthouse_listener
    ${CMAKE_CURRENT_SOURCE_DIR}/ultimate_tcp_lighthouse_listener.cpp)

target_compile_features(ultimate_tcp_lighthouse_listener PRIVATE cxx_std_20)
target_compile_options(ultimate_tcp_lighthouse_listener PRIVATE ${OPTIMIZATION_FLAGS})

target_link_libraries(ultimate_tcp_lighthouse_listener 
    PRIVATE 
    jsonifier::Jsonifier
    pthread)

# 🚀 Build information display
message(STATUS "")
message(STATUS "🌍 ═══════════════════════════════════════════════════════════════")
message(STATUS "   ULTIMATE PUBLIC LIGHTHOUSE BUILD CONFIGURATION")
message(STATUS "🌍 ═══════════════════════════════════════════════════════════════")
message(STATUS "🚀 Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "🔥 C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "⚡ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "💎 Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "🎯 CPU Count: ${CPU_COUNT}")
message(STATUS "🔥 CPU Instructions: ${JSONIFIER_CPU_INSTRUCTIONS}")

if(HAS_AVX512)
    message(STATUS "🚀 SIMD Level: AVX-512 (MAXIMUM POWER FOR GLOBAL BROADCASTING!)")
elseif(HAS_AVX2)
    message(STATUS "⚡ SIMD Level: AVX2 (HIGH PERFORMANCE FOR PUBLIC SERVICE)")
elseif(HAS_AVX)
    message(STATUS "🔥 SIMD Level: AVX (ACCELERATED PUBLIC PERFORMANCE)")
else()
    message(STATUS "📊 SIMD Level: Standard (Still Fast for Global Use!)")
endif()

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
set(CPACK_PACKAGE_DESCRIPTION "Global Internet Broadcasting with RTC's Jsonifier")

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
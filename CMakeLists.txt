# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("graphics-engine")

# Include sub-projects.
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build your own zlib")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build package with shared libraries.")
add_subdirectory ("thirdparty/assimp")

if (WIN32)
	set(BUILD_XAUDIO_WIN10 ON CACHE BOOL "Build for XAudio 2.9")
	set(BUILD_XAUDIO_WIN8 OFF CACHE BOOL "Build for XAudio 2.8")
	add_subdirectory ("thirdparty/DirectXTK")
endif()
add_subdirectory ("thirdparty/imgui-cmake")
add_subdirectory ("thirdparty/lua-cmake")
add_subdirectory ("thirdparty/lua-intf-cmake")

add_subdirectory ("engine")
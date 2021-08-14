# CMakeList.txt : CMake project for engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(engine)

set(TASK_SYSTEM_HEADERS
    include/engine/task_system/constants.hpp
    include/engine/task_system.hpp
)

set(PLATFORM_HEADERS
    include/engine/platform/threading.hpp
)

set(SCRIPT_SYSTEM_HEADERS
    include/engine/script_system.hpp
)

set(HEADERS
    include/engine/engine.hpp
    ${TASK_SYSTEM_HEADERS}
    ${PLATFORM_HEADERS}
    ${SCRIPT_SYSTEM_HEADERS}
)

set(TASK_SYSTEM_SOURCES
    source/task_system.cpp
)

if (WIN32)
    set(PLATFORM win)
endif()

if (PLATFORM)
    set(PLATFORM_SOURCES
        source/platform/${PLATFORM}/threading.cpp
    )
endif()

set(SCRIPT_SYSTEM_SOURCES
    source/script_system.cpp
)

set(SOURCES
    source/main.cpp
    ${TASK_SYSTEM_SOURCES}
    ${PLATFORM_SOURCES}
    ${SCRIPT_SYSTEM_SOURCES}
)

if (WIN32)
    add_executable (${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES})
else()
    add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})
endif()

if (WIN32)
    set(HEADERS_WIN32
        include/engine/win/i.h
    )
    set(SOURCES_WIN32
        source/win/source.cpp
    )
    set(ADDITIONAL_FILES
        DeclareDPIAware.manifest
    )
    target_sources(${PROJECT_NAME} PRIVATE ${HEADERS_WIN32} ${SOURCES_WIN32} ${ADDITIONAL_FILES})
endif()

#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
#target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
target_include_directories(${PROJECT_NAME} PRIVATE include)

set(LIBRARIES
    imgui
    lua
    lua-intf
    assimp
)

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

if (WIN32)
    set(LIBRARIES_WIN32
        DirectXTK
        d3d11
        dxgi
        d3dcompiler
        dxguid
    )

    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_WIN32})
endif()

# TODO: Add tests and install targets if needed.

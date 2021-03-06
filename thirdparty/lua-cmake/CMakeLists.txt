cmake_minimum_required (VERSION 3.8)

project ("lua")

set(SOURCES
    ../lua/lapi.c
    ../lua/lcode.c
    ../lua/lctype.c
    ../lua/ldebug.c
    ../lua/ldo.c
    ../lua/ldump.c
    ../lua/lfunc.c
    ../lua/lgc.c
    ../lua/llex.c
    ../lua/lmem.c
    ../lua/lobject.c
    ../lua/lopcodes.c
    ../lua/lparser.c
    ../lua/lstate.c
    ../lua/lstring.c
    ../lua/ltable.c
    ../lua/ltm.c
    ../lua/lundump.c
    ../lua/lvm.c
    ../lua/lzio.c
    ../lua/lauxlib.c
    ../lua/lbaselib.c
    ../lua/lbitlib.c
    ../lua/lcorolib.c
    ../lua/ldblib.c
    ../lua/liolib.c
    ../lua/lmathlib.c
    ../lua/loslib.c
    ../lua/lstrlib.c
    ../lua/ltablib.c
    ../lua/lutf8lib.c
    ../lua/loadlib.c
    ../lua/linit.c
)

add_library (${PROJECT_NAME} STATIC ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC c_std_11 cxx_std_20)
target_compile_definitions(${PROJECT_NAME} PRIVATE MAKE_LIB)

target_include_directories(${PROJECT_NAME} PUBLIC ../lua)
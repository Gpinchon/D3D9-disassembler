cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

project(D3D9-Disassembler LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(D3D_LIBDIR "$ENV{DXSDK_DIR}/Lib/x64")
    set(D3D_INCLUDE_DIR "$ENV{DXSDK_DIR}/Include")
    find_library(D3D_LIB
        NAMES d3dx9.lib
        PATHS ${D3D_LIBDIR}
        DOC   "Where the D3D9 library resides")
endif (WIN32)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9HLSLStringifyer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9Instruction.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9Opcode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9ParamSrcModifier.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9ParamDstModifier.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9Register.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9RegisterKey.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9RegisterType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9RegisterUsage.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9Shader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9ShaderParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9ShaderType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9Swizzle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9TextureType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DX9Tokens.hpp
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DX9HLSLStringifyer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DX9Register.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DX9ShaderParser.cpp)

add_executable(D3D9Decompiler
    ${HEADER_FILES}
    ${SOURCE_FILES})
target_link_directories(
    D3D9Decompiler PUBLIC
    ${D3D_LIBDIR})
target_include_directories(
    D3D9Decompiler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${D3D_INCLUDE_DIR})
target_link_libraries(
    D3D9Decompiler PUBLIC
    ${D3D_LIB})

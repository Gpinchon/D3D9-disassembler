#pragma once

#include <string>
#include <vector>

#include <DX9Instruction.hpp>
#include <DX9Register.hpp>
#include <DX9ShaderType.hpp>

namespace DX9 {
struct Shader {
    unsigned versionMinor, versionMajor;
    ShaderType shaderType;
    std::string entryPoint = "main";
    Register registers;
    std::vector<Instruction> instructions;
};
}

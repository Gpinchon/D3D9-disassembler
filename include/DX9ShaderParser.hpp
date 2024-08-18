#pragma once

#include <DX9Tokens.hpp>

#include <vector>

namespace DX9 {
struct Shader;
struct ShaderParser {
    void operator()(Shader& a_Shader, Token* const a_Data, const size_t& a_Size);
};
}

#pragma once

#include <DX9RegisterKey.hpp>

namespace DX9 {
struct Register {
    Register();
    const RegisterKey* GetKey(const RegisterType& a_Type, const uint32_t& a_Index) const;
    RegisterKey* GetKey(const RegisterType& a_Type, const uint32_t& a_Index);
    std::array<Input, 10> input; // max 10
    std::array<Temp, 32> temp; // max 32
    std::array<ConstFloat, 224> constFloat; // max 224
    std::array<ConstInt, 16> constInt; // max 16
    std::array<ConstBool, 16> constBool; // max 16
    std::array<Predicate, 1> predicate; // max 1
    std::array<Sampler, 16> sampler; // max 16
    // std::array<DX9Face, 1> face; // max 1
    // std::array<DX9Pos, 1> pos; // max 1
    std::array<LoopCounter, 1> loop; // max 1
    std::array<Texture, 1> texture; // max 1
    std::array<ColorOutput, 32> colorOut;
    std::array<DepthOutput, 1> depthOut;
};

}

#pragma once

#include <DX9RegisterType.hpp>
#include <DX9RegisterUsage.hpp>
#include <DX9TextureType.hpp>

#include <array>
#include <cstdint>

namespace DX9 {
class RegisterKey {
public:
    RegisterType type;
    RegisterUsage usage;
    bool partialPrecision = false;
    uint32_t index = -1, usageIndex = -1, uses = 0;

protected:
    RegisterKey(const RegisterType& a_Type)
        : type(a_Type)
    {
    }
};

class Input : public RegisterKey {
public:
    Input()
        : RegisterKey(RegisterType::Input)
    {
    }
    std::array<float, 4> data = { 0, 0, 0, 0 };
};

class Temp : public RegisterKey {
public:
    Temp()
        : RegisterKey(RegisterType::Temp)
    {
    }
    std::array<float, 4> data = { 0, 0, 0, 0 };
};

class ConstFloat : public RegisterKey {
public:
    ConstFloat()
        : RegisterKey(RegisterType::Const)
    {
    }
    std::array<float, 4> data = { 0, 0, 0, 0 };
};

class ConstInt : public RegisterKey {
public:
    ConstInt()
        : RegisterKey(RegisterType::ConstInt)
    {
    }
    std::array<int32_t, 4> data = { 0, 0, 0, 0 };
};

class ConstBool : public RegisterKey {
public:
    ConstBool()
        : RegisterKey(RegisterType::ConstBool)
    {
    }
    std::array<bool, 1> data = { false };
};

class Predicate : public RegisterKey {
public:
    Predicate()
        : RegisterKey(RegisterType::Predicate)
    {
    }
    std::array<int32_t, 1> data = { 0 };
};

class Sampler : public RegisterKey {
public:
    Sampler()
        : RegisterKey(RegisterType::Sampler)
    {
    }
    DX9TextureType textureType = DX9TextureType::Unknown;
    std::array<int32_t, 4> data = { 0, 0, 0, 0 };
};

// class DX9Face : public RegisterKey {
// public:
//     DX9Face()
//         : RegisterKey(RegisterType::Face)
//     {
//     }
//     std::array<int32_t, 1> data = { 0 };
// };

// class DX9Pos : public RegisterKey {
// public:
//     DX9Pos()
//         : RegisterKey(RegisterType::Pos)
//     {
//     }
//     std::array<float, 4> data = { 0, 0, 0, 0 };
// };

class LoopCounter : public RegisterKey {
public:
    LoopCounter()
        : RegisterKey(RegisterType::Loop)
    {
    }
    std::array<uint32_t, 1> data = { 0 };
};

class Texture : public RegisterKey {
public:
    Texture()
        : RegisterKey(RegisterType::Texture)
    {
    }
    std::array<float, 4> data = { 0, 0, 0, 0 };
};

class ColorOutput : public RegisterKey {
public:
    ColorOutput()
        : RegisterKey(RegisterType::ColorOut)
    {
    }
    std::array<float, 4> data = { 0, 0, 0, 0 };
};

class DepthOutput : public RegisterKey {
public:
    DepthOutput()
        : RegisterKey(RegisterType::DepthOut)
    {
    }
    std::array<float, 4> data = { 0, 0, 0, 0 };
};
}

#pragma once

#include <cstdint>

namespace DX9 {
enum class RegisterType : uint16_t {
    Temp = 0,
    Input = 1,
    Const = 2,
    Addr = 3,
    Texture = 3,
    RastOut = 4,
    AttrOut = 5,
    TexCrdOut = 6,
    Output = 6,
    ConstInt = 7,
    ColorOut = 8,
    DepthOut = 9,
    Sampler = 10,
    Const2 = 11,
    Const3 = 12,
    Const4 = 13,
    ConstBool = 14,
    Loop = 15,
    TempFloat16 = 16,
    MiscType = 17,
    Label = 18,
    Predicate = 19
};

}

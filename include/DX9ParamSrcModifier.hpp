#pragma once
namespace DX9 {
enum class ParamSrcModifier {
    None = 0x0,
    Negate = 0x1,
    Bias = 0x2,
    BiasNegate = 0x3,
    Sign = 0x4,
    SignNegate = 0x5,
    Complement = 0x6,
    X2 = 0x7,
    X2Negate = 0x8,
    DivideZ = 0x9,
    DivideW = 0xa,
    Abs = 0xb,
    AbsNegate = 0xc,
    Not = 0xd
};

}

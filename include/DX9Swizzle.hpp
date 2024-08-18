#pragma once

namespace DX9 {
enum class SwizzleComp {
    X = 0x0,
    Y = 0x1,
    Z = 0x2,
    W = 0x3
};

struct Swizzle {
    SwizzleComp x = SwizzleComp::X;
    SwizzleComp y = SwizzleComp::Y;
    SwizzleComp z = SwizzleComp::Z;
    SwizzleComp w = SwizzleComp::W;
};
}

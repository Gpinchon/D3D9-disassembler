#pragma once

#include <DX9CmpFunc.hpp>
#include <DX9Opcode.hpp>
#include <DX9ParamDstModifier.hpp>
#include <DX9ParamSrcModifier.hpp>
#include <DX9RegisterType.hpp>
#include <DX9Swizzle.hpp>
#include <DX9WriteMask.hpp>

namespace DX9 {
struct DstParam {
    ParamDstModifier modifier;
    WriteMask writeMask;
};

struct SrcParam {
    ParamSrcModifier modifier;
    Swizzle swizzle;
};

struct Param {
    unsigned registerIndex = -1;
    RegisterType registerType = (std::numeric_limits<RegisterType>::max)();
    union {
        DstParam dst = {};
        SrcParam src;
    };
};

struct Instruction {
    Opcode type = (std::numeric_limits<Opcode>::max)();
    union /* control */ {
        CmpFunc cmpFunc;
        uint32_t controls;
    };
    std::vector<Param> params;
};
}

#pragma once

#include <DX9Instruction.hpp>

#include <cassert>

constexpr auto SATURATE = 0x1;
constexpr auto PARTIAL_PRECISION = 0x2;
constexpr auto CENTROID = 0x4;

namespace DX9 {
template <typename Param>
auto GetParamRegisterType(const Param& a_P)
{
    return RegisterType(a_P.registerType1 | a_P.registerType2 << 3);
}

struct VersionToken {
    unsigned versionMinor : 8;
    unsigned versionMajor : 8;
    unsigned shaderType : 16;
};
static_assert(sizeof(VersionToken) == 4);

struct InstructionToken {
    operator Instruction()
    {
        assert(zero == 0 && "Ill formated token");
        Instruction instruction;
        instruction.type = Opcode(opCode);
        instruction.controls = controls;
        return instruction;
    }
    unsigned opCode : 16;
    unsigned controls : 8;
    unsigned size : 4;
    unsigned pred : 1;
    unsigned reserved : 1;
    unsigned co : 1;
    unsigned zero : 1; // if this is not zero, we've got a problem
};
static_assert(sizeof(InstructionToken) == 4);

struct SourceToken {
    operator Param() const
    {
        Param param;
        param.registerIndex = registerNbr;
        param.registerType = GetParamRegisterType(*this);
        param.src.swizzle.x = SwizzleComp(swizzleX);
        param.src.swizzle.y = SwizzleComp(swizzleY);
        param.src.swizzle.z = SwizzleComp(swizzleZ);
        param.src.swizzle.w = SwizzleComp(swizzleW);
        param.src.modifier = ParamSrcModifier(modifier);
        return param;
    }
    unsigned registerNbr : 11;
    unsigned registerType2 : 2; //[3, 4] of register type
    unsigned relative : 1;
    unsigned reserved0 : 2;
    unsigned swizzleX : 2;
    unsigned swizzleY : 2;
    unsigned swizzleZ : 2;
    unsigned swizzleW : 2;
    unsigned modifier : 4;
    unsigned registerType1 : 3;
    unsigned one : 1; // if this is not one, we've got a problem
};
static_assert(sizeof(SourceToken) == 4);

struct DestinationToken {
    operator Param() const
    {
        Param param;
        param.registerIndex = registerNbr;
        param.registerType = GetParamRegisterType(*this);
        param.dst.writeMask.x = writeMaskX;
        param.dst.writeMask.y = writeMaskY;
        param.dst.writeMask.z = writeMaskZ;
        param.dst.writeMask.w = writeMaskW;
        param.dst.modifier.saturate = modifier & SATURATE;
        param.dst.modifier.partialPrecision = modifier & PARTIAL_PRECISION;
        param.dst.modifier.centroid = modifier & CENTROID;
        return param;
    }
    unsigned registerNbr : 11;
    unsigned registerType2 : 2; //[3, 4] of register type
    unsigned relative : 1;
    unsigned reserved0 : 2;
    unsigned writeMaskX : 1;
    unsigned writeMaskY : 1;
    unsigned writeMaskZ : 1;
    unsigned writeMaskW : 1;
    unsigned modifier : 4; // 0x1 saturate, 0x2 partial, 0x4 centroid
    unsigned shift : 4;
    unsigned registerType1 : 3; //[0, 1, 2] of register type
    unsigned one : 1; // if this is not one, we've got a problem
};
static_assert(sizeof(DestinationToken) == 4);

struct LabelToken {
    unsigned registerNbr : 11;
    unsigned registerType2 : 2; //[3, 4] of register type
    unsigned reserved0 : 15;
    unsigned registerType1 : 3; //[0, 1, 2] of register type
    unsigned one : 1; // if this is not one, we've got a problem
};
static_assert(sizeof(LabelToken) == 4);

struct CommentToken {
    bool IsToken() const
    {
        return Opcode(opCode) == Opcode::Comment;
    }
    unsigned opCode : 16;
    unsigned length : 15;
    unsigned zero : 1; // if this is not zero, we've got a problem
};
static_assert(sizeof(CommentToken) == 4);

struct DclSampler {
    unsigned reserved : 27;
    unsigned textureType : 4; // 2D, Cube...
    unsigned one : 1;
};
static_assert(sizeof(DclSampler) == 4);

struct DclIO {
    unsigned usage : 5;
    unsigned reserved0 : 11;
    unsigned index : 4;
    unsigned reserved1 : 11;
    unsigned one : 1;
};
static_assert(sizeof(DclIO) == 4);

union Token {
    unsigned opCode : 16;
    unsigned raw : 32;
    VersionToken version;
    InstructionToken instruction;
    SourceToken source;
    DestinationToken destination;
    LabelToken label;
    CommentToken comment;
    DclSampler dclSampler;
    DclIO dclIO;
};
static_assert(sizeof(Token) == 4);
}

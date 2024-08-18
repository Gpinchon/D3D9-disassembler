#pragma once

#include <DX9HLSLStringifyer.hpp>
#include <DX9Shader.hpp>

#include <cassert>
#include <functional>
#include <unordered_map>
#include <utility>

#define SPACE +std::string(" ") +
#define PLUS +std::string(" + ") +
#define MINUS +std::string(" - ") +
#define MULT +std::string(" * ") +
#define EQUAL +std::string(" = ") +
#define MORE +std::string(" > ") +
#define LESS +std::string(" < ") +
#define MEQUAL +std::string(" >= ") +
#define LEQUAL +std::string(" <= ") +
#define ENDL +std::string(";\n")

namespace DX9 {
std::string GetRegisterKeyType(const RegisterKey& a_RegisterKey, const std::string& a_Swizzle)
{
    std::string type;
    std::string swizzleSize = a_Swizzle.size() > 1 ? std::to_string(a_Swizzle.size()) : "";
    switch (a_RegisterKey.type) {
    case RegisterType::Input:
    case RegisterType::Temp:
    case RegisterType::Const:
    case RegisterType::Texture:
    case RegisterType::ColorOut:
    case RegisterType::DepthOut:
        type = "float" + (a_Swizzle.size() > 0 ? swizzleSize : "");
        break;
    case RegisterType::ConstInt:
        type = "int";
        break;
    case RegisterType::ConstBool:
        type = "bool";
        break;
    case RegisterType::Predicate:
        type = "p";
        break;
    case RegisterType::Sampler:
        type = "sampler";
        break;
        // case RegisterType::Face:
        //     type = "vFace";
        //     break;
        // case RegisterType::Pos:
        //     type = "vPos";
        //     break;
    case RegisterType::Loop:
        type = "int";
        break;
    }
    return type;
}

std::string GetRegisterKeyName(const RegisterKey& a_RegisterKey)
{
    std::string name;
    switch (a_RegisterKey.type) {
    case RegisterType::Input:
        name = "input";
        break;
    case RegisterType::Temp:
        name = "temp";
        break;
    case RegisterType::Const:
        name = "constFloat";
        break;
    case RegisterType::ConstInt:
        name = "constInt";
        break;
    case RegisterType::ConstBool:
        name = "constBool";
        break;
    case RegisterType::Predicate:
        name = "pred";
        break;
    case RegisterType::Sampler:
        name = "sampler";
        break;
        // case RegisterType::Face:
        //     name = "face";
        //     break;
        // case RegisterType::Pos:
        //     name = "pos";
        break;
    case RegisterType::Loop:
        name = "loopCounter";
        break;
    case RegisterType::Texture:
        name = "texCoord";
        break;
    case RegisterType::ColorOut:
        name = "colorOutput";
        break;
    case RegisterType::DepthOut:
        name = "depthOutput";
        break;
    default:
        break;
    }
    name += std::to_string(a_RegisterKey.index);
    return name;
}

std::string GetUsage(const RegisterUsage& a_Usage)
{
    switch (a_Usage) {
    case RegisterUsage::Position:
        return "position";
    case RegisterUsage::BlendWeight:
        return "blendWeight";
    case RegisterUsage::BlendIndices:
        return "blendIndices";
    case RegisterUsage::Normal:
        return "normal";
    case RegisterUsage::PSize:
        return "pSize";
    case RegisterUsage::Texcoord:
        return "texcoord";
    case RegisterUsage::Tangent:
        return "tangent";
    case RegisterUsage::Binormal:
        return "binormal";
    case RegisterUsage::TessFactor:
        return "tessFactor";
    case RegisterUsage::PositionT:
        return "positionT";
    case RegisterUsage::Color:
        return "color";
    case RegisterUsage::Fog:
        return "fog";
    case RegisterUsage::Depth:
        return "depth";
    case RegisterUsage::Sample:
        return "sample";
    }
    return "INCORRECT USAGE";
}

// std::string GetIORegisterName(const RegisterKey& a_RegisterKey)
//{
//     if (a_RegisterKey.type == RegisterType::DepthOut)
//         return "DEPTH";
//     std::string usageIndex = std::to_string(a_RegisterKey.usageIndex);
//     std::string usage;
//     if (a_RegisterKey.type == RegisterType::Input) {
//         usage = "COLOR";
//     } else
//         usage = GetUsage(a_RegisterKey.usage);
//     return usage + usageIndex;
// }

std::string GetRegisterName(const RegisterKey& a_RegisterKey)
{
    std::string name;
    switch (a_RegisterKey.type) {
    case RegisterType::Input:
        name = "v";
        break;
    case RegisterType::Temp:
        name = "r";
        break;
    case RegisterType::Const:
        name = "c";
        break;
    case RegisterType::ConstInt:
        name = "i";
        break;
    case RegisterType::ConstBool:
        name = "b";
        break;
    case RegisterType::Predicate:
        name = "p";
        break;
    case RegisterType::Sampler:
        name = "s";
        break;
        // case RegisterType::Face:
        //     name = "vFace";
        //     break;
        // case RegisterType::Pos:
        //     name = "vPos";
        //     break;
    case RegisterType::Loop:
        name = "aL";
        break;
    case RegisterType::Texture:
        name = "t";
        break;
    case RegisterType::ColorOut:
        name = "Oc";
        break;
    case RegisterType::DepthOut:
        name = "oDepth";
        break;
    default:
        assert(false && "Incorrect register type");
    }
    if (a_RegisterKey.type != RegisterType::DepthOut)
        name += std::to_string(a_RegisterKey.index);
    return name;
}

std::string GetSwizzleComp(const SwizzleComp& a_SwizzleComp)
{
    switch (a_SwizzleComp) {
    case SwizzleComp::X:
        return "x";
    case SwizzleComp::Y:
        return "y";
    case SwizzleComp::Z:
        return "z";
    case SwizzleComp::W:
        return "w";
    default:
        assert(false && "Ill formed swizzle");
    }
    return "";
}

template <typename T, typename U>
bool AllEqual(const T& a_T, const U& a_U)
{
    return a_T == a_U;
}

template <typename T, typename U, typename... Others>
bool AllEqual(const T& a_T, const U& a_U, const Others&... a_Others)
{
    return (a_T == a_U) && AllEqual(a_U, a_Others...);
}

std::string GetSwizzle(const Swizzle& a_Swizzle)
{
    if (AllEqual(a_Swizzle.x, a_Swizzle.y, a_Swizzle.z, a_Swizzle.w)) {
        return "." + GetSwizzleComp(a_Swizzle.x);
    }
    std::string swizzle;
    swizzle += ".";
    swizzle += GetSwizzleComp(a_Swizzle.x);
    swizzle += GetSwizzleComp(a_Swizzle.y);
    swizzle += GetSwizzleComp(a_Swizzle.z);
    swizzle += GetSwizzleComp(a_Swizzle.w);
    return swizzle == ".xyzw" ? "" : swizzle;
}

std::string GetWriteMask(const WriteMask& a_WriteMask)
{
    std::string mask;
    mask += ".";
    mask += a_WriteMask.x ? "x" : "";
    mask += a_WriteMask.y ? "y" : "";
    mask += a_WriteMask.z ? "z" : "";
    mask += a_WriteMask.w ? "w" : "";
    return mask == ".xyzw" ? "" : mask;
}

std::string Op(const std::string& a_Arg0, const std::string a_Op, const std::string& a_Arg1)
{
    return a_Arg0 + " " + a_Op + " " + a_Arg1;
}

template <typename... Params>
std::string Op(const std::string& a_Arg0, const std::string a_Op, const std::string& a_Arg1, const Params&... a_Params)
{
    return Op(Op(a_Arg0, a_Op, a_Arg1), a_Params...);
}

std::string ConcatParams()
{
    return "void";
}

std::string ConcatParams(const std::string& a_Arg0)
{
    return a_Arg0;
}

template <typename... Params>
std::string ConcatParams(const std::string& a_Arg0, const std::string& a_Arg1)
{
    return a_Arg0 + ", " + a_Arg1;
}

template <typename... Params>
std::string ConcatParams(const std::string& a_Arg0, const std::string& a_Arg1, const Params&... a_Params)
{
    return ConcatParams(a_Arg0, a_Arg1) + ", " + ConcatParams(a_Params...);
}

template <typename... Params>
std::string Func(const std::string& a_Func, const Params&... a_Params)
{
    return a_Func + "(" + ConcatParams(a_Params...) + ")";
}

std::string Modifier(const ParamDstModifier& a_Modifier, const std::string& a_Param)
{
    if (a_Modifier.saturate)
        return Func("saturate", a_Param);
    return a_Param;
}

std::string Modifier(const ParamSrcModifier& a_Modifier, const std::string& a_Param)
{
    switch (a_Modifier) {
    case ParamSrcModifier::Negate:
        return "-" + a_Param;
        break;
    case ParamSrcModifier::Bias:
        return "(" + a_Param + " - 0.5)";
        break;
    case ParamSrcModifier::BiasNegate:
        return "-" + Modifier(ParamSrcModifier::BiasNegate, a_Param);
        break;
    case ParamSrcModifier::Sign: // bx2
        return "(2 * (" + a_Param + " - 0.5))";
        break;
    case ParamSrcModifier::SignNegate: //-bx2
        return "-" + Modifier(ParamSrcModifier::Sign, a_Param);
        break;
    case ParamSrcModifier::Complement:
        break;
    case ParamSrcModifier::X2:
        return "(2 * " + a_Param + ")";
        break;
    case ParamSrcModifier::X2Negate:
        return "-" + Modifier(ParamSrcModifier::X2Negate, a_Param);
        break;
    case ParamSrcModifier::DivideZ:
        return Op(a_Param, "/", a_Param + ".z");
        break;
    case ParamSrcModifier::DivideW:
        return Op(a_Param, "/", a_Param + ".w");
        break;
    case ParamSrcModifier::Abs:
        return Func("abs", a_Param);
        break;
    case ParamSrcModifier::AbsNegate:
        return "-" + Modifier(ParamSrcModifier::Abs, a_Param);
        break;
    case ParamSrcModifier::Not:
        return Func("!", a_Param);
        break;
    }
    return a_Param;
}

std::string GetCmpFunc(const CmpFunc& a_Cmp)
{
    switch (a_Cmp) {
    case CmpFunc::Never:
        break;
    case CmpFunc::Less:
        return "<";
    case CmpFunc::Equal:
        return "==";
    case CmpFunc::LessEqual:
        return "<=";
    case CmpFunc::Greater:
        return ">";
    case CmpFunc::NotEqual:
        return "!=";
    case CmpFunc::GreaterEqual:
        return ">=";
    case CmpFunc::Always:
        break;
    }
    return "";
}

std::string If(const std::string& a_Arg0, const CmpFunc& a_Cmp, const std::string& a_Arg1)
{
    return Func("if", Op(a_Arg0, GetCmpFunc(a_Cmp), a_Arg1));
}

std::string GetFullDst(const Shader& a_Shader, const Param& a_Param, bool a_ApplyWriteMask = true)
{
    std::string arg;
    auto key = a_Shader.registers.GetKey(a_Param.registerType, a_Param.registerIndex);
    auto writeMask = a_ApplyWriteMask ? GetWriteMask(a_Param.dst.writeMask) : "";
    if (key->type == RegisterType::Input)
        arg = "PSIn." + GetUsage(key->usage) + std::to_string(key->usageIndex);
    else if (key->type == RegisterType::Texture)
        arg = "PSIn." + GetUsage(key->usage) + std::to_string(key->usageIndex);
    else if (key->type == RegisterType::ColorOut)
        arg = "PSOut." + GetRegisterKeyName(*key);
    else if (key->type == RegisterType::DepthOut)
        arg = "PSOut." + GetRegisterKeyName(*key);
    else
        arg = GetRegisterKeyName(*key);
    return arg + writeMask;
}

std::string GetFullSrc(const Shader& a_Shader, const Param& a_Param, const bool& a_ApplySwizzle = true)
{
    std::string arg;
    auto key = a_Shader.registers.GetKey(a_Param.registerType, a_Param.registerIndex);
    auto swizzle = a_ApplySwizzle ? GetSwizzle(a_Param.src.swizzle) : "";
    if (key->type == RegisterType::Input)
        arg = "PSIn." + GetUsage(key->usage) + std::to_string(key->usageIndex);
    else if (key->type == RegisterType::Texture)
        arg = "PSIn." + GetUsage(key->usage) + std::to_string(key->usageIndex);
    else if (key->type == RegisterType::ColorOut)
        arg = "PSOut." + GetRegisterKeyName(*key);
    else if (key->type == RegisterType::DepthOut)
        arg = "PSOut." + GetRegisterKeyName(*key);
    else
        arg = GetRegisterKeyName(*key);
    return Modifier(a_Param.src.modifier, arg) + swizzle;
}

template <Opcode>
void GenerateCode(const Shader&, const Instruction&, HLSLStringifyer&)
{
    assert(false && "Stringifyer not implemented");
}

template <>
void GenerateCode<Opcode::Abs>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = Func("abs", src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Add>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = src0 PLUS src1;
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Bem>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    assert(a_Shader.versionMajor == 1 && a_Shader.versionMinor == 4 && "BEM unaivalable on versions other than 1.4");
    auto dst = GetFullDst(a_Shader, a_Instr.params[0], false), src0 = GetFullSrc(a_Shader, a_Instr.params[1], false), src1 = GetFullSrc(a_Shader, a_Instr.params[2], false);
    auto stage = std::to_string(a_Instr.params[0].registerIndex);
    auto op = Func("bem", dst, src0, src1, stage);
    a_Stringifyer._mainBody += Modifier(a_Instr.params[0].dst.modifier, op);
}

template <>
void GenerateCode<Opcode::Break>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    a_Stringifyer._mainBody += "break;\n";
}

template <>
void GenerateCode<Opcode::BreakC>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto src0 = GetFullSrc(a_Shader, a_Instr.params[0]), src1 = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = If(src0, a_Instr.cmpFunc, src1) + " break";
    a_Stringifyer._mainBody += op ENDL;
}

template <>
void GenerateCode<Opcode::IfC>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto src0 = GetFullSrc(a_Shader, a_Instr.params[0]), src1 = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = If(src0, a_Instr.cmpFunc, src1) + " {\n";
    a_Stringifyer._mainBody += op;
}

template <>
void GenerateCode<Opcode::Endif>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    a_Stringifyer._mainBody += "}\n";
}

template <>
void GenerateCode<Opcode::Rep>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto src0 = GetFullSrc(a_Shader, a_Instr.params[0]) + ".x";
    auto op = "for(int i = 0; i < " + src0 + "; i++)";
    a_Stringifyer._mainBody += op + "{\n";
}

template <>
void GenerateCode<Opcode::EndRep>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    a_Stringifyer._mainBody += "}\n";
}

template <>
void GenerateCode<Opcode::BreakP>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto pred = GetFullSrc(a_Shader, a_Instr.params[0]);
    auto op = Func("if", pred) + " break";
    a_Stringifyer._mainBody += op ENDL;
}

template <>
void GenerateCode<Opcode::Cmp>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]), src2 = GetFullSrc(a_Shader, a_Instr.params[3]);
    auto op = Func("cmp", dst, src0, src1, src2);
    a_Stringifyer._mainBody += op ENDL;
}

template <>
void GenerateCode<Opcode::Cnd>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]), src2 = GetFullSrc(a_Shader, a_Instr.params[3]);
    auto op = Func("cnd", dst, src0, src1, src2);
    a_Stringifyer._mainBody += op ENDL;
}

template <>
void GenerateCode<Opcode::Crs>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = Func("cross", src0, src1);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Dcl>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto& arg0 = a_Instr.params[0];
    auto key = a_Shader.registers.GetKey(arg0.registerType, arg0.registerIndex);
    if (key->type == RegisterType::Sampler) {
        auto dst = GetRegisterKeyName(*key);
        a_Stringifyer._constBody += "sampler" SPACE dst SPACE ": register(s" + std::to_string(key->index) + ")" ENDL;
    } else if (key->type == RegisterType::Input) {
        auto dst = GetUsage(key->usage) + std::to_string(key->usageIndex); // GetRegisterKeyName(*key);
        auto writeMaskSize = GetWriteMask(arg0.dst.writeMask).size();
        std::string inputType = "float4";
        a_Stringifyer._inputBody += inputType SPACE dst SPACE ": COLOR" + std::to_string(key->index) ENDL;
    }
}

template <>
void GenerateCode<Opcode::Def>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
}

template <>
void GenerateCode<Opcode::Defi>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
}

template <>
void GenerateCode<Opcode::Defb>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
}

template <>
void GenerateCode<Opcode::Dsx>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    std::string ddx;
    if (a_Shader, a_Instr.params[0].dst.modifier.partialPrecision)
        ddx = "ddx_coarse";
    else
        ddx = "ddx_fine";
    auto op = Func(ddx, src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Dsy>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    std::string ddy;
    if (a_Shader, a_Instr.params[0].dst.modifier.partialPrecision)
        ddy = "ddy_coarse";
    else
        ddy = "ddy_fine";
    auto op = Func(ddy, src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Dp2add>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]), src2 = GetFullSrc(a_Shader, a_Instr.params[3]);
    auto op = Func("dot2", src0, src1) PLUS src2;
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Dp3>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = Func("dot3", src0, src1);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Dp4>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = Func("dot4", src0, src1);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Else>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    a_Stringifyer._mainBody += "}\n";
    a_Stringifyer._mainBody += "else {\n";
}

template <>
void GenerateCode<Opcode::End>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
}

template <>
void GenerateCode<Opcode::Endloop>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    a_Stringifyer._mainBody += "}\n";
}

template <>
void GenerateCode<Opcode::Exp>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = Func("exp", src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Frc>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = Func("frac", src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::If>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto src = GetFullSrc(a_Shader, a_Instr.params[1]);
    a_Stringifyer._mainBody += Func("if", src) + " {\n";
}

template <>
void GenerateCode<Opcode::Label>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    // TODO implement
    auto src = GetFullSrc(a_Shader, a_Instr.params[1]);
    a_Stringifyer._mainBody += "//LABEL " + src; // Func("if", src) + " {\n";
}

template <>
void GenerateCode<Opcode::Log>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = Func("log", src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Loop>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto count = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto aL = (LoopCounter*)a_Shader.registers.GetKey(a_Instr.params[0].registerType, a_Instr.params[0].registerIndex);
    auto aLStart = std::to_string(aL->data[0]);
    a_Stringifyer._mainBody += Func("for", "int i" EQUAL aLStart + "; i < " + count + "; i++) {\n");
}

template <>
void GenerateCode<Opcode::Lrp>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]), src2 = GetFullSrc(a_Shader, a_Instr.params[3]);
    auto op = Func("lerp", src1, src2, src0);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Mad>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]), src2 = GetFullSrc(a_Shader, a_Instr.params[3]);
    auto op = Op(src0, "*", src1, "+", src2);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Max>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = Func("max", src0, src1);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Min>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = Func("min", src0, src1);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Mov>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, src) ENDL;
}

template <>
void GenerateCode<Opcode::Mul>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = Op(src0, "*", src1);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Nrm>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = Func("normalize", src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Rcp>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = Op("1.f", "/", src);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Rsq>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src = GetFullSrc(a_Shader, a_Instr.params[1]);
    auto op = Op("1.f", "/", Func("sqrt", src));
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Sub>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto op = src0 MINUS src1;
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

template <>
void GenerateCode<Opcode::Tex>(const Shader& a_Shader, const Instruction& a_Instr, HLSLStringifyer& a_Stringifyer)
{
    auto dst = GetFullDst(a_Shader, a_Instr.params[0]), src0 = GetFullSrc(a_Shader, a_Instr.params[1]), src1 = GetFullSrc(a_Shader, a_Instr.params[2]);
    auto sampler = a_Shader.registers.GetKey(a_Instr.params[2].registerType, a_Instr.params[2].registerIndex);
    std::string sampleFunction;
    switch (((Sampler*)sampler)->textureType) {
    case DX9TextureType::Texture2D:
        sampleFunction = "tex2D";
        break;
    case DX9TextureType::TextureCube:
        sampleFunction = "texCUBE";
        break;
    case DX9TextureType::TextureVolume:
        sampleFunction = "tex3D";
        break;
    default:
        assert(false && "INCORRECT TEXTURE TYPE");
    }
    auto op = Func(sampleFunction, src1, src0);
    a_Stringifyer._mainBody += dst EQUAL Modifier(a_Instr.params[0].dst.modifier, op) ENDL;
}

void GetCode(const Shader& a_Shader, const Instruction& a_Instruction, HLSLStringifyer& a_Stringifyer)
{
    static std::unordered_map<Opcode, std::function<void(const Shader&, const Instruction&, HLSLStringifyer&)>> generators = {
        { Opcode::Abs, GenerateCode<Opcode::Abs> },
        { Opcode::Add, GenerateCode<Opcode::Add> },
        { Opcode::Bem, GenerateCode<Opcode::Bem> },
        { Opcode::Break, GenerateCode<Opcode::Break> },
        { Opcode::BreakC, GenerateCode<Opcode::BreakC> },
        { Opcode::BreakP, GenerateCode<Opcode::BreakP> },
        { Opcode::Call, GenerateCode<Opcode::Call> },
        { Opcode::Callnz, GenerateCode<Opcode::Callnz> },
        { Opcode::Cmp, GenerateCode<Opcode::Cmp> },
        { Opcode::Cnd, GenerateCode<Opcode::Cnd> },
        { Opcode::Crs, GenerateCode<Opcode::Crs> },
        { Opcode::Dcl, GenerateCode<Opcode::Dcl> },
        { Opcode::Def, GenerateCode<Opcode::Def> },
        { Opcode::Defb, GenerateCode<Opcode::Defb> },
        { Opcode::Defi, GenerateCode<Opcode::Defi> },
        { Opcode::Dp2add, GenerateCode<Opcode::Dp2add> },
        { Opcode::Dp3, GenerateCode<Opcode::Dp3> },
        { Opcode::Dp4, GenerateCode<Opcode::Dp4> },
        { Opcode::Dsx, GenerateCode<Opcode::Dsx> },
        { Opcode::Dsy, GenerateCode<Opcode::Dsy> },
        { Opcode::Else, GenerateCode<Opcode::Else> },
        { Opcode::Endif, GenerateCode<Opcode::Endif> },
        { Opcode::Endloop, GenerateCode<Opcode::Endloop> },
        { Opcode::EndRep, GenerateCode<Opcode::EndRep> },
        { Opcode::Exp, GenerateCode<Opcode::Exp> },
        { Opcode::Frc, GenerateCode<Opcode::Frc> },
        { Opcode::If, GenerateCode<Opcode::If> },
        { Opcode::IfC, GenerateCode<Opcode::IfC> },
        { Opcode::Label, GenerateCode<Opcode::Label> },
        { Opcode::Log, GenerateCode<Opcode::Log> },
        { Opcode::Loop, GenerateCode<Opcode::Loop> },
        { Opcode::Lrp, GenerateCode<Opcode::Lrp> },
        { Opcode::M3x2, GenerateCode<Opcode::M3x2> },
        { Opcode::M3x3, GenerateCode<Opcode::M3x3> },
        { Opcode::M3x4, GenerateCode<Opcode::M3x4> },
        { Opcode::M4x3, GenerateCode<Opcode::M4x3> },
        { Opcode::M4x4, GenerateCode<Opcode::M4x4> },
        { Opcode::Mad, GenerateCode<Opcode::Mad> },
        { Opcode::Max, GenerateCode<Opcode::Max> },
        { Opcode::Min, GenerateCode<Opcode::Min> },
        { Opcode::Mov, GenerateCode<Opcode::Mov> },
        { Opcode::Mul, GenerateCode<Opcode::Mul> },
        { Opcode::Nop, GenerateCode<Opcode::Nop> },
        { Opcode::Nrm, GenerateCode<Opcode::Nrm> },
        { Opcode::Pow, GenerateCode<Opcode::Pow> },
        { Opcode::Rcp, GenerateCode<Opcode::Rcp> },
        { Opcode::Rep, GenerateCode<Opcode::Rep> },
        { Opcode::Ret, GenerateCode<Opcode::Ret> },
        { Opcode::Rsq, GenerateCode<Opcode::Rsq> },
        { Opcode::Sincos, GenerateCode<Opcode::Sincos> },
        { Opcode::Sub, GenerateCode<Opcode::Sub> },
        { Opcode::Texkill, GenerateCode<Opcode::Texkill> },
        { Opcode::Tex, GenerateCode<Opcode::Tex> },
        { Opcode::Texldl, GenerateCode<Opcode::Texldl> },
        { Opcode::Texldd, GenerateCode<Opcode::Texldd> },
        { Opcode::End, GenerateCode<Opcode::End> }
    };
    generators.at(a_Instruction.type)(a_Shader, a_Instruction, a_Stringifyer);
}

std::string HLSLStringifyer::operator()(const Shader& a_Shader)
{
    std::string fullCode;

    fullCode += "//DP2/3/4\n";
    fullCode += "float dot2(in float2 x, in float2 y) { return dot(x, y); }\n";
    fullCode += "float dot3(in float3 x, in float3 y) { return dot(x, y); }\n";
    fullCode += "float dot4(in float4 x, in float4 y) { return dot(x, y); }\n";
    fullCode += "//CND\n";
    fullCode += "void cnd(out float d, float s0, float s1, float s2) { d = s0 > 0.5f ? s1 : s2; }\n";
    fullCode += "void cnd(out float2 d, float2 s0, float2 s1, float2 s2) {\n"
                "    for (int i = 0;i < length(s0); i++)\n"
                "           cnd(d[i], s0[i], s1[i], s2[i]);\n"
                "}\n";
    fullCode += "void cnd(out float3 d, float3 s0, float3 s1, float3 s2) {\n"
                "    for (int i = 0;i < length(s0); i++)\n"
                "           cnd(d[i], s0[i], s1[i], s2[i]);\n"
                "}\n";
    fullCode += "void cnd(out float4 d, float4 s0, float4 s1, float4 s2) {\n"
                "    for (int i = 0;i < length(s0); i++)\n"
                "           cnd(d[i], s0[i], s1[i], s2[i]);\n"
                "}\n";
    fullCode += "//CMP\n";
    fullCode += "void cmp(out float d, float s0, float s1, float s2) { d = s0 >= 0 ? s1 : s2; }\n";
    fullCode += "void cmp(out float2 d, float2 s0, float2 s1, float2 s2) {\n"
                "    for (int i = 0;i < length(s0); i++)\n"
                "           cmp(d[i], s0[i], s1[i], s2[i]);\n"
                "}\n";
    fullCode += "void cmp(out float3 d, float3 s0, float3 s1, float3 s2) {\n"
                "    for (int i = 0;i < length(s0); i++)\n"
                "           cmp(d[i], s0[i], s1[i], s2[i]);\n"
                "}\n";
    fullCode += "void cmp(out float4 d, float4 s0, float4 s1, float4 s2) {\n"
                "    for (int i = 0;i < length(s0); i++)\n"
                "           cmp(d[i], s0[i], s1[i], s2[i]);\n"
                "}\n";

    if (a_Shader.versionMajor == 1 && a_Shader.versionMinor == 4)
        fullCode += "void bem(out float2 dest, in float2 src0, in float2 src1, in int stage) {\n"
                    "   dest.r = src0.r + D3DTSS_BUMPENVMAT00(stage) * src1.r\n"
                    "                   + D3DTSS_BUMPENVMAT10(stage) * src1.g\n"
                    "   dest.g = src0.g + D3DTSS_BUMPENVMAT01(stage) * src1.r\n"
                    "                   + D3DTSS_BUMPENVMAT11(stage) * src1.g\n"
                    "}\n";
    fullCode += "\n";

    _constBody += "//Const Floats\n";
    for (auto& key : a_Shader.registers.constFloat) {
        if (key.uses > 0) {
            auto& constFloat = (const ConstFloat&)key;
            _constBody += "float4 " + GetRegisterKeyName(constFloat) + " : register(c" + std::to_string(constFloat.index) + ") = {";
            _constBody += std::to_string(constFloat.data[0]) + ", "
                + std::to_string(constFloat.data[1]) + ", "
                + std::to_string(constFloat.data[2]) + ", "
                + std::to_string(constFloat.data[3])
                + "};\n";
        }
    }

    _constBody += "//Const Bools\n";
    for (auto& key : a_Shader.registers.constBool) {
        if (key.uses > 0) {
            auto& constBool = (const ConstBool&)key;
            _constBody += "float4 " + GetRegisterKeyName(constBool) + " : register(b" + std::to_string(constBool.index) + ") = {";
            _constBody += std::to_string(constBool.data[0]) + "};\n";
        }
    }

    _constBody += "//Const Ints\n";
    for (auto& key : a_Shader.registers.constInt) {
        if (key.uses > 0) {
            auto& constInt = (const ConstInt&)key;
            _constBody += "int4 " + GetRegisterKeyName(constInt) + " : register(i" + std::to_string(constInt.index) + ") = {";
            _constBody += std::to_string(constInt.data[0]) + ", "
                + std::to_string(constInt.data[1]) + ", "
                + std::to_string(constInt.data[2]) + ", "
                + std::to_string(constInt.data[3])
                + "};\n";
        }
    }

    _mainBody += "//Temporary registers\n";
    for (auto& key : a_Shader.registers.temp) {
        if (key.uses > 0) {
            auto temp = (const Temp&)key;
            _mainBody += "float4 " + GetRegisterKeyName(key) + " = {"
                + std::to_string(temp.data[0]) + ", "
                + std::to_string(temp.data[1]) + ", "
                + std::to_string(temp.data[2]) + ", "
                + std::to_string(temp.data[3])
                + "};\n";
        }
    }
    for (auto& instruction : a_Shader.instructions) {
        GetCode(a_Shader, instruction, *this);
    }

    _mainBody += "\n";

    for (auto& key : a_Shader.registers.colorOut) {
        if (key.uses > 0)
            _outputBody += "float4 " + GetRegisterKeyName(key) + " : " + "COLOR" + std::to_string(key.index) + ";\n";
    }
    for (auto& key : a_Shader.registers.depthOut) {
        if (key.uses > 0)
            _outputBody += "float " + GetRegisterKeyName(key) + " : " + "DEPTH;\n";
    }

    fullCode += "//Inputs\n";
    fullCode += "struct PS_IN {\n";
    fullCode += _inputBody;
    fullCode += "};\n\n";
    fullCode += "//Outputs\n";
    fullCode += "struct PS_OUT {\n";
    fullCode += _outputBody;
    fullCode += "};\n\n";
    fullCode += _constBody + "\n";
    fullCode += "PS_OUT main(in PS_IN PSIn) {\n";
    fullCode += "PS_OUT PSOut = (PS_OUT)0;\n";
    fullCode += _mainBody;
    fullCode += "return PSOut;\n";
    fullCode += "};\n";

    return fullCode;
}
}

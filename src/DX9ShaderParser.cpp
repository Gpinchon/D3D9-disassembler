#include <DX9Opcode.hpp>
#include <DX9RegisterType.hpp>
#include <DX9RegisterUsage.hpp>
#include <DX9Shader.hpp>
#include <DX9ShaderType.hpp>
#include <DX9Tokens.hpp>

#include "DX9ShaderParser.hpp"
#include <cassert>
#include <functional>
#include <unordered_map>

namespace DX9 {
template <typename Param>
void Ref(Shader& a_Shader, const Param& a_Param)
{
    a_Shader.registers.GetKey(GetParamRegisterType(a_Param), a_Param.registerNbr)->uses++;
}

template <Opcode OpCode>
Token* DefaultInstructionParser(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    Instruction instruction(instr);
    if (instr.size > 1) {
        auto& dst = (a_TokenPtr + 1)->destination;
        Ref(a_Shader, dst);
        Param param(dst);
        instruction.params.push_back(param);
    }
    for (auto i = 1u; i < instr.size; ++i) {
        auto& src = (a_TokenPtr + i + 1)->source;
        Ref(a_Shader, src);
        Param param(src);
        instruction.params.push_back(param);
    }
    a_Shader.instructions.push_back(instruction);
    return a_TokenPtr + instr.size + 1;
}

template <Opcode OpCode>
Token* TokenParser(Shader& a_Shader, Token* a_TokenPtr)
{
    return DefaultInstructionParser<OpCode>(a_Shader, a_TokenPtr);
}

template <>
Token* TokenParser<Opcode::BreakC>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    assert(instr.size == 2 && "Ill - formed token");
    auto& src0 = (a_TokenPtr + 1)->source;
    auto& src1 = (a_TokenPtr + 2)->source;
    Instruction instruction(instr);
    instruction.params.push_back(src0);
    instruction.params.push_back(src1);
    a_Shader.instructions.push_back(instruction);
    Ref(a_Shader, src0);
    Ref(a_Shader, src1);
    return a_TokenPtr + instr.size + 1;
}

template <>
Token* TokenParser<Opcode::IfC>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    assert(instr.size == 2 && "Ill - formed token");
    auto& src0 = (a_TokenPtr + 1)->source;
    auto& src1 = (a_TokenPtr + 2)->source;
    Instruction instruction(instr);
    instruction.params.push_back(src0);
    instruction.params.push_back(src1);
    a_Shader.instructions.push_back(instruction);
    return a_TokenPtr + instr.size + 1;
}

template <>
Token* TokenParser<Opcode::Nop>(Shader& a_Shader, Token* a_TokenPtr)
{
    assert(a_TokenPtr->raw == 0);
    return DefaultInstructionParser<Opcode::Nop>(a_Shader, a_TokenPtr);
}

template <>
Token* TokenParser<Opcode::Dcl>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    assert(instr.size == 2 && "Ill-formed token");
    auto& dst = (a_TokenPtr + 2)->destination;
    Ref(a_Shader, dst);
    Instruction instruction(instr);
    instruction.params.push_back(dst);
    a_Shader.instructions.push_back(instruction);

    auto key = a_Shader.registers.GetKey(instruction.params[0].registerType, instruction.params[0].registerIndex);
    key->partialPrecision = instruction.params[0].dst.modifier.partialPrecision;
    switch (instruction.params[0].registerType) {
    case RegisterType::Output:
    case RegisterType::Texture:
    case RegisterType::Input: {
        auto& dcl = (a_TokenPtr + 1)->dclIO;
        key->usage = RegisterUsage(dcl.usage);
        key->usageIndex = dcl.index;
        break;
    }
    case RegisterType::Sampler: {
        auto& sampler = (a_TokenPtr + 1)->dclSampler;
        ((Sampler*)key)->textureType = DX9TextureType(sampler.textureType);
    } break;
    default:
        assert(false && "Ill formed DCL");
    }
    return a_TokenPtr + instr.size + 1;
}

template <>
Token* TokenParser<Opcode::Def>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    assert(instr.size == 5 && "Ill-formed token");
    auto& dst = (a_TokenPtr + 1)->destination;
    Ref(a_Shader, dst);
    auto key = (ConstFloat*)a_Shader.registers.GetKey(GetParamRegisterType(dst), dst.registerNbr);
    assert(
        key->type == RegisterType::Const || key->type == RegisterType::Const2 || key->type == RegisterType::Const3 || key->type == RegisterType::Const4);
    std::memcpy(&key->data[0], a_TokenPtr + 2, sizeof(Token));
    std::memcpy(&key->data[1], a_TokenPtr + 3, sizeof(Token));
    std::memcpy(&key->data[2], a_TokenPtr + 4, sizeof(Token));
    std::memcpy(&key->data[3], a_TokenPtr + 5, sizeof(Token));
    Instruction instruction(instr);
    instruction.params.push_back(dst);
    a_Shader.instructions.push_back(instruction);
    return a_TokenPtr + instr.size + 1;
}

template <>
Token* TokenParser<Opcode::Defb>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    assert(instr.size == 2 && "Ill-formed token");
    auto& dst = (a_TokenPtr + 1)->destination;
    Ref(a_Shader, dst);
    auto key = (ConstBool*)a_Shader.registers.GetKey(GetParamRegisterType(dst), dst.registerNbr);
    assert(key->type == RegisterType::ConstBool);
    std::memcpy(&key->data[0], a_TokenPtr + 2, sizeof(Token));
    Instruction instruction(instr);
    instruction.params.push_back(dst);
    a_Shader.instructions.push_back(instruction);
    return a_TokenPtr + instr.size + 1;
}

template <>
Token* TokenParser<Opcode::Defi>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    assert(instr.size == 5 && "Ill-formed token");
    auto& dst = (a_TokenPtr + 1)->destination;
    Ref(a_Shader, dst);
    auto key = (ConstInt*)a_Shader.registers.GetKey(GetParamRegisterType(dst), dst.registerNbr);
    assert(key->type == RegisterType::ConstInt);
    std::memcpy(&key->data[0], a_TokenPtr + 2, sizeof(Token));
    std::memcpy(&key->data[1], a_TokenPtr + 3, sizeof(Token));
    std::memcpy(&key->data[2], a_TokenPtr + 4, sizeof(Token));
    std::memcpy(&key->data[3], a_TokenPtr + 5, sizeof(Token));
    Instruction instruction(instr);
    instruction.params.push_back(dst);
    a_Shader.instructions.push_back(instruction);
    return a_TokenPtr + instr.size + 1;
}

template <>
Token* TokenParser<Opcode::Comment>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& comment = a_TokenPtr->comment;
    return a_TokenPtr + 1 + comment.length;
}

template <>
Token* TokenParser<Opcode::Rep>(Shader& a_Shader, Token* a_TokenPtr)
{
    auto& instr = a_TokenPtr->instruction;
    assert(instr.size == 1 && "Ill-formed token");
    auto& src = (a_TokenPtr + 1)->source;
    Ref(a_Shader, src);
    Instruction instruction(instr);
    instruction.params.push_back(src);
    a_Shader.instructions.push_back(instruction);
    return a_TokenPtr + 1 + instr.size;
}

template <>
Token* TokenParser<Opcode::End>(Shader& a_Shader, Token* a_TokenPtr)
{
    DefaultInstructionParser<Opcode::End>(a_Shader, a_TokenPtr);
    return nullptr;
}

Token* ReadChunk(Shader& a_Shader, Token* a_TokenPtr)
{
    static std::unordered_map<Opcode, std::function<Token*(Shader&, Token*)>> parsers = {
        { Opcode::Nop, TokenParser<Opcode::Nop> },
        { Opcode::Mov, TokenParser<Opcode::Mov> },
        { Opcode::Add, TokenParser<Opcode::Add> },
        { Opcode::Sub, TokenParser<Opcode::Sub> },
        { Opcode::Mad, TokenParser<Opcode::Mad> },
        { Opcode::Mul, TokenParser<Opcode::Mul> },
        { Opcode::Rcp, TokenParser<Opcode::Rcp> },
        { Opcode::Rsq, TokenParser<Opcode::Rsq> },
        { Opcode::Dp3, TokenParser<Opcode::Dp3> },
        { Opcode::Dp4, TokenParser<Opcode::Dp4> },
        { Opcode::Min, TokenParser<Opcode::Min> },
        { Opcode::Max, TokenParser<Opcode::Max> },
        { Opcode::Slt, TokenParser<Opcode::Slt> },
        { Opcode::Sge, TokenParser<Opcode::Sge> },
        { Opcode::Exp, TokenParser<Opcode::Exp> },
        { Opcode::Log, TokenParser<Opcode::Log> },
        { Opcode::Lit, TokenParser<Opcode::Lit> },
        { Opcode::Dst, TokenParser<Opcode::Dst> },
        { Opcode::Lrp, TokenParser<Opcode::Lrp> },
        { Opcode::Frc, TokenParser<Opcode::Frc> },
        { Opcode::M4x4, TokenParser<Opcode::M4x4> },
        { Opcode::M4x3, TokenParser<Opcode::M4x3> },
        { Opcode::M3x4, TokenParser<Opcode::M3x4> },
        { Opcode::M3x3, TokenParser<Opcode::M3x3> },
        { Opcode::M3x2, TokenParser<Opcode::M3x2> },
        { Opcode::Call, TokenParser<Opcode::Call> },
        { Opcode::Callnz, TokenParser<Opcode::Callnz> },
        { Opcode::Loop, TokenParser<Opcode::Loop> },
        { Opcode::Ret, TokenParser<Opcode::Ret> },
        { Opcode::Endloop, TokenParser<Opcode::Endloop> },
        { Opcode::Label, TokenParser<Opcode::Label> },
        { Opcode::Dcl, TokenParser<Opcode::Dcl> },
        { Opcode::Pow, TokenParser<Opcode::Pow> },
        { Opcode::Crs, TokenParser<Opcode::Crs> },
        { Opcode::Sgn, TokenParser<Opcode::Sgn> },
        { Opcode::Abs, TokenParser<Opcode::Abs> },
        { Opcode::Nrm, TokenParser<Opcode::Nrm> },
        { Opcode::Sincos, TokenParser<Opcode::Sincos> },
        { Opcode::Rep, TokenParser<Opcode::Rep> },
        { Opcode::EndRep, TokenParser<Opcode::EndRep> },
        { Opcode::If, TokenParser<Opcode::If> },
        { Opcode::IfC, TokenParser<Opcode::IfC> },
        { Opcode::Else, TokenParser<Opcode::Else> },
        { Opcode::Endif, TokenParser<Opcode::Endif> },
        { Opcode::Break, TokenParser<Opcode::Break> },
        { Opcode::BreakC, TokenParser<Opcode::BreakC> },
        { Opcode::Mova, TokenParser<Opcode::Mova> },
        { Opcode::Defb, TokenParser<Opcode::Defb> },
        { Opcode::Defi, TokenParser<Opcode::Defi> },
        { Opcode::Texcoord, TokenParser<Opcode::Texcoord> },
        { Opcode::Texkill, TokenParser<Opcode::Texkill> },
        { Opcode::Tex, TokenParser<Opcode::Tex> },
        { Opcode::Texbem, TokenParser<Opcode::Texbem> },
        { Opcode::Texbeml, TokenParser<Opcode::Texbeml> },
        { Opcode::Texreg2ar, TokenParser<Opcode::Texreg2ar> },
        { Opcode::Texreg2gb, TokenParser<Opcode::Texreg2gb> },
        { Opcode::Texm3x2pad, TokenParser<Opcode::Texm3x2pad> },
        { Opcode::Texm3x2tex, TokenParser<Opcode::Texm3x2tex> },
        { Opcode::Texm3x3pad, TokenParser<Opcode::Texm3x3pad> },
        { Opcode::Texm3x3tex, TokenParser<Opcode::Texm3x3tex> },
        { Opcode::Reserved0, TokenParser<Opcode::Reserved0> },
        { Opcode::Texm3x3spec, TokenParser<Opcode::Texm3x3spec> },
        { Opcode::Texm3x3vspec, TokenParser<Opcode::Texm3x3vspec> },
        { Opcode::Expp, TokenParser<Opcode::Expp> },
        { Opcode::Logp, TokenParser<Opcode::Logp> },
        { Opcode::Cnd, TokenParser<Opcode::Cnd> },
        { Opcode::Def, TokenParser<Opcode::Def> },
        { Opcode::Texreg2rgb, TokenParser<Opcode::Texreg2rgb> },
        { Opcode::Texdp3tex, TokenParser<Opcode::Texdp3tex> },
        { Opcode::Texm3x2depth, TokenParser<Opcode::Texm3x2depth> },
        { Opcode::Texdp3, TokenParser<Opcode::Texdp3> },
        { Opcode::Texm3x3, TokenParser<Opcode::Texm3x3> },
        { Opcode::Texdepth, TokenParser<Opcode::Texdepth> },
        { Opcode::Cmp, TokenParser<Opcode::Cmp> },
        { Opcode::Bem, TokenParser<Opcode::Bem> },
        { Opcode::Dp2add, TokenParser<Opcode::Dp2add> },
        { Opcode::Dsx, TokenParser<Opcode::Dsx> },
        { Opcode::Dsy, TokenParser<Opcode::Dsy> },
        { Opcode::Texldd, TokenParser<Opcode::Texldd> },
        { Opcode::Setp, TokenParser<Opcode::Setp> },
        { Opcode::Texldl, TokenParser<Opcode::Texldl> },
        { Opcode::BreakP, TokenParser<Opcode::BreakP> },
        { Opcode::Phase, TokenParser<Opcode::Phase> },
        { Opcode::Comment, TokenParser<Opcode::Comment> },
        { Opcode::End, TokenParser<Opcode::End> }
    };
    Token& chunk = *(Token*)a_TokenPtr;
    return parsers.at(Opcode(chunk.opCode))(a_Shader, a_TokenPtr);
}

void DX9::ShaderParser::operator()(Shader& a_Shader, Token* const a_Data, const size_t& a_Size)
{
    a_Shader.versionMinor = a_Data->version.versionMinor;
    a_Shader.versionMajor = a_Data->version.versionMajor;
    a_Shader.shaderType = ShaderType(a_Data->version.shaderType);
    for (auto ptr = a_Data + 1; ptr != nullptr;) {
        ptr = ReadChunk(a_Shader, ptr);
        if (ptr != nullptr)
            assert(std::distance(a_Data, ptr) < a_Size);
    }
}
}

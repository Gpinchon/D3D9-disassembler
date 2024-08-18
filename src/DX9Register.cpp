#include <DX9Register.hpp>

#include <cassert>

namespace DX9 {
Register::Register()
{
    uint32_t index = 0;
    for (auto& reg : input) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : temp) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : constFloat) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : constInt) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : constBool) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : predicate) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : sampler) {
        reg.index = index;
        ++index;
    }
    // index = 0;
    // for (auto& reg : face) {
    //     reg.index = index;
    //     ++index;
    // }
    // index = 0;
    // for (auto& reg : pos) {
    //     reg.index = index;
    //     ++index;
    // }
    index = 0;
    for (auto& reg : loop) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : texture) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : colorOut) {
        reg.index = index;
        ++index;
    }
    index = 0;
    for (auto& reg : depthOut) {
        reg.index = index;
        ++index;
    }
}

const RegisterKey* Register::GetKey(const RegisterType& a_Type, const uint32_t& a_Index) const
{
    switch (a_Type) {
    case RegisterType::Temp:
        return &temp[a_Index];
        break;
    case RegisterType::Input:
        return &input[a_Index];
        break;
    case RegisterType::Const:
        return &constFloat[a_Index];
        break;
        // case RegisterType::Addr:
        //     break;
    case RegisterType::Texture:
        return &texture[a_Index];
        break;
    case RegisterType::RastOut:
        break;
    case RegisterType::AttrOut:
        break;
        // case RegisterType::TexCrdOut:
        //     break;
    case RegisterType::Output:
        break;
    case RegisterType::ConstInt:
        return &constInt[a_Index];
        break;
    case RegisterType::ColorOut:
        return &colorOut[a_Index];
        break;
    case RegisterType::DepthOut:
        return &depthOut[a_Index];
        break;
    case RegisterType::Sampler:
        return &sampler[a_Index];
        break;
    case RegisterType::Const2:
        break;
    case RegisterType::Const3:
        break;
    case RegisterType::Const4:
        break;
    case RegisterType::ConstBool:
        return &constBool[a_Index];
        break;
    case RegisterType::Loop:
        return &loop[a_Index];
        break;
    case RegisterType::TempFloat16:
        break;
    case RegisterType::MiscType:
        break;
    case RegisterType::Label:
        break;
    case RegisterType::Predicate:
        return &predicate[a_Index];
        break;
    default:
        assert(false && "INCORRECT REGISTER TYPE");
    }
    return nullptr;
}

RegisterKey* Register::GetKey(const RegisterType& a_Type, const uint32_t& a_Index)
{
    switch (a_Type) {
    case RegisterType::Temp:
        return &temp[a_Index];
        break;
    case RegisterType::Input:
        return &input[a_Index];
        break;
    case RegisterType::Const:
        return &constFloat[a_Index];
        break;
        // case RegisterType::Addr:
        //     break;
    case RegisterType::Texture:
        return &texture[a_Index];
        break;
    case RegisterType::RastOut:
        break;
    case RegisterType::AttrOut:
        break;
        // case RegisterType::TexCrdOut:
        //     break;
    case RegisterType::Output:
        break;
    case RegisterType::ConstInt:
        return &constInt[a_Index];
        break;
    case RegisterType::ColorOut:
        return &colorOut[a_Index];
        break;
    case RegisterType::DepthOut:
        return &depthOut[a_Index];
        break;
    case RegisterType::Sampler:
        return &sampler[a_Index];
        break;
    case RegisterType::Const2:
        break;
    case RegisterType::Const3:
        break;
    case RegisterType::Const4:
        break;
    case RegisterType::ConstBool:
        return &constBool[a_Index];
        break;
    case RegisterType::Loop:
        return &loop[a_Index];
        break;
    case RegisterType::TempFloat16:
        break;
    case RegisterType::MiscType:
        break;
    case RegisterType::Label:
        break;
    case RegisterType::Predicate:
        return &predicate[a_Index];
        break;
    default:
        assert(false && "INCORRECT REGISTER TYPE");
    }
    return nullptr;
}
}

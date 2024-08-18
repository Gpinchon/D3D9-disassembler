#pragma once

#include <string>

namespace DX9 {
struct Shader;
struct HLSLStringifyer {
    std::string operator()(const Shader& a_Shader);
    std::string _inputBody;
    std::string _outputBody;
    std::string _constBody;
    std::string _mainBody;
};
}

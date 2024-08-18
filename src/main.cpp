// #include <hlslcc.h>

#pragma comment(lib, "d3dx9.lib")
// #pragma comment(lib, "d3dcompiler.lib")
// #include <d3dcompiler.h>
#include <d3dx9.h>

#include <DX9HLSLStringifyer.hpp>
#include <DX9Shader.hpp>
#include <DX9ShaderParser.hpp>

#include <bitset>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::byte> readFile(const char* filename)
{
    // open the file:
    std::basic_ifstream<std::byte> file(filename, std::ios::binary);

    // read the data:
    return std::vector<std::byte>(
        (std::istreambuf_iterator<std::byte>(file)),
        std::istreambuf_iterator<std::byte>());
}

void WriteFile(const std::filesystem::path& a_Path, const std::byte* a_Data, const size_t& a_Size)
{
    std::basic_ofstream<std::byte> file(a_Path, std::ios::binary);
    file.write(a_Data, a_Size);
}

void WriteDXBC(const std::filesystem::path& a_InFilePath, const LPD3DXBUFFER& a_DXBC)
{
    auto outPath = std::filesystem::path(a_InFilePath).replace_extension("dxbc");
    WriteFile(outPath, (std::byte*)a_DXBC->GetBufferPointer(), a_DXBC->GetBufferSize());
}

void WriteHLSL(const std::filesystem::path& a_InFilePath, const std::string& a_HLSL)
{
    auto outPath = std::filesystem::path(a_InFilePath).replace_extension("hlsl");
    WriteFile(outPath, (std::byte*)a_HLSL.c_str(), a_HLSL.size());
}

void TestComp()
{
    std::string testCode = "sampler sampler0 : register(s0);\n"
                           "struct PS_IN {\n"
                           "    float4 texCoord0 : TEXCOORD0;"
                           "    float4 texCoord1 : TEXCOORD1; "
                           "};\n"
                           "float4 main(in PS_IN PSIn) : COLOR0 {\n"
                           "    float4 texCoord = PSIn.texCoord0 + PSIn.texCoord1;"
                           "    for (int i = 0; i < texCoord.x; i++) {\n"
                           "       texCoord.x++;\n"
                           "       if (texCoord.x > 5) break;"
                           "    }\n"
                           "    return tex2D(sampler0, texCoord);"
                           "}\n";
    LPD3DXBUFFER compiledShader = nullptr, errs = nullptr, disassembly = nullptr;
    LPD3DXCONSTANTTABLE constantTable = nullptr;
    D3DXCompileShader(
        testCode.c_str(), testCode.size(),
        nullptr, // Defiles
        nullptr, // Includes
        "main",
        "ps_3_0", // Profile
        0, // Flags
        &compiledShader, &errs, &constantTable);
    if (errs != nullptr)
        std::cout << (char*)errs->GetBufferPointer() << std::endl;
    DX9::Shader shader;
    DX9::ShaderParser {}(shader,
        (DX9::Token*)compiledShader->GetBufferPointer(),
        compiledShader->GetBufferSize() / sizeof(DX9::Token));
    auto hlsl = DX9::HLSLStringifyer {}(shader);
    D3DXDisassembleShader(
        (DWORD*)compiledShader->GetBufferPointer(),
        false, nullptr, &disassembly);
    std::cout << (char*)disassembly->GetBufferPointer() << std::endl;
}

int main(int argc, char* argv[])
{
    TestComp();
    if (argc < 2) {
        std::cerr << "Please provide an MSASM file\n";
        return -1;
    }
    std::filesystem::path filePath = argv[1];
    // Shader shader(argv[1]);
    // DX9HLSLStringifyer stringifyer;
    // auto hlsl = stringifyer(shader);
    LPD3DXBUFFER compiledShader = nullptr;
    LPD3DXBUFFER errs = nullptr;
    LPD3DXBUFFER disassembly = nullptr;
    LPD3DXCONSTANTTABLE constantTable = nullptr;

    D3DXAssembleShaderFromFile(
        argv[1], // FilePath
        nullptr, // Defines
        nullptr, // Includes
        0, // Flags
        &compiledShader,
        &errs);

    WriteDXBC(filePath, compiledShader);
    DX9::Shader shader;
    DX9::ShaderParser {}(shader,
        (DX9::Token*)compiledShader->GetBufferPointer(),
        compiledShader->GetBufferSize() / sizeof(DX9::Token));
    auto hlsl = DX9::HLSLStringifyer {}(shader);
    WriteHLSL(filePath, hlsl);
    // Test result
    std::string shaderProfile;
    shaderProfile += shader.shaderType == DX9::ShaderType::Pixel ? "ps_" : "vs_";
    shaderProfile += std::to_string(shader.versionMajor) + "_" + std::to_string(shader.versionMinor);
    D3DXCompileShader(
        hlsl.c_str(), hlsl.size(),
        nullptr, // Defines
        nullptr, // Includes
        shader.entryPoint.c_str(), // FunctionName
        shaderProfile.c_str(), // Profile
        0, // Flags
        &compiledShader, &errs, &constantTable);
    if (errs != nullptr)
        std::cerr << (char*)errs->GetBufferPointer() << std::endl;
}

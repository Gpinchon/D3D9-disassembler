#pragma once

enum class DX9TextureType : uint16_t {
    Unknown = 0, // uninitialized value
    Texture2D = 2, // dcl_2d s# (for declaring a 2-D texture)
    TextureCube = 3, // dcl_cube s# (for declaring a cube texture)
    TextureVolume = 4, // dcl_volume s# (for declaring a volume texture)
};

#pragma once

#include <map>
#include <string>
#include "Game.h"

class Renderer {
protected:
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indicesBuffer;
    std::map<std::string, ID3DBlob> pixelShaders;
    std::map<std::string, ID3DBlob> vertexShaders;
    
};

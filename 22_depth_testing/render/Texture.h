#pragma once

#include <string>
#include <map>
#include <assimp/scene.h>

namespace render {

struct Texture {
    enum Type { diffuse, specular };
    
    unsigned int id;
    Type type;
    std::string path;
    
    static std::map<std::string, Texture> s_loadedTextures;
    
    static Texture Load(const char* path, Type type);
    static aiTextureType GetAssimpType(Texture::Type type);
};

}

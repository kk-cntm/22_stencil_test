#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "Plane.h"
#include "../Vertex.h"

using namespace render::shape;

int Plane::s_instances = 0;
unsigned int Plane::s_vao;
unsigned int Plane::s_vbo;
unsigned int Plane::s_ebo;

Plane::Plane(glm::vec3 position, Material material):
    m_position(position),
    m_material(material),
    m_matType(MaterialType::simple),
    m_size(1.0f)
{
    Init();
}

Plane::Plane(glm::vec3 position, const std::vector<Texture>& textures):
    m_position(position),
    m_textures(textures),
    m_matType(MaterialType::texture),
    m_size(1.0f)
{
    Init();
}

void Plane::Init() {
    if (s_instances == 0) Setup();
    s_instances++;
}

void Plane::Draw(Shader& shader) const {
    shader.Use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    model = glm::scale(model, m_size);
    
    shader.SetValue("model", model);
    
    switch (m_matType) {
    case Plane::MaterialType::texture:
        DrawWithTexture(shader);
        break;
    case Plane::MaterialType::simple:
        DrawWithSimple(shader);
        break;
    default: std::cerr << "[ERROR] Plane::Draw invalid m_matType\n";
    }
    
    glBindVertexArray(s_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

inline void Plane::DrawWithSimple(Shader& shader) const {
    shader.SetValue("matrial.diffuse", m_material.diffuse);
    shader.SetValue("matrial.specular", m_material.specular);
    shader.SetValue("matrial.shininess", m_material.shininess);
}

inline void Plane::DrawWithTexture(Shader& shader) const {
    unsigned int diffNr = 1;
    unsigned int specNr = 1;
    
    for (int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        
        std::string name;
        
        switch (m_textures[i].type) {
        case render::Texture::diffuse:
            name = "texture_diffuse" + std::to_string(diffNr);
            diffNr++;
            break;
        case render::Texture::specular:
            name = "texture_specular" + std::to_string(specNr);
            specNr++;
            break;
        default: std::cerr << "[ERROR] Plane::DrawWithTexture undefined texture type\n";
        }
        
        shader.SetValue(("material." + name).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
}

void Plane::Setup() {
    glGenBuffers(1, &s_vbo);
    glGenBuffers(1, &s_ebo);
    glGenVertexArrays(1, &s_vao);
    
    glBindVertexArray(s_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, s_vertices.size() * sizeof(Vertex), &s_vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_indices.size() * sizeof(unsigned int), &s_indices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (void*)(offsetof(render::Vertex, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(render::Vertex), (void*)offsetof(render::Vertex, texCoords));
    glEnableVertexAttribArray(2);
}

void Plane::SetSize(const glm::vec3& size) {
    m_size = size;
}

inline glm::vec3 Plane::GetSize() {
    return m_size;
}

const std::array<render::Vertex, 4> Plane::s_vertices = {
    render::Vertex({ glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(2.0f) }),
    render::Vertex({ glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(2.0f, 0.0f) }),
    render::Vertex({ glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }),
    render::Vertex({ glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 2.0f) }),
};

const std::array<unsigned int, 6> Plane::s_indices = { 0, 1, 2, 3, 0, 2 };

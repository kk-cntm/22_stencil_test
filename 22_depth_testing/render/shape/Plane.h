#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "../Vertex.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../IDrawable.h"
#include "../Material.h"

namespace render::shape {

class Plane: public render::IDrawable {
public:
    Plane(glm::vec3 position, Material material);
    Plane(glm::vec3 position, const std::vector<Texture>& textures);
    void Draw(Shader& shader) const;
    void SetSize(const glm::vec3& size);
    inline glm::vec3 GetSize();
private:
    enum class MaterialType { texture, simple };
    
    static const std::array<Vertex, 4> s_vertices;
    static const std::array<unsigned int, 6> s_indices;
    static int s_instances;
    static unsigned int s_vao, s_vbo, s_ebo;
    
    static void Init();
    static void Setup();
    
    glm::vec3 m_position;
    glm::vec3 m_size;
    MaterialType m_matType;
    Material m_material;
    std::vector<Texture> m_textures;
    
    inline void DrawWithSimple(Shader& shader) const;
    inline void DrawWithTexture(Shader& shader) const;
    glm::vec2 GetWrap();
};

}

#pragma once

#include "kat/engine.hpp"
#include "kat/graphics/mesh.hpp"
#include "kat/graphics/texture.hpp"
#include "kat/graphics/shader.hpp"
#include "kat/util/interfaces.hpp"

namespace kat {

    namespace embed::shaders::sprite {
        const std::string vertexSrc = "#version 430 core\n"
                                      "layout(location=0) in vec3 vPosition;\n"
                                      "layout(location=1) in vec2 vTexCoord;\n"
                                      "layout(location=2) in vec4 vTint;\n"
                                      "layout(location=3) in vec3 vNormal;\n"
                                      "out vec4 fTint;\n"
                                      "out vec3 fNormal;\n"
                                      "out vec2 fUV;\n"
                                      "uniform mat4 uViewProjection;\n"
                                      "uniform mat4 uModel;\n"
                                      "void main() {\n"
                                      "    gl_Position = vec4(vPosition, 1.0);\n"
                                      "    fUV = vTexCoord;\n"
                                      "    fTint = vTint;\n"
                                      "    fNormal = vNormal;\n"
                                      "}";

        const std::string fragmentSrc = "#version 430 core\n"
                                        "in vec2 fUV;\n"
                                        "in vec4 fTint;\n"
                                        "in vec3 fNormal;\n"
                                        "in vec4 fPosition;\n"
                                        "out vec4 colorOut;\n"
                                        "uniform sampler2D uTexture;\n"
                                        "uniform vec2 uMinUV;\n"
                                        "uniform vec2 uMaxUV;\n"
                                        "void main() {\n"
                                        "    vec2 uv = fUV * (uMaxUV - uMinUV) + uMinUV;"
                                        "    colorOut = texture(uTexture, uv);\n"
                                        "}";
    }

    class Sprite : public kat::util::IPositionable<glm::vec2> {
    public:
        Sprite(const std::shared_ptr<kat::Texture2D>& texture, const glm::vec2& size);

        void render();

        static void init();
        static void cleanup();
    private:

        kat::Texture2D::Region m_TextureRegion;
        glm::vec2 m_Size;

        static inline std::unique_ptr<kat::GraphicsShader> s_SpriteShader;
        static inline std::unique_ptr<kat::Mesh> s_SpriteMesh;
    };
}

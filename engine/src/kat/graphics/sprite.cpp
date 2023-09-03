#include "sprite.hpp"
#include "kat/graphics.hpp"
#include "kat/util/transform_stack.hpp"

namespace kat {

    void Sprite::init() {
        s_SpriteShader = kat::GraphicsShader::createUnique(
                { std::pair{ ShaderType::Vertex, embed::shaders::sprite::vertexSrc },
                  std::pair{ ShaderType::Fragment, embed::shaders::sprite::fragmentSrc }});

        std::vector<kat::StandardVertex> vertices = {
                { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
                { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
                { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
                { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
        };

        s_SpriteMesh = std::make_unique<kat::Mesh>(vertices, PrimitiveMode::TriangleFan);
    }

    void Sprite::cleanup() {
        s_SpriteShader = nullptr;
        s_SpriteMesh = nullptr;
    }

    Sprite::Sprite(const std::shared_ptr<kat::Texture2D> &texture, const glm::vec2 &size)
                 : m_TextureRegion(texture->getFullRegion()), m_Size(size) {
    }

    void Sprite::render() {
        kat::graphics::polygonMode(kat::graphics::PolygonMode::Fill);
        kat::transform::push();

        kat::transform::translate(m_Position);
        kat::transform::scale(m_Size);

        s_SpriteShader->bind();
        s_SpriteShader->bindTexture("uTexture", 0, m_TextureRegion.texture);

        s_SpriteMesh->render();

        kat::transform::pop();
    }
}
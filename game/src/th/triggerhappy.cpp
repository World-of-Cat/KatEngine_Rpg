#include "th/triggerhappy.hpp"

#include <kat/graphics/mesh.hpp>
#include <kat/graphics/shader.hpp>
#include <kat/graphics/render_target.hpp>

#include <glm/gtx/color_space.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/gtc/matrix_transform.hpp>

glm::vec4 rgba8888(unsigned int v) {
    float r = float((v >> 24) & 0xff) / 255.0f;
    float g = float((v >> 16) & 0xff) / 255.0f;
    float b = float((v >> 8) & 0xff) / 255.0f;
    float a = float(v & 0xff) / 255.0f;
    return { r, g, b, a };
}

namespace th {

}

int main() {

    kat::Window::create(kat::Window::Config{
        "Window",
        glm::uvec2{1280, 720}
//        kat::Window::Fullscreen{0, true}
    });


    std::vector<kat::StandardVertex> screenVertices = {
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    };

    std::vector<unsigned int> screenIndices = {
            0, 1, 2,
            0, 2, 3
    };

    std::vector<kat::StandardVertex> vertices = {
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { {  1.0f,  1.0f, 0.0f }, { 0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    };

    std::vector<unsigned int> indices = {
            0, 1, 2,
            0, 2, 3
    };

    auto mesh = std::make_shared<kat::Mesh>(vertices, indices, kat::PrimitiveMode::Triangles);

    auto shader = kat::GraphicsShader::load({"shaders/shader2.frag", "shaders/shader2.vert"});

    auto screenShader = kat::GraphicsShader::load({"shaders/texturedsg.frag", "shaders/shader.vert"});

    auto screenMesh = std::make_shared<kat::Mesh>(screenVertices, screenIndices, kat::PrimitiveMode::Triangles);

    auto screenFb = std::make_shared<kat::Framebuffer>(glm::uvec2{ 240, 135 });
    auto screenTex = screenFb->addColorAttachment(0, kat::TextureFormat::RGB8);
    screenFb->addPackedDepthStencilRenderBuffer();

    screenTex->setFilter(kat::TextureFilter::Nearest);

    glm::mat4 mat = glm::scale(glm::identity<glm::mat4>(), {0.25f, 0.25f, 0.25f});

    auto tex = kat::Texture2D::load("textures/test.png");
    tex->setFilter(kat::TextureFilter::Nearest);


    while (kat::gbl::activeWindow->isOpen()) {

        glm::dvec2 mp;
        glfwGetCursorPos(kat::gbl::activeWindow->getHandle(), &mp.x, &mp.y);
        float mx = mp.x / 640.0f - 1.0f;
        float my = 1.0f - mp.y / 360.0f;

        screenFb->bindViewport();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader->setFloat("uTime", static_cast<float>(glfwGetTime()));
        shader->setMatrix4f("uMatrix", mat);
        shader->setVec3f("uLightPos", {mx, my, -1});

        tex->bindUnit(0);
        shader->setInteger("uTexture", 0);

        shader->bind();
        mesh->render();

        kat::Framebuffer::bindDefaultViewport();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        screenShader->bind();
        screenTex->bindUnit(0);

        screenShader->setInteger("uTexture", 0);

        screenMesh->render();

        kat::gbl::activeWindow->update();
    }

    kat::gbl::cleanup();
    return EXIT_SUCCESS;
}
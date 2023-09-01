#include "th/triggerhappy.hpp"

#include <kat/graphics/mesh.hpp>
#include <kat/graphics/shader.hpp>
#include <kat/graphics/render_target.hpp>

#include <glm/gtx/color_space.hpp>
#include <glm/gtx/string_cast.hpp>


namespace th {

}

int main() {

    kat::Window::create(kat::Window::Config{
        "Window",
        glm::uvec2{1280, 720}
//        kat::Window::Fullscreen{0, true}
    });

    std::vector<kat::StandardVertex> vertices = {
            { {  0.00f,  0.00f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { {  0.50f, -0.25f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { {  0.50f,  0.25f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { {  0.25f,  0.50f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { { -0.25f,  0.50f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { { -0.50f,  0.25f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { { -0.50f, -0.25f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { { -0.25f, -0.50f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
//            { {  0.25f, -0.50f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f } },
    };

    std::vector<unsigned int> indices = {
//            0,1,2,3,4,5,6,7,8,1
//            1,2,0,3,7,4,6,5
        0
    };

    spdlog::info("{}", glm::to_string(glm::vec4(glm::rgbColor(glm::vec3(180.0f, 1.0f, 1.0f)), 1.0f)));

    int n = 4;

    for (int i = 0 ; i < n ; i++) {
        float x = cosf(float(i) / float(n >> 1) * 3.14159f);
        float y = sinf(float(i) / float(n >> 1) * 3.14159f);
        vertices.push_back({ {x, y, 0.0f}, { 1.0f, 0.0f },
                             glm::vec4(glm::rgbColor(glm::vec3(360.0f * float(i) / float(n), 1.0f, 1.0f)), 1.0f),
                             { 0.0f, 0.0f, 1.0f } });
        indices.push_back(i + 1);
    }
    indices.push_back(1);

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


    auto mesh = std::make_shared<kat::Mesh>(vertices, indices, kat::PrimitiveMode::TriangleFan);

    auto shader = kat::GraphicsShader::load({"shaders/fragsh.glsl", "shaders/shader.vert"});

    auto screenShader = kat::GraphicsShader::load({"shaders/texturedsg.frag", "shaders/shader.vert"});

    auto screenMesh = std::make_shared<kat::Mesh>(screenVertices, screenIndices, kat::PrimitiveMode::TriangleFan);

    auto screenFb = std::make_shared<kat::Framebuffer>(glm::uvec2{ 240, 135 });
    auto screenTex = screenFb->addColorAttachment(0, kat::TextureFormat::RGB8);
    screenFb->addPackedDepthStencilRenderBuffer();

    screenTex->setFilter(kat::TextureFilter::Nearest);


    while (kat::gbl::activeWindow->isOpen()) {

        screenFb->bindViewport();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader->setFloat("uTime", static_cast<float>(glfwGetTime()));

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
#include "th/triggerhappy.hpp"
#include "kat/graphics/colors.hpp"

#include <kat/graphics/mesh.hpp>
#include <kat/graphics/shader.hpp>
#include <kat/graphics/render_target.hpp>
#include <kat/graphics.hpp>
#include <kat/util/camera.hpp>
#include <kat/util/math.hpp>
#include <kat/util/clock.hpp>

#include <glm/gtx/color_space.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/gtc/matrix_transform.hpp>




namespace th {

}

int main() {
    kat::gbl::setup();

    spdlog::set_level(spdlog::level::debug);

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
            { { -64.0f, -64.0f, 0.0f }, { 0.0f, 0.5f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { {  64.0f, -64.0f, 0.0f }, { 0.375f, 0.5f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { {  64.0f,  64.0f, 0.0f }, { 0.375f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
            { { -64.0f,  64.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
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

    auto camera = std::make_shared<kat::util::OrthographicCamera>(-120, 120, -67.5, 67.5);

    auto tex = kat::Texture2D::load("textures/test.png");
    tex->setFilter(kat::TextureFilter::Nearest);

    auto clock = kat::util::Clock();

    while (kat::gbl::activeWindow->isOpen()) {

        float moveScale = 1.0f;
        if (kat::input::isKeyPressed(GLFW_KEY_LEFT_SHIFT) || kat::input::isKeyPressed(GLFW_KEY_RIGHT_SHIFT)) {
            moveScale = 2.0f;
        }
        else if (kat::input::isKeyPressed(GLFW_KEY_LEFT_CONTROL) || kat::input::isKeyPressed(GLFW_KEY_RIGHT_CONTROL)) {
            moveScale = 0.5f;
        }

        if (kat::input::isKeyPressed(GLFW_KEY_A)) {
            camera->move(moveScale * glm::vec3{-128.0f * clock.getDeltaTime().count(), 0.0f, 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_D)) {
            camera->move(moveScale * glm::vec3{128.0f * clock.getDeltaTime().count(), 0.0f, 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_W)) {
            camera->move(moveScale * glm::vec3{0.0f, 128.0f * clock.getDeltaTime().count(), 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_S)) {
            camera->move(moveScale * glm::vec3{0.0f, -128.0f * clock.getDeltaTime().count(), 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_MINUS)) {
            camera->addZoom(-0.25 * moveScale * clock.getDeltaTime().count());
        }

        if (kat::input::isKeyPressed(GLFW_KEY_EQUAL)) {
            camera->addZoom(0.25 * moveScale * clock.getDeltaTime().count());
        }

        camera->update();

        screenFb->bindViewport();

        kat::graphics::clear(kat::colors::BLACK);
        kat::graphics::polygonMode(kat::graphics::PolygonMode::Fill);

        shader->bind();
        shader->bindTexture("uTexture", 0, tex);
        shader->setMatrix4f("uViewProjection", camera->getCombined());
        mesh->render();

        kat::Framebuffer::bindDefaultViewport();
        kat::graphics::polygonMode(kat::graphics::PolygonMode::Fill);

        if (camera->getZoomScale() < 1.0f) {
            kat::graphics::clear({0.75f, 0.75f, 0.75f, 1.0f});
        }

        screenShader->bind();
        screenShader->bindTexture("uTexture", 0, screenTex);
        screenShader->setFloat("uScale", camera->getZoomScale());
        screenMesh->render();

        kat::gbl::activeWindow->update();
        clock.tick();
    }

    kat::gbl::cleanup();
    return EXIT_SUCCESS;
}
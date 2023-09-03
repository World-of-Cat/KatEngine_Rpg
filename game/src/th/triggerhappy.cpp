#include "th/triggerhappy.hpp"

namespace th {
    TriggerHappy::TriggerHappy() {
        kat::gbl::setup();
        setDefaults();
        createWindow();
        loadAssets();
    }

    TriggerHappy::~TriggerHappy() = default;

    void TriggerHappy::run() {
        kat::gbl::clock.tick();

        while (kat::gbl::activeWindow->isOpen()) {
            update(kat::gbl::clock.getDeltaTime().count());

            m_DownscaleFramebuffer->bindViewport();
            renderWorld();

            kat::Framebuffer::bindDefaultViewport();
            renderScreen();

            kat::gbl::activeWindow->update();
            kat::gbl::clock.tick();
        }
    }

    void TriggerHappy::setDefaults() {
        kat::Texture2D::defaultFilter = kat::TextureFilter::Nearest;
        spdlog::set_level(spdlog::level::debug);
    }

    void TriggerHappy::createWindow() {
        kat::Window::create(kat::Window::Config{
                "Window",
                glm::uvec2{1280, 720}
//        kat::Window::Fullscreen{0, true}
        });
    }

    void TriggerHappy::loadAssets() {
        m_ScreenQuad = kat::Mesh::createQuad({-1.0f, -1.0f}, {1.0f, 1.0f}, {{0.0f, 0.0f}, {1.0f, 1.0f}});
        m_TestQuad = kat::Mesh::createQuad({-64.0f, -64.0f}, {64.0f, 64.0f}, { {0.0f, 304.0f / 384.0f}, {0.125f, 1.0f}});
        m_TestShader = kat::GraphicsShader::load({"shaders/shader.frag", "shaders/shader.vert"});
        m_ScreenShader = kat::GraphicsShader::load({"shaders/screen.frag", "shaders/screen.vert"});

        m_DownscaleFramebuffer = kat::Framebuffer::makeSimpleRenderTarget({480, 270});

        m_Camera = std::make_shared<kat::util::OrthographicCamera>(-240, 240, -135, 135);

        m_Texture = kat::Texture2D::load("textures/t4-3.png");
    }

    void TriggerHappy::update(double deltaTime) {
        float moveScale = 1.0f;
        if (kat::input::isKeyPressed(GLFW_KEY_LEFT_SHIFT) || kat::input::isKeyPressed(GLFW_KEY_RIGHT_SHIFT)) {
            moveScale = 2.0f;
        } else if (kat::input::isKeyPressed(GLFW_KEY_LEFT_CONTROL) ||
                   kat::input::isKeyPressed(GLFW_KEY_RIGHT_CONTROL)) {
            moveScale = 0.5f;
        }

        if (kat::input::isKeyPressed(GLFW_KEY_A)) {
            m_Camera->move(moveScale * glm::vec3{-128.0f * deltaTime, 0.0f, 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_D)) {
            m_Camera->move(moveScale * glm::vec3{128.0f * deltaTime, 0.0f, 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_W)) {
            m_Camera->move(moveScale * glm::vec3{0.0f, 128.0f * deltaTime, 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_S)) {
            m_Camera->move(moveScale * glm::vec3{0.0f, -128.0f * deltaTime, 0.0f});
        }

        if (kat::input::isKeyPressed(GLFW_KEY_MINUS)) {
            m_Camera->addZoom(-0.25 * moveScale * deltaTime);
        }

        if (kat::input::isKeyPressed(GLFW_KEY_EQUAL)) {
            m_Camera->addZoom(0.25 * moveScale * deltaTime);
        }

        m_Camera->update();
    }

    void TriggerHappy::renderWorld() {
        kat::transform::push(m_Camera->getCombined());

        kat::graphics::clear(kat::colors::BLACK);
        kat::graphics::polygonMode(kat::graphics::PolygonMode::Fill);

        m_TestShader->bind();
        m_TestShader->bindTexture("uTexture", 0, m_Texture);
        m_TestQuad->render();

        kat::transform::pop();
    }

    void TriggerHappy::renderScreen() {
        kat::graphics::polygonMode(kat::graphics::PolygonMode::Fill);

        if (m_Camera->getZoomScale() < 1.0f) {
            kat::graphics::clear({0.75f, 0.75f, 0.75f, 1.0f});
        }

        m_ScreenShader->bind();
        m_ScreenShader->bindTexture("uTexture", 0, m_DownscaleFramebuffer->getColorAttachment(0));
        m_ScreenShader->setFloat("uScale", m_Camera->getZoomScale());
        m_ScreenQuad->render();
    }
}

void run() {
    auto game = th::TriggerHappy();
    game.run();
}

// setup like this so that cleanup is run after raii cleanup of things in the TriggerHappy object
int main() {
    run();
    kat::gbl::cleanup();
    return EXIT_SUCCESS;
}
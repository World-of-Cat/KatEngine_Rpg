#pragma once

#include <kat/engine.hpp>
#include <kat/os.hpp>
#include "kat/graphics/colors.hpp"

#include <kat/graphics/mesh.hpp>
#include <kat/graphics/shader.hpp>
#include <kat/graphics/render_target.hpp>
#include <kat/graphics.hpp>
#include <kat/util/camera.hpp>
#include <kat/util/clock.hpp>
#include <kat/util/transform_stack.hpp>


namespace th {

    class TriggerHappy {
    public:

        TriggerHappy();
        ~TriggerHappy();

        TriggerHappy(const TriggerHappy&) = delete;
        TriggerHappy(TriggerHappy&&) = delete;

        TriggerHappy& operator=(const TriggerHappy&) = delete;
        TriggerHappy& operator=(TriggerHappy&&) = delete;

        void run();

    private:

        void setDefaults();
        void createWindow();
        void loadAssets();

        void update(double deltaTime);
        void renderWorld();
        void renderScreen();

        std::unique_ptr<kat::Mesh> m_ScreenQuad;
        std::unique_ptr<kat::Mesh> m_TestQuad;

        std::shared_ptr<kat::GraphicsShader> m_TestShader;
        std::shared_ptr<kat::GraphicsShader> m_ScreenShader;

        std::unique_ptr<kat::Framebuffer> m_DownscaleFramebuffer;

        std::shared_ptr<kat::util::OrthographicCamera> m_Camera;
        std::shared_ptr<kat::Texture2D> m_Texture;
    };

}
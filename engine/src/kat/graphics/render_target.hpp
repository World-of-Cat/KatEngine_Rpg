#pragma once

#include "kat/engine.hpp"
#include "kat/graphics/texture.hpp"
#include "kat/graphics/mesh.hpp"

namespace kat {

    class RenderBuffer {
    public:

        RenderBuffer(const glm::uvec2 &size, TextureFormat format);
        RenderBuffer(const glm::uvec2 &size, int format);
        ~RenderBuffer();

        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;

        void bind() const;

    private:
        unsigned int m_Handle;

    };

    // subset of TextureFormat
    enum class DepthFormat : std::underlying_type_t<TextureFormat> {
        Depth16 = static_cast<std::underlying_type_t<TextureFormat>>(TextureFormat::Depth16),
        Depth24 = static_cast<std::underlying_type_t<TextureFormat>>(TextureFormat::Depth24),
        Depth32 = static_cast<std::underlying_type_t<TextureFormat>>(TextureFormat::Depth32)
    };

    class Framebuffer {
    public:
        static constexpr size_t MAX_COLOR_ATTACHMENTS = 32;

        explicit Framebuffer(const glm::uvec2& size);

        [[nodiscard]] const std::array<std::shared_ptr<kat::Texture2D>, MAX_COLOR_ATTACHMENTS>& getColorAttachments() const noexcept;
        [[nodiscard]] const std::shared_ptr<kat::Texture2D>& getColorAttachment(size_t index) const noexcept;
        [[nodiscard]] const std::shared_ptr<kat::Texture2D>& getDepthAttachment() const noexcept;
        [[nodiscard]] const std::shared_ptr<kat::Texture2D>& getStencilAttachment() const noexcept;
        [[nodiscard]] const std::shared_ptr<kat::RenderBuffer>& getDepthStencilRenderBuffer() const noexcept;

        std::shared_ptr<kat::Texture2D> addColorAttachment(size_t index, kat::TextureFormat format);
        std::shared_ptr<kat::Texture2D> addDepthAttachment(DepthFormat format = DepthFormat::Depth32);
        std::shared_ptr<kat::Texture2D> addStencilAttachment();
        std::shared_ptr<kat::RenderBuffer> addPackedDepthStencilRenderBuffer();

        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;

        void bind() const;
        void bindViewport() const;

        static void bindDefault();

        static void bindDefaultViewport();

    private:
        unsigned int m_Handle;

        glm::uvec2 m_Size;
        std::array<std::shared_ptr<kat::Texture2D>, MAX_COLOR_ATTACHMENTS> m_ColorAttachments;
        std::shared_ptr<kat::Texture2D> m_DepthAttachment;
        std::shared_ptr<kat::Texture2D> m_StencilAttachment;
        std::shared_ptr<kat::RenderBuffer> m_DepthStencilRenderBuffer;

    };

    class RenderTarget {
    public:
        explicit RenderTarget(const std::shared_ptr<Framebuffer>& fb);

        virtual void bindFramebuffer();

    private:

        std::shared_ptr<Framebuffer> m_Framebuffer;
    };
}
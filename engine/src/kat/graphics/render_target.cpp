#include "render_target.hpp"
#include "kat/os.hpp"

namespace kat {
    Framebuffer::Framebuffer(const glm::uvec2 &size) : m_Size(size) {
        glCreateFramebuffers(1, &m_Handle);
    }

    const std::array<std::shared_ptr<kat::Texture2D>, Framebuffer::MAX_COLOR_ATTACHMENTS> &Framebuffer::getColorAttachments() const noexcept {
        return m_ColorAttachments;
    }

    const std::shared_ptr<kat::Texture2D> &Framebuffer::getColorAttachment(size_t index) const noexcept {
        return m_ColorAttachments[index];
    }

    const std::shared_ptr<kat::Texture2D> &Framebuffer::getDepthAttachment() const noexcept {
        return m_DepthAttachment;
    }

    const std::shared_ptr<kat::Texture2D> &Framebuffer::getStencilAttachment() const noexcept {
        return m_StencilAttachment;
    }

    const std::shared_ptr<kat::RenderBuffer> &Framebuffer::getDepthStencilRenderBuffer() const noexcept {
        return m_DepthStencilRenderBuffer;
    }

    std::shared_ptr<kat::Texture2D> Framebuffer::addColorAttachment(size_t index, kat::TextureFormat format) {
        assert(index < MAX_COLOR_ATTACHMENTS);

        auto texture = Texture2D::create(m_Size, format);

        glNamedFramebufferTexture(m_Handle, GL_COLOR_ATTACHMENT0 + index, texture->getHandle(), 0);

        m_ColorAttachments[index] = texture;
        return texture;
    }

    std::shared_ptr<kat::Texture2D> Framebuffer::addDepthAttachment(DepthFormat format) {
        auto texture = Texture2D::create(m_Size, static_cast<TextureFormat>(format));

        glNamedFramebufferTexture(m_Handle, GL_DEPTH_ATTACHMENT, texture->getHandle(), 0);

        m_DepthAttachment = texture;
        return texture;
    }

    std::shared_ptr<kat::Texture2D> Framebuffer::addStencilAttachment() {
        auto texture = Texture2D::create(m_Size, TextureFormat::Stencil);

        glNamedFramebufferTexture(m_Handle, GL_STENCIL_ATTACHMENT, texture->getHandle(), 0);

        m_StencilAttachment = texture;
        return texture;
    }

    std::shared_ptr<kat::RenderBuffer> Framebuffer::addPackedDepthStencilRenderBuffer() {
        std::shared_ptr<RenderBuffer> rb = std::make_shared<RenderBuffer>(m_Size, GL_DEPTH24_STENCIL8);

        glNamedFramebufferRenderbuffer(m_Handle, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb->getHandle());

        m_DepthStencilRenderBuffer = rb;

        return rb;
    }

    unsigned int Framebuffer::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int Framebuffer::getHandle() const noexcept {
        return m_Handle;
    }

    void Framebuffer::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
    }

    void Framebuffer::bindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::bindDefaultViewport() {
        bindDefault();
        auto size = kat::gbl::activeWindow->getSize();
        glViewport(0, 0, size.x, size.y);
    }

    void Framebuffer::bindViewport() const {
        bind();
        glViewport(0, 0, static_cast<int>(m_Size.x), static_cast<int>(m_Size.y));
    }

    std::unique_ptr<Framebuffer> Framebuffer::makeSimpleRenderTarget(const glm::uvec2 &size) {
        auto fb = std::make_unique<Framebuffer>(size);
        fb->addColorAttachment(0, kat::TextureFormat::RGBA8);
        fb->addPackedDepthStencilRenderBuffer();
        return fb;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
    RenderBuffer::RenderBuffer(const glm::uvec2 &size, TextureFormat format) {
        glCreateRenderbuffers(1, &m_Handle);

        glNamedRenderbufferStorage(m_Handle, kat::glInternalFormatOf(format), static_cast<int>(size.x), static_cast<int>(size.y));
    }

    RenderBuffer::RenderBuffer(const glm::uvec2 &size, int format) {
        glCreateRenderbuffers(1, &m_Handle);

        glNamedRenderbufferStorage(m_Handle, format, static_cast<int>(size.x), static_cast<int>(size.y));
    }

#pragma clang diagnostic pop

    RenderBuffer::~RenderBuffer() {
        glDeleteRenderbuffers(1, &m_Handle);
    }

    unsigned int RenderBuffer::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int RenderBuffer::getHandle() const noexcept {
        return m_Handle;
    }

    void RenderBuffer::bind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, m_Handle);
    }
}
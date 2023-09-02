#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace kat {
    void ITexture::bindUnit(uint32_t unit) {
        glActiveTexture(GL_TEXTURE0 + unit);
        bind();
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-m_Type-member-init"
    ITexture::ITexture() {
        glGenTextures(1, &m_Handle);
    }

    ITexture::ITexture(unsigned int target) {
        glCreateTextures(target, 1, &m_Handle);
    }
#pragma clang diagnostic pop

    ITexture::~ITexture() {
        glDeleteTextures(1, &m_Handle);
    }

    unsigned int ITexture::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int ITexture::getHandle() const noexcept {
        return m_Handle;
    }

    void ITexture::setFilter(TextureFilter filter) const {
        setFilter(filter, filter);
    }

    void ITexture::setFilter(TextureFilter minFilter, TextureFilter magFilter) const {
        glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, static_cast<int>(minFilter));
        glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, static_cast<int>(magFilter));
    }

    void ITexture::setWrapMode(WrapMode mode) const {
        setWrapMode(mode, mode, mode);
    }

    void ITexture::setWrapMode(WrapMode sMode, WrapMode tMode) const {
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, static_cast<int>(sMode));
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, static_cast<int>(tMode));
    }

    void ITexture::setWrapMode(WrapMode sMode, WrapMode tMode, WrapMode rMode) const {
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, static_cast<int>(sMode));
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, static_cast<int>(tMode));
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_R, static_cast<int>(rMode));
    }

    void ITexture::setBorderColor(const glm::vec4 &color) const {
        glTextureParameterfv(m_Handle, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
    }

    int glInternalFormatOf(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGBA8:
                return GL_RGBA8;
            case TextureFormat::RGBA4:
                return GL_RGBA4;
            case TextureFormat::RGBA32F:
                return GL_RGBA32F;
            case TextureFormat::RGB8:
                return GL_RGB8;
            case TextureFormat::RGB4:
                return GL_RGB4;
            case TextureFormat::RGB32F:
                return GL_RGB32F;
            case TextureFormat::RG8:
                return GL_RG8;
            case TextureFormat::RG32F:
                return GL_RG32F;
            case TextureFormat::R8:
                return GL_R8;
            case TextureFormat::R32F:
                return GL_R32F;
            case TextureFormat::Depth16:
                return GL_DEPTH_COMPONENT16;
            case TextureFormat::Depth24:
                return GL_DEPTH_COMPONENT24;
            case TextureFormat::Depth32:
                return GL_DEPTH_COMPONENT32;
            case TextureFormat::Depth32F:
                return GL_DEPTH_COMPONENT32F;
            case TextureFormat::Stencil:
                return GL_STENCIL_INDEX8;
        }
    }

    unsigned int glFormatOf(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGBA8:
            case TextureFormat::RGBA4:
            case TextureFormat::RGBA32F:
                return GL_RGBA;
            case TextureFormat::RGB8:
            case TextureFormat::RGB4:
            case TextureFormat::RGB32F:
                return GL_RGB;
            case TextureFormat::RG8:
            case TextureFormat::RG32F:
                return GL_RG;
            case TextureFormat::R8:
            case TextureFormat::R32F:
                return GL_RED;
            case TextureFormat::Depth16:
            case TextureFormat::Depth24:
            case TextureFormat::Depth32:
            case TextureFormat::Depth32F:
                return GL_DEPTH_COMPONENT;
            case TextureFormat::Stencil:
                return GL_STENCIL_INDEX;
        }
    }

    TextureFormat formatForChannels(int nc) {
        switch (nc) {
            case 1:
                return TextureFormat::R8;
            case 2:
                return TextureFormat::RG8;
            case 3:
                return TextureFormat::RGB8;
            case 4:
                return TextureFormat::RGBA8;
            default: break;
        }

        return TextureFormat::RGBA8;
    }

    Texture2D::Texture2D(const glm::uvec2 &size, TextureFormat format) : m_Size(size), ITexture(GL_TEXTURE_2D) {
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormatOf(format),
                static_cast<int>(size.x), static_cast<int>(size.y), 0,
                glFormatOf(format), GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture2D::Texture2D(const glm::uvec2 &size, TextureFormat format, const void *data, PixelDataType dataType)
            : m_Size(size), ITexture(GL_TEXTURE_2D) {
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormatOf(format),
                static_cast<int>(size.x), static_cast<int>(size.y), 0,
                glFormatOf(format), static_cast<unsigned int>(dataType), data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::bind() {
        glBindTexture(GL_TEXTURE_2D, m_Handle);
    }

    std::shared_ptr<Texture2D> Texture2D::load(const std::filesystem::path &path) {
        stbi_set_flip_vertically_on_load_thread(true);

        int width, height, nc;

        auto f = path.string();

        unsigned char* data = stbi_load(f.c_str(), &width, &height, &nc, 0);

        auto tex = std::make_shared<Texture2D>(glm::uvec2(width, height), formatForChannels(nc), data, kat::PixelDataType::UnsignedByte);

        stbi_image_free(data);

        return tex;
    }

    std::shared_ptr<Texture2D> Texture2D::load(const std::filesystem::path &path, int desiredChannels) {
        stbi_set_flip_vertically_on_load_thread(true);

        int width, height, nc;

        auto f = path.string();

        unsigned char* data = stbi_load(f.c_str(), &width, &height, &nc, desiredChannels);

        auto tex = std::make_shared<Texture2D>(glm::uvec2(width, height), formatForChannels(desiredChannels), data, kat::PixelDataType::UnsignedByte);

        stbi_image_free(data);

        return tex;
    }

    const glm::uvec2 &Texture2D::getSize() const noexcept {
        return m_Size;
    }

    Texture2D::Region Texture2D::getRegion(glm::uvec2 bottomLeft, glm::uvec2 topRight) {
        return Texture2D::Region(shared_from_this(), bottomLeft, topRight);
    }

    Texture2D::Region Texture2D::getFullRegion() {
        return Texture2D::Region(shared_from_this(), {0, 0}, m_Size);
    }

    std::pair<glm::vec2, glm::vec2> Texture2D::Region::getUVPair() const noexcept {
        return { glm::vec2(bottomLeft) / glm::vec2(texture->getSize()), glm::vec2(topRight) / glm::vec2(texture->getSize()) };
    }
}
#pragma once

#include <filesystem>
#include "kat/engine.hpp"

namespace kat {

    enum class TextureFormat {
        RGBA8, RGBA4, RGBA32F,
        RGB8, RGB4, RGB32F,
        RG8, RG32F,
        R8, R32F,
        Depth16, Depth24, Depth32, Depth32F,
        Depth = Depth32F,
        Stencil
    };

    enum class PixelDataType {
        UnsignedByte = GL_UNSIGNED_BYTE,
        Byte = GL_BYTE,
        UnsignedShort = GL_UNSIGNED_SHORT,
        Short = GL_SHORT,
        UnsignedInt = GL_UNSIGNED_INT,
        Int = GL_INT,
        Float = GL_FLOAT
    };

    template<typename T>
    struct pixel_data_type;

    template<> struct pixel_data_type<unsigned char> { static constexpr PixelDataType type = PixelDataType::UnsignedByte; };
    template<> struct pixel_data_type<char> { static constexpr PixelDataType type = PixelDataType::Byte; };
    template<> struct pixel_data_type<unsigned short> { static constexpr PixelDataType type = PixelDataType::UnsignedShort; };
    template<> struct pixel_data_type<short> { static constexpr PixelDataType type = PixelDataType::Short; };
    template<> struct pixel_data_type<unsigned int> { static constexpr PixelDataType type = PixelDataType::UnsignedInt; };
    template<> struct pixel_data_type<int> { static constexpr PixelDataType type = PixelDataType::Int; };
    template<> struct pixel_data_type<float> { static constexpr PixelDataType type = PixelDataType::Float; };

    template<typename T>
    constexpr PixelDataType pixel_data_type_v = pixel_data_type<T>::type;

    enum class TextureFilter {
        Linear = GL_LINEAR,
        Nearest = GL_NEAREST
    };

    enum class WrapMode {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToBorder = GL_CLAMP_TO_BORDER,
        ClampToEdge = GL_CLAMP_TO_EDGE
    };

    class ITexture {
    public:

        ITexture();
        explicit ITexture(unsigned int target);

        virtual ~ITexture();

        void bind(uint32_t unit);
        virtual void bind() = 0;

        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;

        void setFilter(TextureFilter filter) const;
        void setFilter(TextureFilter minFilter, TextureFilter magFilter) const;

        void setWrapMode(WrapMode mode) const;
        void setWrapMode(WrapMode sMode, WrapMode tMode) const;
        void setWrapMode(WrapMode sMode, WrapMode tMode, WrapMode rMode) const;

        void setBorderColor(const glm::vec4 &color) const;

    protected:
        unsigned int m_Handle;
    };

    class Texture2D : public ITexture {
    public:

        Texture2D(const glm::uvec2& size, TextureFormat format);
        Texture2D(const glm::uvec2& size, TextureFormat format, const void* data, PixelDataType dataType);

        template<typename T>
        static std::shared_ptr<Texture2D> create(const glm::uvec2& size, TextureFormat format, const std::vector<T>& data) {
            return create<T>(size, format, data.data());
        };

        template<typename T>
        static std::shared_ptr<Texture2D> create(const glm::uvec2& size, TextureFormat format, const T* data) {
            return std::make_shared<Texture2D>(size, format, data, pixel_data_type_v<T>);
        };

        static std::shared_ptr<Texture2D> load(const std::filesystem::path& path);
        static std::shared_ptr<Texture2D> load(const std::filesystem::path& path, int desiredChannels);

        void bind() override;
    };
}
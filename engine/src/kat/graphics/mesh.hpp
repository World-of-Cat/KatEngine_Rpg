#pragma once

#include "kat/engine.hpp"

namespace kat {

    enum class BufferUsage {
        StaticDraw = GL_STATIC_DRAW,
        StaticCopy = GL_STATIC_COPY,
        StaticRead = GL_STATIC_READ,

        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicCopy = GL_DYNAMIC_COPY,
        DynamicRead = GL_DYNAMIC_READ,

        StreamDraw = GL_STREAM_DRAW,
        StreamCopy = GL_STREAM_COPY,
        StreamRead = GL_STREAM_READ,
    };

    class Buffer {
    public:

        Buffer();
        explicit Buffer(size_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);

        virtual ~Buffer();

        // Disable copy semantics as they would cause early deletion of resources.
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        virtual void bind() = 0;

        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;

        void data(size_t size, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw) const;

        template<typename T>
        void data(const std::vector<T>& data, BufferUsage usage = BufferUsage::StaticDraw) const {
            data(data.size() * sizeof(T), data.data(), usage);
        }

        void subData(size_t size, const void* data, size_t offset = 0) const;

        template<typename T>
        void subData(const std::vector<T>& data, size_t offset = 0) {
            subData(data.size() * sizeof(T), data.data(), offset);
        }

    protected:
        unsigned int m_Handle;
    };

    class VertexBuffer : public Buffer {
    public:
        VertexBuffer();

        VertexBuffer(size_t size, const void *data, BufferUsage usage);

        void bind() override;
    };

    class IndexBuffer : public Buffer {
    public:

        IndexBuffer();

        IndexBuffer(size_t size, const void *data, BufferUsage usage);

        void bind() override;
    };

    template<typename T>
    concept easy_buffer = requires(size_t size, const void* data, BufferUsage usage) {
        { T(size, data, usage) } -> std::same_as<T>;
    } && std::derived_from<T, Buffer>;

    template<easy_buffer B, typename T>
    std::shared_ptr<B> createBuffer(const std::vector<T>& data, BufferUsage usage = BufferUsage::StaticDraw) {
        return std::make_shared<B>(data.size() * sizeof(T), data.data(), usage);
    }

    struct VertexAttribute {
        size_t size;
        size_t offset;
    };

    enum class PrimitiveMode {
        Points = GL_POINTS,

        Lines = GL_LINES,
        LinesAdjacency = GL_LINES_ADJACENCY,
        LineStrip = GL_LINE_STRIP,
        LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
        LineLoop = GL_LINE_LOOP,

        Triangles = GL_TRIANGLES,
        TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
        TriangleFan = GL_TRIANGLE_FAN,

        Patches = GL_PATCHES
    };


    class VertexArray {
    public:

        VertexArray();
        ~VertexArray();

        // Disable copy semantics as they would cause early deletion of resources.
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;


        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;


        void bindElementBuffer(unsigned int buffer) const;
        void bindElementBuffer(const IndexBuffer* buffer) const;
        void bindElementBuffer(const std::shared_ptr<IndexBuffer>& buffer) const;
        void bindElementBuffer(const std::unique_ptr<IndexBuffer>& buffer) const;
        void bindElementBuffer(const IndexBuffer& buffer) const;

        // These are more specific on vertex format, allowing for custom stride & offsets. requires more effort and caution though.
        void bindVertexBuffer(unsigned int buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t bufferOffset = 0);
        void bindVertexBuffer(const VertexBuffer* buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t bufferOffset = 0);
        void bindVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t bufferOffset = 0);
        void bindVertexBuffer(const std::unique_ptr<VertexBuffer>& buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t bufferOffset = 0);
        void bindVertexBuffer(const VertexBuffer& buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t bufferOffset = 0);

        // These assume a packed vertex format
        void bindVertexBuffer(unsigned int buffer, const std::vector<size_t>& attributes, size_t bufferOffset = 0);
        void bindVertexBuffer(const VertexBuffer* buffer, const std::vector<size_t>& attributes, size_t bufferOffset = 0);
        void bindVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer, const std::vector<size_t>& attributes, size_t bufferOffset = 0);
        void bindVertexBuffer(const std::unique_ptr<VertexBuffer>& buffer, const std::vector<size_t>& attributes, size_t bufferOffset = 0);
        void bindVertexBuffer(const VertexBuffer& buffer, const std::vector<size_t>& attributes, size_t bufferOffset = 0);

        void bind() const;

        void drawElements(PrimitiveMode mode, size_t count, size_t offset = 0) const;
        void drawArrays(PrimitiveMode mode, size_t count, size_t offset = 0) const;

    private:
        unsigned int m_Handle;

        unsigned int m_NextAttrib = 0;
        unsigned int m_NextBinding = 0;
    };

    struct StandardVertex {
        glm::vec3 position;
        glm::vec2 texCoords;
        glm::vec4 tint;
        glm::vec3 normal;

        inline static const std::vector<size_t> ATTRIBUTES = {
                3, 2, 4, 3
        };
    };


    class Mesh {
    public:

        explicit Mesh(const std::vector<StandardVertex>& vertices, PrimitiveMode primitive = PrimitiveMode::Triangles);
        Mesh(const std::vector<StandardVertex>& vertices, const std::vector<unsigned int> &indices, PrimitiveMode primitive = PrimitiveMode::Triangles);
        Mesh(size_t vertexCount, const std::shared_ptr<VertexArray>& vertexArray, const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, size_t offset = 0, PrimitiveMode primitive = PrimitiveMode::Triangles);
        Mesh(size_t indexCount, const std::shared_ptr<VertexArray>& vertexArray, const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<IndexBuffer>& indexBuffer, size_t offset = 0, PrimitiveMode primitive = PrimitiveMode::Triangles);

        ~Mesh();

        void render();
        void render(PrimitiveMode primitiveMode);
        void render(PrimitiveMode primitiveMode, size_t count);
        void render(PrimitiveMode primitiveMode, size_t count, size_t offset);
        void render(size_t count);
        void render(size_t count, size_t offset);


    private:

        std::shared_ptr<VertexArray> m_VertexArray;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        size_t m_Count;
        size_t m_Offset;

        PrimitiveMode m_Primitive;

    };

}
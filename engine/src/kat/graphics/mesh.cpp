#include "mesh.hpp"

namespace kat {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
    Buffer::Buffer() {
        glCreateBuffers(1, &m_Handle);
    }

    Buffer::Buffer(size_t size, const void *data_, BufferUsage usage) {
        glCreateBuffers(1, &m_Handle);
        data(size, data_, usage);
    }
#pragma clang diagnostic pop

    Buffer::~Buffer() {
        glDeleteBuffers(1, &m_Handle);
    }

    unsigned int Buffer::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int Buffer::getHandle() const noexcept {
        return m_Handle;
    }

    void Buffer::data(size_t size, const void *data_, BufferUsage usage) const {
        glNamedBufferData(m_Handle, static_cast<int>(size), data_, static_cast<unsigned int>(usage));
    }

    void Buffer::subData(size_t size, const void *data, size_t offset) const {
        glNamedBufferSubData(m_Handle, static_cast<int>(offset), static_cast<int>(size), data);
    }

    VertexBuffer::VertexBuffer(size_t size, const void *data, BufferUsage usage) : Buffer(size, data, usage) {}

    VertexBuffer::VertexBuffer() = default;

    void VertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
    }

    IndexBuffer::IndexBuffer() = default;

    IndexBuffer::IndexBuffer(size_t size, const void *data, BufferUsage usage) : Buffer(size, data, usage) {}

    void IndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
    VertexArray::VertexArray() {
        glCreateVertexArrays(1, &m_Handle);
    }
#pragma clang diagnostic pop

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_Handle);
    }

    unsigned int VertexArray::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int VertexArray::getHandle() const noexcept {
        return m_Handle;
    }

    void VertexArray::bindElementBuffer(unsigned int buffer) const {
        glVertexArrayElementBuffer(m_Handle, buffer);
    }

    void VertexArray::bindElementBuffer(const IndexBuffer *buffer) const {
        bindElementBuffer(buffer->getHandle());
    }

    void VertexArray::bindElementBuffer(const std::shared_ptr<IndexBuffer> &buffer) const {
        bindElementBuffer(buffer->getHandle());
    }

    void VertexArray::bindElementBuffer(const std::unique_ptr<IndexBuffer> &buffer) const {
        bindElementBuffer(buffer->getHandle());
    }

    void VertexArray::bindElementBuffer(const IndexBuffer &buffer) const {
        bindElementBuffer(buffer.getHandle());
    }

    void
    VertexArray::bindVertexBuffer(unsigned int buffer, const std::vector<VertexAttribute> &attributes, size_t stride, size_t bufferOffset) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        for (const auto& attrib : attributes) {
            glVertexArrayAttribFormat(m_Handle, m_NextAttrib, static_cast<int>(attrib.size), GL_FLOAT, false, attrib.offset);
            glVertexArrayAttribBinding(m_Handle, m_NextAttrib, m_NextBinding);

            glEnableVertexArrayAttrib(m_Handle, m_NextAttrib++);
        }

        glVertexArrayVertexBuffer(m_Handle, m_NextBinding++, buffer, static_cast<int>(bufferOffset), static_cast<int>(stride));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexArray::bindVertexBuffer(const VertexBuffer *buffer, const std::vector<VertexAttribute> &attributes,
                                       size_t stride, size_t bufferOffset) {
        bindVertexBuffer(buffer->getHandle(), attributes, stride, bufferOffset);
    }

    void VertexArray::bindVertexBuffer(const std::shared_ptr<VertexBuffer> &buffer,
                                       const std::vector<VertexAttribute> &attributes, size_t stride, size_t bufferOffset) {
        bindVertexBuffer(buffer->getHandle(), attributes, stride, bufferOffset);
    }

    void VertexArray::bindVertexBuffer(const std::unique_ptr<VertexBuffer> &buffer,
                                       const std::vector<VertexAttribute> &attributes, size_t stride,
                                       size_t bufferOffset) {
        bindVertexBuffer(buffer->getHandle(), attributes, stride, bufferOffset);
    }

    void VertexArray::bindVertexBuffer(const VertexBuffer &buffer, const std::vector<VertexAttribute> &attributes, size_t stride, size_t bufferOffset) {
        bindVertexBuffer(buffer.getHandle(), attributes, stride, bufferOffset);
    }

    void VertexArray::bindVertexBuffer(unsigned int buffer, const std::vector<size_t> &attributes, size_t bufferOffset) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        size_t offset = 0;
        for (size_t attrib : attributes) {
            glVertexArrayAttribFormat(m_Handle, m_NextAttrib, static_cast<int>(attrib), GL_FLOAT, false, offset);
            glVertexArrayAttribBinding(m_Handle, m_NextAttrib, m_NextBinding);

            offset += attrib * sizeof(float);

            glEnableVertexArrayAttrib(m_Handle, m_NextAttrib++);
        }

        glVertexArrayVertexBuffer(m_Handle, m_NextBinding++, buffer, static_cast<int>(bufferOffset), static_cast<int>(offset));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexArray::bindVertexBuffer(const VertexBuffer *buffer, const std::vector<size_t> &attributes, size_t bufferOffset) {
        bindVertexBuffer(buffer->getHandle(), attributes, bufferOffset);
    }

    void
    VertexArray::bindVertexBuffer(const std::shared_ptr<VertexBuffer> &buffer, const std::vector<size_t> &attributes, size_t bufferOffset) {
        bindVertexBuffer(buffer->getHandle(), attributes, bufferOffset);
    }

    void
    VertexArray::bindVertexBuffer(const std::unique_ptr<VertexBuffer> &buffer, const std::vector<size_t> &attributes, size_t bufferOffset) {
        bindVertexBuffer(buffer->getHandle(), attributes, bufferOffset);
    }

    void VertexArray::bindVertexBuffer(const VertexBuffer &buffer, const std::vector<size_t> &attributes, size_t bufferOffset) {
        bindVertexBuffer(buffer.getHandle(), attributes, bufferOffset);
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_Handle);
    }

    void VertexArray::drawElements(PrimitiveMode mode, size_t count, size_t offset) const {
        bind();
        glDrawElements(static_cast<unsigned int>(mode), static_cast<int>(count), GL_UNSIGNED_INT,
                reinterpret_cast<const void*>(offset * sizeof(unsigned int)));
    }

    void VertexArray::drawArrays(PrimitiveMode mode, size_t count, size_t offset) const {
        bind();
        glDrawArrays(static_cast<unsigned int>(mode), static_cast<int>(offset), static_cast<int>(count));
    }

    Mesh::Mesh(const std::vector<StandardVertex> &vertices, PrimitiveMode primitive) : m_Count(vertices.size()),
               m_Primitive(primitive), m_Offset(0) {
        m_VertexBuffers = { createBuffer<VertexBuffer>(vertices) };
        m_VertexArray = std::make_shared<VertexArray>();
        m_VertexArray->bindVertexBuffer(m_VertexBuffers[0], StandardVertex::ATTRIBUTES);
    }

    Mesh::Mesh(const std::vector<StandardVertex> &vertices, const std::vector<unsigned int> &indices,
               PrimitiveMode primitive) : m_Count(indices.size()), m_Primitive(primitive), m_Offset(0) {
        m_VertexBuffers = { createBuffer<VertexBuffer>(vertices) };
        m_IndexBuffer = createBuffer<IndexBuffer>(indices);
        m_VertexArray = std::make_shared<VertexArray>();
        m_VertexArray->bindVertexBuffer(m_VertexBuffers[0], StandardVertex::ATTRIBUTES);
        m_VertexArray->bindElementBuffer(m_IndexBuffer);
    }

    Mesh::Mesh(size_t vertexCount, const std::shared_ptr<VertexArray> &vertexArray,
               const std::vector<std::shared_ptr<VertexBuffer>> &vertexBuffers, size_t offset,
               PrimitiveMode primitive) : m_Count(vertexCount), m_VertexArray(vertexArray),
               m_VertexBuffers(vertexBuffers), m_Offset(offset), m_Primitive(primitive) {

    }

    Mesh::Mesh(size_t indexCount, const std::shared_ptr<VertexArray> &vertexArray,
               const std::vector<std::shared_ptr<VertexBuffer>> &vertexBuffers,
               const std::shared_ptr<IndexBuffer> &indexBuffer, size_t offset, PrimitiveMode primitive)
               : m_Count(indexCount), m_VertexArray(vertexArray), m_VertexBuffers(vertexBuffers),
                 m_IndexBuffer(indexBuffer), m_Offset(offset), m_Primitive(primitive) {

    }

    Mesh::~Mesh() = default;

    void Mesh::render() {
        render(m_Primitive, m_Count, m_Offset);
    }

    void Mesh::render(PrimitiveMode primitiveMode) {
        render(primitiveMode, m_Count, m_Offset);
    }

    void Mesh::render(PrimitiveMode primitiveMode, size_t count) {
        render(primitiveMode, count, m_Offset);
    }

    void Mesh::render(size_t count) {
        render(m_Primitive, count, m_Offset);
    }

    void Mesh::render(size_t count, size_t offset) {
        render(m_Primitive, count, offset);
    }

    void Mesh::render(PrimitiveMode primitiveMode, size_t count, size_t offset) {
        if (m_IndexBuffer) {
            m_VertexArray->drawElements(primitiveMode, count, offset);
        } else {
            m_VertexArray->drawArrays(primitiveMode, count, offset);
        }
    }
}
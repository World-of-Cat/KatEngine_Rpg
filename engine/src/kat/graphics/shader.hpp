#pragma once

#include "kat/engine.hpp"
#include <string>
#include <filesystem>

namespace kat {

    enum class ShaderType {
        Geometry = GL_GEOMETRY_SHADER,
        Vertex = GL_VERTEX_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEvaluation = GL_TESS_EVALUATION_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Compute = GL_COMPUTE_SHADER,
        Unknown
    };

    ShaderType inferShaderType(const std::filesystem::path& path);

    class ShaderModule {
    public:

        static std::shared_ptr<ShaderModule> load(const std::filesystem::path& path); // infers from extension
        static std::shared_ptr<ShaderModule> load(ShaderType type, const std::filesystem::path& path);

        ShaderModule(ShaderType type, const std::string& source);
        ShaderModule(const std::string& source);
        ~ShaderModule();

        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;

        [[nodiscard]] ShaderType getType() const noexcept;

    private:
        ShaderType m_Type;
        unsigned int m_Handle;
    };

    class GraphicsShader {
    public:

        using SDef = std::variant<std::filesystem::path, std::pair<ShaderType, std::filesystem::path>>;
        using SSrcDef = std::variant<std::string, std::pair<ShaderType, std::string>>;

        static std::shared_ptr<GraphicsShader> load(const std::vector<SDef>& paths);

        GraphicsShader(const std::vector<std::shared_ptr<ShaderModule>>& modules);
        GraphicsShader(const std::vector<SSrcDef>& shaders);


        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;

        void bind() const;

        [[nodiscard]] int getUniformLocation(const std::string& name) const;

        void setInteger(const std::string& name, int x) const;
        void setVec2i(const std::string& name, int x, int y) const;
        void setVec3i(const std::string& name, int x, int y, int z) const;
        void setVec4i(const std::string& name, int x, int y, int z, int w) const;
        void setVec2i(const std::string& name, const glm::ivec2& v) const;
        void setVec3i(const std::string& name, const glm::ivec3& v) const;
        void setVec4i(const std::string& name, const glm::ivec4& v) const;

        void setFloat(const std::string& name, float x) const;
        void setVec2f(const std::string& name, float x, float y) const;
        void setVec3f(const std::string& name, float x, float y, float z) const;
        void setVec4f(const std::string& name, float x, float y, float z, float w) const;
        void setVec2f(const std::string& name, const glm::fvec2& v) const;
        void setVec3f(const std::string& name, const glm::fvec3& v) const;
        void setVec4f(const std::string& name, const glm::fvec4& v) const;

        void setUnsignedInt(const std::string& name, unsigned int x) const;
        void setVec2u(const std::string& name, unsigned int x, unsigned int y) const;
        void setVec3u(const std::string& name, unsigned int x, unsigned int y, unsigned int z) const;
        void setVec4u(const std::string& name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const;
        void setVec2u(const std::string& name, const glm::uvec2& v) const;
        void setVec3u(const std::string& name, const glm::uvec3& v) const;
        void setVec4u(const std::string& name, const glm::uvec4& v) const;

        void setMatrix2f(const std::string& name, const glm::mat2& m) const;
        void setMatrix2x2f(const std::string& name, const glm::mat2x2& m) const;
        void setMatrix2x3f(const std::string& name, const glm::mat2x3& m) const;
        void setMatrix2x4f(const std::string& name, const glm::mat2x4& m) const;

        void setMatrix3f(const std::string& name, const glm::mat3& m) const;
        void setMatrix3x2f(const std::string& name, const glm::mat3x2& m) const;
        void setMatrix3x3f(const std::string& name, const glm::mat3x3& m) const;
        void setMatrix3x4f(const std::string& name, const glm::mat3x4& m) const;

        void setMatrix4f(const std::string& name, const glm::mat4& m) const;
        void setMatrix4x2f(const std::string& name, const glm::mat4x2& m) const;
        void setMatrix4x3f(const std::string& name, const glm::mat4x3& m) const;
        void setMatrix4x4f(const std::string& name, const glm::mat4x4& m) const;


    private:

        unsigned int m_Handle;
    };

    class ComputeShader {
    public:

        static std::shared_ptr<ComputeShader> load(const std::filesystem::path& path);

        ComputeShader(const std::shared_ptr<ShaderModule>& module);
        ComputeShader(const std::string& source);

        unsigned int operator*() const noexcept;
        [[nodiscard]] unsigned int getHandle() const noexcept;

        void bind() const;

        void dispatch(unsigned int xGroups, unsigned int yGroups, unsigned int zGroups) const;

        [[nodiscard]] int getUniformLocation(const std::string& name) const;

        void setInteger(const std::string& name, int x) const;
        void setVec2i(const std::string& name, int x, int y) const;
        void setVec3i(const std::string& name, int x, int y, int z) const;
        void setVec4i(const std::string& name, int x, int y, int z, int w) const;
        void setVec2i(const std::string& name, const glm::ivec2& v) const;
        void setVec3i(const std::string& name, const glm::ivec3& v) const;
        void setVec4i(const std::string& name, const glm::ivec4& v) const;

        void setFloat(const std::string& name, float x) const;
        void setVec2f(const std::string& name, float x, float y) const;
        void setVec3f(const std::string& name, float x, float y, float z) const;
        void setVec4f(const std::string& name, float x, float y, float z, float w) const;
        void setVec2f(const std::string& name, const glm::fvec2& v) const;
        void setVec3f(const std::string& name, const glm::fvec3& v) const;
        void setVec4f(const std::string& name, const glm::fvec4& v) const;

        void setUnsignedInt(const std::string& name, unsigned int x) const;
        void setVec2u(const std::string& name, unsigned int x, unsigned int y) const;
        void setVec3u(const std::string& name, unsigned int x, unsigned int y, unsigned int z) const;
        void setVec4u(const std::string& name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const;
        void setVec2u(const std::string& name, const glm::uvec2& v) const;
        void setVec3u(const std::string& name, const glm::uvec3& v) const;
        void setVec4u(const std::string& name, const glm::uvec4& v) const;

        void setMatrix2f(const std::string& name, const glm::mat2& m) const;
        void setMatrix2x2f(const std::string& name, const glm::mat2x2& m) const;
        void setMatrix2x3f(const std::string& name, const glm::mat2x3& m) const;
        void setMatrix2x4f(const std::string& name, const glm::mat2x4& m) const;

        void setMatrix3f(const std::string& name, const glm::mat3& m) const;
        void setMatrix3x2f(const std::string& name, const glm::mat3x2& m) const;
        void setMatrix3x3f(const std::string& name, const glm::mat3x3& m) const;
        void setMatrix3x4f(const std::string& name, const glm::mat3x4& m) const;

        void setMatrix4f(const std::string& name, const glm::mat4& m) const;
        void setMatrix4x2f(const std::string& name, const glm::mat4x2& m) const;
        void setMatrix4x3f(const std::string& name, const glm::mat4x3& m) const;
        void setMatrix4x4f(const std::string& name, const glm::mat4x4& m) const;

    private:

        unsigned int m_Handle;
    };
}
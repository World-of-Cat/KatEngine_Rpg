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
    private:

        unsigned int m_Handle;
    };
}
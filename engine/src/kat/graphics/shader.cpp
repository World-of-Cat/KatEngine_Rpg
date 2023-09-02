#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>
#include "kat/graphics/texture.hpp"

namespace kat {
    ShaderType getTypeFromName(const std::string& name) {
        if (name == "v" || name == "vert" || name == "vertex") {
            return ShaderType::Vertex;
        }

        if (name == "g" || name == "geom" || name == "geometry") {
            return ShaderType::Geometry;
        }

        if (name == "tc" || name == "tesc" || name == "tess-control") {
            return ShaderType::TessControl;
        }

        if (name == "te" || name == "tese" || name == "tess-eval" || name == "tess-evaluation") {
            return ShaderType::TessEvaluation;
        }

        if (name == "f" || name == "frag" || name == "fragment") {
            return ShaderType::Fragment;
        }

        if (name == "c" || name == "comp" || name == "compute") {
            return ShaderType::Compute;
        }

        return ShaderType::Unknown;
    }


    ShaderType inferShaderType(const std::filesystem::path &path) {
        return getTypeFromName(path.extension().string().substr(1));
//        if (path.extension() == "geom") {
//            return ShaderType::Geometry;
//        }
//        if (path.extension() == "vert") {
//            return ShaderType::Vertex;
//        }
//        if (path.extension() == "tesc") {
//            return ShaderType::TessControl;
//        }
//        if (path.extension() == "tese") {
//            return ShaderType::TessEvaluation;
//        }
//        if (path.extension() == "frag") {
//            return ShaderType::Fragment;
//        }
//        if (path.extension() == "comp") {
//            return ShaderType::Compute;
//        }
//        return ShaderType::Unknown;
    }


    ShaderType getTypeFromSource(const std::string& src) {
        auto ss = std::stringstream(src);
        for (std::string line; std::getline(ss, line); ) {
            std::string stripped = util::strip(line);
            if (stripped.starts_with("#type")) {
                std::string ty = stripped.substr(6);
                return getTypeFromName(ty);
            } else if (stripped.starts_with("#version")) {
                return ShaderType::Unknown;
            }
        }

        return ShaderType::Unknown;
    }

    std::shared_ptr<ShaderModule> ShaderModule::load(const std::filesystem::path &path) {
        std::string src = util::readFile(path);
        ShaderType ty = getTypeFromSource(src);
        if (ty == ShaderType::Unknown)
            ty = inferShaderType(path);
        if (ty == ShaderType::Unknown)
            throw std::runtime_error("Failed to infer shader type");

        return std::make_shared<ShaderModule>(ty, src);
    }

    std::shared_ptr<ShaderModule> ShaderModule::load(ShaderType type, const std::filesystem::path &path) {
        return std::make_shared<ShaderModule>(type, util::readFile(path));
    }

    ShaderModule::ShaderModule(ShaderType type, const std::string &source) : m_Type(type) {
        auto i = source.find("#version"); // cut to the start of the source
        std::string src = source.substr(i);

        m_Handle = glCreateShader(static_cast<unsigned int>(m_Type));
        const char* csrc = src.c_str();
        glShaderSource(m_Handle, 1, &csrc, nullptr);
        glCompileShader(m_Handle);

        int status;
        glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &status);

            char* buf = new char[status];
            glGetShaderInfoLog(m_Handle, status, nullptr, buf);

            spdlog::critical("Failed to compile shader: {}", buf);

            delete[] buf;
        }
    }

    ShaderModule::ShaderModule(const std::string &source) {
        ShaderType type = getTypeFromSource(source);
        if (type == ShaderType::Unknown)
            throw std::runtime_error("Failed to infer shader type");

        auto i = source.find("#version"); // cut to the start of the source
        std::string src = source.substr(i);

        m_Handle = glCreateShader(static_cast<unsigned int>(m_Type));
        const char* csrc = src.c_str();
        glShaderSource(m_Handle, 1, &csrc, nullptr);
        glCompileShader(m_Handle);

        int status;
        glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &status);

            char* buf = new char[status];
            glGetShaderInfoLog(m_Handle, status, nullptr, buf);

            spdlog::critical("Failed to compile shader: {}", buf);

            delete[] buf;
        }
    }

    ShaderModule::~ShaderModule() {
        glDeleteShader(m_Handle);
    }

    unsigned int ShaderModule::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int ShaderModule::getHandle() const noexcept {
        return m_Handle;
    }

    ShaderType ShaderModule::getType() const noexcept {
        return m_Type;
    }

    std::shared_ptr<GraphicsShader> GraphicsShader::load(const std::vector<SDef> &paths) {
        std::vector<std::shared_ptr<ShaderModule>> modules;
        for (const auto& sd : paths) {
            if (sd.index() == 0)
                modules.push_back(ShaderModule::load(std::get<0>(sd)));
            else if (sd.index() == 1) {
                auto p = std::get<1>(sd);
                modules.push_back(ShaderModule::load(p.first, p.second));
            }
        }

        return std::make_shared<GraphicsShader>(modules);
    }

    GraphicsShader::GraphicsShader(const std::vector<std::shared_ptr<ShaderModule>> &modules) {
        m_Handle = glCreateProgram();

        for (const auto& m : modules) {
            glAttachShader(m_Handle, m->getHandle());
        }

        glLinkProgram(m_Handle);
        int status;
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &status);
            char* buf = new char[status];
            glGetProgramInfoLog(m_Handle, status, nullptr, buf);

            spdlog::critical("Failed to link shader program: {}", buf);

            delete[] buf;
        }
    }

    GraphicsShader::GraphicsShader(const std::vector<SSrcDef> &shaders) {
        m_Handle = glCreateProgram();

        std::vector<ShaderModule> modules;
        for (const auto& srcdef : shaders) {
            if (srcdef.index() == 0) glAttachShader(m_Handle, modules.emplace_back(std::get<0>(srcdef)).getHandle());
            else if (srcdef.index() == 1) {
                auto sd = get<1>(srcdef);
                glAttachShader(m_Handle, modules.emplace_back(sd.first, sd.second).getHandle());
            }
        }

        glLinkProgram(m_Handle);
        int status;
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &status);
            char* buf = new char[status];
            glGetProgramInfoLog(m_Handle, status, nullptr, buf);

            spdlog::critical("Failed to link shader program: {}", buf);

            delete[] buf;
        }
    }

    unsigned int GraphicsShader::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int GraphicsShader::getHandle() const noexcept {
        return m_Handle;
    }

    void GraphicsShader::bind() const {
        glUseProgram(m_Handle);
    }

    int GraphicsShader::getUniformLocation(const std::string &name) const {
        return glGetUniformLocation(m_Handle, name.c_str());
    }

    void GraphicsShader::setInteger(const std::string &name, int x) const {
        glProgramUniform1i(m_Handle, getUniformLocation(name), x);
    }

    void GraphicsShader::setVec2i(const std::string &name, int x, int y) const {
        glProgramUniform2i(m_Handle, getUniformLocation(name), x, y);
    }

    void GraphicsShader::setVec3i(const std::string &name, int x, int y, int z) const {
        glProgramUniform3i(m_Handle, getUniformLocation(name), x, y, z);
    }

    void GraphicsShader::setVec4i(const std::string &name, int x, int y, int z, int w) const {
        glProgramUniform4i(m_Handle, getUniformLocation(name), x, y, z, w);
    }

    void GraphicsShader::setVec2i(const std::string &name, const glm::ivec2 &v) const {
        glProgramUniform2iv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setVec3i(const std::string &name, const glm::ivec3 &v) const {
        glProgramUniform3iv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setVec4i(const std::string &name, const glm::ivec4 &v) const {
        glProgramUniform4iv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setFloat(const std::string &name, float x) const {
        glProgramUniform1f(m_Handle, getUniformLocation(name), x);
    }

    void GraphicsShader::setVec2f(const std::string &name, float x, float y) const {
        glProgramUniform2f(m_Handle, getUniformLocation(name), x, y);
    }

    void GraphicsShader::setVec3f(const std::string &name, float x, float y, float z) const {
        glProgramUniform3f(m_Handle, getUniformLocation(name), x, y, z);
    }

    void GraphicsShader::setVec4f(const std::string &name, float x, float y, float z, float w) const {
        glProgramUniform4f(m_Handle, getUniformLocation(name), x, y, z, w);
    }

    void GraphicsShader::setVec2f(const std::string &name, const glm::fvec2 &v) const {
        glProgramUniform2fv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setVec3f(const std::string &name, const glm::fvec3 &v) const {
        glProgramUniform3fv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setVec4f(const std::string &name, const glm::fvec4 &v) const {
        glProgramUniform4fv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setUnsignedInt(const std::string &name, unsigned int x) const {
        glProgramUniform1ui(m_Handle, getUniformLocation(name), x);
    }

    void GraphicsShader::setVec2u(const std::string &name, unsigned int x, unsigned int y) const {
        glProgramUniform2ui(m_Handle, getUniformLocation(name), x, y);
    }

    void GraphicsShader::setVec3u(const std::string &name, unsigned int x, unsigned int y, unsigned int z) const {
        glProgramUniform3ui(m_Handle, getUniformLocation(name), x, y, z);
    }

    void GraphicsShader::setVec4u(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const {
        glProgramUniform4ui(m_Handle, getUniformLocation(name), x, y, z, w);
    }

    void GraphicsShader::setVec2u(const std::string &name, const glm::uvec2 &v) const {
        glProgramUniform2uiv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setVec3u(const std::string &name, const glm::uvec3 &v) const {
        glProgramUniform3uiv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void GraphicsShader::setVec4u(const std::string &name, const glm::uvec4 &v) const {
        glProgramUniform4uiv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }


    void GraphicsShader::setMatrix2f(const std::string &name, const glm::mat2 &m) const {
        glProgramUniformMatrix2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix2x2f(const std::string &name, const glm::mat2x2 &m) const {
        glProgramUniformMatrix2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix2x3f(const std::string &name, const glm::mat2x3 &m) const {
        glProgramUniformMatrix2x3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix2x4f(const std::string &name, const glm::mat2x4 &m) const {
        glProgramUniformMatrix2x4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix3f(const std::string &name, const glm::mat3 &m) const {
        glProgramUniformMatrix3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix3x2f(const std::string &name, const glm::mat3x2 &m) const {
        glProgramUniformMatrix3x2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix3x3f(const std::string &name, const glm::mat3x3 &m) const {
        glProgramUniformMatrix3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix3x4f(const std::string &name, const glm::mat3x4 &m) const {
        glProgramUniformMatrix3x4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix4f(const std::string &name, const glm::mat4 &m) const {
        glProgramUniformMatrix4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix4x2f(const std::string &name, const glm::mat4x2 &m) const {
        glProgramUniformMatrix4x2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix4x3f(const std::string &name, const glm::mat4x3 &m) const {
        glProgramUniformMatrix4x3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void GraphicsShader::setMatrix4x4f(const std::string &name, const glm::mat4x4 &m) const {
        glProgramUniformMatrix4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    std::shared_ptr<ComputeShader> ComputeShader::load(const std::filesystem::path& path) {
        return std::make_shared<ComputeShader>(util::readFile(path));
    }

    ComputeShader::ComputeShader(const std::shared_ptr<ShaderModule> &module) {
        assert(module->getType() == ShaderType::Compute);
        m_Handle = glCreateProgram();

        glAttachShader(m_Handle, module->getHandle());

        glLinkProgram(m_Handle);
        int status;
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &status);
            char* buf = new char[status];
            glGetProgramInfoLog(m_Handle, status, nullptr, buf);

            spdlog::critical("Failed to link shader program: {}", buf);

            delete[] buf;
        }
    }

    ComputeShader::ComputeShader(const std::string &source) {
        std::shared_ptr<ShaderModule> module = std::make_shared<ShaderModule>(ShaderType::Compute, source);

        m_Handle = glCreateProgram();

        glAttachShader(m_Handle, module->getHandle());

        glLinkProgram(m_Handle);
        int status;
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &status);
            char* buf = new char[status];
            glGetProgramInfoLog(m_Handle, status, nullptr, buf);

            spdlog::critical("Failed to link shader program: {}", buf);

            delete[] buf;
        }
    }

    unsigned int ComputeShader::operator*() const noexcept {
        return m_Handle;
    }

    unsigned int ComputeShader::getHandle() const noexcept {
        return m_Handle;
    }

    void ComputeShader::bind() const {
        glUseProgram(m_Handle);
    }

    void ComputeShader::dispatch(unsigned int xGroups, unsigned int yGroups, unsigned int zGroups) const {
        bind();
        glDispatchCompute(xGroups, yGroups, zGroups);
    }

    int ComputeShader::getUniformLocation(const std::string &name) const {
        return glGetUniformLocation(m_Handle, name.c_str());
    }

    void ComputeShader::setInteger(const std::string &name, int x) const {
        glProgramUniform1i(m_Handle, getUniformLocation(name), x);
    }

    void ComputeShader::setVec2i(const std::string &name, int x, int y) const {
        glProgramUniform2i(m_Handle, getUniformLocation(name), x, y);
    }

    void ComputeShader::setVec3i(const std::string &name, int x, int y, int z) const {
        glProgramUniform3i(m_Handle, getUniformLocation(name), x, y, z);
    }

    void ComputeShader::setVec4i(const std::string &name, int x, int y, int z, int w) const {
        glProgramUniform4i(m_Handle, getUniformLocation(name), x, y, z, w);
    }

    void ComputeShader::setVec2i(const std::string &name, const glm::ivec2 &v) const {
        glProgramUniform2iv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setVec3i(const std::string &name, const glm::ivec3 &v) const {
        glProgramUniform3iv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setVec4i(const std::string &name, const glm::ivec4 &v) const {
        glProgramUniform4iv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setFloat(const std::string &name, float x) const {
        glProgramUniform1f(m_Handle, getUniformLocation(name), x);
    }

    void ComputeShader::setVec2f(const std::string &name, float x, float y) const {
        glProgramUniform2f(m_Handle, getUniformLocation(name), x, y);
    }

    void ComputeShader::setVec3f(const std::string &name, float x, float y, float z) const {
        glProgramUniform3f(m_Handle, getUniformLocation(name), x, y, z);
    }

    void ComputeShader::setVec4f(const std::string &name, float x, float y, float z, float w) const {
        glProgramUniform4f(m_Handle, getUniformLocation(name), x, y, z, w);
    }

    void ComputeShader::setVec2f(const std::string &name, const glm::fvec2 &v) const {
        glProgramUniform2fv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setVec3f(const std::string &name, const glm::fvec3 &v) const {
        glProgramUniform3fv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setVec4f(const std::string &name, const glm::fvec4 &v) const {
        glProgramUniform4fv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setUnsignedInt(const std::string &name, unsigned int x) const {
        glProgramUniform1ui(m_Handle, getUniformLocation(name), x);
    }

    void ComputeShader::setVec2u(const std::string &name, unsigned int x, unsigned int y) const {
        glProgramUniform2ui(m_Handle, getUniformLocation(name), x, y);
    }

    void ComputeShader::setVec3u(const std::string &name, unsigned int x, unsigned int y, unsigned int z) const {
        glProgramUniform3ui(m_Handle, getUniformLocation(name), x, y, z);
    }

    void ComputeShader::setVec4u(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const {
        glProgramUniform4ui(m_Handle, getUniformLocation(name), x, y, z, w);
    }

    void ComputeShader::setVec2u(const std::string &name, const glm::uvec2 &v) const {
        glProgramUniform2uiv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setVec3u(const std::string &name, const glm::uvec3 &v) const {
        glProgramUniform3uiv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }

    void ComputeShader::setVec4u(const std::string &name, const glm::uvec4 &v) const {
        glProgramUniform4uiv(m_Handle, getUniformLocation(name), 1, glm::value_ptr(v));
    }


    void ComputeShader::setMatrix2f(const std::string &name, const glm::mat2 &m) const {
        glProgramUniformMatrix2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix2x2f(const std::string &name, const glm::mat2x2 &m) const {
        glProgramUniformMatrix2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix2x3f(const std::string &name, const glm::mat2x3 &m) const {
        glProgramUniformMatrix2x3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix2x4f(const std::string &name, const glm::mat2x4 &m) const {
        glProgramUniformMatrix2x4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix3f(const std::string &name, const glm::mat3 &m) const {
        glProgramUniformMatrix3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix3x2f(const std::string &name, const glm::mat3x2 &m) const {
        glProgramUniformMatrix3x2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix3x3f(const std::string &name, const glm::mat3x3 &m) const {
        glProgramUniformMatrix3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix3x4f(const std::string &name, const glm::mat3x4 &m) const {
        glProgramUniformMatrix3x4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix4f(const std::string &name, const glm::mat4 &m) const {
        glProgramUniformMatrix4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix4x2f(const std::string &name, const glm::mat4x2 &m) const {
        glProgramUniformMatrix4x2fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix4x3f(const std::string &name, const glm::mat4x3 &m) const {
        glProgramUniformMatrix4x3fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    void ComputeShader::setMatrix4x4f(const std::string &name, const glm::mat4x4 &m) const {
        glProgramUniformMatrix4fv(m_Handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
    }

    // Texture extension
    void
    GraphicsShader::bindTexture(const std::string &name, int unit, const std::shared_ptr<Texture2D> &texture) {
        texture->bindUnit(0);
        setInteger(name, unit);
    }

    void
    ComputeShader::bindTexture(const std::string &name, int unit, const std::shared_ptr<Texture2D> &texture) {
        texture->bindUnit(0);
        setInteger(name, unit);
    }

}
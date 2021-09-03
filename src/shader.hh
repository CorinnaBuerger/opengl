#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "common.hh"
#include "logger.hh"

class Shader {
public:
    unsigned int id = 0;
    Logger logger {};

    // Constructor reads the shader definition  and builds the shaders
    // and shader program.
    Shader(const char* vertex_path, const char* fragment_path)
    {
        // 1. Retrieve source code of vertex/fragment shader from file.
        std::string vertex_code, fragment_code;
        std::ifstream v_shader_file, f_shader_file;

        // check for exceptions in shader files
        v_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open files
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);
            std::stringstream v_shader_stream, f_shader_stream;
            // read file's buffer contents into streams
            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();
            // close file handlers
            v_shader_file.close();
            f_shader_file.close();
            // convert stream into string
            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();
        } catch(std::ifstream::failure &e) {
            this->logger.panic("reading of shader files was unsuccessful");
        }

        const char* v_shader_code = vertex_code.c_str();
        const char* f_shader_code = fragment_code.c_str();

        // 2. Compile Shaders.
        unsigned int vertex, fragment;
        int success;
        char info_log[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, info_log);
            this->logger.panic("compilation of vertex shader failed: ",
                               info_log);
        }

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, info_log);
            this->logger.panic("compilation of fragment shader failed: ",
                               info_log);
        }

        // shader program
        this->id = glCreateProgram();
        glAttachShader(this->id, vertex);
        glAttachShader(this->id, fragment);
        glLinkProgram(this->id);
        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->id, 512, NULL, info_log);
            this->logger.panic("linking of shader program failed: ", info_log);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    void use(void)
    {
        assert(this->id > 0);
        glUseProgram(this->id);
    }

    void set_uniform(std::string_view name, bool value) const
    {
        GLint loc = glGetUniformLocation(this->id, name.data());
        if (loc < 0) this->logger.warn("a uniform with name `", name,
                                        "' doesn't exist");
        else glUniform1i(loc, static_cast<int>(value));
    }

    void set_uniform(std::string_view name, int value) const
    {
        GLint loc = glGetUniformLocation(this->id, name.data());
        if (loc < 0) this->logger.warn("a uniform with name`", name,
                                       "' doesn't exist");
        else glUniform1i(loc, value);
    }

    void set_uniform(std::string_view name, float value) const
    {
        GLint loc = glGetUniformLocation(this->id, name.data());
        if (loc < 0) this->logger.warn("a uniform with name `", name,
                                       "' doesn't exist");
        else glUniform1f(loc, value);
    }

    void set_uniform(std::string_view name, const glm::vec3& vec) const
    {
        GLint loc = glGetUniformLocation(this->id, name.data());
        if (loc < 0) this->logger.warn("a uniform with name `", name,
                                       "' doesn't exist");
        else glUniform3f(loc, vec.x, vec.y, vec.z);
    }

    void set_uniform(std::string_view name, const glm::mat4& mat) const
    {
        int loc { glGetUniformLocation(this->id, name.data()) };
        if (loc < 0) this->logger.warn("a uniform with name `", name,
                                       "' doesn't exist");
        else glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }
};

#endif /* _SHADER_H_ */

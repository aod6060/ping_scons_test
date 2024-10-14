#include "glm/ext/matrix_clip_space.hpp"
#include "sys.h"


namespace engine {

    void Render::init(Context* context) {
        this->context = context;

        glDisable(GL_DEPTH_TEST);

        // Shader
        this->vertex_shader = this->createShader(GL_VERTEX_SHADER, "data/shaders/main.vert.glsl");
        this->frag_shader = this->createShader(GL_FRAGMENT_SHADER, "data/shaders/main.frag.glsl");

        // Program
        this->program = this->createProgram({this->vertex_shader, this->frag_shader});

        // VertexArray
        glGenVertexArrays(1, &this->vertex_array);

        // Setup Program
        glUseProgram(this->program);
        // Uniform
        this->u_proj = glGetUniformLocation(this->program, "proj");
        this->u_view = glGetUniformLocation(this->program, "view");
        this->u_model = glGetUniformLocation(this->program, "model");

        // Setup VertexArray
        glBindVertexArray(this->vertex_array);
        glEnableVertexAttribArray(this->A_VERTICES);
        glBindVertexArray(0);

        glUseProgram(0);

        // Creating Default Vertices Buffer
        glGenBuffers(1, &this->vertices_buffer);

        this->vertices_list.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        this->vertices_list.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        this->vertices_list.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        this->vertices_list.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

        glBindBuffer(GL_ARRAY_BUFFER, this->vertices_buffer);
        glBufferData(GL_ARRAY_BUFFER, this->vertices_list.size() * sizeof(glm::vec3), this->vertices_list.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Creating Default Indencies Buffer
        glGenBuffers(1, &this->indencies_buffer);

        this->indencies_list.push_back(0);
        this->indencies_list.push_back(1);
        this->indencies_list.push_back(2);

        this->indencies_list.push_back(2);
        this->indencies_list.push_back(1);
        this->indencies_list.push_back(3);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indencies_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indencies_list.size() * sizeof(uint32_t), this->indencies_list.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Render::release() {
        // Indencies Buffer
        glDeleteBuffers(1, &this->indencies_buffer);
        // Vertices Buffer
        glDeleteBuffers(1, &this->vertices_buffer);
        // Vertex Array
        glDeleteVertexArrays(1, &this->vertex_array);

        // Program
        this->deleteProgram(this->program, {this->vertex_shader, this->frag_shader});
        // Shaders
        glDeleteShader(this->vertex_shader);
        glDeleteShader(this->frag_shader);

        this->context = nullptr;
    }


    void Render::clear(const glm::vec4& color) {
        // This will always be 640x480 because the game will 
        glViewport(0, 0, this->getWidth(), this->getHeight());
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Render::startFrame() {
        glUseProgram(this->program);
        // Add stuff for post processing later

        this->setProjection(glm::ortho(0.0f, (float)this->width, (float)this->height, 0.0f));

    }

    void Render::endFrame() {
        // Add stuff for post process
        glUseProgram(0);
        // Render Framebuffer will add this later one
    }

    void Render::setProjection(const glm::mat4& proj) {
        glUniformMatrix4fv(this->u_proj, 1, GL_FALSE, &proj[0][0]);
    }

    void Render::setView(const glm::mat4& view) {
        glUniformMatrix4fv(this->u_view, 1, GL_FALSE, &view[0][0]);
    }

    void Render::setModel(const glm::mat4& model) {
        glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &model[0][0]);
    }

    void Render::draw() {
        glBindVertexArray(this->vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, this->vertices_buffer);
        glVertexAttribPointer(this->A_VERTICES, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indencies_buffer);
        glDrawElements(GL_TRIANGLES, this->indencies_list.size(), GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    uint32_t Render::getWidth() {
        return this->width;
    }

    uint32_t Render::getHeight() {
        return this->height;
    }

    uint32_t Render::createShader(GLenum type, std::string path) {
        uint32_t temp = glCreateShader(type);

        // Load shader from file
        std::ifstream in(path);
        std::string contents;

        if(in.is_open()) {
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read((char*)contents.data(), contents.size());
            in.close();
        }

        std::cout << contents << "\n";

        const char* c_src = contents.c_str();

        glShaderSource(temp, 1, &c_src, nullptr);

        glCompileShader(temp);

        // Check for errors
        int32_t len = 0;
        glGetShaderiv(temp, GL_INFO_LOG_LENGTH, &len);

        if(len > 0) {
            std::string log;
            log.resize(len);
            glGetShaderInfoLog(temp, log.size(), nullptr, (char*)log.data());
            std::cout << log << "\n";
            log.clear();
        }

        return temp;
    }

    // Create and Destroy Program
    uint32_t Render::createProgram(const std::vector<uint32_t>& shaders) {
        uint32_t temp = glCreateProgram();

        std::for_each(shaders.begin(), shaders.end(), [&](uint32_t shader) {
            glAttachShader(temp, shader);
        });

        glLinkProgram(temp);

        int32_t len = 0;

        glGetProgramiv(temp, GL_INFO_LOG_LENGTH, &len);

        if(len > 0) {
            std::string log;
            log.resize(len);
            //glGetShaderInfoLog(temp, log.size(), nullptr, (char*)log.data());
            glGetProgramInfoLog(temp, log.size(), nullptr, (char*)log.data());
            std::cout << log << "\n";
            log.clear();
        }

        return temp;
    }

    void Render::deleteProgram(uint32_t id, const std::vector<uint32_t>& shaders) {
        std::for_each(shaders.begin(), shaders.end(), [&](uint32_t shader) {
            glDetachShader(id, shader);
        });
        glDeleteProgram(id);
    }

}
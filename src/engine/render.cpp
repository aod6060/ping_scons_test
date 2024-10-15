#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
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
        glEnableVertexAttribArray(this->A_TEXCOORDS);
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

        /// Creating Default TexCoord Buffer
        glGenBuffers(1, &this->texCoord_buffer);

        this->texCoord_list.push_back(glm::vec2(0.0f, 0.0f));
        this->texCoord_list.push_back(glm::vec2(1.0f, 0.0f));
        this->texCoord_list.push_back(glm::vec2(0.0f, 1.0f));
        this->texCoord_list.push_back(glm::vec2(1.0f, 1.0f));

        glBindBuffer(GL_ARRAY_BUFFER, this->texCoord_buffer);
        glBufferData(GL_ARRAY_BUFFER, this->texCoord_list.size() * sizeof(glm::vec2), this->texCoord_list.data(), GL_DYNAMIC_DRAW);
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

        // Framebuffer Section
        glGenFramebuffers(1, &this->screen_framebuffer);

        // Texture Section
        glGenTextures(1, &this->screen_texture);

        // Setup Texture
        glBindTexture(GL_TEXTURE_2D, this->screen_texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            this->width,
            this->height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Setup Framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, this->screen_framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screen_texture, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer: wasn't created correctly!\n";
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        this->scanlinePost.init(this);
        this->blurPost.init(this);
    }

    void Render::release() {
        this->blurPost.release();
        this->scanlinePost.release();

        // Texture Section
        glDeleteTextures(1, &this->screen_texture);
        // Framebuffer Section
        glDeleteFramebuffers(1, &this->screen_framebuffer);
        // Indencies Buffer
        glDeleteBuffers(1, &this->indencies_buffer);
        // TexCoords Buffer
        glDeleteBuffers(1, &this->texCoord_buffer);
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

    void Render::startFrame() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->screen_framebuffer);
        glViewport(0, 0, this->getWidth(), this->getHeight());
    }

    void Render::endFrame() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // Handle Scanline Drawing
        this->scanlinePost.bindFramebuffer();

        this->clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        this->scanlinePost.bind();

        this->scanlinePost.setProjection(glm::ortho(0.0f, (float)this->getWidth(), 0.0f, (float)this->getHeight()));
        this->scanlinePost.setView(glm::mat4(1.0f));
        this->scanlinePost.setModel(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3((float)this->getWidth(), (float)this->getHeight(), 0.0f))
        );

        this->scanlinePost.setScreenWidth(this->getWidth());
        this->scanlinePost.setScreenHeight(this->getHeight());
        
        glBindTexture(GL_TEXTURE_2D, this->screen_texture);
        this->scanlinePost.draw();
        glBindTexture(GL_TEXTURE_2D, 0);

        this->scanlinePost.unbind();

        this->scanlinePost.unbindFramebuffer();

        // Handle Blur Drawing
        this->clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        
        this->blurPost.bind();
        this->blurPost.setProjection(glm::ortho(0.0f, (float)this->getWidth(), 0.0f, (float)this->getHeight()));
        this->blurPost.setView(glm::mat4(1.0f));
        this->blurPost.setModel(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3((float)this->getWidth(), (float)this->getHeight(), 0.0f))
        );

        this->blurPost.setScreenWidth(this->getWidth());
        this->blurPost.setScreenHeight(this->getHeight());
        this->blurPost.setBlurAmount(4.0f);

        glBindTexture(GL_TEXTURE_2D, this->scanlinePost.screen_texture);
        this->blurPost.draw();
        glBindTexture(GL_TEXTURE_2D, 0);

        this->blurPost.unbind();        
    }

    void Render::clear(const glm::vec4& color) {
        // This will always be 640x480 because the game will 
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Render::bindShader() {
        glUseProgram(this->program);
        // Add stuff for post processing later
        this->setProjection(glm::ortho(0.0f, (float)this->width, (float)this->height, 0.0f));
    }

    void Render::unbindShader() {
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
        glBindBuffer(GL_ARRAY_BUFFER, this->texCoord_buffer);
        glVertexAttribPointer(this->A_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
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


    // ScanlineProprocess
    void Render::ScanlinePostprocess::init(Render* render) {
        this->render = render;

        // Shaders
        this->vertex_shader = this->render->createShader(GL_VERTEX_SHADER, "data/shaders/postprocessing/scanline.vert.glsl");
        this->fragment_shader = this->render->createShader(GL_FRAGMENT_SHADER, "data/shaders/postprocessing/scanline.frag.glsl");

        // Program
        this->program = this->render->createProgram({this->vertex_shader, this->fragment_shader});

        // Vertex Array
        glGenVertexArrays(1, &this->vertex_array);

        // Setup Program
        glUseProgram(this->program);
        
        // Uniform
        this->u_proj = glGetUniformLocation(this->program, "proj");
        this->u_view = glGetUniformLocation(this->program, "view");
        this->u_model = glGetUniformLocation(this->program, "model");
        this->u_tex0 = glGetUniformLocation(this->program, "tex0");
        this->u_width = glGetUniformLocation(this->program, "width");
        this->u_height = glGetUniformLocation(this->program, "height");
        //this->u_toggle = glGetUniformLocation(this->program, "toggle");

        glUniform1i(this->u_tex0, 0);

        // Attributes
        glBindVertexArray(this->vertex_array);
        glEnableVertexAttribArray(this->A_VERTICES);
        glEnableVertexAttribArray(this->A_TEXCOORDS);
        glBindVertexArray(0);

        glUseProgram(0);


        // Framebuffer Init
        glGenFramebuffers(1, &this->screen_framebuffer);

        // Texture2D
        glGenTextures(1, &this->screen_texture);

        // Setup Texture2D
        glBindTexture(GL_TEXTURE_2D, this->screen_texture);
                glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            this->render->width,
            this->render->height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Setup Framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, this->screen_framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screen_texture, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer: wasn't created correctly!\n";
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


    }

    void Render::ScanlinePostprocess::release() {
        // Texture2D
        glDeleteTextures(1, &this->screen_texture);

        // Framebuffer
        glDeleteFramebuffers(1, &this->screen_framebuffer);
        // Vertex Array
        glDeleteVertexArrays(1, &this->vertex_array);

        // Program
        this->render->deleteProgram(this->program, {this->vertex_shader, this->fragment_shader});

        // Shaders
        glDeleteShader(this->vertex_shader);
        glDeleteShader(this->fragment_shader);

        this->render = nullptr;
    }

    void Render::ScanlinePostprocess::bind() {
        glUseProgram(this->program);
    }

    void Render::ScanlinePostprocess::unbind() {
        glUseProgram(0);
    }

    void Render::ScanlinePostprocess::setProjection(const glm::mat4& proj) {
        glUniformMatrix4fv(u_proj, 1, GL_FALSE, &proj[0][0]);
    }

    void Render::ScanlinePostprocess::setView(const glm::mat4& view) {
        glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
    }

    void Render::ScanlinePostprocess::setModel(const glm::mat4& model) {
        glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
    }

    void Render::ScanlinePostprocess::draw() {
        glBindVertexArray(this->vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, this->render->vertices_buffer);
        glVertexAttribPointer(this->A_VERTICES, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, this->render->texCoord_buffer);
        glVertexAttribPointer(this->A_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->render->indencies_buffer);
        glDrawElements(GL_TRIANGLES, this->render->indencies_list.size(), GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void Render::ScanlinePostprocess::setScreenHeight(float height) {
        glUniform1f(this->u_height, height);
    }

    void Render::ScanlinePostprocess::setScreenWidth(float width) {
        glUniform1f(this->u_width, width);
    }

    void Render::ScanlinePostprocess::bindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->screen_framebuffer);
    }

    void Render::ScanlinePostprocess::unbindFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // BlurPostprocess
    void Render::BlurPostprocess::init(Render* render) {
        this->render = render;

        // Shader
        this->vertex_shader = this->render->createShader(GL_VERTEX_SHADER, "data/shaders/postprocessing/blur.vert.glsl");
        this->fragment_shader = this->render->createShader(GL_FRAGMENT_SHADER, "data/shaders/postprocessing/blur.frag.glsl");

        // Program
        this->program = this->render->createProgram({this->vertex_shader, this->fragment_shader});

        // Vertex Array
        glGenVertexArrays(1, &this->vertex_array);

        // Setup Program
        glUseProgram(this->program);
        // Uniform
        this->u_proj = glGetUniformLocation(this->program, "proj");
        this->u_view = glGetUniformLocation(this->program, "view");
        this->u_model = glGetUniformLocation(this->program, "model");
        this->u_tex0 = glGetUniformLocation(this->program, "tex0");
        glUniform1f(this->u_tex0, 0);
        this->u_width = glGetUniformLocation(this->program, "width");
        this->u_height = glGetUniformLocation(this->program, "height");
        this->u_blur_amount = glGetUniformLocation(this->program, "blurAmount");
        this->setBlurAmount(1.0f);

        // Attributes
        glBindVertexArray(this->vertex_array);
        glEnableVertexAttribArray(this->A_VERTICES);
        glEnableVertexAttribArray(this->A_TEXCOORDS);
        glUseProgram(0);
    }

    void Render::BlurPostprocess::release() {

        // Vertex Array
        glDeleteVertexArrays(1, &this->vertex_array);
        // Program
        this->render->deleteProgram(this->program, {this->vertex_shader, this->fragment_shader});
        // Shader
        glDeleteShader(this->vertex_shader);
        glDeleteShader(this->fragment_shader);
    }

    void Render::BlurPostprocess::bind() {
        glUseProgram(this->program);
    }

    void Render::BlurPostprocess::unbind() {
        glUseProgram(0);
    }


    void Render::BlurPostprocess::setProjection(const glm::mat4& m) {
        glUniformMatrix4fv(this->u_proj, 1, GL_FALSE, &m[0][0]);
    }

    void Render::BlurPostprocess::setView(const glm::mat4& m) {
        glUniformMatrix4fv(this->u_view, 1, GL_FALSE, &m[0][0]);
    }

    void Render::BlurPostprocess::setModel(const glm::mat4& m) {
        glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &m[0][0]);
    }

    void Render::BlurPostprocess::setScreenWidth(float width) {
        glUniform1f(this->u_width, width);
    }

    void Render::BlurPostprocess::setScreenHeight(float height) {
        glUniform1f(this->u_height, height);
    }

    void Render::BlurPostprocess::setBlurAmount(float blurAmount) {
        glUniform1f(this->u_blur_amount, blurAmount);
    }

    void Render::BlurPostprocess::draw() {
        glBindVertexArray(this->vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, this->render->vertices_buffer);
        glVertexAttribPointer(this->A_VERTICES, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, this->render->texCoord_buffer);
        glVertexAttribPointer(this->A_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->render->indencies_buffer);
        glDrawElements(GL_TRIANGLES, this->render->indencies_list.size(), GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

}
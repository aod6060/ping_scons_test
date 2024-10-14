#include "SDL_image.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include "sys.h"


namespace engine {
    // Context
    void Context::init() {

        std::cout << VERSION_FULL_NAME << "\n";
        
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        this->window = SDL_CreateWindow(
            this->config->caption.c_str(), 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            this->config->width, 
            this->config->height, 
            SDL_WINDOW_OPENGL);
        this->gl_context = SDL_GL_CreateContext(this->window);
        glewInit();

        // Setup Icon
        SDL_Surface* icon = IMG_Load("data/icon/icon_32.png");
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);

        this->input.init();
        this->render.init(this);

        if(this->config->callbacks) {
            this->config->callbacks->init(this);
        }
    }

    void Context::update() {
        this->pre_time = SDL_GetTicks();

        while(this->isRunning) {
            // Calculate Delta
            this->current_time = SDL_GetTicks();
            this->delta = (this->current_time - this->pre_time) / 1000.0f;
            this->pre_time = this->current_time;

            // Handle Event
            while(SDL_PollEvent(&this->event)) {
                if(this->event.type == SDL_QUIT) {
                    this->exit();
                }

                this->input.handleEvent(this);

                if(this->config->callbacks) {
                    this->config->callbacks->handleEvents(this);
                }
            }

            if(this->config->callbacks) {
                // Update Method
                this->config->callbacks->update(this);
                // Render Method
                this->config->callbacks->render(this);
            }

            this->input.update(this);

            SDL_GL_SwapWindow(this->window);
        }
    }

    void Context::release() {
        if(this->config->callbacks) {
            this->config->callbacks->release(this);
        }

        this->render.release();
        this->input.release();
        
        SDL_GL_DeleteContext(this->gl_context);
        SDL_DestroyWindow(this->window);
    }
    
    std::string Context::getCaption() {
        return this->config->caption;
    }

    uint32_t Context::getWidth() {
        return this->config->height;
    }

    uint32_t Context::getHeight() {
        return this->config->width;
    }

    float Context::getDelta() {
        return this->delta;
    }

    void Context::exit() {
        this->isRunning = false;
    }

    // Main
    void Main::init() {
        context.init();
    }

    void Main::update() {
        context.update();
    }

    void Main::release() {
        context.release();
    }

    void Main::setConfig(Config* config) {
        this->context.config = config;
    }

}
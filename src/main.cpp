#include "game.h"
#include "sys.h"


int main(int argc, char** argv) {
    game::GameApplication app;

    engine::Config config;

    config.caption = "Ping";
    config.width = 640;
    config.height = 480;
    config.callbacks = &app;

    engine::Main m;
    m.setConfig(&config);
    m.init();
    m.update();
    m.release();

    return 0;
}
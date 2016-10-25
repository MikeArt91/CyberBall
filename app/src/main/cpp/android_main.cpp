#include "Core/NativeEngine.hpp"

extern "C" {
void android_main(struct android_app* state);
};

void android_main(struct android_app* app) {
    app_dummy(); // Make sure glue isn't stripped.
    NativeEngine *engine = new NativeEngine(app);
    engine->GameLoop();
    delete engine;
}


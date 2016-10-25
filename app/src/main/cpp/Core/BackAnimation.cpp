#include "BackAnimation.hpp"
#include "Util.hpp"

void RenderBackgroundAnimation(ShapeRenderer *r) {
    float aspect = SceneManager::GetInstance()->GetScreenAspect();
    static const int BG_RECTS = 50;
    static const float RECT_W = 0.1f;
    static const float RECT_H = 0.1f;
    static float rectX[BG_RECTS];
    static float rectY[BG_RECTS];
    static bool rectsInitted = false;
    int i;

    if (!rectsInitted) {
        for (i = 0; i < BG_RECTS; i++) {
            rectX[i] = aspect * (Random(100) / 100.0f);
            rectY[i] = Random(100) / 100.0f;
        }
        rectsInitted = true;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    for (i = 0; i < BG_RECTS; i++) {
        // setting shades of blue
        float c = 0.1f + 0.1f * (i % 4);
        r->SetColor(c,c,c*2);

        r->RenderRect(rectX[i], rectY[i], RECT_W, RECT_H);

        rectY[i] -= (0.005f + 0.005f * (i % 4));
        if (rectY[i] < -RECT_H * 0.5f) {
            rectY[i] = 1.0f + RECT_H * 0.5f;
            rectX[i] = Random(100) / 100.0f;
        }
    }
}

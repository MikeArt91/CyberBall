#ifndef CYBERBALL_TEXTRENDERER_HPP
#define CYBERBALL_TEXTRENDERER_HPP

#include "Core/Engine.hpp"

/* Renders text to the screen. Uses the "normalized 2D coordinate system" as
 * described in the README. */
class TextRenderer {
private:
    static const int CHAR_CODES = 128;
    SimpleGeom* mCharGeom[CHAR_CODES];
    TrivialShader *mTrivialShader;

    float mFontScale;
    float mColor[3];
    glm::mat4 mMatrix;

public:
    TextRenderer(TrivialShader *t);
    ~TextRenderer();

    TextRenderer* SetMatrix(glm::mat4 mat);
    TextRenderer* SetFontScale(float size);
    TextRenderer* RenderText(const char *str, float centerX, float centerY);
    void SetColor(float r, float g, float b) {
        mColor[0] = r, mColor[1] = g, mColor[2] = b;
    }
    void SetColor(const float *c) {
        mColor[0] = c[0], mColor[1] = c[1], mColor[2] = c[2];
    }
    void ResetColor() {
        SetColor(1.0f, 1.0f, 1.0f);
    }

    TextRenderer* ResetMatrix() {
        return SetMatrix(glm::mat4(1.0f));
    }

    static void MeasureText(const char *str, float fontScale,
                            float *outWidth, float *outHeight);

    static float MeasureTextWidth(const char *str, float fontScale) {
        float w;
        TextRenderer::MeasureText(str, fontScale, &w, NULL);
        return w;
    }

    static float MeasureTextHeight(const char *str, float fontScale) {
        float h;
        TextRenderer::MeasureText(str, fontScale, NULL, &h);
        return h;
    }
};

#endif //CYBERBALL_TEXTRENDERER_HPP

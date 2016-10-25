#include "ShapeRenderer.hpp"
#include "Core/Util.hpp"

// geometry
static GLfloat RECT_VERTICES[] = {
        //  x      y     z      r     g     b     a
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  // vertex 0
        0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  // vertex 1
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  // vertex 2
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f   // vertex 3
};

// indices
static GLushort RECT_INDICES[] = { 0, 1, 2, 0, 2, 3 };

ShapeRenderer::ShapeRenderer(TrivialShader *ts) {
    mTrivialShader = ts;
    mColor[0] = mColor[1] = mColor[2] = 1.0f;
    mGeom = NULL;

    // create geometry
    VertexBuf *vbuf = new VertexBuf(RECT_VERTICES, sizeof(RECT_VERTICES), 7 * sizeof(GLfloat));
    vbuf->SetColorsOffset(3 * sizeof(GLfloat));
    IndexBuf *ibuf = new IndexBuf(RECT_INDICES, sizeof(RECT_INDICES));
    mGeom = new SimpleGeom(vbuf, ibuf);
}

ShapeRenderer::~ShapeRenderer() {
    // destroy geometry
    CleanUp(&mGeom);
}

void ShapeRenderer::RenderRect(float centerX, float centerY, float width, float height) {
    float aspect = SceneManager::GetInstance()->GetScreenAspect();
    glm::mat4 orthoMat = glm::ortho(0.0f, aspect, 0.0f, 1.0f);
    glm::mat4 modelMat, mat;
    modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(centerX, centerY, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(width, height, 1.0f));
    mat = orthoMat * modelMat;
    mTrivialShader->SetTintColor(mColor[0], mColor[1], mColor[2]);
    mTrivialShader->RenderSimpleGeom(&mat, mGeom);
}
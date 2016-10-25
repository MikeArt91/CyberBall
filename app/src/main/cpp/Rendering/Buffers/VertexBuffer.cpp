#include "VertexBuffer.hpp"

VertexBuf::VertexBuf(GLfloat *geomData, int dataSize, int stride) {
    MY_ASSERT(dataSize % stride == 0);

    mPrimitive = GL_TRIANGLES;
    mVbo = 0;
    mStride = stride;
    mColorsOffset = mTexCoordsOffset = 0;
    mCount = dataSize / stride;

    // build VBO
    glGenBuffers(1, &mVbo);
    BindBuffer();
    glBufferData(GL_ARRAY_BUFFER, dataSize, geomData, GL_STATIC_DRAW);
    UnbindBuffer();
}

void VertexBuf::BindBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
}

void VertexBuf::UnbindBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuf::~VertexBuf() {
    glDeleteBuffers(1, &mVbo);
    mVbo = 0;
}
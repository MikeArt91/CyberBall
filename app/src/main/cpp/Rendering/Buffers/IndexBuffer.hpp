#ifndef CYBERBALL_INDEXBUFFER_HPP
#define CYBERBALL_INDEXBUFFER_HPP

#include "Core/Log.hpp"

/* Represents an index buffer (IBO). */
class IndexBuf {
public:
    IndexBuf(GLushort *data, int dataSizeBytes);
    ~IndexBuf();

    void BindBuffer();
    void UnbindBuffer();
    int GetCount() { return mCount; }

private:
    GLuint mIbo;
    int mCount;
};

#endif //CYBERBALL_INDEXBUFFER_HPP

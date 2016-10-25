#ifndef CYBERBALL_SIMPLEGEOM_HPP
#define CYBERBALL_SIMPLEGEOM_HPP

#include "Rendering/Buffers/IndexBuffer.hpp"
#include "Rendering/Buffers/VertexBuffer.hpp"

// Convenience class that represents a geometry in terms of a
// vertex buffer + index buffer.
class SimpleGeom {
public:
    VertexBuf *vbuf;
    IndexBuf *ibuf;

    SimpleGeom() {
        vbuf = NULL;
        ibuf = NULL;
    }

    SimpleGeom(VertexBuf *vb, IndexBuf *ib) {
        vbuf = vb;
        ibuf = ib;
    }

    SimpleGeom(VertexBuf *vb) {
        vbuf = vb;
        ibuf = NULL;
    }

    ~SimpleGeom() {
        if (vbuf) {
            delete vbuf;
        }
        if (ibuf) {
            delete ibuf;
        }
    }
};

#endif //CYBERBALL_SIMPLEGEOM_HPP

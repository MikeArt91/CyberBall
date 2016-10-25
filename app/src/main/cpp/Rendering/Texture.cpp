#include "Core/Log.hpp"
#include "Texture.hpp"

void Texture::InitFromRawRGB(int width, int height, bool hasAlpha, const unsigned char *data) {
    GLenum format = hasAlpha ? GL_RGBA : GL_RGB;

    glGenTextures(1, &mTextureH);
    glBindTexture(GL_TEXTURE_2D, mTextureH);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(int unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, mTextureH);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}


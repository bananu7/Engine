#pragma once

#include "TextureEnums.hpp"
#include "gl_id.h"

#include <utility>
#include <GL/glew.h>

namespace engine {

template<texture_desc::Type type>
class Texture {
    gl_id id;
    texture_desc::Format lastFormat;

    void _generateId() {
        if (id == 0)
            glGenTextures(1, id.get_ptr());
    }
public:
    void bind() {
        _generateId();
        glBindTexture(static_cast<GLenum>(type), id);
    }

    void bind(unsigned textureUnit) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        bind();
    }

    void setFiltering (texture_desc::FilteringDirection direction, texture_desc::FilteringMode mode) {
        bind();
        glTexParameteri(static_cast<GLenum>(type), static_cast<GLenum>(direction), static_cast<GLint>(mode));
    }

    inline void imageData(unsigned width,
                   unsigned height,
                   texture_desc::Format format,
                   texture_desc::InternalFormat internalFormat,
                   texture_desc::DataType dataType,
                   const void* data);

    GLuint getId() const { return id; }
    texture_desc::Format getFormat() const { return lastFormat; }

    Texture () : id(0) {
    }

    Texture(Texture&& other) 
        : id(std::move(other.id)) {
    }

    Texture& operator= (Texture&& other) {
        id = std::move(other.id);
        return *this;
    }

    ~Texture() {
        if (id) glDeleteTextures(1, id.get_ptr());
    }

private:
    Texture (Texture const&)/* = delete*/;
    Texture& operator= (Texture const&)/* = delete*/;
};

}
#pragma once
#include <GL/glew.h>

namespace engine {

// Safe-moving non-copyable OpenGL id wrapper

/*
Improvement idea: add a deleter to it, so the classes using it
could fully utilize Rule of Zero. 

Example deleters : glDeleteTextures, glDeleteShader

Problem : whilst single-target deleters are simple, what about multi-target ones?
It would probably need to have two destructor versions, based on the deleter arguments.

As it is complicated, I leave it for later.

*/

class gl_id { 
    GLuint innerId;

    // =delete commented out due to MSVS support
    gl_id(gl_id const&) /* = delete*/;
    gl_id& operator=(gl_id const&) /* = delete*/;

public:
    operator GLuint () const { return innerId; }

    // meant to be used with glGenXs and glDeleteXs functions
    const GLuint* const get_ptr() const { return &innerId; }
    GLuint* get_ptr() { return &innerId; }

    gl_id() : innerId(0) {
    }
    
    gl_id(GLuint id) : innerId(id) {
    }

    gl_id(gl_id&& other) {
        innerId = other.innerId;
        other.innerId = 0;
    }

    gl_id& operator= (gl_id&& other) {
        innerId = other.innerId;
        other.innerId = 0;
        return *this;
    }

    gl_id& operator= (GLuint id) {
        innerId = id;
        return *this;
    }
};

}
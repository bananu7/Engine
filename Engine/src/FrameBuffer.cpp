
#include "FrameBuffer.h"
#include <iostream>

namespace engine {

using namespace std;

Framebuffer::Framebuffer()
  : m_fboId(_GenerateFboId()),
    m_savedFboId(0)
{
  // Bind this FBO so that it actually gets created now
  _GuardedBind();
  _GuardedUnbind();
}

Framebuffer::~Framebuffer() 
{
  glDeleteFramebuffers(1, m_fboId.get_ptr());
}

void Framebuffer::Bind() 
{
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_fboId);
}

void Framebuffer::Disable() 
{
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

void
Framebuffer::AttachTexture( GLenum texTarget, GLuint texId, 
                                  GLenum attachment, int mipLevel, int zSlice )
{
  _GuardedBind();

#ifndef NDEBUG
  if( GetAttachedId(attachment) != texId ) {
#endif

    _FramebufferTextureND( attachment, texTarget,
                           texId, mipLevel, zSlice );

#ifndef NDEBUG
  }
  else {
    cerr << "Framebuffer::AttachTexture PERFORMANCE WARNING:\n"
      << "\tRedundant bind of texture (id = " << texId << ").\n"
      << "\tHINT : Compile with -DNDEBUG to remove this warning.\n";
  }
#endif

  _GuardedUnbind();
}

void
Framebuffer::AttachTextures( int numTextures, GLenum texTarget[], GLuint texId[],
                                  GLenum attachment[], int mipLevel[], int zSlice[] )
{
  for(int i = 0; i < numTextures; ++i) {
    AttachTexture( texTarget[i], texId[i], 
                   attachment ? attachment[i] : (GL_COLOR_ATTACHMENT0 + i), 
                   mipLevel ? mipLevel[i] : 0, 
                   zSlice ? zSlice[i] : 0 );
  }
}

void
Framebuffer::AttachRenderBuffer( GLuint buffId, GLenum attachment )
{
  _GuardedBind();

#ifndef NDEBUG
  if( GetAttachedId(attachment) != buffId ) {
#endif

    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, attachment, 
                                 GL_RENDERBUFFER, buffId);

#ifndef NDEBUG
  }
  else {
    cerr << "Framebuffer::AttachRenderBuffer PERFORMANCE WARNING:\n"
      << "\tRedundant bind of Renderbuffer (id = " << buffId << ")\n"
      << "\tHINT : Compile with -DNDEBUG to remove this warning.\n";
  }
#endif

  _GuardedUnbind();
}

void
Framebuffer::AttachRenderBuffers( int numBuffers, GLuint buffId[], GLenum attachment[] )
{
  for(int i = 0; i < numBuffers; ++i) {
    AttachRenderBuffer( buffId[i], 
                        attachment ? attachment[i] : (GL_COLOR_ATTACHMENT0 + i) );
  }
}

void
Framebuffer::Unattach( GLenum attachment )
{
  _GuardedBind();
  GLenum type = GetAttachedType(attachment);

  switch(type) {
  case GL_NONE:
    break;
  case GL_RENDERBUFFER_EXT:
    AttachRenderBuffer( 0, attachment );
    break;
  case GL_TEXTURE:
    AttachTexture( GL_TEXTURE_2D, 0, attachment );
    break;
  default:
    cerr << "Framebuffer::unbind_attachment ERROR: Unknown attached resource type\n";
  }
  _GuardedUnbind();
}

void
Framebuffer::UnattachAll()
{
  int numAttachments = GetMaxColorAttachments();
  for(int i = 0; i < numAttachments; ++i) {
    Unattach( GL_COLOR_ATTACHMENT0_EXT + i );
  }
}

GLint Framebuffer::GetMaxColorAttachments()
{
  GLint maxAttach = 0;
  glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS_EXT, &maxAttach );
  return maxAttach;
}

GLuint Framebuffer::_GenerateFboId()
{
  GLuint id = 0;
  glGenFramebuffersEXT(1, &id);
  return id;
}

void Framebuffer::_GuardedBind() 
{
  // Only binds if m_fboId is different than the currently bound FBO
  int temp;
  glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &temp );
  m_savedFboId = static_cast<GLuint>(temp);
  if (m_fboId != m_savedFboId) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
  }
}

void Framebuffer::_GuardedUnbind() 
{
  // Returns FBO binding to the previously enabled FBO
  if (m_fboId != m_savedFboId) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_savedFboId);
  }
}

void
Framebuffer::_FramebufferTextureND( GLenum attachment, GLenum texTarget,
                                         GLuint texId, int mipLevel,
                                         int zSlice )
{
  if (texTarget == GL_TEXTURE_1D) {
    glFramebufferTexture1DEXT( GL_FRAMEBUFFER_EXT, attachment,
                               GL_TEXTURE_1D, texId, mipLevel );
  }
  else if (texTarget == GL_TEXTURE_3D) {
    glFramebufferTexture3DEXT( GL_FRAMEBUFFER_EXT, attachment,
                               GL_TEXTURE_3D, texId, mipLevel, zSlice );
  }
  else {
    // Default is GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_ARB, or cube faces
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, attachment,
                               texTarget, texId, mipLevel );
  }
}

#ifndef NDEBUG
bool Framebuffer::IsValid( ostream& ostr )
{
  _GuardedBind();

  bool isOK = false;

  GLenum status;                                            
  status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  switch(status) {                                          
  case GL_FRAMEBUFFER_COMPLETE_EXT: // Everything's OK
    isOK = true;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n";
    isOK = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n";
    isOK = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n";
    isOK = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n";
    isOK = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n";
    isOK = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n";
    isOK = false;
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "GL_FRAMEBUFFER_UNSUPPORTED_EXT\n";
    isOK = false;
    break;
  default:
    ostr << "glift::CheckFramebufferStatus() ERROR:\n\t"
      << "Unknown ERROR\n";
    isOK = false;
  }

  _GuardedUnbind();
  return isOK;
}
#endif // NDEBUG

/// Accessors
GLenum Framebuffer::GetAttachedType( GLenum attachment )
{
  // Returns GL_RENDERBUFFER_EXT or GL_TEXTURE
  _GuardedBind();
  GLint type = 0;
  glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, attachment,
                                           GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT, 
                                           &type);
  _GuardedUnbind();
  return GLenum(type);
}

GLuint Framebuffer::GetAttachedId( GLenum attachment )
{
  _GuardedBind();
  GLint id = 0;
  glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, attachment,
                                           GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT,
                                           &id);
  _GuardedUnbind();
  return GLuint(id);
}

GLint Framebuffer::GetAttachedMipLevel( GLenum attachment )
{
  _GuardedBind();
  GLint level = 0;
  glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, attachment,
                                           GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT, 
                                           &level);
  _GuardedUnbind();
  return level;
}

GLint Framebuffer::GetAttachedCubeFace( GLenum attachment )
{
  _GuardedBind();
  GLint level = 0;
  glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, attachment,
                                           GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT,
                                           &level);
  _GuardedUnbind();
  return level;
}

GLint Framebuffer::GetAttachedZSlice( GLenum attachment )
{
  _GuardedBind();
  GLint slice = 0;
  glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, attachment,
                                           GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT,
                                           &slice);
  _GuardedUnbind();
  return slice;
}


} // namespace engine

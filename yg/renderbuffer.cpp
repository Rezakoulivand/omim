#include "../base/SRC_FIRST.hpp"

#include "internal/opengl.hpp"
#include "renderbuffer.hpp"
#include "utils.hpp"

#include "../base/logging.hpp"

#include "../std/list.hpp"

namespace yg
{
  namespace gl
  {
    list<unsigned int> renderBufferStack;

    int RenderBuffer::current()
    {
      int id;
      OGLCHECK(glGetIntegerv(GL_RENDERBUFFER_BINDING_MWM, &id));
      LOG(LDEBUG, ("glGetIntegerv", id));
      return id;
    }

    RenderBuffer::RenderBuffer(size_t width, size_t height, bool isDepthBuffer)
      : m_id(0), m_isDepthBuffer(isDepthBuffer), m_width(width), m_height(height)
    {
//      if (!m_isDepthBuffer)
      {
        OGLCHECK(glGenRenderbuffersFn(1, &m_id));
        LOG(LDEBUG, ("glGenRenderbuffers", m_id));

        makeCurrent();

        GLenum target = GL_RENDERBUFFER_MWM;
        GLenum internalFormat = m_isDepthBuffer ? GL_DEPTH_COMPONENT16_MWM : GL_RGBA8_MWM;

        OGLCHECK(glRenderbufferStorageFn(target,
                                         internalFormat,
                                         m_width,
                                         m_height));
        LOG(LDEBUG, ("glRenderbufferStorage", m_isDepthBuffer ? "GL_DEPTH_COMPONENT16_MWM" : "GL_RGBA8_MWM",
                     m_width, m_height));
      }
/*      else
      {
        OGLCHECK(glGenTextures(1, &m_id));

        OGLCHECK(glBindTexture(GL_TEXTURE_2D, m_id));

        OGLCHECK(glTexImage2D(GL_TEXTURE_2D,
                              0,
                              GL_DEPTH_COMPONENT24_MWM,
                              m_width,
                              m_height,
                              0,
                              GL_DEPTH_COMPONENT24_MWM,
                              GL_UNSIGNED_INT_24_8_MWM,
                              0));
      }*/
    }

    RenderBuffer::~RenderBuffer()
    {
      if (g_hasContext)
      {
//        if(m_isDepthBuffer)
//          OGLCHECK(glDeleteTextures(1, &m_id));
//        else
          OGLCHECK(glDeleteRenderbuffersFn(1, &m_id));
      }
    }

    unsigned int RenderBuffer::id() const
    {
      return m_id;
    }

    void RenderBuffer::attachToFrameBuffer()
    {
/*      if (m_isDepthBuffer)
        OGLCHECK(glFramebufferTexture2DFn(
                   GL_FRAMEBUFFER_MWM,
                   GL_DEPTH_ATTACHMENT_MWM,
                   GL_TEXTURE_2D,
                   id(),
                   0
                   ));
      else*/
        OGLCHECK(glFramebufferRenderbufferFn(
            GL_FRAMEBUFFER_MWM,
            isDepthBuffer() ? GL_DEPTH_ATTACHMENT_MWM : GL_COLOR_ATTACHMENT0_MWM,
            GL_RENDERBUFFER_MWM,
            id()));

      if (!isDepthBuffer())
        utils::setupCoordinates(width(), height(), false);
    }

    void RenderBuffer::detachFromFrameBuffer()
    {
      OGLCHECK(glFramebufferRenderbufferFn(
               GL_FRAMEBUFFER_MWM,
               isDepthBuffer() ? GL_DEPTH_ATTACHMENT_MWM : GL_COLOR_ATTACHMENT0_MWM,
               GL_RENDERBUFFER_MWM,
               0));
    }

    void RenderBuffer::makeCurrent() const
    {
      if (m_id != current())
      {
        OGLCHECK(glBindRenderbufferFn(GL_RENDERBUFFER_MWM, m_id));
        LOG(LDEBUG, ("glBindRenderbuffer", m_id));
      }
    }

    bool RenderBuffer::isDepthBuffer() const
    {
      return m_isDepthBuffer;
    }

    unsigned RenderBuffer::width() const
    {
      return m_width;
    }

    unsigned RenderBuffer::height() const
    {
      return m_height;
    }
  }
}

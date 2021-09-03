#include "debug.hh"

static void APIENTRY debug_callback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length,
                                    const GLchar *msg, const void* data)
{
    const Logger* logger = static_cast<const Logger*>(data);
    const char* _source;
    const char* _type;

    switch (source) {
    case GL_DEBUG_SOURCE_API:             _source = "API";             break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   _source = "WINDOW SYSTEM";   break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: _source = "SHADER COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     _source = "THIRD PARTY";     break;
    case GL_DEBUG_SOURCE_APPLICATION:     _source = "APPLICATION";     break;
    case GL_DEBUG_SOURCE_OTHER:           _source = "UNKNOWN";         break;
    default:                              _source = "UNKNOWN";         break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               _type = "ERROR";               break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: _type = "DEPRECATED BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  _type = "UNDEF BEHAVIOR";      break;
    case GL_DEBUG_TYPE_PORTABILITY:         _type = "PORTABILITY";         break;
    case GL_DEBUG_TYPE_PERFORMANCE:         _type = "PERFORMANCE";         break;
    case GL_DEBUG_TYPE_OTHER:               _type = "OTHER";               break;
    case GL_DEBUG_TYPE_MARKER:              _type = "MARKER";              break;
    default:                                _type = "UNKNOWN";             break;
    }

    char buf[4096] {};
    snprintf(buf, sizeof(buf)-1, "from %s: %*s (id=%d, type=%s)",
             _source, length, msg, id, _type);

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         logger->panic(buf); break;
    case GL_DEBUG_SEVERITY_MEDIUM:       logger->warn(buf);  break;
    case GL_DEBUG_SEVERITY_LOW:          logger->log(buf);   break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: logger->log(buf);   break;
    default:                             logger->warn(buf);  break;
    }
}

void set_debug_callback(const Logger* logger)
{
    logger->log("enabling debug callbacks");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debug_callback, (void*)logger);
}

/* DO NOT EDIT - This file is autogenerated */

/* WARNING: this file must only be included once */

#include <stddef.h>

#include "glcontext.h"

#define M (1 << 0)

static const struct gldefinition {
    const char *name;
    size_t offset;
    int flags;
} gldefinitions[] = {
    {"glActiveTexture", offsetof(struct glfunctions, ActiveTexture), M},
    {"glAttachShader", offsetof(struct glfunctions, AttachShader), M},
    {"glBeginQuery", offsetof(struct glfunctions, BeginQuery), 0},
    {"glBeginQueryEXT", offsetof(struct glfunctions, BeginQueryEXT), 0},
    {"glBindAttribLocation", offsetof(struct glfunctions, BindAttribLocation), M},
    {"glBindBuffer", offsetof(struct glfunctions, BindBuffer), M},
    {"glBindBufferBase", offsetof(struct glfunctions, BindBufferBase), 0},
    {"glBindBufferRange", offsetof(struct glfunctions, BindBufferRange), 0},
    {"glBindFramebuffer", offsetof(struct glfunctions, BindFramebuffer), M},
    {"glBindImageTexture", offsetof(struct glfunctions, BindImageTexture), 0},
    {"glBindRenderbuffer", offsetof(struct glfunctions, BindRenderbuffer), M},
    {"glBindTexture", offsetof(struct glfunctions, BindTexture), M},
    {"glBindVertexArray", offsetof(struct glfunctions, BindVertexArray), 0},
    {"glBlendColor", offsetof(struct glfunctions, BlendColor), M},
    {"glBlendEquation", offsetof(struct glfunctions, BlendEquation), M},
    {"glBlendEquationSeparate", offsetof(struct glfunctions, BlendEquationSeparate), M},
    {"glBlendFunc", offsetof(struct glfunctions, BlendFunc), M},
    {"glBlendFuncSeparate", offsetof(struct glfunctions, BlendFuncSeparate), M},
    {"glBlitFramebuffer", offsetof(struct glfunctions, BlitFramebuffer), 0},
    {"glBufferData", offsetof(struct glfunctions, BufferData), M},
    {"glBufferSubData", offsetof(struct glfunctions, BufferSubData), M},
    {"glCheckFramebufferStatus", offsetof(struct glfunctions, CheckFramebufferStatus), M},
    {"glClear", offsetof(struct glfunctions, Clear), M},
    {"glClearColor", offsetof(struct glfunctions, ClearColor), M},
    {"glClientWaitSync", offsetof(struct glfunctions, ClientWaitSync), 0},
    {"glColorMask", offsetof(struct glfunctions, ColorMask), M},
    {"glCompileShader", offsetof(struct glfunctions, CompileShader), M},
    {"glCreateProgram", offsetof(struct glfunctions, CreateProgram), M},
    {"glCreateShader", offsetof(struct glfunctions, CreateShader), M},
    {"glCullFace", offsetof(struct glfunctions, CullFace), M},
    {"glDeleteBuffers", offsetof(struct glfunctions, DeleteBuffers), M},
    {"glDeleteFramebuffers", offsetof(struct glfunctions, DeleteFramebuffers), M},
    {"glDeleteProgram", offsetof(struct glfunctions, DeleteProgram), M},
    {"glDeleteQueries", offsetof(struct glfunctions, DeleteQueries), 0},
    {"glDeleteQueriesEXT", offsetof(struct glfunctions, DeleteQueriesEXT), 0},
    {"glDeleteRenderbuffers", offsetof(struct glfunctions, DeleteRenderbuffers), M},
    {"glDeleteShader", offsetof(struct glfunctions, DeleteShader), M},
    {"glDeleteTextures", offsetof(struct glfunctions, DeleteTextures), M},
    {"glDeleteVertexArrays", offsetof(struct glfunctions, DeleteVertexArrays), 0},
    {"glDepthFunc", offsetof(struct glfunctions, DepthFunc), M},
    {"glDepthMask", offsetof(struct glfunctions, DepthMask), M},
    {"glDetachShader", offsetof(struct glfunctions, DetachShader), M},
    {"glDisable", offsetof(struct glfunctions, Disable), M},
    {"glDisableVertexAttribArray", offsetof(struct glfunctions, DisableVertexAttribArray), M},
    {"glDispatchCompute", offsetof(struct glfunctions, DispatchCompute), 0},
    {"glDrawArrays", offsetof(struct glfunctions, DrawArrays), M},
    {"glDrawArraysInstanced", offsetof(struct glfunctions, DrawArraysInstanced), 0},
    {"glDrawBuffers", offsetof(struct glfunctions, DrawBuffers), 0},
    {"glDrawElements", offsetof(struct glfunctions, DrawElements), M},
    {"glDrawElementsInstanced", offsetof(struct glfunctions, DrawElementsInstanced), 0},
    {"glEGLImageTargetTexture2DOES", offsetof(struct glfunctions, EGLImageTargetTexture2DOES), 0},
    {"glEnable", offsetof(struct glfunctions, Enable), M},
    {"glEnableVertexAttribArray", offsetof(struct glfunctions, EnableVertexAttribArray), M},
    {"glEndQuery", offsetof(struct glfunctions, EndQuery), 0},
    {"glEndQueryEXT", offsetof(struct glfunctions, EndQueryEXT), 0},
    {"glFenceSync", offsetof(struct glfunctions, FenceSync), 0},
    {"glFinish", offsetof(struct glfunctions, Finish), M},
    {"glFlush", offsetof(struct glfunctions, Flush), M},
    {"glFramebufferRenderbuffer", offsetof(struct glfunctions, FramebufferRenderbuffer), M},
    {"glFramebufferTexture2D", offsetof(struct glfunctions, FramebufferTexture2D), M},
    {"glGenBuffers", offsetof(struct glfunctions, GenBuffers), M},
    {"glGenFramebuffers", offsetof(struct glfunctions, GenFramebuffers), M},
    {"glGenQueries", offsetof(struct glfunctions, GenQueries), 0},
    {"glGenQueriesEXT", offsetof(struct glfunctions, GenQueriesEXT), 0},
    {"glGenRenderbuffers", offsetof(struct glfunctions, GenRenderbuffers), M},
    {"glGenTextures", offsetof(struct glfunctions, GenTextures), M},
    {"glGenVertexArrays", offsetof(struct glfunctions, GenVertexArrays), 0},
    {"glGenerateMipmap", offsetof(struct glfunctions, GenerateMipmap), M},
    {"glGetActiveAttrib", offsetof(struct glfunctions, GetActiveAttrib), M},
    {"glGetActiveUniform", offsetof(struct glfunctions, GetActiveUniform), M},
    {"glGetActiveUniformBlockName", offsetof(struct glfunctions, GetActiveUniformBlockName), 0},
    {"glGetActiveUniformBlockiv", offsetof(struct glfunctions, GetActiveUniformBlockiv), 0},
    {"glGetAttachedShaders", offsetof(struct glfunctions, GetAttachedShaders), M},
    {"glGetAttribLocation", offsetof(struct glfunctions, GetAttribLocation), M},
    {"glGetBooleanv", offsetof(struct glfunctions, GetBooleanv), M},
    {"glGetError", offsetof(struct glfunctions, GetError), M},
    {"glGetIntegeri_v", offsetof(struct glfunctions, GetIntegeri_v), M},
    {"glGetIntegerv", offsetof(struct glfunctions, GetIntegerv), M},
    {"glGetInternalformativ", offsetof(struct glfunctions, GetInternalformativ), 0},
    {"glGetProgramInfoLog", offsetof(struct glfunctions, GetProgramInfoLog), M},
    {"glGetProgramInterfaceiv", offsetof(struct glfunctions, GetProgramInterfaceiv), 0},
    {"glGetProgramResourceIndex", offsetof(struct glfunctions, GetProgramResourceIndex), 0},
    {"glGetProgramResourceLocation", offsetof(struct glfunctions, GetProgramResourceLocation), 0},
    {"glGetProgramResourceName", offsetof(struct glfunctions, GetProgramResourceName), 0},
    {"glGetProgramResourceiv", offsetof(struct glfunctions, GetProgramResourceiv), 0},
    {"glGetProgramiv", offsetof(struct glfunctions, GetProgramiv), M},
    {"glGetQueryObjectui64v", offsetof(struct glfunctions, GetQueryObjectui64v), 0},
    {"glGetQueryObjectui64vEXT", offsetof(struct glfunctions, GetQueryObjectui64vEXT), 0},
    {"glGetRenderbufferParameteriv", offsetof(struct glfunctions, GetRenderbufferParameteriv), M},
    {"glGetShaderInfoLog", offsetof(struct glfunctions, GetShaderInfoLog), M},
    {"glGetShaderSource", offsetof(struct glfunctions, GetShaderSource), M},
    {"glGetShaderiv", offsetof(struct glfunctions, GetShaderiv), M},
    {"glGetString", offsetof(struct glfunctions, GetString), M},
    {"glGetStringi", offsetof(struct glfunctions, GetStringi), M},
    {"glGetUniformBlockIndex", offsetof(struct glfunctions, GetUniformBlockIndex), 0},
    {"glGetUniformLocation", offsetof(struct glfunctions, GetUniformLocation), M},
    {"glGetUniformiv", offsetof(struct glfunctions, GetUniformiv), M},
    {"glInvalidateFramebuffer", offsetof(struct glfunctions, InvalidateFramebuffer), 0},
    {"glLinkProgram", offsetof(struct glfunctions, LinkProgram), M},
    {"glMemoryBarrier", offsetof(struct glfunctions, MemoryBarrier), 0},
    {"glPixelStorei", offsetof(struct glfunctions, PixelStorei), M},
    {"glPolygonMode", offsetof(struct glfunctions, PolygonMode), 0},
    {"glReadBuffer", offsetof(struct glfunctions, ReadBuffer), 0},
    {"glReadPixels", offsetof(struct glfunctions, ReadPixels), M},
    {"glReleaseShaderCompiler", offsetof(struct glfunctions, ReleaseShaderCompiler), M},
    {"glRenderbufferStorage", offsetof(struct glfunctions, RenderbufferStorage), M},
    {"glRenderbufferStorageMultisample", offsetof(struct glfunctions, RenderbufferStorageMultisample), 0},
    {"glShaderBinary", offsetof(struct glfunctions, ShaderBinary), M},
    {"glShaderSource", offsetof(struct glfunctions, ShaderSource), M},
    {"glStencilFunc", offsetof(struct glfunctions, StencilFunc), M},
    {"glStencilFuncSeparate", offsetof(struct glfunctions, StencilFuncSeparate), M},
    {"glStencilMask", offsetof(struct glfunctions, StencilMask), M},
    {"glStencilMaskSeparate", offsetof(struct glfunctions, StencilMaskSeparate), M},
    {"glStencilOp", offsetof(struct glfunctions, StencilOp), M},
    {"glStencilOpSeparate", offsetof(struct glfunctions, StencilOpSeparate), M},
    {"glTexImage2D", offsetof(struct glfunctions, TexImage2D), M},
    {"glTexImage3D", offsetof(struct glfunctions, TexImage3D), 0},
    {"glTexParameteri", offsetof(struct glfunctions, TexParameteri), M},
    {"glTexStorage2D", offsetof(struct glfunctions, TexStorage2D), 0},
    {"glTexStorage3D", offsetof(struct glfunctions, TexStorage3D), 0},
    {"glTexSubImage2D", offsetof(struct glfunctions, TexSubImage2D), M},
    {"glTexSubImage3D", offsetof(struct glfunctions, TexSubImage3D), 0},
    {"glUniform1f", offsetof(struct glfunctions, Uniform1f), M},
    {"glUniform1fv", offsetof(struct glfunctions, Uniform1fv), M},
    {"glUniform1i", offsetof(struct glfunctions, Uniform1i), M},
    {"glUniform1iv", offsetof(struct glfunctions, Uniform1iv), M},
    {"glUniform2f", offsetof(struct glfunctions, Uniform2f), M},
    {"glUniform2fv", offsetof(struct glfunctions, Uniform2fv), M},
    {"glUniform2i", offsetof(struct glfunctions, Uniform2i), M},
    {"glUniform2iv", offsetof(struct glfunctions, Uniform2iv), M},
    {"glUniform3f", offsetof(struct glfunctions, Uniform3f), M},
    {"glUniform3fv", offsetof(struct glfunctions, Uniform3fv), M},
    {"glUniform3i", offsetof(struct glfunctions, Uniform3i), M},
    {"glUniform3iv", offsetof(struct glfunctions, Uniform3iv), M},
    {"glUniform4f", offsetof(struct glfunctions, Uniform4f), M},
    {"glUniform4fv", offsetof(struct glfunctions, Uniform4fv), M},
    {"glUniform4i", offsetof(struct glfunctions, Uniform4i), M},
    {"glUniform4iv", offsetof(struct glfunctions, Uniform4iv), M},
    {"glUniformBlockBinding", offsetof(struct glfunctions, UniformBlockBinding), 0},
    {"glUniformMatrix2fv", offsetof(struct glfunctions, UniformMatrix2fv), M},
    {"glUniformMatrix3fv", offsetof(struct glfunctions, UniformMatrix3fv), M},
    {"glUniformMatrix4fv", offsetof(struct glfunctions, UniformMatrix4fv), M},
    {"glUseProgram", offsetof(struct glfunctions, UseProgram), M},
    {"glVertexAttribDivisor", offsetof(struct glfunctions, VertexAttribDivisor), 0},
    {"glVertexAttribPointer", offsetof(struct glfunctions, VertexAttribPointer), M},
    {"glViewport", offsetof(struct glfunctions, Viewport), M},
    {"glWaitSync", offsetof(struct glfunctions, WaitSync), 0},
};

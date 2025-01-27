/*
 * Copyright 2023 Nope Forge
 * Copyright 2018-2022 GoPro Inc.
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdlib.h>
#include <string.h>

#include "bstr.h"
#include "gpu_ctx_gl.h"
#include "glincludes.h"
#include "log.h"
#include "memory.h"
#include "program_gl.h"
#include "type.h"

static int program_check_status(const struct glcontext *gl, GLuint id, GLenum status)
{
    char *info_log = NULL;
    int info_log_length = 0;

    void (*get_info)(const struct glcontext *gl, GLuint id, GLenum pname, GLint *params);
    void (*get_log)(const struct glcontext *gl, GLuint id, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    const char *type_str;

    if (status == GL_COMPILE_STATUS) {
        type_str = "compile";
        get_info = ngli_glGetShaderiv;
        get_log  = ngli_glGetShaderInfoLog;
    } else if (status == GL_LINK_STATUS) {
        type_str = "link";
        get_info = ngli_glGetProgramiv;
        get_log  = ngli_glGetProgramInfoLog;
    } else {
        ngli_assert(0);
    }

    GLint result = GL_FALSE;
    get_info(gl, id, status, &result);
    if (result == GL_TRUE)
        return 0;

    get_info(gl, id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (!info_log_length)
        return NGL_ERROR_BUG;

    info_log = ngli_malloc(info_log_length);
    if (!info_log)
        return NGL_ERROR_MEMORY;

    get_log(gl, id, info_log_length, NULL, info_log);
    while (info_log_length && strchr(" \r\n", info_log[info_log_length - 1]))
        info_log_length--;

    LOG(ERROR, "could not %s shader: %.*s", type_str, info_log_length, info_log);
    ngli_free(info_log);
    return NGL_ERROR_INVALID_DATA;
}

static void free_pinfo(void *user_arg, void *data)
{
    ngli_free(data);
}

static struct program_variable_info *program_variable_info_create()
{
    struct program_variable_info *info = ngli_calloc(1, sizeof(*info));
    if (!info)
        return NULL;
    info->binding  = -1;
    info->location = -1;
    return info;
}

static struct hmap *program_probe_uniforms(struct glcontext *gl, GLuint pid)
{
    struct hmap *umap = ngli_hmap_create();
    if (!umap)
        return NULL;
    ngli_hmap_set_free_func(umap, free_pinfo, NULL);

    GLint nb_active_uniforms;
    ngli_glGetProgramiv(gl, pid, GL_ACTIVE_UNIFORMS, &nb_active_uniforms);
    for (GLint i = 0; i < nb_active_uniforms; i++) {
        char name[MAX_ID_LEN];
        struct program_variable_info *info = program_variable_info_create();
        if (!info) {
            ngli_hmap_freep(&umap);
            return NULL;
        }

        GLenum type;
        GLint size;
        ngli_glGetActiveUniform(gl, pid, i, sizeof(name), NULL, &size, &type, name);

        /* Remove [0] suffix from names of uniform arrays */
        name[strcspn(name, "[")] = 0;
        info->location = ngli_glGetUniformLocation(gl, pid, name);

        if (type == GL_IMAGE_2D) {
            ngli_glGetUniformiv(gl, pid, info->location, &info->binding);
        } else {
            info->binding = -1;
        }

        LOG(DEBUG, "uniform[%d/%d]: %s location:%d binding=%d",
            i + 1, nb_active_uniforms, name, info->location, info->binding);

        int ret = ngli_hmap_set(umap, name, info);
        if (ret < 0) {
            free_pinfo(NULL, info);
            ngli_hmap_freep(&umap);
            return NULL;
        }
    }

    return umap;
}

static struct hmap *program_probe_attributes(struct glcontext *gl, GLuint pid)
{
    struct hmap *amap = ngli_hmap_create();
    if (!amap)
        return NULL;
    ngli_hmap_set_free_func(amap, free_pinfo, NULL);

    GLint nb_active_attributes;
    ngli_glGetProgramiv(gl, pid, GL_ACTIVE_ATTRIBUTES, &nb_active_attributes);
    for (GLint i = 0; i < nb_active_attributes; i++) {
        char name[MAX_ID_LEN];
        struct program_variable_info *info = program_variable_info_create();
        if (!info) {
            ngli_hmap_freep(&amap);
            return NULL;
        }

        GLenum type;
        GLint size;
        ngli_glGetActiveAttrib(gl, pid, i, sizeof(name), NULL, &size, &type, name);
        info->location = ngli_glGetAttribLocation(gl, pid, name);
        LOG(DEBUG, "attribute[%d/%d]: %s location:%d",
            i + 1, nb_active_attributes, name, info->location);

        int ret = ngli_hmap_set(amap, name, info);
        if (ret < 0) {
            free_pinfo(NULL, info);
            ngli_hmap_freep(&amap);
            return NULL;
        }
    }

    return amap;
}

static struct hmap *program_probe_buffer_blocks(struct glcontext *gl, GLuint pid)
{
    struct hmap *bmap = ngli_hmap_create();
    if (!bmap)
        return NULL;
    ngli_hmap_set_free_func(bmap, free_pinfo, NULL);

    /* Uniform Buffers */
    GLint nb_active_uniform_buffers;
    ngli_glGetProgramiv(gl, pid, GL_ACTIVE_UNIFORM_BLOCKS, &nb_active_uniform_buffers);
    for (GLint i = 0; i < nb_active_uniform_buffers; i++) {
        struct program_variable_info *info = program_variable_info_create();
        if (!info) {
            ngli_hmap_freep(&bmap);
            return NULL;
        }

        char name[MAX_ID_LEN] = {0};
        ngli_glGetActiveUniformBlockName(gl, pid, i, sizeof(name), NULL, name);

        const GLuint block_index = ngli_glGetUniformBlockIndex(gl, pid, name);
        ngli_glGetActiveUniformBlockiv(gl, pid, block_index, GL_UNIFORM_BLOCK_BINDING, &info->binding);

        GLint block_size = 0;
        ngli_glGetActiveUniformBlockiv(gl, pid, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

        LOG(DEBUG, "ubo[%d/%d]: %s binding:%d size:%d",
            i + 1, nb_active_uniform_buffers, name, info->binding, block_size);

        int ret = ngli_hmap_set(bmap, name, info);
        if (ret < 0) {
            free_pinfo(NULL, info);
            ngli_hmap_freep(&bmap);
            return NULL;
        }
    }

    if (!((gl->features & NGLI_FEATURE_GL_PROGRAM_INTERFACE_QUERY) &&
          (gl->features & NGLI_FEATURE_GL_SHADER_STORAGE_BUFFER_OBJECT)))
        return bmap;

    /* Shader Storage Buffers */
    GLint nb_active_buffers;
    ngli_glGetProgramInterfaceiv(gl, pid, GL_SHADER_STORAGE_BLOCK,
                                 GL_ACTIVE_RESOURCES, &nb_active_buffers);
    for (GLint i = 0; i < nb_active_buffers; i++) {
        char name[MAX_ID_LEN] = {0};
        struct program_variable_info *info = program_variable_info_create();
        if (!info) {
            ngli_hmap_freep(&bmap);
            return NULL;
        }

        ngli_glGetProgramResourceName(gl, pid, GL_SHADER_STORAGE_BLOCK, i, sizeof(name), NULL, name);
        GLuint block_index = ngli_glGetProgramResourceIndex(gl, pid, GL_SHADER_STORAGE_BLOCK, name);
        static const GLenum props[] = {GL_BUFFER_BINDING};
        ngli_glGetProgramResourceiv(gl, pid, GL_SHADER_STORAGE_BLOCK, block_index,
                                    (GLsizei)NGLI_ARRAY_NB(props), props, 1, NULL, &info->binding);

        LOG(DEBUG, "ssbo[%d/%d]: %s binding:%d",
            i + 1, nb_active_buffers, name, info->binding);

        int ret = ngli_hmap_set(bmap, name, info);
        if (ret < 0) {
            free_pinfo(NULL, info);
            ngli_hmap_freep(&bmap);
            return NULL;
        }
    }

    return bmap;
}

struct program *ngli_program_gl_create(struct gpu_ctx *gpu_ctx)
{
    struct program_gl *s = ngli_calloc(1, sizeof(*s));
    if (!s)
        return NULL;
    s->parent.gpu_ctx = gpu_ctx;
    return (struct program *)s;
}

int ngli_program_gl_init(struct program *s, const struct program_params *params)
{
    struct program_gl *s_priv = (struct program_gl *)s;

    int ret = 0;
    struct {
        const char *name;
        GLenum type;
        const char *src;
        GLuint id;
    } shaders[] = {
        [NGLI_PROGRAM_SHADER_VERT] = {"vertex",   GL_VERTEX_SHADER,   params->vertex,   0},
        [NGLI_PROGRAM_SHADER_FRAG] = {"fragment", GL_FRAGMENT_SHADER, params->fragment, 0},
        [NGLI_PROGRAM_SHADER_COMP] = {"compute",  GL_COMPUTE_SHADER,  params->compute,  0},
    };

    struct gpu_ctx_gl *gpu_ctx_gl = (struct gpu_ctx_gl *)s->gpu_ctx;
    struct glcontext *gl = gpu_ctx_gl->glcontext;

    const uint64_t features = NGLI_FEATURE_GL_COMPUTE_SHADER_ALL;
    if (params->compute && (gl->features & features) != features) {
        LOG(ERROR, "context does not support compute shaders");
        return NGL_ERROR_GRAPHICS_UNSUPPORTED;
    }

    s_priv->id = ngli_glCreateProgram(gl);

    for (size_t i = 0; i < NGLI_ARRAY_NB(shaders); i++) {
        if (!shaders[i].src)
            continue;
        GLuint shader = ngli_glCreateShader(gl, shaders[i].type);
        shaders[i].id = shader;
        ngli_glShaderSource(gl, shader, 1, &shaders[i].src, NULL);
        ngli_glCompileShader(gl, shader);
        ret = program_check_status(gl, shader, GL_COMPILE_STATUS);
        if (ret < 0) {
            char *s_with_numbers = ngli_numbered_lines(shaders[i].src);
            if (s_with_numbers) {
                LOG(ERROR, "failed to compile shader \"%s\":\n%s",
                    params->label ? params->label : "", s_with_numbers);
                ngli_free(s_with_numbers);
            }
            goto fail;
        }
        ngli_glAttachShader(gl, s_priv->id, shader);
    }

    ngli_glLinkProgram(gl, s_priv->id);
    ret = program_check_status(gl, s_priv->id, GL_LINK_STATUS);
    if (ret < 0) {
        struct bstr *bstr = ngli_bstr_create();
        if (bstr) {
            ngli_bstr_printf(bstr, "failed to link shaders \"%s\":",
                             params->label ? params->label : "");
            for (size_t i = 0; i < NGLI_ARRAY_NB(shaders); i++) {
                if (!shaders[i].src)
                    continue;
                char *s_with_numbers = ngli_numbered_lines(shaders[i].src);
                if (s_with_numbers) {
                    ngli_bstr_printf(bstr, "\n\n%s shader:\n%s", shaders[i].name, s_with_numbers);
                    ngli_free(s_with_numbers);
                }
            }
            LOG(ERROR, "%s", ngli_bstr_strptr(bstr));
            ngli_bstr_freep(&bstr);
        }
        goto fail;
    }

    for (size_t i = 0; i < NGLI_ARRAY_NB(shaders); i++)
        ngli_glDeleteShader(gl, shaders[i].id);

    s->uniforms = program_probe_uniforms(gl, s_priv->id);
    s->attributes = program_probe_attributes(gl, s_priv->id);
    s->buffer_blocks = program_probe_buffer_blocks(gl, s_priv->id);
    if (!s->uniforms || !s->attributes || !s->buffer_blocks) {
        ret = NGL_ERROR_MEMORY;
        goto fail;
    }

    return 0;

fail:
    for (size_t i = 0; i < NGLI_ARRAY_NB(shaders); i++)
        ngli_glDeleteShader(gl, shaders[i].id);

    return ret;
}

void ngli_program_gl_freep(struct program **sp)
{
    if (!*sp)
        return;
    struct program *s = *sp;
    struct program_gl *s_priv = (struct program_gl *)s;
    ngli_hmap_freep(&s->uniforms);
    ngli_hmap_freep(&s->attributes);
    ngli_hmap_freep(&s->buffer_blocks);
    struct gpu_ctx_gl *gpu_ctx_gl = (struct gpu_ctx_gl *)s->gpu_ctx;
    struct glcontext *gl = gpu_ctx_gl->glcontext;
    ngli_glDeleteProgram(gl, s_priv->id);
    ngli_freep(sp);
}

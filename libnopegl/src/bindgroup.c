/*
 * Copyright 2023 Matthieu Bouron <matthieu.bouron@gmail.com>
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

#include "bindgroup.h"
#include "gpu_ctx.h"
#include "log.h"
#include "memory.h"
#include "utils.h"

static int layout_entry_is_compatible(const struct bindgroup_layout_entry *a,
                                      const struct bindgroup_layout_entry *b)
{
    return a->type    == b->type    &&
           a->binding == b->binding &&
           a->access  == b->access  &&
           a->stage   == b->stage;
}

static void bindgroup_layout_freep(struct bindgroup_layout **sp)
{
    if (!*sp)
        return;

    struct bindgroup_layout *s = *sp;
    ngli_freep(&s->buffers);
    ngli_freep(&s->textures);

    (*sp)->gpu_ctx->cls->bindgroup_layout_freep(sp);
}

struct bindgroup_layout *ngli_bindgroup_layout_create(struct gpu_ctx *gpu_ctx)
{
    struct bindgroup_layout *s = gpu_ctx->cls->bindgroup_layout_create(gpu_ctx);
    s->rc = NGLI_RC_CREATE(bindgroup_layout_freep);
    return s;
}

int ngli_bindgroup_layout_init(struct bindgroup_layout *s,
                               const struct bindgroup_layout_params *params)
{
    NGLI_ARRAY_MEMDUP(*s, *params, textures);
    NGLI_ARRAY_MEMDUP(*s, *params, buffers);

    size_t nb_uniform_buffers_dynamic = 0;
    size_t nb_storage_buffers_dynamic = 0;
    for (size_t i = 0; i < s->nb_buffers; i++) {
        const struct bindgroup_layout_entry *entry = &s->buffers[i];
        if (entry->type == NGLI_TYPE_UNIFORM_BUFFER_DYNAMIC)
            nb_uniform_buffers_dynamic++;
        else if (entry->type == NGLI_TYPE_STORAGE_BUFFER_DYNAMIC)
            nb_storage_buffers_dynamic++;
    }
    ngli_assert(nb_uniform_buffers_dynamic <= NGLI_MAX_UNIFORM_BUFFERS_DYNAMIC);
    ngli_assert(nb_storage_buffers_dynamic <= NGLI_MAX_STORAGE_BUFFERS_DYNAMIC);
    s->nb_dynamic_offsets = nb_uniform_buffers_dynamic + nb_storage_buffers_dynamic;

    return s->gpu_ctx->cls->bindgroup_layout_init(s);
}

int ngli_bindgroup_layout_is_compatible(const struct bindgroup_layout *a, const struct bindgroup_layout *b)
{
    if (a->nb_buffers  != b->nb_buffers ||
        a->nb_textures != b->nb_textures)
        return 0;

    for (size_t i = 0; i < a->nb_buffers; i++) {
        if (!layout_entry_is_compatible(&a->buffers[i], &b->buffers[i]))
            return 0;
    }

    for (size_t i = 0; i < a->nb_textures; i++) {
        if (!layout_entry_is_compatible(&a->textures[i], &b->textures[i]))
            return 0;
    }

    return 1;
}

void ngli_bindgroup_layout_freep(struct bindgroup_layout **sp)
{
    NGLI_RC_UNREFP(sp);
}

static void bindgroup_freep(struct bindgroup **sp)
{
    if (!*sp)
        return;

    (*sp)->gpu_ctx->cls->bindgroup_freep(sp);
}

struct bindgroup *ngli_bindgroup_create(struct gpu_ctx *gpu_ctx)
{
    struct bindgroup *s = gpu_ctx->cls->bindgroup_create(gpu_ctx);
    s->rc = NGLI_RC_CREATE(bindgroup_freep);
    return s;
}

int ngli_bindgroup_init(struct bindgroup *s, const struct bindgroup_params *params)
{
    return s->gpu_ctx->cls->bindgroup_init(s, params);
}

int ngli_bindgroup_update_texture(struct bindgroup *s, int32_t index, const struct texture_binding *binding)
{
    if (index == -1)
        return NGL_ERROR_NOT_FOUND;

    ngli_assert(index < s->layout->nb_textures);

    if (binding->texture) {
        const struct texture *texture = binding->texture;
        const struct bindgroup_layout_entry *entry = &s->layout->textures[index];
        switch (entry->type) {
        case NGLI_TYPE_SAMPLER_2D:
        case NGLI_TYPE_SAMPLER_2D_ARRAY:
        case NGLI_TYPE_SAMPLER_2D_RECT:
        case NGLI_TYPE_SAMPLER_3D:
        case NGLI_TYPE_SAMPLER_CUBE:
        case NGLI_TYPE_SAMPLER_EXTERNAL_OES:
        case NGLI_TYPE_SAMPLER_EXTERNAL_2D_Y2Y_EXT:
            ngli_assert(texture->params.usage & NGLI_TEXTURE_USAGE_SAMPLED_BIT);
            break;
        case NGLI_TYPE_IMAGE_2D:
        case NGLI_TYPE_IMAGE_2D_ARRAY:
        case NGLI_TYPE_IMAGE_3D:
        case NGLI_TYPE_IMAGE_CUBE:
            ngli_assert(texture->params.usage & NGLI_TEXTURE_USAGE_STORAGE_BIT);
            break;
        default:
            ngli_assert(0);
        }
    }

    return s->gpu_ctx->cls->bindgroup_update_texture(s, index, binding);
}

int ngli_bindgroup_update_buffer(struct bindgroup *s, int32_t index, const struct buffer_binding *binding)
{
    if (index == -1)
        return NGL_ERROR_NOT_FOUND;

    ngli_assert(index < s->layout->nb_buffers);

    if (binding->buffer) {
        const struct buffer *buffer = binding->buffer;
        const size_t size = binding->size;
        const struct gpu_limits *limits = &s->gpu_ctx->limits;
        const struct bindgroup_layout_entry *entry = &s->layout->buffers[index];
        if (entry->type == NGLI_TYPE_UNIFORM_BUFFER ||
            entry->type == NGLI_TYPE_UNIFORM_BUFFER_DYNAMIC) {
            ngli_assert(buffer->usage & NGLI_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
            if (size > limits->max_uniform_block_size) {
                LOG(ERROR, "buffer (binding=%d) size (%zu) exceeds max uniform block size (%d)",
                    entry->binding, buffer->size, limits->max_uniform_block_size);
                return NGL_ERROR_GRAPHICS_LIMIT_EXCEEDED;
            }
        } else if (entry->type == NGLI_TYPE_STORAGE_BUFFER ||
                   entry->type == NGLI_TYPE_STORAGE_BUFFER_DYNAMIC) {
            ngli_assert(buffer->usage & NGLI_BUFFER_USAGE_STORAGE_BUFFER_BIT);
            if (size > limits->max_storage_block_size) {
                LOG(ERROR, "buffer (binding=%d) size (%zu) exceeds max storage block size (%d)",
                    entry->binding, buffer->size, limits->max_storage_block_size);
                return NGL_ERROR_GRAPHICS_LIMIT_EXCEEDED;
            }
        } else {
            ngli_assert(0);
        }
    }

    return s->gpu_ctx->cls->bindgroup_update_buffer(s, index, binding);
}

void ngli_bindgroup_freep(struct bindgroup **sp)
{
    NGLI_RC_UNREFP(sp);
}

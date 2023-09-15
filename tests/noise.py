#
# Copyright 2023 Matthieu Bouron <matthieu.bouron@gmail.com>
# Copyright 2023 Nope Foundry
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

from pynopegl_utils.misc import SceneCfg, scene
from pynopegl_utils.tests.cmp_fingerprint import test_fingerprint

import pynopegl as ngl


@test_fingerprint(keyframes=10, tolerance=1)
@scene()
def noise_blocky(cfg: SceneCfg):
    cfg.aspect_ratio = (1, 1)
    cfg.duration = 5

    return ngl.RenderNoise(type="blocky", octaves=3, scale=(9, 9), evolution=ngl.Time())


@test_fingerprint(keyframes=10, tolerance=1)
@scene()
def noise_perlin(cfg: SceneCfg):
    cfg.aspect_ratio = (1, 1)
    cfg.duration = 5

    return ngl.RenderNoise(type="perlin", octaves=3, scale=(2, 2), evolution=ngl.Time())
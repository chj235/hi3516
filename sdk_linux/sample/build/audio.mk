#  Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

# Hisilicon audio sample makefile

# 编译所需的公共文件
include ./mk.param

# 编译后生成的可执行文件名
TARGET := ohos_audio_demo

# 编译此工程需要依赖的.c或.cpp文件
SMP_SRCS += $(AUDIO_SAMPLE_DIR)/adp/audio_aac_adp.c
SMP_SRCS += $(AUDIO_SAMPLE_DIR)/adp/audio_dl_adp.c
SMP_SRCS += $(AUDIO_SAMPLE_DIR)/sample_audio.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_audio.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_sys.c

# 编译此工程需要包含的头文件
CFLAGS += -I$(AUDIO_SAMPLE_DIR)/adp

# 根据实际情况配置AUDIO的一下设置
HI_SUPPORT_MP3 = -DHI_SUPPORT_MP3=NO
# CONFIG_USER_SPACE = -DCONFIG_USER_SPACE=YES
CFLAGS += $(HI_SUPPORT_MP3)
# CFLAGS += $(CONFIG_USER_SPACE)

# 编译工程所需的mk文件
include ./sdk_linux.mk

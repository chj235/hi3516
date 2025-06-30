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

# Hisilicon ive sample makefile

# 编译所需的公共文件
include ./mk.param

# 编译后生成的可执行文件名
TARGET := ohos_ive_demo

# 编译此工程需要依赖的.c或.cpp文件
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample_ive_main.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_test_memory.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_st_and_lk.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_sobel_with_cached_mem.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_queue.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_persptrans.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_od.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_md.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_kcf.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_gmm2.c
SMP_SRCS += $(IVE_SAMPLE_DIR)/sample/sample_ive_canny.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_isp.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_sys.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_venc.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_vi.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_vo.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_vpss.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_audio.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/sample_comm_region.c
SMP_SRCS += $(PLATRORM_COMMON_DIR)/loadbmp.c
SMP_SRCS += $(SVP_COMMON_DIR)/sample_comm_ive.c
SMP_SRCS += $(SVP_COMMON_DIR)/sample_comm_nnie.c
SMP_SRCS += $(SVP_COMMON_DIR)/sample_comm_svp.c

# 编译此工程需要包含的头文件
CFLAGS += -I$(IVE_SAMPLE_DIR)/sample
CFLAGS += -I$(AUDIO_SAMPLE_DIR)/adp
CFLAGS += -I$(VIO_SAMPLE_DIR)
CFLAGS += -I$(IVE_SAMPLE_DIR)

# 根据实际的类型设置，可以用set_sensor脚本设置
SENSOR0_TYPE = -DSENSOR0_TYPE=SONY_IMX335_MIPI_4M_30FPS_12BIT
SENSOR1_TYPE = -DSENSOR1_TYPE=SONY_IMX335_MIPI_4M_30FPS_12BIT
CFLAGS += $(SENSOR0_TYPE)
CFLAGS += $(SENSOR1_TYPE)


# 编译工程所需的mk文件
include ./sdk_linux.mk

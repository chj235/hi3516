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

# Hisilicon higv sample makefile

# 编译所需的公共文件
include ./mk.param

# 编译后生成的可执行文件名
TARGET := ohos_higv_demo

# 编译此工程需要依赖的.c或.cpp文件
SMP_SRCS += $(HIGV_SAMPLE_DIR)/app.c
SMP_SRCS += $(HIGV_SAMPLE_DIR)/button.c
SMP_SRCS += $(HIGV_SAMPLE_DIR)/higv_cextfile.c
SMP_SRCS += $(HIGV_SAMPLE_DIR)/higv_mw_media.c
SMP_SRCS += $(HIGV_SAMPLE_DIR)/image.c
SMP_SRCS += $(HIGV_SAMPLE_DIR)/label.c
SMP_SRCS += $(HIGV_SAMPLE_DIR)/listbox.c
SMP_SRCS += $(HIGV_SAMPLE_DIR)/scrollview.c

# 编译此工程需要包含的头文件
CFLAGS += -I$(HIGV_SAMPLE_DIR)/xml
CFLAGS += -I$(HIGV_SAMPLE_DIR)/include
CFLAGS += -I$(HIGV_SAMPLE_DIR)


# 根据实际的类型设置，可以用set_sensor脚本设置
SENSOR0_TYPE = -DSENSOR0_TYPE=SONY_IMX335_MIPI_4M_30FPS_12BIT
SENSOR1_TYPE = -DSENSOR1_TYPE=SONY_IMX335_MIPI_4M_30FPS_12BIT
CFLAGS += $(SENSOR0_TYPE)
CFLAGS += $(SENSOR1_TYPE)

# 编译此工程需要依赖的库函数
MPI_LIBS += $(HIGV_SAMPLE_DIR)/lib/libhigvsample.a

# 编译工程所需的mk文件
include ./sdk_linux.mk

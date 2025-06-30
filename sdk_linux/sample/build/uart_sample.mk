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

# Hisilicon uart demo makefile

# 编译所需的公共文件
include ./mk.param

# 编译后生成的可执行文件名
TARGET := ohos_camera_uart_demo

# 编译此工程需要依赖的.c或.cpp文件
SMP_SRCS += $(UART_SAMPLE_DIR)/uart.c

# 编译此工程需要包含的头文件
CFLAGS += -I$(UART_SAMPLE_DIR)

# 编译工程所需的mk文件
include ./sdk_linux.mk
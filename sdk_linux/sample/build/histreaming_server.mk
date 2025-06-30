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

# Hisilicon histreaming_server sample makefile

# 编译所需的公共文件
include ./mk.param

# 编译后生成的可执行文件名
TARGET := ohos_histreaming_server

# 编译此工程需要依赖的.c或.cpp文件
SMP_SRCS += $(HISM_SERVER_SAMPLE_DIR)/src/hisignalling.c
SMP_SRCS += $(HISM_SERVER_SAMPLE_DIR)/src/histreaming_client_server.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/address.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/net.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_event.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/encode.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/option.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/uri.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_session.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/pdu.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_debug.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/block.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_hashkey.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_openssl.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_time.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_gnutls.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_notls.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_tinydtls.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/resource.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/coap_io.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/str.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/mem.c
SMP_SRCS += $(THIRD_PARTY_DIR)/libcoap/src/subscribe.c

# 编译此工程需要包含的头文件
CFLAGS += -I$(HISM_SERVER_SAMPLE_DIR)/include
CFLAGS += -I$(THIRD_PARTY_DIR)/cJSON
CFLAGS += -I$(THIRD_PARTY_DIR)/libcoap
CFLAGS += -I$(THIRD_PARTY_DIR)/libcoap/include
CFLAGS += -I$(THIRD_PARTY_DIR)/libcoap/include/coap2

# 根据实际的类型设置，可以用set_sensor脚本设置
_PLATFORM_LINUX = -D_PLATFORM_LINUX
WITH_POSIX = -DWITH_POSIX
CFLAGS += $(_PLATFORM_LINUX)
CFLAGS += $(WITH_POSIX)

# 编译此工程需要依赖的库函数
MPI_LIBS += $(HISM_SERVER_SAMPLE_DIR)/lib/libhistreaminglink.a
MPI_LIBS += $(HISM_SERVER_SAMPLE_DIR)/lib/libcoap_static.a

# 编译工程所需的mk文件
include ./sdk_linux.mk

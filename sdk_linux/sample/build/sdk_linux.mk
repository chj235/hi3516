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

#====================================================
#	Module Specified Configure
#====================================================

FLAG_SYSROOT = --sysroot=$(TAURUS_OUT_DIR)/sysroot
FLAG_TARGET = --target=arm-linux-ohos

#====================================================
#	SDK libs
#====================================================
MPI_LIBS += -L$(TARGET_DIR)
MPI_LIBS += -L$(SDK_LINUX_OUT_LIB)
MPI_LIBS += -L$(OUT_USR_LIB)
MPI_LIBS += -L$(OUT_UNSTRIPPED_LIB)
MPI_LIBS += -laaccomm -laacdec -laacenc -laacsbrdec -laacsbrenc -ldnvqe -lhdmi -lhi3516cv500_acodec -lhi3516cv500_adec -lhi3516cv500_aenc -lhi3516cv500_aio
MPI_LIBS += -lhi3516cv500_ai -lhi3516cv500_ao -lhi3516cv500_base -lhi3516cv500_chnl -lhi3516cv500_dis -lhi3516cv500_gdc -lhi3516cv500_h264e -lhi3516cv500_h265e
MPI_LIBS += -lhi3516cv500_isp -lhi3516cv500_ive -lhi3516cv500_jpegd -lhi3516cv500_jpege -lhi3516cv500_nnie -lhi3516cv500_rc -lhi3516cv500_rgn -lhi3516cv500_sys
MPI_LIBS += -lhi3516cv500_vdec -lhi3516cv500_vedu -lhi3516cv500_venc -lhi3516cv500_vfmw -lhi3516cv500_vgs -lhi3516cv500_vi -lhi3516cv500_vo -lhi3516cv500_vpss
MPI_LIBS += -l_hiacs -l_hiae -l_hiawb_natura -l_hiawb -l_hicalcflicker -l_hidehaze -l_hidrc -lhifisheyecalibrate -l_hiir_auto -lhi_irq -l_hildci -lhi_osal
MPI_LIBS += -lhi_sensor_i2c -lhive_AEC -lhive_AGC -lhive_ANR -lhive_common -lhive_EQ -lhive_HPF -lhive_record -lhive_RES_ext -lhive_RES -lhive_talkv2 -lhive_wnr
MPI_LIBS += -lisp -live -lmd -lmpi -lmpp_vbs -lnnie -lsdk -lsecurec -lsns_imx335 -ltde -lupvqe -lvb_server -lVoiceEngine -lcjson_shared -lc++

# 设置浮点运算, cpu类型, 链接器的相关设置
FLAG_LD += -mfloat-abi=softfp -mfpu=neon-vfpv4 -mcpu=cortex-a7 -fuse-ld=lld -L . -Wl,-rpath=.

#====================================================
#	SDK include
#====================================================
CFLAGS += -I$(SDK_LINUX_INCLUDE_DIR)
CFLAGS += -I$(SDK_LINUX_CBB_DIR)
CFLAGS += -I$(SDK_LINUX_INIT_DIR)
CFLAGS += -I$(BOUNDS_CHECKING_DIR)
CFLAGS += -I$(PLATRORM_COMMON_DIR)
CFLAGS += -I$(SVP_COMMON_DIR)

#====================================================
#	Source FileList
#====================================================
SOURCE_TREE := $(ROOT_DIR)
BUILD_TREE  := $(ROOT_DIR)

#====================================================
#	Objects FileList
#====================================================
SRC_ROOT := $(SOURCE_TREE)
OBJ_ROOT := $(BUILD_TREE)/objs
OBJS	 := $(SMP_SRCS:$(SRC_ROOT)%.c=$(OBJ_ROOT)%.o)
OBJS	 := $(sort $(OBJS))
OBJS_DIR := $(sort $(dir $(OBJS)))
CPP_OBJS := $(CPP_SRCS:$(SRC_ROOT)%.cpp=$(OBJ_ROOT)%.o)
OBJS_DIR += $(sort $(dir $(CPP_OBJS)))

#====================================================
#	Create Directory
#====================================================
CreateDir = $(shell [ -d $1 ] || mkdir -p $1 || echo ":mkdir '$1' fail")
CreateResult :=
dummy := $(call CreateDir, $(OBJS_ROOT))
dummy += $(foreach dir, $(OBJS_DIR), CreateResult += $(call CreateDir, $(dir)))
ifneq ($(strip CreateDir),)
	err = $(error $(CreateResult))
endif

#====================================================
#	Compile rules
#====================================================
MKDIRS := $(TARGET_DIR)

.PHONY : clean all
all: $(TARGET)

$(TARGET): $(OBJS) $(CPP_OBJS) | $(MKDIRS)
	@echo ... ld $@
#	@echo ... $(TARGET)
	@ $(CC) $(CFLAGS) $(FLAG_SYSROOT) $(FLAG_TARGET) $(FLAG_LD) -o $(TARGET_DIR)/$@ $^ -Wl,--start-group $(MPI_LIBS) -Wl,--end-group

$(CPP_OBJS) : $(OBJ_ROOT)/%.o : $(SRC_ROOT)/%.cpp
	@echo @@@ c++ $^
	@$(CXXCPP) $(FLAG_SYSROOT) $(FLAG_TARGET) $(FLAG_LD) $(CFLAGS) -c "$<" -o "$@" -w

$(OBJS) : $(OBJ_ROOT)/%.o : $(SRC_ROOT)/%.c
	@echo @@@ cc $^
	@$(CC) $(FLAG_SYSROOT) $(FLAG_TARGET) $(FLAG_LD) $(CFLAGS) -c "$<" -o "$@" -w

$(MKDIRS):
	mkdir -p $@

echo ===========  $(TARGET) build success  =================

clean:
	@rm -rf $(OUTPUT_DIR)/$(TARGET)
	@rm -rf $(OBJ_ROOT)

cleanstream:
	@rm -f *.h264
	@rm -f *.h265
	@rm -f *.jpg
	@rm -f *.mjp
	@rm -f *.mp4

/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SAMPLE_LCD_H
#define SAMPLE_LCD_H

#include <stdint.h>
#include "sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef SAMPLE_PRT
#define SAMPLE_PRT(fmt...)   \
    do { \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt); \
    } while (0)
#endif

#define APIPE0    0
#define APIPE1    1
#define APIPE2    2
#define APIPE3    3

#define AIC_VPSS_GRP            0 // default use VPSS group
#define AIC_VPSS_ZIN_CHN        0 // default use VPSS amplification channel
#define AIC_VPSS_ZOUT_CHN       1 // default use VPSS narrowing channel

#define AICSTART_VI_OUTWIDTH    1920
#define AICSTART_VI_OUTHEIGHT   1080

/*
 * 比特常量
 * Bit constant
 */
#define HI_BIT0             0x01U
#define HI_BIT1             0x02U
#define HI_BIT2             0x04U
#define HI_BIT3             0x08U
#define HI_BIT4             0x10U
#define HI_BIT5             0x20U
#define HI_BIT6             0x40U
#define HI_BIT7             0x80U
#define HI_BIT8             0x0100U
#define HI_BIT9             0x0200U
#define HI_BIT10            0x0400U
#define HI_BIT11            0x0800U
#define HI_BIT12            0x1000U
#define HI_BIT13            0x2000U
#define HI_BIT14            0x4000U
#define HI_BIT15            0x8000U

/*
 * 标记组件，该组件是否启用
 * Flags to mark whether the component is enabled
 */
#define MPP_VI      HI_BIT0
#define MPP_VDEC    HI_BIT1
#define MPP_VPSS    HI_BIT2
#define MPP_VENC    HI_BIT3
#define MPP_VO      HI_BIT5

typedef struct SampleVoModeMux {
    HI_U32 u32WndNum;
    HI_U32 u32Square;
    HI_U32 u32Row;
    HI_U32 u32Col;
} SampleVoModeMux;

typedef SAMPLE_VI_CONFIG_S ViCfg;
typedef SAMPLE_VO_CONFIG_S VoCfg;
typedef VB_CONFIG_S VbCfg;

/*
 * VPSS通道配置，用来设置一个通道的属性
 * VPSS channel config. Used to set the attributes of a channel.
 */
typedef struct VpssChnCfg {
    int id; // VpssChn ID
    VPSS_CHN_ATTR_S attr; // VpssChn attributes
} VpssChnCfg;

/*
 * VPSS配置.每个VpssCfg对应1个VPSS组和1个或多个VPSS通道
 * VPSS config. Each VpssCfg corresponds to 1 VPSS group and 1 or more VPSS channels.
 */
typedef struct VpssCfg {
    VPSS_GRP grpId; // VpssGrp ID
    VPSS_GRP_ATTR_S grpAttr; // VpssGrp attributes

    int chnNum; // Configure the number of chnnels used
    VpssChnCfg chnCfgs[VPSS_MAX_PHY_CHN_NUM]; // pssChnCfg array, the first chnNum-1 elements are valid
} VpssCfg;

/*
 * MppSess集合.
 * MppSess对应的create()函数会将需要的cfg值复制到对象中，并启动session
 * MppSess未提供构造函数，user只能通过create()函数创建对象，并用MppSess_destroy()销毁对象
 * MppSess中定义了当前MppSess使用的资源ID，viCfg, vpssCfg, vpssGrp, vpssChn0, vpssChn1,
 * 这些值由create()设置，used未标识的组件的对应ID会被置为-1.这些资源ID从create()传入的
 * 参数xxxCfg中获取，复制到对象中可简化APP使用
 * 目前没有定义VI的channel ID，其总是会与VPSS绑定后使用，通过VPSS chn即可获得VI的数据
 *
 * MppSess collection.
 * The create() function corresponding to MppSess will copy the required cfg value to the object and start the session.
 * MppSess does not provide a constructor. The user can only create an object through the create() function
 * and destroy the object with MppSess_destroy().
 * MppSess defines the resource ID, vpssGrp, vpssChn0, vpssChn1, vdecChn, vencChn currently used by MppSess.
 * These values are set by create(), and the corresponding IDs of components not identified by used will be set to -1.
 * These resource IDs are obtained from the parameter xxxCfg passed in by create()
 * and copied to the object to simplify APP use.
 * There is currently no defined channel ID of the VI, it will always be used after being bound to the VPSS,
 * and the VI data can be obtained through the VPSS chn.
 */
typedef struct MppSess {
    uint32_t used; // The set of enabled components, set by the create() function

    ViCfg viCfg; // Optional ViCfg
    VpssCfg vpssCfg; // Optional VpssCfg

    VPSS_GRP vpssGrp; // VPSS group ID, -1 means that the corresponding MPP component is not started.
    VPSS_CHN vpssChn0; // VPSS channel[0] ID, -1 means that the corresponding MPP component is not started.
    VPSS_CHN vpssChn1; // VPSS channel[1] ID, -1 means that the corresponding MPP component is not started.
} MppSess;



typedef struct SampleStoreInfo {
    SIZE_S stSize[2];
    VENC_GOP_MODE_E enGopMode;
    VENC_GOP_ATTR_S stGopAttr;
    SAMPLE_RC_E     enRcMode;
        VPSS_GRP vpssGrp;
    VPSS_CHN vpssChn0;
    VPSS_CHN vpssChn1;
    VDEC_CHN vdecChn;
    VENC_CHN vencChn;
    VoCfg voCfg;
    VbCfg vbCfg;
    ViCfg viCfg;
    MppSess *viSess; // VI(sensor)+VPSS
    int vpssFd;
    PIC_SIZE_E enPicSize;
    HI_U32  u32BlkSize;
} SampleStoreInfo;

int SAMPLE_VENC_H265_H264(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef SAMPLE_STORE_H */

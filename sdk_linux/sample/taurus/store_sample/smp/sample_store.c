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

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "hi_mipi_tx.h"
#include "sample_comm.h"
#include "sample_comm_ive.h"
#include "sample_store.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define PIC_WIDTH 720
#define PIC_HEIGHT 576
#define USLEEP_TIME   1000 // 1000: usleep time, in microseconds
#define G_MBUF_LENGTH 50 // 50: length of g_mbuf
#define ALIGN_DOWN_SIZE 2

#define G_MBUF_ARRAY_SUBSCRIPT_0     0
#define G_MBUF_ARRAY_SUBSCRIPT_1     1
#define G_MBUF_ARRAY_SUBSCRIPT_2     2
#define G_MBUF_ARRAY_SUBSCRIPT_3     3
#define G_MBUF_ARRAY_SUBSCRIPT_4     4
#define G_MBUF_ARRAY_SUBSCRIPT_5     5
#define G_MBUF_ARRAY_SUBSCRIPT_6     6
#define G_MBUF_ARRAY_SUBSCRIPT_7     7
#define G_MBUF_ARRAY_SUBSCRIPT_8     8
#define G_MBUF_ARRAY_SUBSCRIPT_9     9
#define G_MBUF_ARRAY_SUBSCRIPT_10    10
#define G_MBUF_ARRAY_SUBSCRIPT_11    11
#define G_MBUF_ARRAY_SUBSCRIPT_12    12
#define G_MBUF_ARRAY_SUBSCRIPT_13    13
#define G_MBUF_ARRAY_SUBSCRIPT_14    14
#define G_MBUF_ARRAY_SUBSCRIPT_15    15
#define G_MBUF_ARRAY_SUBSCRIPT_16    16

#define LANE_ID_SUBSCRIPT_0    0
#define LANE_ID_SUBSCRIPT_1    1
#define LANE_ID_SUBSCRIPT_2    2
#define LANE_ID_SUBSCRIPT_3    3

static unsigned char g_mBuf[G_MBUF_LENGTH];
SampleStoreInfo g_sampleStoreInfo = {0};
static SampleVoModeMux g_sampleVoModeMux = {0};
static VPSS_CHN VpssChn[2] = {0, 1};
static PIC_SIZE_E enSize[2] = {PIC_D1_MIPI, PIC_D1_MIPI};
static VENC_CHN VencChn[2] = {0, 1};
static PAYLOAD_TYPE_E enPayLoad[2] = {PT_H265, PT_H264};
static HI_U32 u32Profile[2] = {0, 0};
static HI_BOOL bRcnRefShareBuf = HI_TRUE;
static HI_BOOL abChnEnable[VPSS_MAX_PHY_CHN_NUM] = {1, 1, 0};

static VO_PUB_ATTR_S stVoPubAttr = {0};
static VO_VIDEO_LAYER_ATTR_S  stLayerAttr    = {0};
static VO_CSC_S               stVideoCSC     = {0};
static RECT_S stDefDispRect  = {0, 0, 800, 480};
static SIZE_S stDefImageSize = {800, 480};
static HI_U32 u32SupplementConfig = HI_FALSE;


HI_VOID SAMPLE_VOU_SYS_Exit(void)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();
}

HI_VOID SAMPLE_VO_GetUserLayerAttr(VO_VIDEO_LAYER_ATTR_S *pstLayerAttr, SIZE_S *pstDevSize)
{
    pstLayerAttr->bClusterMode = HI_FALSE;
    pstLayerAttr->bDoubleFrame = HI_FALSE;
    pstLayerAttr->enDstDynamicRange = DYNAMIC_RANGE_SDR8;
    pstLayerAttr->enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;

    pstLayerAttr->stDispRect.s32X = 0;
    pstLayerAttr->stDispRect.s32Y = 0;
    pstLayerAttr->stDispRect.u32Height = pstDevSize->u32Height;
    pstLayerAttr->stDispRect.u32Width  = pstDevSize->u32Width;

    pstLayerAttr->stImageSize.u32Height = pstDevSize->u32Height;
    pstLayerAttr->stImageSize.u32Width = pstDevSize->u32Width;

    return;
}

HI_VOID SAMPLE_VO_GetUserChnAttr(VO_CHN_ATTR_S *pstChnAttr, SIZE_S *pstDevSize, HI_S32 VoChnNum)
{
    HI_S32 i;
    for (i = 0; i < VoChnNum; i++) {
        pstChnAttr[i].bDeflicker = HI_FALSE;
        pstChnAttr[i].u32Priority = 0;
        pstChnAttr[i].stRect.s32X = 0;
        pstChnAttr[i].stRect.s32Y = 0;
        pstChnAttr[i].stRect.u32Height = pstDevSize->u32Height;
        pstChnAttr[i].stRect.u32Width = pstDevSize->u32Width;
        }

    return;
}

/*
 * 打开MIPI Tx设备
 * Open MIPI Tx device
 */
HI_S32 SampleOpenMipiTxFd(HI_VOID)
{
    HI_S32 fd;

    fd = open("/dev/hi_mipi_tx", O_RDWR);
    if (fd < 0) {
        printf("open hi_mipi_tx dev failed\n");
    }
    return fd;
}

/*
 * 关闭MIPI Tx设备
 * Close MIPI Tx device
 */
HI_VOID SampleCloseMipiTxFd(HI_S32 fd)
{
    close(fd);
    return;
}

/*
 * 获取MIPI Tx配置信息
 * Get MIPI Tx config information
 */
HI_VOID SAMPLE_GetMipiTxConfig(combo_dev_cfg_t *pstMipiTxConfig)
{
    /*
     * 用户需要设置MIPI设备配置
     * User need set MIPI device config
     */
    pstMipiTxConfig->devno = 0;
    pstMipiTxConfig->lane_id[LANE_ID_SUBSCRIPT_0] = 0;
    pstMipiTxConfig->lane_id[LANE_ID_SUBSCRIPT_1] = 1;
    // -1: 2 lane mode configuration,lane_id[4] = {0, 1, -1, -1}
    pstMipiTxConfig->lane_id[LANE_ID_SUBSCRIPT_2] = -1;
    // -1: 2 lane mode configuration,lane_id[4] = {0, 1, -1, -1}
    pstMipiTxConfig->lane_id[LANE_ID_SUBSCRIPT_3] = -1;
    pstMipiTxConfig->output_mode = OUTPUT_MODE_DSI_VIDEO;
    pstMipiTxConfig->output_format = OUT_FORMAT_RGB_24_BIT;
    pstMipiTxConfig->video_mode = BURST_MODE;
    pstMipiTxConfig->sync_info.vid_pkt_size = 480; // 480: received packet size
    pstMipiTxConfig->sync_info.vid_hsa_pixels = 10; // 10: The number of pixels in the input line sync pulse area
    pstMipiTxConfig->sync_info.vid_hbp_pixels = 50; // 50: Number of pixels in blanking area after input
    pstMipiTxConfig->sync_info.vid_hline_pixels = 590; // 590: The total number of pixels detected per line
    pstMipiTxConfig->sync_info.vid_vsa_lines = 4; // 4: Number of frame sync pulse lines detected
    pstMipiTxConfig->sync_info.vid_vbp_lines = 20; // 20: Number of blanking area lines after frame sync pulse
    pstMipiTxConfig->sync_info.vid_vfp_lines = 20; // 20：Number of blanking area lines before frame sync pulse
    pstMipiTxConfig->sync_info.vid_active_lines = 800; // 800: VACTIVE rows
    pstMipiTxConfig->sync_info.edpi_cmd_size = 0; // 0: Write memory command bytes
    pstMipiTxConfig->phy_data_rate = 359; // 359: MIPI Tx output rate
    pstMipiTxConfig->pixel_clk = 29878; // 29878: pixel clock. The unit is KHz

    return;
}

/*
 * 设置MIPI Tx配置信息
 * Set MIPI Tx config information
 */
HI_S32 SAMPLE_SetMipiTxConfig(HI_S32 fd, combo_dev_cfg_t *pstMipiTxConfig)
{
    HI_S32 s32Ret = ioctl(fd, HI_MIPI_TX_SET_DEV_CFG, pstMipiTxConfig);
    if (s32Ret != HI_SUCCESS) {
        printf("MIPI_TX SET_DEV_CONFIG failed\n");
        SampleCloseMipiTxFd(fd);
        return s32Ret;
    }
    return s32Ret;
}

/*
 * 设置MIPI Tx设备属性
 * Set MIPI Tx device attr
 */
HI_S32 SampleSetMipiTxDevAttr(HI_S32 fd)
{
    HI_S32 s32Ret;
    combo_dev_cfg_t stMipiTxConfig;

    SAMPLE_GetMipiTxConfig(&stMipiTxConfig); // USER GET MIPI DEV CONFIG
    s32Ret = SAMPLE_SetMipiTxConfig(fd, &stMipiTxConfig); // USER SET MIPI DEV CONFIG

    return s32Ret;
}

/*
 * 初始化MIPI Tx设备
 * Init MIPI Tx device
 */
HI_S32 SAMPLE_USER_INIT_MIPITx(HI_S32 fd, cmd_info_t *pcmd_info)
{
    HI_S32 s32Ret = ioctl(fd, HI_MIPI_TX_SET_CMD, pcmd_info);
    if (s32Ret !=  HI_SUCCESS) {
        printf("MIPI_TX SET CMD failed\n");
        SampleCloseMipiTxFd(fd);
        return s32Ret;
    }

    return HI_SUCCESS;
}

/*
 * 配置MIPI Tx初始化序列
 * Config MIPI Tx initialization sequence
 */
HI_S32 SampleVoInitMipiTxScreen(HI_S32 fd)
{
    HI_S32 s32Ret;
    cmd_info_t cmd_info;
    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x77;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x13;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 6; // 6: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x08ef;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x77;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x10;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 6; // 6: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xC0;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x63;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x00;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xC1;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x10;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x02;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xC2;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x08;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x18CC;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xB0;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x40;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0xC9;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x8F;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x0D;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x11;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0x07;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0x02;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_8] = 0x09;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_9] = 0x09;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_10] = 0x1F;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_11] = 0x04;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_12] = 0x50;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_13] = 0x0F;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_14] = 0xE4;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_15] = 0x29;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_16] = 0xDF;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 17; // 17: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xB1;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x40;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0xCB;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0xD3;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x11;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x8F;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0x04;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_8] = 0x08;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_9] = 0x07;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_10] = 0x1C;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_11] = 0x06;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_12] = 0x53;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_13] = 0x12;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_14] = 0x63;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_15] = 0xEB;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_16] = 0xDF;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 17; // 17: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x77;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x11;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 6; // 6: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x65b0;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x34b1;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x87b2;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x80b3;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x49b5;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x85b7;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x20b8;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x10b9;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x78c1;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x78c2;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x88d0;
    cmd_info.data_type = 0x23;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(100000);  // 100000: The process hangs for a period of time, in microseconds

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE0;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x19;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x02;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 4; // 4: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE1;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x05;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0xA0;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x07;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0xA0;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x04;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0xA0;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0x06;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_8] = 0xA0;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_9] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_10] = 0x44;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_11] = 0x44;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 12; // 12: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE2;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_8] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_9] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_10] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_11] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_12] = 0x00;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 13; // 13: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE3;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x33;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x33;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 5; // 5: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE4;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x44;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x44;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE5;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x0D;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x31;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0xAF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x0F;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0x33;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_8] = 0xAF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_9] = 0x09;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_10] = 0x2D;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_11] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_12] = 0xAF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_13] = 0x0B;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_14] = 0x2F;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_15] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_16] = 0xAF;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 17; // 17: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE6;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x33;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x33;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 5; // 5: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE7;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x44;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x44;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x0C;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x30;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0xAF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x0E;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0x32;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_8] = 0xAF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_9] = 0x08;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_10] = 0x2C;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_11] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_12] = 0xAF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_13] = 0x0A;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_14] = 0x2E;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_15] = 0xC8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_16] = 0xAF;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 17; // 17: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xEB;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x02;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0xE4;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0xE4;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x44;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0x40;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 8; // 8: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xEC;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x3C;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x00;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xED;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0xAB;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x89;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x76;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x54;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_7] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_8] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_9] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_10] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_11] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_12] = 0x10;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_13] = 0x45;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_14] = 0x67;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_15] = 0x98;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_16] = 0xBA;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 17; // 17: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xEF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x08;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x08;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x08;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x45;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x3F;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_6] = 0x54;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 7; // 7: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x77;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x00;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 6; // 6: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x77;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x13;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 6; // 6: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x0E;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x11;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 4; // 4: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(120000); // 120000: The process hangs for a period of time, in microseconds

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x0C;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(10000); // 10000: The process hangs for a period of time, in microseconds

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xE8;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x00;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 3; // 3: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(10000); // 10000: The process hangs for a period of time, in microseconds

    memset_s(g_mBuf, sizeof(g_mBuf), 0, G_MBUF_LENGTH);
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_0] = 0xFF;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_1] = 0x77;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_2] = 0x01;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_3] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_4] = 0x00;
    g_mBuf[G_MBUF_ARRAY_SUBSCRIPT_5] = 0x00;
    cmd_info.devno = 0;
    cmd_info.cmd_size = 6; // 6: command data size
    cmd_info.data_type = 0x29;
    cmd_info.cmd = g_mBuf;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(10000); // 10000: The process hangs for a period of time, in microseconds

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x11;
    cmd_info.data_type = 0x05;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(150000); // 150000: The process hangs for a period of time, in microseconds

    cmd_info.devno = 0;
    cmd_info.cmd_size = 0x29;
    cmd_info.data_type = 0x05;
    cmd_info.cmd = NULL;
    s32Ret = SAMPLE_USER_INIT_MIPITx(fd, &cmd_info);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }
    usleep(USLEEP_TIME);
    usleep(50000); // 50000: The process hangs for a period of time, in microseconds

    return HI_SUCCESS;
}

/*
 * 启用MIPI Tx设备
 * Enable MIPI Tx device
 */
HI_S32 SAMPLE_VO_ENABLE_MIPITx(HI_S32 fd)
{
    HI_S32 s32Ret = ioctl(fd, HI_MIPI_TX_ENABLE);
    if (s32Ret != HI_SUCCESS) {
        printf("MIPI_TX enable failed\n");
        return s32Ret;
    }

    return s32Ret;
}

/*
 * 禁用MIPI Tx设备
 * Disable MIPI Tx device
 */
HI_S32 SAMPLE_VO_DISABLE_MIPITx(HI_S32 fd)
{
    HI_S32 s32Ret = ioctl(fd, HI_MIPI_TX_DISABLE);
    if (s32Ret != HI_SUCCESS) {
        printf("MIPI_TX disable failed\n");
        return s32Ret;
    }

    return s32Ret;
}

/*
 * 设置VO至MIPI通路，获取MIPI设备
 * Set VO config to MIPI, get MIPI device
 */
HI_S32 SAMPLE_VO_CONFIG_MIPI(HI_S32* mipiFD)
{
    HI_S32 s32Ret;
    HI_S32  fd;

    /*
     * 打开MIPI FD
     * Open MIPI FD
     */
    fd = SampleOpenMipiTxFd();
    if (fd < 0) {
        return HI_FAILURE;
    }
	*mipiFD = fd;

    /*
     * 设置MIPI Tx设备属性
     * Set MIPI Tx device attribution
     */
    s32Ret = SampleSetMipiTxDevAttr(fd);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    usleep(10000); // 10000: The process hangs for a period of time, in microseconds
    system("cd /sys/class/gpio/;echo 55 > export;echo out > gpio55/direction;echo 1 > gpio55/value");;
    system("cd /sys/class/gpio/;echo 5 > export;echo out > gpio5/direction;echo 1 > gpio5/value");
    usleep(200000); // 200000: The process hangs for a period of time, in microseconds
    system("echo 0 > /sys/class/gpio/gpio5/value");
    usleep(200000); // 200000: The process hangs for a period of time, in microseconds
    system("echo 1 > /sys/class/gpio/gpio5/value");
    usleep(20000); // 20000: The process hangs for a period of time, in microseconds

    /*
     * 配置MIPI Tx初始化序列
     * Config MIPI Tx initialization sequence
     */
    s32Ret = SampleVoInitMipiTxScreen(fd);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    /*
     * 启用MIPI Tx设备
     * Enable MIPI Tx device
     */
    s32Ret = SAMPLE_VO_ENABLE_MIPITx(fd);
    if (s32Ret != HI_SUCCESS) {
        return s32Ret;
    }

    return s32Ret;
}

/*
 * 获得mipi设备的宽和高
 * Get mipi device Height and width
 */
HI_S32 SampleCommVoGetWhMipi(VO_INTF_SYNC_E enIntfSync, HI_U32* pu32W, HI_U32* pu32H, HI_U32* pu32Frm)
{
    switch (enIntfSync) {
        case VO_OUTPUT_1080P24:
            *pu32W = 1920; // 1920: VO_OUTPUT_1080P24-Width
            *pu32H = 1080; // 1080: VO_OUTPUT_1080P24-Height
            *pu32Frm = 24; // 24: VO_OUTPUT_1080P24-Frame rate
            break;
        case VO_OUTPUT_1080P25:
            *pu32W = 1920; // 1920: VO_OUTPUT_1080P25-Width
            *pu32H = 1080; // 1080: VO_OUTPUT_1080P25-Height
            *pu32Frm = 25; // 25: VO_OUTPUT_1080P25-Frame rate
            break;
        case VO_OUTPUT_1080P30:
            *pu32W = 1920; // 1920: VO_OUTPUT_1080P30-Width
            *pu32H = 1080; // 1080: VO_OUTPUT_1080P30-Height
            *pu32Frm = 30; // 30: VO_OUTPUT_1080P30-Frame rate
            break;
        case VO_OUTPUT_720P50:
            *pu32W = 1280; // 1280: VO_OUTPUT_720P50-Width
            *pu32H = 720; // 720: VO_OUTPUT_720P50-Height
            *pu32Frm = 50; // 50: VO_OUTPUT_720P50-Frame rate
            break;
        case VO_OUTPUT_720P60:
            *pu32W = 1280; // 1280: VO_OUTPUT_720P60-Width
            *pu32H = 720; // 720: VO_OUTPUT_720P60-Height
            *pu32Frm = 60; // 60: VO_OUTPUT_720P60-Frame rate
            break;
        case VO_OUTPUT_1080P50:
            *pu32W = 1920; // 1920: VO_OUTPUT_1080P50-Width
            *pu32H = 1080; // 1080: VO_OUTPUT_1080P50-Height
            *pu32Frm = 50; // 50: VO_OUTPUT_1080P50-Frame rate
            break;
        case VO_OUTPUT_1080P60:
            *pu32W = 1920; // 1920: VO_OUTPUT_1080P60-Width
            *pu32H = 1080; // 1080: VO_OUTPUT_1080P60-Height
            *pu32Frm = 60; // 60: VO_OUTPUT_1080P60-Frame rate
            break;
        case VO_OUTPUT_USER:
            *pu32W = 800; // 800: VO_OUTPUT_USER-Width
            *pu32H = 480; // 480: VO_OUTPUT_USER-Height
            *pu32Frm = 60; // 60: VO_OUTPUT_USER-Frame rate
            break;
        default:
            SAMPLE_PRT("vo enIntfSync %d not support, please config self!\n", enIntfSync);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SampleCommVoStartDevMipi(VO_DEV VoDev, VO_PUB_ATTR_S* pstPubAttr)
{
    HI_S32 s32Ret;
    VO_USER_INTFSYNC_INFO_S stUserInfo = {0};

    stUserInfo.bClkReverse = HI_TRUE;
    stUserInfo.u32DevDiv = 1;
    stUserInfo.u32PreDiv = 1;
    stUserInfo.stUserIntfSyncAttr.enClkSource = VO_CLK_SOURCE_PLL;
    stUserInfo.stUserIntfSyncAttr.stUserSyncPll.u32Fbdiv = 244; // 244: PLL integer frequency multiplier coefficient
    stUserInfo.stUserIntfSyncAttr.stUserSyncPll.u32Frac = 0x1A36;
    stUserInfo.stUserIntfSyncAttr.stUserSyncPll.u32Refdiv = 4; // 4: PLL reference clock frequency division coefficient
    // 7: PLL first stage output frequency division coefficient
    stUserInfo.stUserIntfSyncAttr.stUserSyncPll.u32Postdiv1 = 7;
    // 7: PLL second stage output frequency division coefficient
    stUserInfo.stUserIntfSyncAttr.stUserSyncPll.u32Postdiv2 = 7;
    HI_U32 u32Framerate = 60; // 60: device frame rate

    /*
     * 配置视频输出设备的公共属性
     * Set the common properties of the video output device
     */
    s32Ret = HI_MPI_VO_SetPubAttr(VoDev, pstPubAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    /*
     * 设置设备用户时序下设备帧率
     * Set the device frame rate under the device user timing
     */
    s32Ret = HI_MPI_VO_SetDevFrameRate(VoDev, u32Framerate);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    /*
     * 设置用户接口时序信息，用于配置时钟源、时钟大小和时钟分频比
     *
     * Set user interface timing information, used to configure clock source,
     * clock size and clock frequency division ratio
     */
    s32Ret = HI_MPI_VO_SetUserIntfSyncInfo(VoDev, &stUserInfo);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    /*
     * 启用视频输出设备
     * Enable video output device
     */
    s32Ret = HI_MPI_VO_Enable(VoDev);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 SampleCommVoStartChnModeMux(SAMPLE_VO_MODE_E enMode)
{
    int s32Ret;
    switch (enMode) {
        case VO_MODE_1MUX:
            g_sampleVoModeMux.u32WndNum = 1;
            g_sampleVoModeMux.u32Square = 1;
            break;
        case VO_MODE_2MUX:
            g_sampleVoModeMux.u32WndNum = 2; // 2: 2MUX-WndNum
            g_sampleVoModeMux.u32Square = 2; // 2: 2MUX-Square
            break;
        case VO_MODE_4MUX:
            g_sampleVoModeMux.u32WndNum = 4; // 4: 4MUX-WndNum
            g_sampleVoModeMux.u32Square = 2; // 2: 4MUX-Square
            break;
        case VO_MODE_8MUX:
            g_sampleVoModeMux.u32WndNum = 8; // 8: 8MUX-WndNum
            g_sampleVoModeMux.u32Square = 3; // 3: 8MUX-Square
            break;
        case VO_MODE_9MUX:
            g_sampleVoModeMux.u32WndNum = 9; // 9: 9MUX-WndNum
            g_sampleVoModeMux.u32Square = 3; // 3: 9MUX-Square
            break;
        case VO_MODE_16MUX:
            g_sampleVoModeMux.u32WndNum = 16; // 16: 16MUX-WndNum
            g_sampleVoModeMux.u32Square = 4; // 4: 16MUX-Square
            break;
        case VO_MODE_25MUX:
            g_sampleVoModeMux.u32WndNum = 25; // 25: 25MUX-WndNum
            g_sampleVoModeMux.u32Square = 5; // 5: 25MUX-Square
            break;
        case VO_MODE_36MUX:
            g_sampleVoModeMux.u32WndNum = 36; // 36: 36MUX-WndNum
            g_sampleVoModeMux.u32Square = 6; // 6: 36MUX-Square
            break;
        case VO_MODE_49MUX:
            g_sampleVoModeMux.u32WndNum = 49; // 49: 49MUX-WndNum
            g_sampleVoModeMux.u32Square = 7; // 7: 49MUX-Square
            break;
        case VO_MODE_2X4:
            g_sampleVoModeMux.u32WndNum = 8; // 8: 2X4-WndNum
            g_sampleVoModeMux.u32Square = 3; // 3: 2X4-Square
            g_sampleVoModeMux.u32Row    = 4; // 4: 2X4-Row
            g_sampleVoModeMux.u32Col    = 2; // 2: 2X4-Col
            break;
        default:
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SampleCommVoStartChnMipi(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode)
{
    HI_S32 i;
    HI_S32 s32Ret    = HI_SUCCESS;
    HI_U32 u32Width  = 0;
    HI_U32 u32Height = 0;
    VO_CHN_ATTR_S         stChnAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    s32Ret = SampleCommVoStartChnModeMux(enMode);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "for SampleCommVoStartChnModeMux FAIL, s32Ret=%x\n", s32Ret);

    /*
     * 获取视频层属性
     * Get video layer properties
     */
    s32Ret = HI_MPI_VO_GetVideoLayerAttr(VoLayer, &stLayerAttr);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "for HI_MPI_VO_GetVideoLayerAttr FAIL, s32Ret=%x\n", s32Ret);
    u32Width  = stLayerAttr.stImageSize.u32Width;
    u32Height = stLayerAttr.stImageSize.u32Height;
    SAMPLE_PRT("enMode:%d, u32Width:%d, u32Height:%d, u32Square:%d\n", enMode,
        u32Width, u32Height, g_sampleVoModeMux.u32Square);

    for (i = 0; i < g_sampleVoModeMux.u32WndNum; i++) {
        if (enMode == VO_MODE_1MUX  || enMode == VO_MODE_2MUX  || enMode == VO_MODE_4MUX  ||
            enMode == VO_MODE_8MUX  || enMode == VO_MODE_9MUX  || enMode == VO_MODE_16MUX ||
            enMode == VO_MODE_25MUX || enMode == VO_MODE_36MUX || enMode == VO_MODE_49MUX) {
            stChnAttr.stRect.s32X       = HI_ALIGN_DOWN((u32Width / g_sampleVoModeMux.u32Square) *
                (i % g_sampleVoModeMux.u32Square), ALIGN_DOWN_SIZE);
            stChnAttr.stRect.s32Y       = HI_ALIGN_DOWN((u32Height / g_sampleVoModeMux.u32Square) *
                (i / g_sampleVoModeMux.u32Square), ALIGN_DOWN_SIZE);
            stChnAttr.stRect.u32Width   = HI_ALIGN_DOWN(u32Width / g_sampleVoModeMux.u32Square, ALIGN_DOWN_SIZE);
            stChnAttr.stRect.u32Height  = HI_ALIGN_DOWN(u32Height / g_sampleVoModeMux.u32Square, ALIGN_DOWN_SIZE);
            stChnAttr.u32Priority       = 0;
            stChnAttr.bDeflicker        = HI_FALSE;
        } else if (enMode == VO_MODE_2X4) {
            stChnAttr.stRect.s32X       = HI_ALIGN_DOWN((u32Width / g_sampleVoModeMux.u32Col) *
                (i % g_sampleVoModeMux.u32Col), ALIGN_DOWN_SIZE);
            stChnAttr.stRect.s32Y       = HI_ALIGN_DOWN((u32Height / g_sampleVoModeMux.u32Row) *
                (i / g_sampleVoModeMux.u32Col), ALIGN_DOWN_SIZE);
            stChnAttr.stRect.u32Width   = HI_ALIGN_DOWN(u32Width / g_sampleVoModeMux.u32Col, ALIGN_DOWN_SIZE);
            stChnAttr.stRect.u32Height  = HI_ALIGN_DOWN(u32Height / g_sampleVoModeMux.u32Row, ALIGN_DOWN_SIZE);
            stChnAttr.u32Priority       = 0;
            stChnAttr.bDeflicker        = HI_FALSE;
        }

        /*
         * 配置指定视频输出通道的属性
         * Set properties for the specified video output channel
         */
        s32Ret = HI_MPI_VO_SetChnAttr(VoLayer, i, &stChnAttr);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "for HI_MPI_VO_SetChnAttr FAIL, s32Ret=%x\n", s32Ret);

        /*
         * 设置指定视频输出通道的旋转角度
         * Set video output channel rotation angle
         */
        s32Ret = HI_MPI_VO_SetChnRotation(VoLayer, i, ROTATION_90);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "for HI_MPI_VO_SetChnRotation FAIL, s32Ret=%x\n", s32Ret);

        /*
         * 启用指定的视频输出通道
         * Enables the specified video output channel
         */
        s32Ret = HI_MPI_VO_EnableChn(VoLayer, i);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "for HI_MPI_VO_EnableChn FAIL, s32Ret=%x\n", s32Ret);
    }

    return HI_SUCCESS;
}

static HI_VOID StVoPubAttrCfg(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    HI_ASSERT(pstVoConfig);
    /*
     * 定义视频输出公共属性结构体
     * Define the video output public attribute structure
     */
    stVoPubAttr.enIntfType  = VO_INTF_MIPI;
    stVoPubAttr.enIntfSync  = VO_OUTPUT_USER;
    stVoPubAttr.stSyncInfo.bSynm = 0;
    stVoPubAttr.stSyncInfo.bIop = 1;
    stVoPubAttr.stSyncInfo.u8Intfb = 0;

    stVoPubAttr.stSyncInfo.u16Hmid = 1;
    stVoPubAttr.stSyncInfo.u16Bvact = 1;
    stVoPubAttr.stSyncInfo.u16Bvbb = 1;
    stVoPubAttr.stSyncInfo.u16Bvfb = 1;

    stVoPubAttr.stSyncInfo.bIdv = 0;
    stVoPubAttr.stSyncInfo.bIhs = 0;
    stVoPubAttr.stSyncInfo.bIvs = 0;

    stVoPubAttr.stSyncInfo.u16Hact = 480; // 480: Horizontal effective area. Unit: pixel
    stVoPubAttr.stSyncInfo.u16Hbb = 60; // 60: Horizontal blanking of the rear shoulder. Unit: pixel
    stVoPubAttr.stSyncInfo.u16Hfb = 50; // 50: Horizontal blanking of the front shoulder. Unit: pixel
    stVoPubAttr.stSyncInfo.u16Hpw = 10; // 10: The width of the horizontal sync signal. Unit: pixel
    stVoPubAttr.stSyncInfo.u16Vact = 800; // 800: Vertical effective area. Unit: line
    stVoPubAttr.stSyncInfo.u16Vbb = 24; // 24: Vertical blanking of the rear shoulder.  Unit: line
    stVoPubAttr.stSyncInfo.u16Vfb = 20; // 20: Vertical blanking of the front shoulder.  Unit: line
    stVoPubAttr.stSyncInfo.u16Vpw = 4; // 4: The width of the vertical sync signal. Unit: line
    stVoPubAttr.u32BgColor  = pstVoConfig->u32BgColor;
}

static HI_VOID StLayerAttrCfg(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    HI_ASSERT(pstVoConfig);
    stLayerAttr.bClusterMode     = HI_FALSE;
    stLayerAttr.bDoubleFrame    = HI_FALSE;
    stLayerAttr.enPixFormat       = pstVoConfig->enPixFormat;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    stLayerAttr.enDstDynamicRange     = pstVoConfig->enDstDynamicRange;
}

/*
 * 启动VO到MIPI lcd通路
 * Start VO to MIPI lcd
 */
HI_S32 SampleCommVoStartMipi(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    HI_S32 s32Ret;

    HI_ASSERT(pstVoConfig);
    StVoPubAttrCfg(pstVoConfig);
    s32Ret = SampleCommVoStartDevMipi(pstVoConfig->VoDev, &stVoPubAttr);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "StartDevMipi FAIL, ret=%x\n", s32Ret);
    /*
     * 获得MIPI设备的宽和高
     * Get MIPI device Height and width
     */
    s32Ret = SampleCommVoGetWhMipi(stVoPubAttr.enIntfSync,
        &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height, &stLayerAttr.u32DispFrmRt);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL, "VoGetWhMipi fail, err(%#x)\n", s32Ret);

    StLayerAttrCfg(pstVoConfig);

    /*
     * 如果存在变化，设置显示矩形框
     * Set display rectangle if changed
     */
    if (memcmp(&pstVoConfig->stDispRect, &stDefDispRect, sizeof(RECT_S)) != 0) {
        memcpy_s(&stLayerAttr.stDispRect, sizeof(stLayerAttr.stDispRect),
            &pstVoConfig->stDispRect, sizeof(RECT_S));
    }

    /*
     * 如果存在变化，设置图片大小
     * Set image size if changed
     */
    if (memcmp(&pstVoConfig->stImageSize, &stDefImageSize, sizeof(SIZE_S)) != 0) {
        memcpy_s(&stLayerAttr.stImageSize, sizeof(stLayerAttr.stImageSize),
            &pstVoConfig->stImageSize, sizeof(SIZE_S));
    }
    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width = 480; // 480: video layer canvas Width
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height = 800; // 800: video layer canvas Height

    if (pstVoConfig->u32DisBufLen) {
        /*
         * 设置显示缓冲的长度
         * Set buffer length
         */
        s32Ret = HI_MPI_VO_SetDisplayBufLen(pstVoConfig->VoDev, pstVoConfig->u32DisBufLen);
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL, "HI_MPI_VO_SetDisplayBufLen fail, err(%#x)\n", s32Ret);
    }
    if (VO_PART_MODE_MULTI == pstVoConfig->enVoPartMode) {
        /*
         * 设置视频层的分割模式
         * Set the segmentation mode of the video layer
         */
        s32Ret = HI_MPI_VO_SetVideoLayerPartitionMode(pstVoConfig->VoDev, pstVoConfig->enVoPartMode);
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL, "SetVideoLayerMode fail, err(%#x)\n", s32Ret);
    }

    s32Ret = SAMPLE_COMM_VO_StartLayer(pstVoConfig->VoDev, &stLayerAttr); // start layer
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL, "VO_StartLayer fail, err(%#x)\n", s32Ret);

    if (VO_INTF_MIPI == pstVoConfig->enVoIntfType) {
        s32Ret = HI_MPI_VO_GetVideoLayerCSC(pstVoConfig->VoDev, &stVideoCSC); // get video layerCSC
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL, "GetVideoLayerCSC fail, err(%#x)\n", s32Ret);
        stVideoCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;
        s32Ret = HI_MPI_VO_SetVideoLayerCSC(pstVoConfig->VoDev, &stVideoCSC); // Set video layer CSC
        SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL, "SetVideoLayerCSC fail, err(%#x)\n", s32Ret);
    }

    s32Ret = SampleCommVoStartChnMipi(pstVoConfig->VoDev, pstVoConfig->enVoMode); // start vo channels
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, FAIL1, "VoStartChnMipi fail, err(%#x)\n", s32Ret);
    return HI_SUCCESS;

FAIL1:
    SAMPLE_COMM_VO_StopLayer(pstVoConfig->VoDev);
FAIL:
    SAMPLE_COMM_VO_StopDev(pstVoConfig->VoDev);
    return s32Ret;
}

HI_VOID StVoParamCfg(VoCfg *self)
{
    SAMPLE_COMM_VO_GetDefConfig(self);
    self->enDstDynamicRange = DYNAMIC_RANGE_SDR8;

    self->enVoIntfType = VO_INTF_MIPI; /* set VO int type */
    self->enIntfSync = VO_OUTPUT_USER; /* set VO output information */

    self->enPicSize = g_sampleStoreInfo.enPicSize;
}

VENC_GOP_MODE_E SAMPLE_VENC_GetGopMode(void)
{
    char c;
    HI_S32 s32Ret = HI_SUCCESS;
    VENC_GOP_MODE_E enGopMode = 0;

Begin_Get:

    printf("please input choose gop mode!\n");
    printf("\t 0) NORMALP.\n");
    printf("\t 1) DUALP.\n");
    printf("\t 2) SMARTP.\n");

    while ((c = getchar()) != '\n') {
        switch (c) {
            case '0':
                enGopMode = VENC_GOPMODE_NORMALP;
                break;
            case '1':
                enGopMode = VENC_GOPMODE_DUALP;
                break;
            case '2':
                enGopMode = VENC_GOPMODE_SMARTP;
                break;
            default:
                s32Ret = HI_FAILURE;
                SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, Begin_Get,
                    "input gopmode: %c, is invaild!\n", c);
        }
    }

    return enGopMode;
}

SAMPLE_RC_E SAMPLE_VENC_GetRcMode(void)
{
    char c;
    HI_S32 s32Ret = HI_SUCCESS;
    SAMPLE_RC_E  enRcMode = 0;

Begin_Get:

    printf("please input choose rc mode!\n");
    printf("\t c) cbr.\n");
    printf("\t v) vbr.\n");
    printf("\t a) avbr.\n");
    printf("\t x) cvbr.\n");
    printf("\t q) qvbr.\n");
    printf("\t f) fixQp\n");

    while ((c = getchar()) != '\n') {
        switch (c) {
            case 'c':
                enRcMode = SAMPLE_RC_CBR;
                break;
            case 'v':
                enRcMode = SAMPLE_RC_VBR;
                break;
            case 'a':
                enRcMode = SAMPLE_RC_AVBR;
                break;
            case 'q':
                enRcMode = SAMPLE_RC_QVBR;
                break;
            case 'x':
                enRcMode = SAMPLE_RC_CVBR;
                break;
            case 'f':
                enRcMode = SAMPLE_RC_FIXQP;
                break;
            default:
                s32Ret = HI_FAILURE;
                SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, Begin_Get,
                    "input rcmode: %c, is invaild!\n", c);
        }
    }

    return enRcMode;
}

HI_S32 SAMPLE_VENC_SYS_Init(HI_U32 u32SupplementConfig, SAMPLE_SNS_TYPE_E enSnsType)
{
    HI_S32 s32Ret;
    HI_U64 u64BlkSize;
    VB_CONFIG_S stVbConf;
    PIC_SIZE_E enSnsSize;
    SIZE_S stSnsSize;

    memset_s(&stVbConf, sizeof(VB_CONFIG_S), 0, sizeof(VB_CONFIG_S));

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    u64BlkSize = COMMON_GetPicBufferSize(stSnsSize.u32Width, stSnsSize.u32Height,
        PIXEL_FORMAT_YVU_SEMIPLANAR_422, DATA_BITWIDTH_8, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[0].u64BlkSize = u64BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 10; // 10: Number of cache blocks per cache pool

    u64BlkSize = COMMON_GetPicBufferSize(PIC_WIDTH, PIC_HEIGHT, PIXEL_FORMAT_YVU_SEMIPLANAR_422,
        DATA_BITWIDTH_8, COMPRESS_MODE_SEG, DEFAULT_ALIGN);
    stVbConf.astCommPool[1].u64BlkSize = u64BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 10; // 10: Number of cache blocks per cache pool

    stVbConf.u32MaxPoolCnt = 2; // 2: The number of buffer pools that can be accommodated in the entire system

    if (u32SupplementConfig == 0) {
        s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    } else {
        s32Ret = SAMPLE_COMM_SYS_InitWithVbSupplement(&stVbConf, u32SupplementConfig);
    }
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_VI_Init(SAMPLE_VI_CONFIG_S *pstViConfig, HI_BOOL bLowDelay, HI_U32 u32SupplementConfig)
{
    HI_S32  s32Ret;
    SAMPLE_SNS_TYPE_E enSnsType;
    ISP_CTRL_PARAM_S stIspCtrlParam;
    HI_U32 u32FrameRate;

    enSnsType = pstViConfig->astViInfo[0].stSnsInfo.enSnsType;

    pstViConfig->as32WorkingViId[0] = 0;
    pstViConfig->astViInfo[0].stSnsInfo.MipiDev =
        SAMPLE_COMM_VI_GetComboDevBySensor(pstViConfig->astViInfo[0].stSnsInfo.enSnsType, 0);
    pstViConfig->astViInfo[0].stSnsInfo.s32BusId = 0;
    pstViConfig->astViInfo[0].stDevInfo.enWDRMode = WDR_MODE_NONE;

    // if (HI_TRUE == bLowDelay) {
     pstViConfig->astViInfo[0].stPipeInfo.enMastPipeMode = 0;
    // } else {
    //     pstViConfig->astViInfo[0].stPipeInfo.enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE;
    // }
    s32Ret = SAMPLE_VENC_SYS_Init(u32SupplementConfig, enSnsType);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("Init SYS err for %#x!\n", s32Ret);
        return s32Ret;
    }

    pstViConfig->astViInfo[0].stPipeInfo.aPipe[1] = -1;  // -1: the value of aPipe[1]
    pstViConfig->astViInfo[0].stPipeInfo.aPipe[2] = -1;  // -1: the value of aPipe[2]
    pstViConfig->astViInfo[0].stPipeInfo.aPipe[3]  = -1;  // -1: the value of aPipe[3]
    pstViConfig->astViInfo[0].stChnInfo.enVideoFormat = VIDEO_FORMAT_LINEAR;
    pstViConfig->astViInfo[0].stChnInfo.enCompressMode = COMPRESS_MODE_SEG; // COMPRESS_MODE_SEG

    s32Ret = SAMPLE_COMM_VI_SetParam(pstViConfig);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    SAMPLE_COMM_VI_GetFrameRateBySensor(enSnsType, &u32FrameRate);

    s32Ret = HI_MPI_ISP_GetCtrlParam(pstViConfig->astViInfo[0].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("HI_MPI_ISP_GetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    // 30: ISP statistics update frequency is equal to (FrameRate / 30)
    stIspCtrlParam.u32StatIntvl  = u32FrameRate / 30;

    s32Ret = HI_MPI_ISP_SetCtrlParam(pstViConfig->astViInfo[0].stPipeInfo.aPipe[0], &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("HI_MPI_ISP_SetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_COMM_SYS_Exit();
        SAMPLE_PRT("SAMPLE_COMM_VI_StartVi failed with %d!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_ModifyResolution(SAMPLE_SNS_TYPE_E enSnsType, PIC_SIZE_E *penSize, SIZE_S *pstSize)
{
    HI_S32 s32Ret;
    SIZE_S stSnsSize;
    PIC_SIZE_E enSnsSize;

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    *penSize = enSnsSize;
    pstSize->u32Width  = stSnsSize.u32Width;
    pstSize->u32Height = stSnsSize.u32Height;

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_CheckSensor(SAMPLE_SNS_TYPE_E enSnsType, SIZE_S stSize)
{
    HI_S32 s32Ret;
    SIZE_S          stSnsSize;
    PIC_SIZE_E      enSnsSize;

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    if ((stSnsSize.u32Width < stSize.u32Width) || (stSnsSize.u32Height < stSize.u32Height)) {
        SAMPLE_PRT("Sensor size is (%d,%d), but encode chnl is (%d,%d) !\n",
            stSnsSize.u32Width, stSnsSize.u32Height, stSize.u32Width, stSize.u32Height);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VENC_VPSS_Init(VPSS_GRP VpssGrp, HI_BOOL* pabChnEnable, DYNAMIC_RANGE_E enDynamicRange,
    PIXEL_FORMAT_E enPixelFormat, SIZE_S stSize[], SAMPLE_SNS_TYPE_E enSnsType)
{
    HI_S32 i;
    HI_S32 s32Ret;
    PIC_SIZE_E      enSnsSize;
    SIZE_S          stSnsSize;
    VPSS_GRP_ATTR_S stVpssGrpAttr = {0};
    VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_PHY_CHN_NUM];

    s32Ret = SAMPLE_COMM_VI_GetSizeBySensor(enSnsType, &enSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_VI_GetSizeBySensor failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSnsSize, &stSnsSize);
    if (HI_SUCCESS != s32Ret) {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        return s32Ret;
    }

    stVpssGrpAttr.enDynamicRange = enDynamicRange;
    stVpssGrpAttr.enPixelFormat  = enPixelFormat;
    stVpssGrpAttr.u32MaxW        = stSnsSize.u32Width;
    stVpssGrpAttr.u32MaxH        = stSnsSize.u32Height;
    stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1; // -1: The source frame rate, no frame rate control
    stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1; // -1: The destination frame rate, no frame rate control
    stVpssGrpAttr.bNrEn = HI_TRUE;
    stVpssGrpAttr.stNrAttr.enNrType = VPSS_NR_TYPE_VIDEO;
    stVpssGrpAttr.stNrAttr.enNrMotionMode = NR_MOTION_MODE_NORMAL;
    stVpssGrpAttr.stNrAttr.enCompressMode = COMPRESS_MODE_FRAME;

    for (i = 0; i < VPSS_MAX_PHY_CHN_NUM; i++) {
        if (HI_TRUE == pabChnEnable[i]) {
            stVpssChnAttr[i].u32Width                     = stSize[i].u32Width;
            stVpssChnAttr[i].u32Height                    = stSize[i].u32Height;
            stVpssChnAttr[i].enChnMode                    = VPSS_CHN_MODE_USER;
            stVpssChnAttr[i].enCompressMode               = COMPRESS_MODE_NONE; // COMPRESS_MODE_SEG
            stVpssChnAttr[i].enDynamicRange               = enDynamicRange;
            stVpssChnAttr[i].enPixelFormat                = enPixelFormat;
            stVpssChnAttr[i].stFrameRate.s32SrcFrameRate  = -1; // -1: The source frame rate, no frame rate control
            stVpssChnAttr[i].stFrameRate.s32DstFrameRate  = -1; // -1: The destination frame rate, no frame rate control
            stVpssChnAttr[i].u32Depth                     = 0;
            stVpssChnAttr[i].bMirror                      = HI_FALSE;
            stVpssChnAttr[i].bFlip                        = HI_FALSE;
            stVpssChnAttr[i].enVideoFormat                = VIDEO_FORMAT_LINEAR;
            stVpssChnAttr[i].stAspectRatio.enMode         = ASPECT_RATIO_NONE;
        }
    }

    s32Ret = SAMPLE_COMM_VPSS_Start(VpssGrp, pabChnEnable, &stVpssGrpAttr, stVpssChnAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("start VPSS fail for %#x!\n", s32Ret);
    }

    return s32Ret;
}

/*
 * 初始化Vi配置
 * Init ViCfg
 */
HI_S32 ViCfgInit(ViCfg *self)
{
    HI_S32 s32Ret;
    HI_ASSERT(self);
    if (memset_s(self, sizeof(*self), 0, sizeof(*self)) != EOK) {
        HI_ASSERT(0);
    }

    SAMPLE_COMM_VI_GetSensorInfo(self);
    if (SAMPLE_SNS_TYPE_BUTT == self->astViInfo[0].stSnsInfo.enSnsType) {
        SAMPLE_PRT("Not set SENSOR%d_TYPE !\n", 0);
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_VENC_CheckSensor(self->astViInfo[0].stSnsInfo.enSnsType, g_sampleStoreInfo.stSize[0]);
    if (s32Ret != HI_SUCCESS) {
        s32Ret = SAMPLE_VENC_ModifyResolution(self->astViInfo[0].stSnsInfo.enSnsType,
            &enSize[0], &g_sampleStoreInfo.stSize[0]);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "SAMPLE_VENC_ModifyResolution failed!\n", s32Ret);
    }

    return HI_SUCCESS;
}

HI_VOID ViCfgSetDevPipeChnInfo(ViCfg *self)
{
    HI_ASSERT(self);
    self->s32WorkingViNum = 1;
    self->astViInfo[0].stDevInfo.ViDev = 0;
    self->astViInfo[0].stPipeInfo.aPipe[0] = 0;
    self->astViInfo[0].stChnInfo.ViChn = 0;
    self->astViInfo[0].stChnInfo.enDynamicRange = DYNAMIC_RANGE_SDR8;
    self->astViInfo[0].stChnInfo.enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
}

static HI_VOID Pause(HI_VOID)
{
    printf("---------------press Enter key to exit!---------------\n");
    (void)getchar();
}

static HI_VOID EnRcModeGopModeCfg(SampleStoreInfo *storeInfo)
{
    storeInfo->enRcMode = SAMPLE_RC_CBR;
    storeInfo->enGopMode = VENC_GOPMODE_NORMALP;
}

int SAMPLE_VENC_H265_H264(void)
{
    HI_S32 s32Ret;
    HI_S32             fd = 0;

    for (int i = 0; i < 2; i++) { // 2: channel number
        s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSize[i], &g_sampleStoreInfo.stSize[i]);
        SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "SAMPLE_COMM_SYS_GetPicSize failed!\n", s32Ret);
    }
    /* start vi */
    s32Ret = ViCfgInit(&g_sampleStoreInfo.viCfg);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "ViCfgInit failed!\n", s32Ret);
    ViCfgSetDevPipeChnInfo(&g_sampleStoreInfo.viCfg);
    /* set VO config to mipi, get mipi device  */
    s32Ret = SAMPLE_VO_CONFIG_MIPI(&fd);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VI_STOP, "CONFIG MIPI FAIL.s32Ret:0x%x\n", s32Ret);

    s32Ret = SAMPLE_VENC_VI_Init(&g_sampleStoreInfo.viCfg, HI_FALSE, u32SupplementConfig);
    SAMPLE_CHECK_EXPR_RET(s32Ret != HI_SUCCESS, s32Ret, "Init VI err for %#x!\n", s32Ret);
    /* start vpss */
    s32Ret = SAMPLE_VENC_VPSS_Init(0, abChnEnable, DYNAMIC_RANGE_SDR8, PIXEL_FORMAT_YVU_SEMIPLANAR_420,
        g_sampleStoreInfo.stSize, g_sampleStoreInfo.viCfg.astViInfo[0].stSnsInfo.enSnsType);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VI_STOP, "Init VPSS err for %#x!\n", s32Ret);

    s32Ret = SAMPLE_COMM_VI_Bind_VPSS(0, 0, 0);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VPSS_STOP, "VI Bind VPSS err for %#x!\n", s32Ret);

    /* start vo */
    StVoParamCfg(&g_sampleStoreInfo.voCfg);
    s32Ret = SampleCommVoStartMipi(&g_sampleStoreInfo.voCfg);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VPSS_STOP, "start vo FAIL. s32Ret: 0x%x\n", s32Ret);

    s32Ret = SAMPLE_COMM_VPSS_Bind_VO(0, VpssChn[1], 0, 0);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VO_STOP, "vo bind vpss FAIL. s32Ret: 0x%x\n", s32Ret);
    SAMPLE_PRT("vpssChn:%d\n", VpssChn[1]);

    /*
     * 开启编码流
     * Start stream venc
     */
    EnRcModeGopModeCfg(&g_sampleStoreInfo);
    s32Ret = SAMPLE_COMM_VENC_GetGopAttr(g_sampleStoreInfo.enGopMode, &g_sampleStoreInfo.stGopAttr);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VI_VPSS_UNBIND, "Venc Get GopAttr for %#x!\n", s32Ret);

    /*
     * 编码h.265
     * Encode h.265
     */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[0], enPayLoad[0], enSize[0],
        g_sampleStoreInfo.enRcMode, u32Profile[0], bRcnRefShareBuf, &g_sampleStoreInfo.stGopAttr);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VI_VPSS_UNBIND, "Venc Start failed for %#x!\n", s32Ret);

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(0, VpssChn[0], VencChn[0]);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H265_STOP, "Venc Get GopAttr failed for %#x!\n", s32Ret);

    /* encode h.264 */
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn[1], enPayLoad[1], enSize[1],
        g_sampleStoreInfo.enRcMode, u32Profile[1], bRcnRefShareBuf, &g_sampleStoreInfo.stGopAttr);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H265_UnBind, "Venc Start failed for %#x!\n", s32Ret);

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(0, VpssChn[1], VencChn[1]);
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H264_STOP, "Venc bind Vpss failed for %#x!\n", s32Ret);

    /* stream save process */
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(VencChn, 2); // 2: channel number
    SAMPLE_CHECK_EXPR_GOTO(s32Ret != HI_SUCCESS, EXIT_VENC_H264_UnBind, "Start Venc failed!\n", s32Ret);

    Pause();

    /*  exit process */
    SAMPLE_COMM_VENC_StopGetStream();
    SAMPLE_COMM_VPSS_UnBind_VO(0, VpssChn[1], 0, 0);
    SAMPLE_VO_DISABLE_MIPITx(fd);
    SampleCloseMipiTxFd(fd);
    system("echo 0 > /sys/class/gpio/gpio55/value");

EXIT_VO_STOP:
    SAMPLE_COMM_VPSS_UnBind_VO(0, VpssChn[1], 0, 0);
    SAMPLE_COMM_VO_StopVO(&g_sampleStoreInfo.voCfg);
EXIT_VENC_H264_UnBind:
    SAMPLE_COMM_VPSS_UnBind_VENC(0, VpssChn[1], VencChn[1]);
EXIT_VENC_H264_STOP:
    SAMPLE_COMM_VENC_Stop(VencChn[1]);
EXIT_VENC_H265_UnBind:
    SAMPLE_COMM_VPSS_UnBind_VENC(0, VpssChn[0], VencChn[0]);
EXIT_VENC_H265_STOP:
    SAMPLE_COMM_VENC_Stop(VencChn[0]);
EXIT_VI_VPSS_UNBIND:
    SAMPLE_COMM_VI_UnBind_VPSS(0, 0, 0);
EXIT_VPSS_STOP:
    SAMPLE_COMM_VPSS_Stop(0, abChnEnable);
EXIT_VI_STOP:
    SAMPLE_COMM_VI_StopVi(&g_sampleStoreInfo.viCfg);
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

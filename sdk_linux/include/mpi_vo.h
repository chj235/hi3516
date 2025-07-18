/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MPI_VO_H__
#define __MPI_VO_H__

#include "hi_comm_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/* Video Relative Settings */
HI_S32 HI_MPI_VO_SetVideoLayerAttr(VO_LAYER VoLayer, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
HI_S32 HI_MPI_VO_GetVideoLayerAttr(VO_LAYER VoLayer, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);

HI_S32 HI_MPI_VO_EnableVideoLayer(VO_LAYER VoLayer);
HI_S32 HI_MPI_VO_DisableVideoLayer(VO_LAYER VoLayer);

HI_S32 HI_MPI_VO_BindVideoLayer(VO_LAYER VoLayer, VO_DEV VoDev);
HI_S32 HI_MPI_VO_UnBindVideoLayer(VO_LAYER VoLayer, VO_DEV VoDev);

HI_S32 HI_MPI_VO_SetVideoLayerPriority(VO_LAYER VoLayer, HI_U32 u32Priority);
HI_S32 HI_MPI_VO_GetVideoLayerPriority(VO_LAYER VoLayer, HI_U32 *pu32Priority);

HI_S32 HI_MPI_VO_SetVideoLayerCSC(VO_LAYER VoLayer, const VO_CSC_S *pstVideoCSC);
HI_S32 HI_MPI_VO_GetVideoLayerCSC(VO_LAYER VoLayer, VO_CSC_S *pstVideoCSC);

HI_S32 HI_MPI_VO_SetVideoLayerPartitionMode(VO_LAYER VoLayer, VO_PART_MODE_E enPartMode);
HI_S32 HI_MPI_VO_GetVideoLayerPartitionMode(VO_LAYER VoLayer, VO_PART_MODE_E *penPartMode);

HI_S32 HI_MPI_VO_BatchBegin(VO_LAYER VoLayer);
HI_S32 HI_MPI_VO_BatchEnd(VO_LAYER VoLayer);

HI_S32 HI_MPI_VO_SetVideoLayerBoundary(VO_LAYER VoLayer, const VO_LAYER_BOUNDARY_S *pstLayerBoundary);
HI_S32 HI_MPI_VO_GetVideoLayerBoundary(VO_LAYER VoLayer, VO_LAYER_BOUNDARY_S *pstLayerBoundary);

HI_S32 HI_MPI_VO_SetVideoLayerParam(VO_LAYER VoLayer, const VO_LAYER_PARAM_S *pstLayerParam);
HI_S32 HI_MPI_VO_GetVideoLayerParam(VO_LAYER VoLayer, VO_LAYER_PARAM_S *pstLayerParam);

HI_S32 HI_MPI_VO_SetVideoLayerDecompress(VO_LAYER VoLayer, HI_BOOL bSupportDecompress);
HI_S32 HI_MPI_VO_GetVideoLayerDecompress(VO_LAYER VoLayer, HI_BOOL *pbSupportDecompress);

HI_S32 HI_MPI_VO_SetVideoLayerCrop(VO_LAYER VoLayer, const CROP_INFO_S *pstCropInfo);
HI_S32 HI_MPI_VO_GetVideoLayerCrop(VO_LAYER VoLayer, CROP_INFO_S *pstCropInfo);

/* Display relative operations */
HI_S32 HI_MPI_VO_SetPlayToleration(VO_LAYER VoLayer, HI_U32 u32Toleration);
HI_S32 HI_MPI_VO_GetPlayToleration(VO_LAYER VoLayer, HI_U32 *pu32Toleration);

HI_S32 HI_MPI_VO_GetScreenFrame(VO_LAYER VoLayer, VIDEO_FRAME_INFO_S *pstVFrame, HI_S32 s32MilliSec);
HI_S32 HI_MPI_VO_ReleaseScreenFrame(VO_LAYER VoLayer, const VIDEO_FRAME_INFO_S *pstVFrame);

HI_S32 HI_MPI_VO_SetDisplayBufLen(VO_LAYER VoLayer, HI_U32 u32BufLen);
HI_S32 HI_MPI_VO_GetDisplayBufLen(VO_LAYER VoLayer, HI_U32 *pu32BufLen);

/* Channel Relative Operations */
HI_S32 HI_MPI_VO_SetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_ATTR_S *pstChnAttr);
HI_S32 HI_MPI_VO_GetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_ATTR_S *pstChnAttr);

HI_S32 HI_MPI_VO_EnableChn(VO_LAYER VoLayer, VO_CHN VoChn);
HI_S32 HI_MPI_VO_DisableChn(VO_LAYER VoLayer, VO_CHN VoChn);

HI_S32 HI_MPI_VO_SetChnParam(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_PARAM_S *pstChnParam);
HI_S32 HI_MPI_VO_GetChnParam(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_PARAM_S *pstChnParam);

HI_S32 HI_MPI_VO_SetChnDisplayPosition(VO_LAYER VoLayer, VO_CHN VoChn, const POINT_S *pstDispPos);
HI_S32 HI_MPI_VO_GetChnDisplayPosition(VO_LAYER VoLayer, VO_CHN VoChn, POINT_S *pstDispPos);

HI_S32 HI_MPI_VO_SetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn, HI_S32 s32ChnFrmRate);
HI_S32 HI_MPI_VO_GetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn, HI_S32 *ps32ChnFrmRate);

HI_S32 HI_MPI_VO_GetChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstFrame, HI_S32 s32MilliSec);
HI_S32 HI_MPI_VO_ReleaseChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, const VIDEO_FRAME_INFO_S *pstFrame);

HI_S32 HI_MPI_VO_PauseChn(VO_LAYER VoLayer, VO_CHN VoChn);
HI_S32 HI_MPI_VO_ResumeChn(VO_LAYER VoLayer, VO_CHN VoChn);
HI_S32 HI_MPI_VO_StepChn(VO_LAYER VoLayer, VO_CHN VoChn);

HI_S32 HI_MPI_VO_RefreshChn(VO_LAYER VoLayer, VO_CHN VoChn);

HI_S32 HI_MPI_VO_ShowChn(VO_LAYER VoLayer, VO_CHN VoChn);
HI_S32 HI_MPI_VO_HideChn(VO_LAYER VoLayer, VO_CHN VoChn);

HI_S32 HI_MPI_VO_SetZoomInWindow(VO_LAYER VoLayer, VO_CHN VoChn, const VO_ZOOM_ATTR_S *pstZoomAttr);
HI_S32 HI_MPI_VO_GetZoomInWindow(VO_LAYER VoLayer, VO_CHN VoChn, VO_ZOOM_ATTR_S *pstZoomAttr);

HI_S32 HI_MPI_VO_GetChnPTS(VO_LAYER VoLayer, VO_CHN VoChn, HI_U64 *pu64ChnPTS);
HI_S32 HI_MPI_VO_QueryChnStatus(VO_LAYER VoLayer, VO_CHN VoChn, VO_QUERY_STATUS_S *pstStatus);

HI_S32 HI_MPI_VO_SendFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstVFrame, HI_S32 s32MilliSec);

HI_S32 HI_MPI_VO_ClearChnBuf(VO_LAYER VoLayer, VO_CHN VoChn, HI_BOOL bClrAll);

HI_S32 HI_MPI_VO_SetChnBorder(VO_LAYER VoLayer, VO_CHN VoChn, const VO_BORDER_S *pstBorder);
HI_S32 HI_MPI_VO_GetChnBorder(VO_LAYER VoLayer, VO_CHN VoChn, VO_BORDER_S *pstBorder);

HI_S32 HI_MPI_VO_SetChnBoundary(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_BOUNDARY_S *pstChnBoundary);
HI_S32 HI_MPI_VO_GetChnBoundary(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_BOUNDARY_S *pstChnBoundary);

HI_S32 HI_MPI_VO_SetChnRecvThreshold(VO_LAYER VoLayer, VO_CHN VoChn, HI_U32 u32Threshold);
HI_S32 HI_MPI_VO_GetChnRecvThreshold(VO_LAYER VoLayer, VO_CHN VoChn, HI_U32 *pu32Threshold);

HI_S32 HI_MPI_VO_SetChnRotation(VO_LAYER VoLayer, VO_CHN VoChn, ROTATION_E enRotation);
HI_S32 HI_MPI_VO_GetChnRotation(VO_LAYER VoLayer, VO_CHN VoChn, ROTATION_E *penRotation);

HI_S32 HI_MPI_VO_GetChnRegionLuma(VO_LAYER VoLayer, VO_CHN VoChn, VO_REGION_INFO_S *pstRegionInfo,
                                  HI_U64 *pu64LumaData, HI_S32 s32MilliSec);

/* WBC(Write Back Control) Relative Settings */
HI_S32 HI_MPI_VO_SetWBCSource(VO_WBC VoWBC, const VO_WBC_SOURCE_S *pstWBCSource);
HI_S32 HI_MPI_VO_GetWBCSource(VO_WBC VoWBC, VO_WBC_SOURCE_S *pstWBCSources);

HI_S32 HI_MPI_VO_SetWBCAttr(VO_WBC VoWBC, const VO_WBC_ATTR_S *pstWBCAttr);
HI_S32 HI_MPI_VO_GetWBCAttr(VO_WBC VoWBC, VO_WBC_ATTR_S *pstWBCAttr);

HI_S32 HI_MPI_VO_EnableWBC(VO_WBC VoWBC);
HI_S32 HI_MPI_VO_DisableWBC(VO_WBC VoWBC);

HI_S32 HI_MPI_VO_SetWBCMode(VO_WBC VoWBC, VO_WBC_MODE_E enWBCMode);
HI_S32 HI_MPI_VO_GetWBCMode(VO_WBC VoWBC, VO_WBC_MODE_E *penWBCMode);

HI_S32 HI_MPI_VO_SetWBCDepth(VO_WBC VoWBC, HI_U32 u32Depth);
HI_S32 HI_MPI_VO_GetWBCDepth(VO_WBC VoWBC, HI_U32 *pu32Depth);

HI_S32 HI_MPI_VO_GetWBCFrame(VO_WBC VoWBC, VIDEO_FRAME_INFO_S *pstVFrame, HI_S32 s32MilliSec);
HI_S32 HI_MPI_VO_ReleaseWBCFrame(VO_WBC VoWBC, const VIDEO_FRAME_INFO_S *pstVFrame);

HI_S32 HI_MPI_VO_SetChnMirror(VO_LAYER VoLayer, VO_CHN VoChn, VO_MIRROR_MODE_E enMirrorMode);
HI_S32 HI_MPI_VO_GetChnMirror(VO_LAYER VoLayer, VO_CHN VoChn, VO_MIRROR_MODE_E *penMirrorMode);
HI_S32 HI_MPI_VO_SetVideoLayerEarlyDisplay(VO_LAYER VoLayer, HI_BOOL bEarlyDisplayEn);
HI_S32 HI_MPI_VO_GetVideoLayerEarlyDisplay(VO_LAYER VoLayer, HI_BOOL *pbEarlyDisplayEn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_VO_H__ */

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

#ifndef __HI_COMM_SNS_H__
#define __HI_COMM_SNS_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_CMOS_BLACK_LEVEL_S {
    HI_BOOL bUpdate;
    HI_U16  au16BlackLevel[ISP_BAYER_CHN_NUM];
} ISP_CMOS_BLACK_LEVEL_S;

typedef struct hiISP_SNS_ATTR_INFO_S {
    SENSOR_ID            eSensorId;
} ISP_SNS_ATTR_INFO_S;

typedef struct hiISP_CMOS_DEMOSAIC_S {
    HI_BOOL bEnable;
    HI_U8   au8NonDirStr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8   au8NonDirMFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8   au8NonDirHFDetailEhcStr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8   au8DetailSmoothRange[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_DEMOSAIC_S;

#define WDR_MAX_FRAME          2
#define BAYER_CALIBRATION_PARA_NUM 3
typedef struct hiISP_CMOS_BAYERNR_S {
    HI_BOOL  bEnable;
    HI_BOOL  bBnrMonoSensorEn;
    HI_BOOL  bNrLscEnable;
    HI_U8    u8BnrLscMaxGain;
    HI_U16   u16BnrLscCmpStrength;

    HI_U8    au8LutFineStr[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8ChromaStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8WDRFrameStr[WDR_MAX_FRAME_NUM];
    HI_U8    au8FusionFrameStr[WDR_MAX_FRAME_NUM];
    HI_U16   au16CoarseStr[ISP_BAYER_CHN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16   au16LutCoringWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16   au16LutCoringRatio[HI_ISP_BAYERNR_LUT_LENGTH];
} ISP_CMOS_BAYERNR_S;

typedef struct hiISP_CMOS_NOISE_CALIBRATION_S {
    HI_U16   u16CalibrationLutNum;
    HI_FLOAT afCalibrationCoef[BAYER_CALIBTAION_MAX_NUM][BAYER_CALIBRATION_PARA_NUM];
} ISP_CMOS_NOISE_CALIBRATION_S;

typedef struct hiISP_CMOS_DETAIL_S {
    ISP_DE_ATTR_S stAttr;
} ISP_CMOS_DETAIL_S;

typedef struct HiISP_CMOS_SHARPEN_MANUAL_S {
    HI_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM];     /* Range:[0, 127]; Adjust the sharpen strength according to luma.
                                                    Sharpen strength will be weaker when it decrease. */
    HI_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM]; /* Range:[0, 4095]; Format:7.5;Undirectional sharpen strength
                                                    for texture and detail enhancement */
    HI_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM];    /* Range:[0, 4095]; Format:7.5;Directional sharpen strength for
                                                    edge enhancement */
    HI_U16 u16TextureFreq;   /* Range:[0, 4095];Format:6.6; Texture frequency adjustment. Texture and detail will be
                                finer when it increase */
    HI_U16 u16EdgeFreq;      /* Range:[0, 4095];Format:6.6; Edge frequency adjustment.
                                Edge will be narrower and thiner when it increase */
    HI_U8  u8OverShoot;      /* Range:[0, 127]; u8OvershootAmt */
    HI_U8  u8UnderShoot;     /* Range:[0, 127]; u8UndershootAmt */
    HI_U8  u8ShootSupStr;    /* Range:[0, 255]; overshoot and undershoot suppression strength, the amplitude and width
                                of shoot will be decrease when shootSupSt increase */
    HI_U8  u8ShootSupAdj;    /* Range:[0, 15]; overshoot and undershoot suppression adjusting, adjust the edge shoot
                                suppression strength */
    HI_U8  u8DetailCtrl;     /* Range:[0, 255]; Different sharpen strength for detail and edge. When it is bigger than
                                128, detail sharpen strength will be stronger than edge. */
    HI_U8  u8DetailCtrlThr;  /* Range:[0, 255];The threshold of DetailCtrl,it is used to distinguish detail and edge */
    HI_U8  u8EdgeFiltStr;    /* Range:[0, 63]; The strength of edge filtering. */
    HI_U8  u8EdgeFiltMaxCap; /* Range:[0, 47]; The max capacity of edge filtering. */
    HI_U8  u8RGain;          /* Range:[0, 31];Sharpen Gain for Red Area */
    HI_U8  u8GGain;          /* Range:[0, 255]; Sharpen Gain for Green Area */
    HI_U8  u8BGain;          /* Range:[0, 31]; Sharpen Gain for Blue Area */
    HI_U8  u8SkinGain;       /* Range:[0, 31]; Sharpen Gain for Skin Area */
    HI_U16 u16MaxSharpGain;  /* Range:[0, 0x7FF]; Format:8.3; Maximum sharpen gain */
} ISP_CMOS_SHARPEN_MANUAL_S;

typedef struct HiISP_CMOS_SHARPEN_AUTO_S {
    HI_U8  au8LumaWgt[ISP_SHARPEN_LUMA_NUM][ISP_AUTO_ISO_STRENGTH_NUM];  /* Range:[0, 127]; Adjust the sharpen
                                                                           strength according to luma. Sharpen
                                                                           strength will be weaker when it decrease */
    HI_U16 au16TextureStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM]; /* Range:[0, 4095]; Format:7.5;Undirectional
                                                                               sharpen strength for texture and detail
                                                                               enhancement */
    HI_U16 au16EdgeStr[ISP_SHARPEN_GAIN_NUM][ISP_AUTO_ISO_STRENGTH_NUM];    /* Range:[0, 4095]; Format:7.5;Directional
                                                                               sharpen strength for edge enhancement */
    HI_U16 au16TextureFreq[ISP_AUTO_ISO_STRENGTH_NUM];   /* Range:[0, 4095]; Format:6.6;Texture frequency adjustment.
                                                            Texture and detail will be finer when it increase */
    HI_U16 au16EdgeFreq[ISP_AUTO_ISO_STRENGTH_NUM];      /* Range:[0, 4095]; Format:6.6;Edge frequency adjustment.
                                                            Edge will be narrower and thiner when it increase */
    HI_U8  au8OverShoot[ISP_AUTO_ISO_STRENGTH_NUM];      /* Range:[0, 127] */
    HI_U8  au8UnderShoot[ISP_AUTO_ISO_STRENGTH_NUM];     /* Range:[0, 127] */
    HI_U8  au8ShootSupStr[ISP_AUTO_ISO_STRENGTH_NUM];    /* Range:[0, 255]; overshoot and undershoot suppression
                                                            strength, the amplitude and width of shoot will be decrease
                                                            when shootSupSt increase */
    HI_U8  au8ShootSupAdj[ISP_AUTO_ISO_STRENGTH_NUM];    /* Range:[0, 15]; overshoot and undershoot suppression
                                                            adjusting, adjust the edge shoot suppression strength */
    HI_U8  au8DetailCtrl[ISP_AUTO_ISO_STRENGTH_NUM];     /* Range:[0, 255];Different sharpen strength for detail and
                                                            edge. When it is bigger than 128, detail sharpen strength
                                                            will be stronger than edge. */
    HI_U8  au8DetailCtrlThr[ISP_AUTO_ISO_STRENGTH_NUM];  /* Range:[0, 255]; The threshold of DetailCtrl, it is used to
                                                            distinguish detail and edge. */
    HI_U8  au8EdgeFiltStr[ISP_AUTO_ISO_STRENGTH_NUM];    /* Range:[0, 63]; The strength of edge filtering. */
    HI_U8  au8EdgeFiltMaxCap[ISP_AUTO_ISO_STRENGTH_NUM]; /* Range:[0, 47]; The max capacity of edge filtering. */
    HI_U8  au8RGain[ISP_AUTO_ISO_STRENGTH_NUM];          /* Range:[0, 31]; Sharpen Gain for Red Area */
    HI_U8  au8GGain[ISP_AUTO_ISO_STRENGTH_NUM];          /* Range:[0, 255]; Sharpen Gain for Green Area */
    HI_U8  au8BGain[ISP_AUTO_ISO_STRENGTH_NUM];          /* Range:[0, 31]; Sharpen Gain for Blue Area */
    HI_U8  au8SkinGain[ISP_AUTO_ISO_STRENGTH_NUM];       /* Range:[0, 31]; */
    HI_U16 au16MaxSharpGain[ISP_AUTO_ISO_STRENGTH_NUM];  /* Range:[0, 0x7FF]; Format:8.3; */
} ISP_CMOS_SHARPEN_AUTO_S;

typedef struct hiISP_CMOS_SHARPEN_S {
    HI_U8 u8SkinUmin;
    HI_U8 u8SkinVmin;
    HI_U8 u8SkinUmax;
    HI_U8 u8SkinVmax;
    ISP_CMOS_SHARPEN_MANUAL_S stManual;
    ISP_CMOS_SHARPEN_AUTO_S   stAuto;
} ISP_CMOS_SHARPEN_S;

typedef struct hiISP_CMOS_EDGEMARK_S {
    HI_BOOL bEnable;               /* RW; Range:[0, 1]; Format:1.0;Enable/Disable Edge Mark */
    HI_U8   u8Threshold;           /* RW; range: [0, 255];  Format:8.0; */
    HI_U32  u32Color;              /* RW; range: [0, 0xFFFFFF];  Format:32.0; */
} ISP_CMOS_EDGEMARK_S;

typedef struct hiISP_CMOS_HLC_S {
    HI_BOOL bEnable;         /* RW; Range:[0, 1]; Format:1.0;Enable/Disable HLC module */
    HI_U8   u8LumaThr;       /* RW; range: [0, 255];  Format:8.0; */
    HI_U8   u8LumaTarget;    /* RW; range: [0, 255];  Format:8.0; */
} ISP_CMOS_HLC_S;

typedef struct hiISP_CMOS_DRC_S {
    HI_BOOL bEnable;
    ISP_OP_TYPE_E enOpType;

    HI_U16  u16ManualStrength;
    HI_U16  u16AutoStrength;

    HI_U8   u8SpatialFltCoef;
    HI_U8   u8RangeFltCoef;
    HI_U8   u8ContrastControl;
    HI_S8   s8DetailAdjustFactor;
    HI_U8   u8RangeAdaMax;

    HI_U8   u8FltScaleFine;
    HI_U8   u8FltScaleCoarse;
    HI_U8   u8GradRevMax;
    HI_U8   u8GradRevThr;

    HI_U8   u8BrightGainLmt;
    HI_U8   u8BrightGainLmtStep;
    HI_U8   u8DarkGainLmtY;
    HI_U8   u8DarkGainLmtC;
    HI_U8   u8PDStrength;
    HI_U8   u8LocalMixingBrightMax;
    HI_U8   u8LocalMixingBrightMin;
    HI_U8   u8LocalMixingBrightThr;
    HI_S8   s8LocalMixingBrightSlo;
    HI_U8   u8LocalMixingDarkMax;
    HI_U8   u8LocalMixingDarkMin;
    HI_U8   u8LocalMixingDarkThr;
    HI_S8   s8LocalMixingDarkSlo;
    HI_U16  u16ColorCorrectionLut[HI_ISP_DRC_CC_NODE_NUM];
    HI_U16  u16ToneMappingValue[HI_ISP_DRC_TM_NODE_NUM];

    HI_U8   u8Asymmetry;
    HI_U8   u8SecondPole;
    HI_U8   u8Stretch;
    HI_U8   u8Compress;

    HI_U8   u8CurveSel;

    HI_U16 au16Xpoint[HI_ISP_DRC_CUBIC_POINT_NUM];
    HI_U16 au16Ypoint[HI_ISP_DRC_CUBIC_POINT_NUM];
    HI_U16 au16Slope[HI_ISP_DRC_CUBIC_POINT_NUM];
} ISP_CMOS_DRC_S;

typedef struct hiISP_CMOS_WDR_S {
    HI_BOOL  bFusionMode;
    HI_BOOL  bMotionComp;

    HI_U16   u16ShortThr;
    HI_U16   u16LongThr;

    HI_BOOL  bForceLong;
    HI_U16   u16ForceLongLowThr;
    HI_U16   u16ForceLongHigThr;

    HI_BOOL  bShortExpoChk;
    HI_U16   u16ShortCheckThd;
    HI_BOOL  bMDRefFlicker;

    HI_U8    au8MdThrLowGain[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8MdThrHigGain[ISP_AUTO_ISO_STRENGTH_NUM];

    ISP_BNR_MODE_E  enBnrMode;
    HI_U16   au16FusionThr[WDR_MAX_FRAME];
    HI_U8    u8MdtStillThd;
    HI_U8    u8MdtLongBlend;
} ISP_CMOS_WDR_S;

typedef struct HiISP_CMOS_DEHAZE_S {
    HI_BOOL       bEnable;           /* RW;Range:[0,1];Format:1.0; */
    HI_BOOL       bUserLutEnable;    /* RW;Range:[0,1];0:Auto Lut 1:User Lut */
    ISP_OP_TYPE_E enOpType;
    HI_U8         u8AutoStrength;
    HI_U8         u8ManualStrength;
} ISP_CMOS_DEHAZE_S;

typedef struct hiISP_CMOS_PREGAMMA_S {
    HI_BOOL bEnable;
    HI_U32  au32PreGamma[PREGAMMA_NODE_NUM];
} ISP_CMOS_PREGAMMA_S;

#define GAMMA_NODE_NUMBER      257   /* Update NODE NUMBER */
#define GAMMA_EXT_NODE_NUMBER  1025  /* Update NODE NUMBER */
#define GAMMA_OUTSEG_NUMBER    8     /* Update NODE NUMBER */
typedef struct hiISP_CMOS_GAMMA_S {
    HI_U16  au16Gamma[GAMMA_EXT_NODE_NUMBER];
} ISP_CMOS_GAMMA_S;

typedef struct hiISP_CMOS_SENSOR_MAX_RESOLUTION_S {
    HI_U32  u32MaxWidth;
    HI_U32  u32MaxHeight;
} ISP_CMOS_SENSOR_MAX_RESOLUTION_S;

typedef struct hiISP_CMOS_DPC_S {
    HI_U16  au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16  au16BlendRatio[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_DPC_S;

typedef struct hiISP_LSC_CABLI_TABLE_S {
    HI_U16 au16R_Gain[HI_ISP_LSC_GRID_POINTS];
    HI_U16 au16Gr_Gain[HI_ISP_LSC_GRID_POINTS];
    HI_U16 au16Gb_Gain[HI_ISP_LSC_GRID_POINTS];
    HI_U16 au16B_Gain[HI_ISP_LSC_GRID_POINTS];
} ISP_LSC_CABLI_TABLE_S;

typedef struct hiISP_BNR_LSC_CALIB_TABLE_S {
    HI_U16 au16R_Gain[HI_ISP_RLSC_POINTS];
    HI_U16 au16Gr_Gain[HI_ISP_RLSC_POINTS];
    HI_U16 au16Gb_Gain[HI_ISP_RLSC_POINTS];
    HI_U16 au16B_Gain[HI_ISP_RLSC_POINTS];
} ISP_BNR_LSC_CALIB_TABLE_S;

typedef struct hiISP_CMOS_LSC_S {
    HI_U16  u16MeshStrength;
    HI_U8   u8MeshScale;
    ISP_LSC_CABLI_TABLE_S astLscCalibTable[HI_ISP_MLSC_GAIN_LUT_NUM];
    ISP_BNR_LSC_CALIB_TABLE_S stBNRLscCalibTable;
} ISP_CMOS_LSC_S;

typedef struct hiISP_ACS_Y_SHADING_LUT_S {
    HI_U16 u16GParamHighCT[HI_ISP_LSC_GRID_POINTS];
    HI_U16 u16GParamLowCT[HI_ISP_LSC_GRID_POINTS];
} ISP_ACS_Y_SHADING_LUT_S;

typedef struct hiISP_ACS_COLOR_SHADING_LUT_S {
    HI_FLOAT f32AvgRgMap[HI_ISP_LSC_GRID_POINTS];
    HI_FLOAT f32AvgBgMap[HI_ISP_LSC_GRID_POINTS];
    HI_FLOAT f32ProfRgMap[HI_ISP_LSC_GRID_POINTS];
    HI_FLOAT f32ProfBgMap[HI_ISP_LSC_GRID_POINTS];
} ISP_ACS_COLOR_SHADING_LUT_S;

typedef struct hiISP_ACS_CALIB_PARAM_S {
    HI_S16   u16LightIndex[HI_ISP_ACS_LIGHT_NUM * HI_ISP_ACS_CHN_NUM];
    HI_FLOAT f32ModelARMin;
    HI_FLOAT f32ModelARStep;
    HI_FLOAT f32ModelABMin;
    HI_FLOAT f32ModelABStep;
    HI_S16   u16LightTypeGHigh;
    HI_S16   u16LightTypeGLow;
} ISP_ACS_CALIB_PARAM_S;

typedef struct hiISP_CMOS_ACS_S {
    HI_BOOL                      bAcsEn;
    HI_U16                       u16AcsShadingStr;
    ISP_ACS_CALIB_PARAM_S        stAcsCalibParam;
    ISP_ACS_Y_SHADING_LUT_S      stAcsYShadingLut;
    ISP_ACS_COLOR_SHADING_LUT_S  stAcsColorShadingLut;
} ISP_CMOS_ACS_S;

typedef struct hiISP_CMOS_CA_S {
    HI_BOOL   bEnable;
    HI_U16    au16YRatioLut[HI_ISP_CA_YRATIO_LUT_LENGTH];  /* 1.10bit  Y Ratio For UV; Max = 2047 FW Limit */
    HI_S16    as16ISORatio[ISP_AUTO_ISO_STRENGTH_NUM];     /* 1.10bit  ISO Ratio  For UV,Max = 2047 FW Limi */
} ISP_CMOS_CA_S;

typedef struct hiISP_CMOS_CLUT_S {
    HI_BOOL bEnable;
    HI_U32  u32GainR;
    HI_U32  u32GainG;
    HI_U32  u32GainB;
    ISP_CLUT_LUT_S stClutLut;
} ISP_CMOS_CLUT_S;

typedef struct hiISP_CMOS_EXPANDER_POINT_S {
    HI_U16 u16X;                    /* RW;Range:[0x0,0x100];Format:8.0;The X point of the knee */
    HI_U32 u32Y;                    /* RW;Range:[0x0,0x100000];Format:21.0;The Y point of the  knee */
} ISP_CMOS_EXPANDER_POINT_S;

#define ISP_EXPANDER_POINT_NUM_MAX (EXPANDER_NODE_NUM - 1)
typedef struct hiISP_CMOS_EXPANDER_S {
    HI_BOOL bEnable;
    HI_U8   u8BitDepthIn;            /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of input */
    HI_U8   u8BitDepthOut;           /* RW;Range:[0xC,0x14];Format:5.0;The Bit depth of output */
    HI_U16  u16ExpanderPointNum;     /* Range:[1, 256] */
    ISP_CMOS_EXPANDER_POINT_S astExpanderPoint[ISP_EXPANDER_POINT_NUM_MAX];
} ISP_CMOS_EXPANDER_S;

typedef struct hiISP_CMOS_GE_S {
    HI_BOOL bEnable;                                 /* RW,Range: [   0, 1]      */
    HI_U8  u8Slope;                                  /* RW,Range: [   0, 0xE]    */
    HI_U8  u8SensiSlope;                             /* RW,Range: [   0, 0xE]    */
    HI_U16 u16SensiThr;                              /* RW,Range: [   0, 0x3FFF] */
    HI_U16 au16Threshold[ISP_AUTO_ISO_STRENGTH_NUM]; /* RW,Range: [   0, 0x3FFF] */
    HI_U16 au16Strength[ISP_AUTO_ISO_STRENGTH_NUM];  /* RW,Range: [   0, 0x100]  */
    HI_U16 au16NpOffset[ISP_AUTO_ISO_STRENGTH_NUM];  /* RW,Range: [0x200, 0x3FFF] */
} ISP_CMOS_GE_S;

typedef struct hiISP_CMOS_ANTIFALSECOLOR_S {
    HI_BOOL bEnable;                                               /* RW;Range:[0x0,0x1];Format:1.0; */
    HI_U8   au8AntiFalseColorThreshold[ISP_AUTO_ISO_STRENGTH_NUM]; /* RW;Range:[0x0,0x20];Format:6.0; */
    HI_U8   au8AntiFalseColorStrength[ISP_AUTO_ISO_STRENGTH_NUM];  /* RW;Range:[0x0,0x1F];Format:5.0; */
} ISP_CMOS_ANTIFALSECOLOR_S;

typedef struct hiISP_CMOS_LDCI_S {
    HI_BOOL  bEnable;
    HI_U8    u8GaussLPFSigma;
    HI_U8    au8HePosWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8HePosSigma[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8HePosMean[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8HeNegWgt[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8HeNegSigma[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U8    au8HeNegMean[ISP_AUTO_ISO_STRENGTH_NUM];
    HI_U16   au16BlcCtrl[ISP_AUTO_ISO_STRENGTH_NUM];
} ISP_CMOS_LDCI_S;

typedef struct hiISP_CMOS_LCAC_MANUAL_S {
    HI_U8  u8DePurpleCrStr;
    HI_U8  u8DePurpleCbStr;
} ISP_CMOS_LCAC_MANUAL_S;

typedef struct hiISP_CMOS_LCAC_AUTO_S {
    HI_U8  au8DePurpleCrStr[ISP_EXP_RATIO_STRENGTH_NUM];
    HI_U8  au8DePurpleCbStr[ISP_EXP_RATIO_STRENGTH_NUM];
} ISP_CMOS_LCAC_AUTO_S;

typedef struct hiISP_CMOS_LCAC_S {
    HI_BOOL                 bEnable;
    ISP_OP_TYPE_E           OpMode;
    HI_U16                  u16VarThr;
    HI_U16                  au16RLumaThd[ISP_LCAC_DET_NUM];
    HI_U16                  au16GLumaThd[ISP_LCAC_DET_NUM];
    HI_U16                  au16BLumaThd[ISP_LCAC_DET_NUM];
    HI_U16                  au16YLumaThd[ISP_LCAC_DET_NUM];
    HI_S16                  as16CbCrRatio[ISP_LCAC_DET_NUM];
    HI_U16                  u16PurpleDetRange;
    ISP_CMOS_LCAC_MANUAL_S  stManual;
    ISP_CMOS_LCAC_AUTO_S    stAuto;
} ISP_CMOS_LCAC_S;

typedef struct hiISP_CMOS_SENSOR_MODE_S {
    HI_U32  u32SensorID;
    HI_U8   u8SensorMode;
    HI_BOOL bValidDngRawFormat;
    DNG_RAW_FORMAT_S stDngRawFormat;
} ISP_CMOS_SENSOR_MODE_S;

typedef struct hiISP_CMOS_DNG_COLORPARAM_S {
    ISP_DNG_WBGAIN_S stWbGain1; /* the calibration White balance gain of colorcheker in A Light */
    ISP_DNG_WBGAIN_S stWbGain2; /* the calibration White balance gain of colorcheker in D50 Light */
} ISP_CMOS_DNG_COLORPARAM_S;

typedef struct hiISP_CMOS_WDR_SWITCH_ATTR_S {
    HI_U32   au32ExpRatio[EXP_RATIO_NUM];
} ISP_CMOS_WDR_SWITCH_ATTR_S;

typedef union hiISP_CMOS_ALG_KEY_U {
    HI_U64  u64Key;
    struct {
        HI_U64  bit1Drc             : 1;   /* [0] */
        HI_U64  bit1Demosaic        : 1;   /* [1] */
        HI_U64  bit1PreGamma        : 1;   /* [2] */
        HI_U64  bit1Gamma           : 1;   /* [3] */
        HI_U64  bit1Sharpen         : 1;   /* [4] */
        HI_U64  bit1EdgeMark        : 1;   /* [5] */
        HI_U64  bit1Hlc             : 1;   /* [6] */
        HI_U64  bit1Ldci            : 1;   /* [7] */
        HI_U64  bit1Dpc             : 1;   /* [8] */
        HI_U64  bit1Lsc             : 1;   /* [9] */
        HI_U64  bit1Ge              : 1;   /* [10] */
        HI_U64  bit1AntiFalseColor  : 1;   /* [11] */
        HI_U64  bit1BayerNr         : 1;   /* [12] */
        HI_U64  bit1Detail          : 1;   /* [13] */
        HI_U64  bit1Ca              : 1;   /* [14] */
        HI_U64  bit1Expander        : 1;   /* [15] */
        HI_U64  bit1Clut            : 1;   /* [16] */
        HI_U64  bit1Wdr             : 1;   /* [17] */
        HI_U64  bit1Dehaze          : 1;   /* [18] */
        HI_U64  bit1Lcac            : 1;   /* [19] */
        HI_U64  bit1Acs             : 1;   /* [20] */
        HI_U64  bit43Rsv            : 43;   /* [21:63] */
    };
} ISP_CMOS_ALG_KEY_U;

typedef struct hiISP_CMOS_DEFAULT_S {
    ISP_CMOS_ALG_KEY_U               unKey;
    const ISP_CMOS_DRC_S             *pstDrc;
    const ISP_CMOS_DEMOSAIC_S        *pstDemosaic;
    const ISP_CMOS_PREGAMMA_S        *pstPreGamma;
    const ISP_CMOS_GAMMA_S           *pstGamma;
    const ISP_CMOS_SHARPEN_S         *pstSharpen;
    const ISP_CMOS_EDGEMARK_S        *pstEdgeMark;
    const ISP_CMOS_HLC_S             *pstHlc;
    const ISP_CMOS_LDCI_S            *pstLdci;
    const ISP_CMOS_DPC_S             *pstDpc;
    const ISP_CMOS_LSC_S             *pstLsc;
    const ISP_CMOS_GE_S              *pstGe;
    const ISP_CMOS_ANTIFALSECOLOR_S  *pstAntiFalseColor;
    const ISP_CMOS_BAYERNR_S         *pstBayerNr;
    const ISP_CMOS_DETAIL_S          *pstDetail;
    const ISP_CMOS_CA_S              *pstCa;
    const ISP_CMOS_EXPANDER_S        *pstExpander;
    const ISP_CMOS_CLUT_S            *pstClut;
    const ISP_CMOS_WDR_S             *pstWdr;
    const ISP_CMOS_DEHAZE_S          *pstDehaze;
    const ISP_CMOS_LCAC_S            *pstLcac;
    const ISP_CMOS_ACS_S             *pstAcs;
    ISP_CMOS_NOISE_CALIBRATION_S     stNoiseCalibration;
    ISP_CMOS_SENSOR_MAX_RESOLUTION_S stSensorMaxResolution;
    ISP_CMOS_SENSOR_MODE_S           stSensorMode;
    ISP_CMOS_DNG_COLORPARAM_S        stDngColorParam;
    ISP_CMOS_WDR_SWITCH_ATTR_S       stWdrSwitchAttr;
} ISP_CMOS_DEFAULT_S;

typedef struct hiISP_CMOS_SENSOR_IMAGE_MODE_S {
    HI_U16   u16Width;
    HI_U16   u16Height;
    HI_FLOAT f32Fps;
    HI_U8    u8SnsMode;
} ISP_CMOS_SENSOR_IMAGE_MODE_S;

typedef struct hiISP_SENSOR_EXP_FUNC_S {
    HI_VOID (*pfn_cmos_sensor_init)(VI_PIPE ViPipe);
    HI_VOID (*pfn_cmos_sensor_exit)(VI_PIPE ViPipe);
    HI_VOID (*pfn_cmos_sensor_global_init)(VI_PIPE ViPipe);
    HI_S32 (*pfn_cmos_set_image_mode)(VI_PIPE ViPipe, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSensorImageMode);
    HI_S32 (*pfn_cmos_set_wdr_mode)(VI_PIPE ViPipe, HI_U8 u8Mode);

    /* the algs get data which is associated with sensor, except 3a */
    HI_S32 (*pfn_cmos_get_isp_default)(VI_PIPE ViPipe, ISP_CMOS_DEFAULT_S *pstDef);
    HI_S32 (*pfn_cmos_get_isp_black_level)(VI_PIPE ViPipe, ISP_CMOS_BLACK_LEVEL_S *pstBlackLevel);
    HI_S32 (*pfn_cmos_get_sns_reg_info)(VI_PIPE ViPipe, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo);

    /* the function of sensor set pixel detect */
    HI_VOID (*pfn_cmos_set_pixel_detect)(VI_PIPE ViPipe, HI_BOOL bEnable);
    HI_S32 (*pfn_cmos_get_awb_gains)(VI_PIPE ViPipe, HI_U32 *pu32SensorAwbGain);
} ISP_SENSOR_EXP_FUNC_S;

typedef struct hiISP_SENSOR_REGISTER_S {
    ISP_SENSOR_EXP_FUNC_S stSnsExp;
} ISP_SENSOR_REGISTER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __HI_COMM_SNS_H__ */

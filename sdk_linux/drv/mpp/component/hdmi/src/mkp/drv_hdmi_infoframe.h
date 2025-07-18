/*
 * Copyright (C) 2021 HiSilicon (Shanghai) Technologies CO., LIMITED.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __DRV_HDMI_INFOFRAME_H__
#define __DRV_HDMI_INFOFRAME_H__

#include "hi_type.h"
#include "autoconf.h"

#define HDMI_INFOFRAME_HEADER_SIZE    4
#define HDMI_AVI_INFOFRAME_SIZE       13
#define HDMI_SPD_INFOFRAME_SIZE       25
#define HDMI_AUDIO_INFOFRAME_SIZE     10
/* user data len:31 - 4(pkt header) - 5(4K- the length must be 0x05)  */
#define HDMI_VENDOR_USER_DATA_MAX_LEN 22

enum hdmi_infoframe_type {
    HDMI_INFOFRAME_TYPE_NULL,
    HDMI_INFOFRAME_TYPE_VENDOR = 0x81,
    HDMI_INFOFRAME_TYPE_AVI,
    HDMI_INFOFRAME_TYPE_SPD,
    HDMI_INFOFRAME_TYPE_AUDIO,
    HDMI_INFOFRAME_TYPE_MPEG,
    HDMI_INFOFRAME_TYPE_GBD,
    HDMI_INFOFRAME_TYPE_DRM,
    HDMI_INFOFRAME_TYPE_BUTT
};

typedef enum {
    HDMI_COLORSPACE_RGB,
    HDMI_COLORSPACE_YCBCR422,
    HDMI_COLORSPACE_YCBCR444,
    HDMI_COLORSPACE_YCBCR420,
    HDMI_COLORSPACE_BUTT
} hdmi_colorspace;

enum hdmi_scan_mode {
    HDMI_SCAN_MODE_NONE,
    HDMI_SCAN_MODE_OVERSCAN,
    HDMI_SCAN_MODE_UNDERSCAN,
    HDMI_SCAN_MODE_FUTURE,
    HDMI_SCAN_MODE_BUTT
};

enum hdmi_colorimetry {
    HDMI_COLORIMETRY_NONE,
    HDMI_COLORIMETRY_ITU_601,
    HDMI_COLORIMETRY_ITU_709,
    HDMI_COLORIMETRY_EXTEND
};

typedef enum {
    HDMI_PICTURE_ASPECT_NONE,
    HDMI_PICTURE_ASPECT_4_3,
    HDMI_PICTURE_ASPECT_16_9,
    HDMI_PICTURE_ASPECT_64_27,
    HDMI_PICTURE_ASPECT_256_135,
    HDMI_PICTURE_ASPECT_FUTURE,
    HDMI_PICTURE_ASPECT_BUTT
} hdmi_picture_aspect;

typedef enum {
    HDMI_ACTIVE_ASPECT_16_9_TOP = 2,
    HDMI_ACTIVE_ASPECT_14_9_TOP,
    HDMI_ACTIVE_ASPECT_16_9_CENTER,
    HDMI_ACTIVE_ASPECT_PICTURE = 8,
    HDMI_ACTIVE_ASPECT_4_3,
    HDMI_ACTIVE_ASPECT_16_9,
    HDMI_ACTIVE_ASPECT_14_9,
    HDMI_ACTIVE_ASPECT_4_3_SP_14_9 = 13,
    HDMI_ACTIVE_ASPECT_16_9_SP_14_9,
    HDMI_ACTIVE_ASPECT_16_9_SP_4_3,
    HDMI_ACTIVE_ASPECT_BUTT
} hdmi_active_aspect;

typedef enum {
    HDMI_EXTENDED_COLORIMETRY_XV_YCC_601,
    HDMI_EXTENDED_COLORIMETRY_XV_YCC_709,
    HDMI_EXTENDED_COLORIMETRY_S_YCC_601,
    HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601,
    HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB,
    HDMI_EXTENDED_COLORIMETRY_2020_CONST_LUMINOUS,
    HDMI_EXTENDED_COLORIMETRY_2020_NON_CONST_LUMINOUS,
    HDMI_EXTENDED_COLORIMETRY_ADDITIONAL,
    HDMI_EXTENDED_COLORIMETRY_BUTT
} hdmi_extended_colormetry;

typedef enum {
    HDMI_QUANTIZATION_RANGE_DEFAULT,
    HDMI_QUANTIZATION_RANGE_LIMITED,
    HDMI_QUANTIZATION_RANGE_FULL,
    HDMI_QUANTIZATION_RANGE_BUTT
} hdmi_quantization_range;

enum hdmi_nups {
    HDMI_NUPS_UNKNOWN,
    HDMI_NUPS_HORIZONTAL,
    HDMI_NUPS_VERTICAL,
    HDMI_NUPS_BOTH
};

typedef enum {
    HDMI_YCC_QUANTIZATION_RANGE_LIMITED,
    HDMI_YCC_QUANTIZATION_RANGE_FULL,
    HDMI_YCC_QUANTIZATION_RANGE_BUTT
} hdmi_ycc_quantization_range;

enum hdmi_content_type {
    HDMI_CONTENT_TYPE_NONE,
    HDMI_CONTENT_TYPE_PHOTO,
    HDMI_CONTENT_TYPE_CINEMA,
    HDMI_CONTENT_TYPE_GAME
};

typedef enum {
    HDMI_640X480P60_4_3 = 1,
    HDMI_720X480P60_4_3,
    HDMI_720X480P60_16_9,
    HDMI_1280X720P60_16_9,
    HDMI_1920X1080I60_16_9,
    HDMI_1440X480I60_4_3,
    HDMI_1440X480I60_16_9,  // 7
    HDMI_1440X240P60_4_3,
    HDMI_1440X240P60_16_9,
    HDMI_2880X480I60_4_3,
    HDMI_2880X480I60_16_9,
    HDMI_2880X240P60_4_3,   // 12
    HDMI_2880X240P60_16_9,
    HDMI_1440X480P60_4_3,
    HDMI_1440X480P60_16_9,
    HDMI_1920X1080P60_16_9,
    HDMI_720X576P50_4_3,
    HDMI_720X576P50_16_9,
    HDMI_1280X720P50_16_9,
    HDMI_1920X1080I50_16_9,
    HDMI_1440X576I50_4_3,
    HDMI_1440X576I50_16_9,
    HDMI_1440X288P50_4_3,
    HDMI_1440X288P50_16_9,  // 24
    HDMI_2880X576I50_4_3,
    HDMI_2880X576I50_16_9,
    HDMI_2880X288P50_4_3,
    HDMI_2880X288P50_16_9,
    HDMI_1440X576P50_4_3,
    HDMI_1440X576P50_16_9,  // 30
    HDMI_1920X1080P50_16_9,
    HDMI_1920X1080P24_16_9,
    HDMI_1920X1080P25_16_9,
    HDMI_1920X1080P30_16_9,
    HDMI_2880X480P60_4_3,
    HDMI_2880X480P60_16_9,
    HDMI_2880X576P50_4_3,
    HDMI_2880X576P50_16_9,  // 38
    HDMI_1920X1080I50_16_9_1250,
    HDMI_1920X1080I100_16_9,
    HDMI_1280X720P100_16_9,
    HDMI_720X576P100_4_3,
    HDMI_720X576P100_16_9,
    HDMI_1440X576I100_4_3,
    HDMI_1440X576I100_16_9, // 45
    HDMI_1920X1080I120_16_9,
    HDMI_1280X720P120_16_9,
    HDMI_720X480P120_4_3,
    HDMI_720X480P120_16_9,
    HDMI_1440X480I120_4_3,
    HDMI_1440X480I120_16_9, // 51
    HDMI_720X576P200_4_3,
    HDMI_720X576P200_16_9,
    HDMI_1440X576I200_4_3,
    HDMI_1440X576I200_16_9,
    HDMI_720X480P240_4_3,
    HDMI_720X480P240_16_9,
    HDMI_1440X480I240_4_3,
    HDMI_1440X480I240_16_9, // 59
    HDMI_1280X720P24_16_9,
    HDMI_1280X720P25_16_9,
    HDMI_1280X720P30_16_9,
    HDMI_1920X1080P120_16_9,
    HDMI_1920X1080P100_16_9,
    HDMI_1280X720P24_64_27,
    HDMI_1280X720P25_64_27,
    HDMI_1280X720P30_64_27,
    HDMI_1280X720P50_64_27,
    HDMI_1280X720P60_64_27, // 69
    HDMI_1280X720P100_64_27,
    HDMI_1280X720P120_64_27,
    HDMI_1920X1080P24_64_27,
    HDMI_1920X1080P25_64_27,
    HDMI_1920X1080P30_64_27,
    HDMI_1920X1080P50_64_27,
    HDMI_1920X1080P60_64_27, // 76
    HDMI_1920X1080P100_64_27,
    HDMI_1920X1080P120_64_27,
    HDMI_1680X720P24_64_27,
    HDMI_1680X720P25_64_27,
    HDMI_1680X720P30_64_27,
    HDMI_1680X720P50_64_27,
    HDMI_1680X720P60_64_27, // 83
    HDMI_1680X720P100_64_27,
    HDMI_1680X720P120_64_27,
    HDMI_2560X1080P24_64_27,
    HDMI_2560X1080P25_64_27,
    HDMI_2560X1080P30_64_27,
    HDMI_2560X1080P50_64_27,
    HDMI_2560X1080P60_64_27,
    HDMI_2560X1080P100_64_27,
    HDMI_2560X1080P120_64_27, // 92
    HDMI_3840X2160P24_16_9,
    HDMI_3840X2160P25_16_9,
    HDMI_3840X2160P30_16_9,
    HDMI_3840X2160P50_16_9,
    HDMI_3840X2160P60_16_9,
    HDMI_4096X2160P24_256_135,
    HDMI_4096X2160P25_256_135,
    HDMI_4096X2160P30_256_135,
    HDMI_4096X2160P50_256_135,
    HDMI_4096X2160P60_256_135,
    HDMI_3840X2160P24_64_27,
    HDMI_3840X2160P25_64_27,
    HDMI_3840X2160P30_64_27,
    HDMI_3840X2160P50_64_27,
    HDMI_3840X2160P60_64_27, // 107
    HDMI_1280X720P48_16_9,
    HDMI_1280X720P48_64_27,
    HDMI_1680X720P48_64_27,
    HDMI_1920X1080P48_16_9,
    HDMI_1920X1080P48_64_27,
    HDMI_2560X1080P48_64_27,
    HDMI_3840X2160P48_16_9,
    HDMI_4096X2160P48_256_135,
    HDMI_3840X2160P48_64_27,
    HDMI_3840X2160P100_16_9,
    HDMI_3840X2160P120_16_9,
    HDMI_3840X2160P100_64_27,
    HDMI_3840X2160P120_64_27,
    HDMI_5120X2160P24_64_27, // 121
    HDMI_5120X2160P25_64_27,
    HDMI_5120X2160P30_64_27,
    HDMI_5120X2160P48_64_27,
    HDMI_5120X2160P50_64_27,
    HDMI_5120X2160P60_64_27,
    HDMI_5120X2160P100_64_27, // 127
    HDMI_5120X2160P120_64_27 = 193,
    HDMI_7680X4320P24_16_9,
    HDMI_7680X4320P25_16_9,
    HDMI_7680X4320P30_16_9,
    HDMI_7680X4320P48_16_9,
    HDMI_7680X4320P50_16_9,
    HDMI_7680X4320P60_16_9,
    HDMI_7680X4320P100_16_9, // 200
    HDMI_7680X4320P120_16_9,
    HDMI_7680X4320P24_64_27,
    HDMI_7680X4320P25_64_27,
    HDMI_7680X4320P30_64_27,
    HDMI_7680X4320P48_64_27,
    HDMI_7680X4320P50_64_27,
    HDMI_7680X4320P60_64_27,
    HDMI_7680X4320P100_64_27,
    HDMI_7680X4320P120_64_27,
    HDMI_10240X4320P24_64_27,
    HDMI_10240X4320P25_64_27,
    HDMI_10240X4320P30_64_27,
    HDMI_10240X4320P48_64_27,
    HDMI_10240X4320P50_64_27, // 214
    HDMI_10240X4320P60_64_27,
    HDMI_10240X4320P100_64_27,
    HDMI_10240X4320P120_64_27,
    HDMI_4096X2160P100_256_135,
    HDMI_4096X2160P120_256_135, // 219
    HDMI_VIDEO_CODE_BUTT
} hdmi_video_code_vic;

typedef enum {
    HDMI_VIDEO_TIMING_UNKNOWN,
    HDMI_VIDEO_TIMING_640X480P_60000,
    HDMI_VIDEO_TIMING_720X480P_60000,
    HDMI_VIDEO_TIMING_720X480P_120000,
    HDMI_VIDEO_TIMING_720X480P_240000,  // 4
    HDMI_VIDEO_TIMING_720X576P_50000,
    HDMI_VIDEO_TIMING_720X576P_100000,
    HDMI_VIDEO_TIMING_720X576P_200000,  // 7
    HDMI_VIDEO_TIMING_1280X720P_24000,
    HDMI_VIDEO_TIMING_1280X720P_25000,
    HDMI_VIDEO_TIMING_1280X720P_30000,  // 10
    HDMI_VIDEO_TIMING_1280X720P_48000,
    HDMI_VIDEO_TIMING_1280X720P_50000,
    HDMI_VIDEO_TIMING_1280X720P_60000,
    HDMI_VIDEO_TIMING_1280X720P_100000,
    HDMI_VIDEO_TIMING_1280X720P_120000,
    HDMI_VIDEO_TIMING_1440X240P_60000,  // 15
    HDMI_VIDEO_TIMING_1440X288P_50000,
    HDMI_VIDEO_TIMING_1440X480I_60000,
    HDMI_VIDEO_TIMING_1440X480P_60000,
    HDMI_VIDEO_TIMING_1440X480I_120000,
    HDMI_VIDEO_TIMING_1440X480I_240000,
    HDMI_VIDEO_TIMING_1440X576I_50000,  // 21
    HDMI_VIDEO_TIMING_1440X576P_50000,
    HDMI_VIDEO_TIMING_1440X576I_60000,
    HDMI_VIDEO_TIMING_1440X576I_100000,
    HDMI_VIDEO_TIMING_1440X576I_200000,
    HDMI_VIDEO_TIMING_2880X288P_50000,  // 26
    HDMI_VIDEO_TIMING_2880X480I_60000,
    HDMI_VIDEO_TIMING_2880X480P_60000,
    HDMI_VIDEO_TIMING_2880X240I_60000,
    HDMI_VIDEO_TIMING_2880X576I_50000,
    HDMI_VIDEO_TIMING_2880X576P_50000,
    HDMI_VIDEO_TIMING_1680X720P_24000,  // 32
    HDMI_VIDEO_TIMING_1680X720P_25000,
    HDMI_VIDEO_TIMING_1680X720P_30000,
    HDMI_VIDEO_TIMING_1680X720P_48000,
    HDMI_VIDEO_TIMING_1680X720P_50000,
    HDMI_VIDEO_TIMING_1680X720P_60000,
    HDMI_VIDEO_TIMING_1680X720P_100000,
    HDMI_VIDEO_TIMING_1680X720P_120000,
    HDMI_VIDEO_TIMING_2560X1080P_24000, // 39
    HDMI_VIDEO_TIMING_2560X1080P_25000,
    HDMI_VIDEO_TIMING_2560X1080P_30000,
    HDMI_VIDEO_TIMING_2560X1080P_48000,
    HDMI_VIDEO_TIMING_2560X1080P_50000,
    HDMI_VIDEO_TIMING_2560X1080P_60000,
    HDMI_VIDEO_TIMING_2560X1080P_100000,
    HDMI_VIDEO_TIMING_2560X1080P_120000, // 45
    HDMI_VIDEO_TIMING_1920X1080I_60000,
    HDMI_VIDEO_TIMING_1920X1080P_60000,
    HDMI_VIDEO_TIMING_1920X1080I_50000,
    HDMI_VIDEO_TIMING_1920X1080P_50000,
    HDMI_VIDEO_TIMING_1920X1080P_24000,
    HDMI_VIDEO_TIMING_1920X1080P_25000,
    HDMI_VIDEO_TIMING_1920X1080P_30000,
    HDMI_VIDEO_TIMING_1920X1080P_48000,
    HDMI_VIDEO_TIMING_1920X1080I_100000,
    HDMI_VIDEO_TIMING_1920X1080I_120000,
    HDMI_VIDEO_TIMING_1920X1080P_120000,
    HDMI_VIDEO_TIMING_1920X1080P_100000, // 56
    HDMI_VIDEO_TIMING_3840X2160P_24000,
    HDMI_VIDEO_TIMING_3840X2160P_25000,
    HDMI_VIDEO_TIMING_3840X2160P_30000,
    HDMI_VIDEO_TIMING_3840X2160P_48000,
    HDMI_VIDEO_TIMING_3840X2160P_50000,
    HDMI_VIDEO_TIMING_3840X2160P_60000,
    HDMI_VIDEO_TIMING_3840X2160P_100000,
    HDMI_VIDEO_TIMING_3840X2160P_120000,
    HDMI_VIDEO_TIMING_4096X2160P_24000,
    HDMI_VIDEO_TIMING_4096X2160P_25000,
    HDMI_VIDEO_TIMING_4096X2160P_30000,
    HDMI_VIDEO_TIMING_4096X2160P_48000,
    HDMI_VIDEO_TIMING_4096X2160P_50000,
    HDMI_VIDEO_TIMING_4096X2160P_60000,
    HDMI_VIDEO_TIMING_4096X2160P_100000,
    HDMI_VIDEO_TIMING_4096X2160P_120000,
    HDMI_VIDEO_TIMING_5120X2160P_24000,
    HDMI_VIDEO_TIMING_5120X2160P_25000,
    HDMI_VIDEO_TIMING_5120X2160P_30000,
    HDMI_VIDEO_TIMING_5120X2160P_48000,
    HDMI_VIDEO_TIMING_5120X2160P_50000,
    HDMI_VIDEO_TIMING_5120X2160P_60000,
    HDMI_VIDEO_TIMING_5120X2160P_100000,
    HDMI_VIDEO_TIMING_5120X2160P_120000,
    HDMI_VIDEO_TIMING_7680X4320P_24000,
    HDMI_VIDEO_TIMING_7680X4320P_25000,
    HDMI_VIDEO_TIMING_7680X4320P_30000,
    HDMI_VIDEO_TIMING_7680X4320P_48000,
    HDMI_VIDEO_TIMING_7680X4320P_50000,
    HDMI_VIDEO_TIMING_7680X4320P_60000,
    HDMI_VIDEO_TIMING_7680X4320P_100000,
    HDMI_VIDEO_TIMING_7680X4320P_120000,
    HDMI_VIDEO_TIMING_10240X4320P_24000,
    HDMI_VIDEO_TIMING_10240X4320P_25000,
    HDMI_VIDEO_TIMING_10240X4320P_30000,
    HDMI_VIDEO_TIMING_10240X4320P_48000,
    HDMI_VIDEO_TIMING_10240X4320P_50000,
    HDMI_VIDEO_TIMING_10240X4320P_60000,
    HDMI_VIDEO_TIMING_10240X4320P_100000,
    HDMI_VIDEO_TIMING_10240X4320P_120000,
    HDMI_VIDEO_TIMING_VESA_DEFINE,
    HDMI_VIDEO_TIMING_VESA_800X600_60,
    HDMI_VIDEO_TIMING_VESA_848X480_60,
    HDMI_VIDEO_TIMING_VESA_1024X768_60,
    HDMI_VIDEO_TIMING_VESA_1280X720_60,
    HDMI_VIDEO_TIMING_VESA_1280X768_60,
    HDMI_VIDEO_TIMING_VESA_1280X768_60_RB,
    HDMI_VIDEO_TIMING_VESA_1280X800_60,
    HDMI_VIDEO_TIMING_VESA_1280X800_60_RB,
    HDMI_VIDEO_TIMING_VESA_1280X960_60,
    HDMI_VIDEO_TIMING_VESA_1280X1024_60,
    HDMI_VIDEO_TIMING_VESA_1360X768_60,
    HDMI_VIDEO_TIMING_VESA_1366X768_60,
    HDMI_VIDEO_TIMING_VESA_1400X1050_60,
    HDMI_VIDEO_TIMING_VESA_1440X900_60,
    HDMI_VIDEO_TIMING_VESA_1440X900_60_RB,
    HDMI_VIDEO_TIMING_VESA_1440X1050_60,
    HDMI_VIDEO_TIMING_VESA_1440X1050_60_RB,
    HDMI_VIDEO_TIMING_VESA_1600X900_60_RB,
    HDMI_VIDEO_TIMING_VESA_1600X1200_60,
    HDMI_VIDEO_TIMING_VESA_1680X1050_60,
    HDMI_VIDEO_TIMING_VESA_1680X1050_60_RB,
    HDMI_VIDEO_TIMING_VESA_1792X1344_60,
    HDMI_VIDEO_TIMING_VESA_1856X1392_60,
    HDMI_VIDEO_TIMING_VESA_1920X1080_60,
    HDMI_VIDEO_TIMING_VESA_1920X1200_60,
    HDMI_VIDEO_TIMING_VESA_1920X1200_60_RB,
    HDMI_VIDEO_TIMING_VESA_1920X1440_60,
    HDMI_VIDEO_TIMING_VESA_2048X1152_60,
    HDMI_VIDEO_TIMING_VESA_2560X1440_60_RB,
    HDMI_VIDEO_TIMING_VESA_2560X1600_60,
    HDMI_VIDEO_TIMING_VESA_2560X1600_60_RB,
    HDMI_VIDEO_TIMING_USER_DEFINE,
    HDMI_VIDEO_TIMING_USER_1920X2160_30,
    HDMI_VIDEO_TIMING_USER_2560X1440_30,
    HDMI_VIDEO_TIMING_USER_2560X1440_60,
    HDMI_VIDEO_TIMING_USER_1280X720_60,
    HDMI_VIDEO_TIMING_USER_1366X768_60,
    HDMI_VIDEO_TIMING_USER_1600X900_60_RB,
    HDMI_VIDEO_TIMING_USER_1920X1080_60,
    HDMI_VIDEO_TIMING_USER_2048X1152_60,
    HDMI_VIDEO_TIMING_BUTT
} hdmi_video_timing;

typedef enum {
    HDMI_VIDEO_FORMAT_1080P_60,
    HDMI_VIDEO_FORMAT_1080P_50,
    HDMI_VIDEO_FORMAT_1080P_30,
    HDMI_VIDEO_FORMAT_1080P_25,
    HDMI_VIDEO_FORMAT_1080P_24,
    HDMI_VIDEO_FORMAT_1080I_60,
    HDMI_VIDEO_FORMAT_1080I_50,
    HDMI_VIDEO_FORMAT_720P_60,
    HDMI_VIDEO_FORMAT_720P_50,
    HDMI_VIDEO_FORMAT_576P_50,
    HDMI_VIDEO_FORMAT_480P_60,
    HDMI_VIDEO_FORMAT_PAL,
    HDMI_VIDEO_FORMAT_NTSC,
    HDMI_VIDEO_FORMAT_861D_640X480_60,
    HDMI_VIDEO_FORMAT_VESA_800X600_60,
    HDMI_VIDEO_FORMAT_VESA_1024X768_60,
    HDMI_VIDEO_FORMAT_VESA_1280X800_60,
    HDMI_VIDEO_FORMAT_VESA_1280X1024_60,
    HDMI_VIDEO_FORMAT_VESA_1366X768_60,
    HDMI_VIDEO_FORMAT_VESA_1440X900_60,
#ifdef CONFIG_HI_PLATFORM_H8
    HDMI_VIDEO_FORMAT_VESA_1400X1050_60,
#endif
    HDMI_VIDEO_FORMAT_VESA_1600X1200_60,
    HDMI_VIDEO_FORMAT_VESA_1680X1050_60,
    HDMI_VIDEO_FORMAT_VESA_1920X1200_60,
    HDMI_VIDEO_FORMAT_2560X1440_30,
    HDMI_VIDEO_FORMAT_2560X1440_60,
    HDMI_VIDEO_FORMAT_2560X1600_60,
    HDMI_VIDEO_FORMAT_1920X2160_30,
    HDMI_VIDEO_FORMAT_3840X2160P_24,
    HDMI_VIDEO_FORMAT_3840X2160P_25,
    HDMI_VIDEO_FORMAT_3840X2160P_30,
    HDMI_VIDEO_FORMAT_3840X2160P_50,
    HDMI_VIDEO_FORMAT_3840X2160P_60,
    HDMI_VIDEO_FORMAT_4096X2160P_24,
    HDMI_VIDEO_FORMAT_4096X2160P_25,
    HDMI_VIDEO_FORMAT_4096X2160P_30,
    HDMI_VIDEO_FORMAT_4096X2160P_50,
    HDMI_VIDEO_FORMAT_4096X2160P_60,
    HDMI_VIDEO_FORMAT_3840X2160P_120,
    HDMI_VIDEO_FORMAT_4096X2160P_120,
    HDMI_VIDEO_FORMAT_7680X4320P_30,
    HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE,
    HDMI_VIDEO_FORMAT_BUTT
} hdmi_disp_format;

struct hdmi_avi_infoframe {
    enum hdmi_infoframe_type    type;
    hi_u8                       version;
    hi_u8                       length;
    hdmi_colorspace             colorspace;
    hi_bool                     active_info_valid;
    hi_bool                     horizontal_bar_valid;
    hi_bool                     vertical_bar_valid;
    enum hdmi_scan_mode         scan_mode;
    enum hdmi_colorimetry       colorimetry;
    hdmi_picture_aspect         picture_aspect;
    hdmi_active_aspect          active_aspect;
    hi_bool                     itc;
    hdmi_extended_colormetry    extended_colorimetry;
    hdmi_quantization_range     quantization_range;
    enum hdmi_nups              nups;
    hdmi_video_timing           video_timing;
    hdmi_disp_format            disp_fmt;
    hdmi_ycc_quantization_range ycc_quantization_range;
    enum hdmi_content_type      content_type;
    hi_u8                       pixel_repeat;
    hi_u16                      top_bar;
    hi_u16                      bottom_bar;
    hi_u16                      left_bar;
    hi_u16                      right_bar;
};

typedef enum {
    HDMI_EOTF_SDR_LUMIN,     /* traditional gamma - SDR luminance range */
    HDMI_EOTF_HDR_LUMIN,     /* traditional gamma - HDR luminance range */
    HDMI_EOTF_SMPTE_ST_2048, /* SMPTE ST 2084 */
    HDMI_EOTF_HLG,           /* hybrid log-gamma(HLG) */
    HDMI_EOTF_BUTT
} hdmi_eotf_type;

enum hdmi_static_metadata_type {
    HDMI_HDR_METADATA_ID_0, /* static metadata type 1 */
    HDMI_HDR_METADATA_ID_BUTT
};

struct hdmi_meta_descriptor_1st {
    hi_u16 primaries0_x;        /* display_primaries_x[0], range[0,50000], in units of 0.00002 */
    hi_u16 primaries0_y;        /* display_primaries_y[0], range[0,50000], in units of 0.00002 */
    hi_u16 primaries1_x;        /* display_primaries_x[1], range[0,50000], in units of 0.00002 */
    hi_u16 primaries1_y;        /* display_primaries_y[1], range[0,50000], in units of 0.00002 */
    hi_u16 primaries2_x;        /* display_primaries_x[2], range[0,50000], in units of 0.00002 */
    hi_u16 primaries2_y;        /* display_primaries_y[2], range[0,50000], in units of 0.00002 */
    hi_u16 white_point_x;       /* white_point_x, range[0,50000], in units of 0.00002 */
    hi_u16 white_point_y;       /* white_point_y, range[0,50000], in units of 0.00002 */
    hi_u16 max_luminance;       /* max_display_mastering_luminance, in units of 1 cd/m^2 */
    hi_u16 min_luminance;       /* min_display_mastering_luminance, in units of 0.0001 cd/m^2 */
    hi_u16 max_light_level;     /* maximum content light level, in units of 1 cd/m2 */
    hi_u16 average_light_level; /* maximum frame-average light level,in units of 1 cd/m^2 */
};

union hdmi_meta_descriptor {
    struct hdmi_meta_descriptor_1st type1; /* static metadata descriptor type1 */
};

struct hdmi_drm_infoframe {
    enum hdmi_infoframe_type type;
    hi_u8 version;
    hi_u8 length;
    hdmi_eotf_type eotf_type;
    enum hdmi_static_metadata_type metadata_type;
    union hdmi_meta_descriptor meta_descriptor;
};

enum hdmi_audio_sample_size {
    HDMI_AUDIO_SAMPLE_SIZE_STREAM,
    HDMI_AUDIO_SAMPLE_SIZE_16,
    HDMI_AUDIO_SAMPLE_SIZE_20,
    HDMI_AUDIO_SAMPLE_SIZE_24
};

enum hdmi_audio_sample_frequency {
    HDMI_AUDIO_SAMPLE_FREQUENCY_STREAM,
    HDMI_AUDIO_SAMPLE_FREQUENCY_32000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_44100,
    HDMI_AUDIO_SAMPLE_FREQUENCY_48000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_88200,
    HDMI_AUDIO_SAMPLE_FREQUENCY_96000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_176400,
    HDMI_AUDIO_SAMPLE_FREQUENCY_192000
};

enum hdmi_audio_coding_type_ext {
    HDMI_AUDIO_CODING_TYPE_EXT_STREAM,
    HDMI_AUDIO_CODING_TYPE_EXT_HE_AAC,
    HDMI_AUDIO_CODING_TYPE_EXT_HE_AAC_V2,
    HDMI_AUDIO_CODING_TYPE_EXT_MPEG_SURROUND
};

enum hdmi_audio_lfe_playback_level {
    HDMI_AUDIO_LFE_PLAYBACK_NO,
    HDMI_AUDIO_LFE_PLAYBACK_0_DB,
    HDMI_AUDIO_LFE_PLAYBACK_10_DB,
    HDMI_AUDIO_LFE_PLAYBACK_RESERVED
};

enum hdmi_audio_code {
    HDMI_AUDIO_CODING_TYPE_STREAM,
    HDMI_AUDIO_CODING_TYPE_PCM,
    HDMI_AUDIO_CODING_TYPE_AC3,
    HDMI_AUDIO_CODING_TYPE_MPEG1,
    HDMI_AUDIO_CODING_TYPE_MP3,
    HDMI_AUDIO_CODING_TYPE_MPEG2,
    HDMI_AUDIO_CODING_TYPE_AAC_LC,
    HDMI_AUDIO_CODING_TYPE_DTS,
    HDMI_AUDIO_CODING_TYPE_ATRAC,
    HDMI_AUDIO_CODING_TYPE_DSD,
    HDMI_AUDIO_CODING_TYPE_EAC3,
    HDMI_AUDIO_CODING_TYPE_DTS_HD,
    HDMI_AUDIO_CODING_TYPE_MLP,
    HDMI_AUDIO_CODING_TYPE_DST,
    HDMI_AUDIO_CODING_TYPE_WMA_PRO,
    HDMI_AUDIO_CODING_TYPE_RESERVED,
    HDMI_AUDIO_CODING_TYPE_BUTT
};

struct hdmi_audio_infoframe {
    enum hdmi_infoframe_type           type;
    hi_u8                              version;
    hi_u8                              length;
    hi_u8                              channels;
    enum hdmi_audio_code               coding_type;
    enum hdmi_audio_sample_size        sample_size;
    enum hdmi_audio_sample_frequency   sample_frequency;
    enum hdmi_audio_coding_type_ext    coding_type_ext;
    hi_u8                              channel_allocation;
    enum hdmi_audio_lfe_playback_level lfe_playback_level;
    hi_u8                              level_shift_value;
    hi_bool                            downmix_inhibit;
};

enum hdmi_ieee_oui {
    HDMI14_IEEE_OUI = 0x00000c03, // HDMI1.4
    HDMI20_IEEE_OUI = 0x00c45dd8  // HDMI2.0
};

enum hdmi_video_format {
    HDMI_VIDEO_FORMAT_NONE,
    HDMI_VIDEO_FORMAT_4K,
    HDMI_VIDEO_FORMAT_3D
};

enum hdmi_vic {
    HDMI_VIC_4K_3840X2160_30 = 0x1,
    HDMI_VIC_4K_3840X2160_25,
    HDMI_VIC_4K_3840X2160_24,
    HDMI_VIC_4K_4096X2160_24
};

enum hdmi_3d_structure {
    HDMI_3D_FRAME_PACKING,
    HDMI_3D_FIELD_ALTERNATIVE,
    HDMI_3D_LINE_ALTERNATIVE,
    HDMI_3D_SIDE_BY_SIDE_FULL,
    HDMI_3D_L_DEPTH,
    HDMI_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH,
    HDMI_3D_TOP_AND_BOTTOM,
    HDMI_3D_SIDE_BY_SIDE_HALF = 0x08,
    HDMI_3D_BUTT
};

enum hdmi_3d_ext_data {
    HDMI_3D_PICTURE_ODD_LEFT_ODD_RIGHT = 0x4,
    HDMI_3D_PICTURE_ODD_LEFT_EVEN_RIGHT,
    HDMI_3D_PICTURE_EVEN_LEFT_ODD_RIGHT,
    HDMI_3D_PICTURE_EVEN_LEFT_EVEN_RIGHT
};

typedef struct {
    enum hdmi_ieee_oui     ieee;
    enum hdmi_video_format format;
    enum hdmi_vic          vic;
    enum hdmi_3d_structure _3d_structure;
    hi_u8                  _3d_meta_present;
    enum hdmi_3d_ext_data  _3d_ext_data;
    hi_u8                  _3d_metadata_type;
    hi_u8                  _3d_metadata_length;
    hi_u8                  _3d_metadata[20];
} hdmi_14_vsif_content;

typedef struct {
    enum hdmi_ieee_oui     ieee;
    enum hdmi_video_format format;
    enum hdmi_vic          vic;
    enum hdmi_3d_structure _3d_structure;
    hi_u8                  _3d_meta_present;
    hi_u8                  len;
    hi_u8                  data[HDMI_VENDOR_USER_DATA_MAX_LEN];
} hdmi_user_vsif_content;

typedef struct {
    enum hdmi_ieee_oui ieee;
    hi_u8 version;
    hi_u8 _3d_valid;
    hi_u8 _3d_f_structure;
    hi_u8 _3d_additional_info_present;
    hi_u8 _3d_disparity_data_present;
    hi_u8 _3d_meta_present;
    hi_u8 _3d_f_ext_data;
    hi_u8 _3d_dual_view;
    hi_u8 _3d_view_dependency;
    hi_u8 _3d_preferred_2d_view;
    hi_u8 _3d_disparity_data_version;
    hi_u8 _3d_disparity_data_length;
    hi_u8 _3d_disparity_data[10];
    hi_u8 _3d_ext_data;
    hi_u8 _3d_metadata_type;
    hi_u8 _3d_metadata_length;
    hi_u8 _3d_metadata[10];
} hdmi_forum_vsif_content;

struct hdmi_vendor_specific_infoframe {
    enum hdmi_infoframe_type type;
    hi_u8 version;
    hi_u8 length;
    union {
        hdmi_user_vsif_content  cea861_vsif;
        hdmi_14_vsif_content    h14_vsif_content;
        hdmi_forum_vsif_content hf_vsif_content;
    } vsif_content;
};

typedef union {
    struct {
        hi_u8 affected_gamut_seq_num : 4; // [0..3]
        hi_u8 gbd_profile            : 3; // [4..6]
        hi_u8 next_field             : 1; // [7]
    } bits;
    hi_u8 u8;
} hdmi_gumat_hb1;

typedef union {
    struct {
        hi_u8 current_gamut_seq_num : 4; // [0..3]
        hi_u8 packet_seq            : 2; // [4..5]
        hi_u8 reserved               : 1; // [6]
        hi_u8 no_crnt_gbd           : 1; // [7]
    } bits;
    hi_u8 u8;
} hdmi_gumat_hb2;

typedef union {
    struct {
        hi_u8 gbd_color_space     : 3; // [0..2]
        hi_u8 gbd_color_precision : 2; // [3..4]
        hi_u8 reserved0            : 1; // [5]
        hi_u8 reserved1            : 1; // [6]
        hi_u8 format_flag         : 1; // [7]
    } bits;
    hi_u8 u8;
} hdmi_gumat_range_gdb0;

typedef struct {
    hi_u8 hb0;
    hdmi_gumat_hb1 hb1;
    hdmi_gumat_hb2 hb2;
    hdmi_gumat_range_gdb0 gdb0;
    hi_u8 min_red_data_h;
    hi_u8 mid_red_data_m;   // max_red_data_l||max_red_data_h
    hi_u8 max_red_data_l;
    hi_u8 min_green_data_h;
    hi_u8 mid_green_data_m; // max_green_data_l||max_green_data_h
    hi_u8 max_green_data_l;
    hi_u8 min_blue_data_h;
    hi_u8 mid_blue_data_m;  // max_blue_data_l||max_blue_data_h
    hi_u8 max_blue_data_l;
} hdmi_gdb_pack;

typedef enum   hdmi_3d_structure              hdmi_3d_mode;
typedef enum   hdmi_colorimetry               hdmi_colormetry;
typedef enum   hdmi_audio_code                hdmi_audio_format_code;
typedef enum   hdmi_vic                       hdmi_vsif_vic;
typedef enum   hdmi_infoframe_type            hdmi_infoframe_id;
typedef enum   hdmi_static_metadata_type      hdmi_static_meta_type_id;
typedef union  hdmi_meta_descriptor           hdmi_meta_descriptor;
typedef struct hdmi_avi_infoframe             hdmi_avi_infoframe;
typedef struct hdmi_audio_infoframe           hdmi_audio_infoframe;
typedef struct hdmi_vendor_specific_infoframe hdmi_vendor_infoframe;
typedef struct hdmi_meta_descriptor_1st       hdmi_meta_des_1st;
typedef struct hdmi_drm_infoframe             hdmi_drm_infoframe;

typedef struct {
    hdmi_avi_infoframe    avi_infoframe;
    hdmi_audio_infoframe  audio_infoframe;
    hdmi_vendor_infoframe vendor_infoframe;
    hdmi_gdb_pack         gdb_pack;
    hdmi_drm_infoframe    drm_infoframe;
} hdmi_infoframe_type;

typedef union {
    hdmi_avi_infoframe    avi_infoframe;
    hdmi_audio_infoframe  audio_infoframe;
    hdmi_vendor_infoframe vendor_infoframe;
    hdmi_gdb_pack         gdb_pack;
    hdmi_drm_infoframe    drm_infoframe;
} hdmi_infoframe;

hi_s32 drv_hdmi_avi_infoframe_send(hdmi_infoframe_type *info_frame, hi_bool enable);

hi_s32 drv_hdmi_vendor_infoframe_send(hdmi_infoframe_type *info_frame, hi_bool enable, hi_bool dolby_enable);

hi_s32 drv_hdmi_audio_infoframe_send(hdmi_infoframe_type *info_frame, hi_bool enable);

hi_void drv_hdmi_infoframe_send(hdmi_infoframe_type *info_frame,
    hdmi_infoframe_id infoframe_id, hdmi_infoframe *infoframe);

hi_void drv_hdmi_infoframe_enable_set(hdmi_infoframe_type *info_frame,
    hdmi_infoframe_id infoframe_id, hi_bool enable);

hdmi_video_timing drv_hdmi_video_timing_lookup(hdmi_video_code_vic vic, hdmi_picture_aspect aspect);

hdmi_video_timing drv_hdmi_vsif_video_timing_lookup(hdmi_vsif_vic vic);

hi_s32 drv_hdmi_drm_infoframe_send(hdmi_infoframe_type *info_frame, hi_bool enable);

hi_s32 drv_hdmi_null_packet_send(hdmi_infoframe_type *info_frame, hi_bool enable);

#endif /* __DRV_HDMI_INFOFRAME_H__ */


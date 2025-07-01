#ifndef CNN_EMOTION_CLASSIFY_H
#define CNN_EMOTION_CLASSIFY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "hi_comm_video.h"
#include "osd_img.h"

// 情感分类阈值
#define THRESH_MIN          30      // 可接受的置信度阈值

// 情感类别定义
typedef enum {
    EMOTION_NEUTRAL = 0,
    EMOTION_HAPPY,
    EMOTION_SAD,
    EMOTION_SURPRISE,
    EMOTION_FEAR,
    EMOTION_DISGUST,
    EMOTION_ANGER,
    EMOTION_UNKNOWN
} EmotionType;

#if __cplusplus
extern "C" {
#endif

// 输入尺寸
#define FRM_WIDTH           224
#define FRM_HEIGHT          224

// 情感类别数量
#define EMOTION_CLASSES     7

// 函数声明
HI_S32 CnnEmotionClassifyLoadModel(uintptr_t* model, OsdSet* osds);
HI_S32 CnnEmotionClassifyUnloadModel(uintptr_t model);
HI_S32 CnnEmotionClassifyCal(uintptr_t model, VIDEO_FRAME_INFO_S *srcFrm, VIDEO_FRAME_INFO_S *resFrm);

#if __cplusplus
}
#endif
#endif

/* 
 * 情绪分类实现 - 基于ResNet18模型
 * 输入: 224x224 YUV420SP图像
 * 输出: 7种情绪分类
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/prctl.h>

#include "sample_comm_nnie.h"
#include "sample_media_ai.h"
#include "ai_infer_process.h"
#include "vgs_img.h"
#include "ive_img.h"
#include "posix_help.h"
#include "audio_aac_adp.h"
#include "base_interface.h"
#include "osd_img.h"
#include "cnn_emotion_classify.h" // 更新头文件名

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

// 更新模型路径
// #define MODEL_FILE_TRASH    "/userdata/models/cnn_trash_classify/resnet_inst.wk" // 注释掉
#define MODEL_FILE_EMOTION    "/userdata/models/cnn_emotion_classify/emotion_inst.wk"

// 输入尺寸改为224x224
#define FRM_WIDTH           224
#define FRM_HEIGHT          224

// 情感类别数量
#define EMOTION_CLASSES     7

// 情感类别名称映射
static const char* EmotionNames[] = {
    [EMOTION_NEUTRAL] = "Neutral",
    [EMOTION_HAPPY] = "Happy",
    [EMOTION_SAD] = "Sad",
    [EMOTION_SURPRISE] = "Surprise",
    [EMOTION_FEAR] = "Fear",
    [EMOTION_DISGUST] = "Disgust",
    [EMOTION_ANGER] = "Anger",
    [EMOTION_UNKNOWN] = "Unknown"
};

// 全局变量 (与垃圾分类类似，但重命名)
static int g_emotionNum = -1;
static int g_count = 0;
#define AUDIO_CASE_TWO     2
#define AUDIO_SCORE        40       // 可自行配置的置信度
#define AUDIO_FRAME        14       // 每15帧识别一次

static HI_BOOL g_bAudioProcessStopSignal = HI_FALSE;
static pthread_t g_audioProcessThread = 0;
static OsdSet* g_osdsEmotion = NULL;
static HI_S32 g_osd0Emotion = -1;

static SkPair g_stmChn = {
    .in = -1,
    .out = -1
};

/* 音频播放函数 - 修改为播放情绪 */
static HI_VOID PlayEmotionAudio(const RecogNumInfo items)
{
    if  (g_count < AUDIO_FRAME) {
        g_count++;
        return;
    }

    const RecogNumInfo *item = &items;
    uint32_t score = item->score * 100 / SCORE_MAX;
    if ((score > AUDIO_SCORE) && (g_emotionNum != item->num)) {
        g_emotionNum = item->num;
        if (g_emotionNum < EMOTION_UNKNOWN) {
            // 播放对应情绪的音频
            AudioTest(g_emotionNum, -1);
        }
    }
    g_count = 0;
}

/* 模型加载函数 - 修改模型路径 */
HI_S32 CnnEmotionClassifyLoadModel(uintptr_t* model, OsdSet* osds)
{
    SAMPLE_SVP_NNIE_CFG_S *self = NULL;
    HI_S32 ret;
    HI_CHAR audioThreadName[16] = {0};

    ret = OsdLibInit();
    HI_ASSERT(ret == HI_SUCCESS);

    g_osdsEmotion = osds;
    HI_ASSERT(g_osdsEmotion);
    g_osd0Emotion = OsdsCreateRgn(g_osdsEmotion);
    HI_ASSERT(g_osd0Emotion >= 0);

    // 使用情绪分类模型路径
    ret = CnnCreate(&self, MODEL_FILE_EMOTION);
    *model = ret < 0 ? 0 : (uintptr_t)self;
    SAMPLE_PRT("Load emotion classify model, ret:%d\n", ret);

    if (GetCfgBool("audio_player:support_audio", true)) {
        ret = SkPairCreate(&g_stmChn);
        HI_ASSERT(ret == 0);
        if (snprintf_s(audioThreadName, sizeof(audioThreadName), sizeof(audioThreadName) - 1, "EmotionAudio") < 0) {
            HI_ASSERT(0);
        }
        prctl(PR_SET_NAME, (unsigned long)audioThreadName, 0, 0, 0);
        ret = pthread_create(&g_audioProcessThread, NULL, GetAudioFileName, NULL);
        if (ret != 0) {
            SAMPLE_PRT("Audio process thread create fail:%s\n", strerror(ret));
            return ret;
        }
    }

    return ret;
}

/* 模型卸载函数 */
HI_S32 CnnEmotionClassifyUnloadModel(uintptr_t model)
{
    CnnDestroy((SAMPLE_SVP_NNIE_CFG_S*)model);
    SAMPLE_PRT("Unload emotion model success\n");
    OsdsClear(g_osdsEmotion);

    if (GetCfgBool("audio_player:support_audio", true)) {
        SkPairDestroy(&g_stmChn);
        SAMPLE_PRT("SkPairDestroy success\n");
        g_bAudioProcessStopSignal = HI_TRUE;
        pthread_join(g_audioProcessThread, NULL);
        g_audioProcessThread = 0;
    }

    return HI_SUCCESS;
}

/* 结果处理函数 - 修改为处理情绪分类 */
static HI_S32 CnnEmotionClassifyFlag(const RecogNumInfo items[], HI_S32 itemNum, HI_CHAR* buf, HI_S32 size)
{
    HI_S32 offset = 0;
    EmotionType emotionType = EMOTION_UNKNOWN;
    uint32_t maxScore = 0;

    offset += snprintf_s(buf + offset, size - offset, size - offset - 1, "Emotion: {");
    
    // 找出置信度最高的情绪
    for (HI_U32 i = 0; i < itemNum; i++) {
        const RecogNumInfo *item = &items[i];
        uint32_t score = item->score * 100 / SCORE_MAX;
        
        if (score < THRESH_MIN) {
            continue;
        }
        
        SAMPLE_PRT("----emotion item----num:%d, score:%d\n", item->num, score);
        
        // 更新最高分
        if (score > maxScore) {
            maxScore = score;
            switch (item->num) {
                case 0: emotionType = EMOTION_NEUTRAL; break;
                case 1: emotionType = EMOTION_HAPPY; break;
                case 2: emotionType = EMOTION_SAD; break;
                case 3: emotionType = EMOTION_SURPRISE; break;
                case 4: emotionType = EMOTION_FEAR; break;
                case 5: emotionType = EMOTION_DISGUST; break;
                case 6: emotionType = EMOTION_ANGER; break;
                default: emotionType = EMOTION_UNKNOWN; break;
            }
        }
    }
    
    if (emotionType != EMOTION_UNKNOWN) {
        offset += snprintf_s(buf + offset, size - offset, size - offset - 1,
            "%s %d%%", EmotionNames[emotionType], maxScore);
    } else {
        offset += snprintf_s(buf + offset, size - offset, size - offset - 1,
            "Unknown");
    }
    
    offset += snprintf_s(buf + offset, size - offset, size - offset - 1, " }");
    return HI_SUCCESS;
}

/* 主推理函数 - 修改输入尺寸和结果处理 */
HI_S32 CnnEmotionClassifyCal(uintptr_t model, VIDEO_FRAME_INFO_S *srcFrm, VIDEO_FRAME_INFO_S *resFrm)
{
    SAMPLE_SVP_NNIE_CFG_S *self = (SAMPLE_SVP_NNIE_CFG_S*)model;
    VIDEO_FRAME_INFO_S resizeFrm;
    IVE_IMAGE_S img;
    static HI_CHAR prevOsd[NORM_BUF_SIZE] = "";
    HI_CHAR osdBuf[NORM_BUF_SIZE] = "";
    RecogNumInfo resBuf[RET_NUM_MAX] = {0};
    HI_S32 resLen = 0;
    HI_S32 ret;
    IVE_IMAGE_S imgIn;

    // 1. 缩放至224x224 (情绪分类模型要求)
    ret = MppFrmResize(srcFrm, &resizeFrm, FRM_WIDTH, FRM_HEIGHT);
    if (ret != HI_SUCCESS) {
        SAMPLE_PRT("Resize to %dx%d failed! ret=0x%x\n", FRM_WIDTH, FRM_HEIGHT, ret);
        return ret;
    }

    // 2. 转换为YUV图像结构 (VVU420SP格式)
    ret = FrmToOrigImg(&resizeFrm, &img);
    if (ret != HI_SUCCESS) {
        SAMPLE_PRT("Convert to YUV failed! ret=0x%x\n", ret);
        MppFrmDestroy(&resizeFrm);
        return ret;
    }

    // 3. 直接使用整张图像 (情绪分类不需要裁剪)
    imgIn = img;

    // 4. 执行推理
    ret = CnnCalImg(self, &imgIn, resBuf, sizeof(resBuf) / sizeof((resBuf)[0]), &resLen);
    if (ret < 0) {
        SAMPLE_PRT("Emotion inference failed! ret=0x%x\n", ret);
        goto cleanup;
    }

    // 5. 处理情绪结果
    ret = ProcessEmotionResults(resBuf, resLen, osdBuf, sizeof(osdBuf));
    if (ret < 0) {
        SAMPLE_PRT("Process emotion results failed! ret=0x%x\n", ret);
        goto cleanup;
    }

    // 6. 音频处理
    if (GetCfgBool("audio_player:support_audio", true)) {
        if (FdWriteMsg(g_stmChn.out, &resBuf[0], sizeof(RecogNumInfo)) != sizeof(RecogNumInfo)) {
            SAMPLE_PRT("FdWriteMsg FAIL\n");
        }
    }

    // 7. 更新OSD显示
    if (strcmp(osdBuf, prevOsd) != 0) {
        strcpy(prevOsd, osdBuf);
        HI_OSD_ATTR_S rgn;
        TxtRgnInit(&rgn, osdBuf, TXT_BEGX, TXT_BEGY, ARGB1555_YELLOW2);
        OsdsSetRgn(g_osdsEmotion, g_osd0Emotion, &rgn);
        
        // 发送到VPSS
        ret = HI_MPI_VPSS_SendFrame(0, 0, srcFrm, 0);
        if (ret != HI_SUCCESS) {
            SAMPLE_PRT("Error(%#x), HI_MPI_VPSS_SendFrame failed!\n", ret);
        }
    }

cleanup:
    MppFrmDestroy(&resizeFrm);
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
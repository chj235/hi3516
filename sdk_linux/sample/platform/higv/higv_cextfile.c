/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: cextFile
 * Author: HiMobileCam middleware develop team
 * Create: 2019-01-14
 */
#include "higv_cextfile.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

HIGV_MSG_PROC g_pfunHIGVAppEventFunc[18] = {
    (HIGV_MSG_PROC)BUTTON_WIN_Onrefresh,
    (HIGV_MSG_PROC)BUTTON_WIN_Ontimer,
    (HIGV_MSG_PROC)BUTTON_WIN_Onhide,
    (HIGV_MSG_PROC)IMAGE_WIN_Onshow,
    (HIGV_MSG_PROC)IMAGE_WIN_Onhide,
    (HIGV_MSG_PROC)IMAGE_WIN_Ontimer,
    (HIGV_MSG_PROC)LABEL_WIN_Onrefresh,
    (HIGV_MSG_PROC)LABEL_WIN_Ontimer,
    (HIGV_MSG_PROC)LABEL_WIN_Onhide,
    (HIGV_MSG_PROC)LISTBOX_WIN_Onshow,
    (HIGV_MSG_PROC)LISTBOX_WIN_Onrefresh,
    (HIGV_MSG_PROC)LISTBOX_WIN_Ontimer,
    (HIGV_MSG_PROC)LISTBOX_WIN_Onhide,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Onshow,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Onrefresh,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Ontimer,
    (HIGV_MSG_PROC)SCROLLVIEW_WIN_Onhide,
    (HIGV_MSG_PROC)OnViewMoveListener
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

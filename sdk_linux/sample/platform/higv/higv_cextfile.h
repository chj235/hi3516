/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: cextFile
 * Author: HiMobileCam middleware develop team
 * Create: 2019-01-14
 */
#ifndef HIGV_CEXTFILE_H
#define HIGV_CEXTFILE_H

#include "hi_gv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*  __cplusplus  */

#define BUTTON_WIN (1)
#define BUTTON_WIN_BUTTON1 (3)
#define ADM_SET_STRINGID (6)
#define SIMHEI_FONT_TEXT_30 (5)
#define SIMHEI_FONT_TEXT_20 (7)
#define IMAGE_WIN (8)
#define PNG_IMAGE (9)
#define JPG_IMAGE (10)
#define BMP_IMAGE (11)
#define SHOW_IMAGE (12)
#define LABEL_WIN (13)
#define LABEL_WIN_LABEL (14)
#define LISTBOX_WIN (16)
#define LISTBOX_WIN_LISTBOX1 (17)
#define MAIN_WIN_LISTBOX1_LISTCOL1 (21)
#define SCROLLVIEW_WIN (22)
#define SCROLLVIEW_WIN_SCROLLVIEW1 (23)
#define CHILDBUTTON_01 (24)
#define CHILDBUTTON_02 (27)
#define CHILDBUTTON_03 (28)
#define CHILDBUTTON_04 (29)
#define CHILDBUTTON_05 (30)
#define CHILDBUTTON_06 (31)
#define GROUP_SKIN (2)
#define COMMON_SKIN (32)
#define LABEL_SKIN (15)
#define BUTTON_SKIN (33)
#define LIST_NORMAL_SKIN (4)
#define LIST_ACTIVE_SKIN (18)
#define LISTBOX_SELECT_SKIN (19)
#define LISTBOX_ROW_NORMAL (20)
#define BUTTON_NORMAL_SKIN (25)
#define BUTTON_ACTIVE_SKIN (26)

extern HIGV_MSG_PROC g_pfunHIGVAppEventFunc[18];

extern HI_S32 BUTTON_WIN_Onrefresh(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 BUTTON_WIN_Ontimer(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 BUTTON_WIN_Onhide(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 IMAGE_WIN_Onshow(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 IMAGE_WIN_Onhide(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 IMAGE_WIN_Ontimer(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 LABEL_WIN_Onrefresh(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 LABEL_WIN_Ontimer(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 LABEL_WIN_Onhide(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 LISTBOX_WIN_Onshow(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 LISTBOX_WIN_Onrefresh(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 LISTBOX_WIN_Ontimer(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 LISTBOX_WIN_Onhide(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 SCROLLVIEW_WIN_Onshow(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 SCROLLVIEW_WIN_Onrefresh(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 SCROLLVIEW_WIN_Ontimer(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 SCROLLVIEW_WIN_Onhide(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);
extern HI_S32 OnViewMoveListener(HI_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

#endif  /* HIGV_CEXTFILE_H */


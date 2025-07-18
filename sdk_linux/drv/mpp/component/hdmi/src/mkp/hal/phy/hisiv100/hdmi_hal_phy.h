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
#ifndef __HDMI_HAL_PHY_H__
#define __HDMI_HAL_PHY_H__

#include "hdmi_hal_intf.h"
#include "drv_hdmi_common.h"

typedef enum {
    PHY_RPRE_50,
    PHY_RPRE_56,
    PHY_RPRE_71,
    PHY_RPRE_83,
    PHY_RPRE_100,
    PHY_RPRE_125,
    PHY_RPRE_250,
    PHY_RPRE_500,
    PHY_RPRE_BUTT
} phy_rpre;

typedef enum {
    PHY_RTERM_MODE_SINGLE,
    PHY_RTERM_MODE_SOURCE,
    PHY_RTERM_MODE_LOAD,
    PHY_RTERM_MODE_DOUBLEDIFF,
    PHY_RTERM_MODE_BUTT
} phy_rterm_mode;

typedef struct {
    hi_u32 ssc_amptd; // 1/1M
    hi_u32 ssc_freq;  // 1hz
} phy_ssc_cfg;

typedef struct {
    hi_bool     ssc_enable;
    hi_bool     ssc_debug_en;
    phy_ssc_cfg ssc_cfg;
} hdmi_phy_ssc;

typedef struct {
    hi_u32          pix_clk;    /* pixel colck, in k_hz */
    hi_u32          tmds_clk;   /* TMDS colck, in k_hz */
    hdmi_deep_color deep_color; /* deep color(color depth) */
    hdmi_phy_ssc    phy_ssc;    /* spread spectrum ctrl(ssc) para */
} hdmi_phy_ssc_cfg;

typedef struct {
    hi_u32         i_de_main_clk;
    hi_u32         i_de_main_d0;
    hi_u32         i_de_main_d1;
    hi_u32         i_de_main_d2;
    hi_u32         i_main_clk;
    hi_u32         i_main_d0;
    hi_u32         i_main_d1;
    hi_u32         i_main_d2;
    hi_u32         i_de_pre_clk;
    hi_u32         i_de_pre_d0;
    hi_u32         i_de_pre_d1;
    hi_u32         i_de_pre_d2;
    hi_u32         i_pre_clk;
    hi_u32         i_pre_d0;
    hi_u32         i_pre_d1;
    hi_u32         i_pre_d2;
    phy_rpre       r_de_pre_clk;
    phy_rpre       r_de_pre_d0;
    phy_rpre       r_de_pre_d1;
    phy_rpre       r_de_pre_d2;
    phy_rpre       r_pre_clk;
    phy_rpre       r_pre_d0;
    phy_rpre       r_pre_d1;
    phy_rpre       r_pre_d2;
    phy_rterm_mode r_term_mode_clk;
    hi_u32         r_term_clk;
    phy_rterm_mode r_term_mode_d0;
    hi_u32         r_term_d0;
    phy_rterm_mode r_term_mode_d1;
    hi_u32         r_term_d1;
    phy_rterm_mode r_term_mode_d2;
    hi_u32         r_term_d2;
} phy_hw_spec_cfg;

typedef enum {
    PHY_FEEDTHRU_CAP_DELAY_18,
    PHY_FEEDTHRU_CAP_DELAY_35,
    PHY_FEEDTHRU_CAP_DELAY_45,
    PHY_FEEDTHRU_CAP_DELAY_55
} phy_feedthru_cap_delay;

/* HW spec struct new version, HI3531DV200 */
typedef struct {
    hi_u32                 i_demain_clk;
    hi_u32                 i_demain_d0;
    hi_u32                 i_demain_d1;
    hi_u32                 i_demain_d2;
    hi_u32                 i_main_clk;
    hi_u32                 i_main_d0;
    hi_u32                 i_main_d1;
    hi_u32                 i_main_d2;
    hi_bool                feedthru_cap_clk_en;
    hi_bool                feedthru_cap_d0_en;
    hi_bool                feedthru_cap_d1_en;
    hi_bool                feedthru_cap_d2_en;
    phy_feedthru_cap_delay feedthru_cap_delay_clk;
    phy_feedthru_cap_delay feedthru_cap_delay_d0;
    phy_feedthru_cap_delay feedthru_cap_delay_d1;
    phy_feedthru_cap_delay feedthru_cap_delay_d2;
    phy_rterm_mode         rterm_mode_clk;
    hi_u32                 rterm_clk;
    phy_rterm_mode         rterm_mode_d0;
    hi_u32                 rterm_d0;
    phy_rterm_mode         rterm_mode_d1;
    hi_u32                 rterm_d1;
    phy_rterm_mode         rterm_mode_d2;
    hi_u32                 rterm_d2;
    hi_u32                 vcm_sel;
} phy_hwspec_enhance_cfg;

typedef struct {
    hi_bool                hw_spec_debug_en;
    phy_hw_spec_cfg        spec_cfg;
    phy_hwspec_enhance_cfg enhance_cfg;
} hdmi_phy_hw_spec;

typedef struct {
    hi_u32           tmds_clk; /* TMDS colck, in k_hz */
    hdmi_trace_len   trace_len;
    hdmi_phy_hw_spec hdmi_phy_spec; /* phy specification para */
} hdmi_phy_hw_spec_cfg;

typedef struct {
    hi_u32            pixel_clk;
    hi_u32            tmds_clk;   /* TMDS colck, in k_hz */
    hi_bool           emi_enable;
    hdmi_deep_color   deep_color; /* deep color(color depth) */
    hdmi_phy_mode_cfg mode_cfg;   /* TMDS/FRL/tx_ffe */
    hdmi_trace_len    trace_len;
} hdmi_phy_tmds_cfg;

typedef struct {
    hi_bool                init;
    hi_bool                power_enable;
    hi_bool                oe_enable;
    hdmi_phy_tmds_cfg      tmds_cfg;
    hdmi_phy_ssc           ssc_cfg;
    phy_hw_spec_cfg        hw_spec_cfg;
    hdmi_hw_spec           hw_spec;
    phy_hwspec_enhance_cfg spec_enhance_cfg;
} hdmi_phy_info;

typedef struct {
    hdmi_hw_spec  hw_spec_cfg;
    hdmi_hw_spec  hw_spec_def;
    hdmi_hw_param hw_param_cur;
} hdmi_phy_hw_param;

hi_s32 hal_hdmi_phy_init(const hdmi_hal_init *hal_init);

hi_s32 hal_hdmi_phy_deinit(hi_void);

hi_void hal_hdmi_phy_reset(hi_void);

hi_s32 hal_hdmi_phy_power_get(hi_bool *enable);

hi_void hal_hdmi_phy_power_set(hi_bool enable);

hi_void hal_hdmi_phy_oe_set(hi_bool enable);

hi_s32 hal_hdmi_phy_oe_get(hi_bool *enable);

hi_s32 hal_hdmi_phy_tmds_set(hdmi_phy_tmds_cfg *tmds_cfg);

hi_s32 hal_hdmi_phy_spec_set(hdmi_phy_hw_spec_cfg *hdmi_spec_cfg);

hi_s32 hal_hdmi_phy_info_get(hdmi_phy_info *phy_status);

hi_void hal_hdmi_phy_ssc_set(hdmi_phy_ssc_cfg *hdmi_ssc_cfg);

hi_s32 hal_hdmi_phy_ssc_get(hi_bool *enable);

hi_s32 hal_hdmi_phy_params_set(hi_u32 tmds_clk, const hdmi_hw_spec *hw_spec);

hi_s32 hal_hdmi_phy_params_get(hdmi_phy_hw_param *hw_param);

#endif /* __HDMI_HAL_PHY_H__ */


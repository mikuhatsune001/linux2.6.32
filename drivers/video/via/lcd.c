/*
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "global.h"
#include "lcdtbl.h"

static struct iga2_shadow_crtc_timing iga2_shadow_crtc_reg = {
	/* IGA2 Shadow Horizontal Total */
	{IGA2_SHADOW_HOR_TOTAL_REG_NUM, {{CR6D, 0, 7}, {CR71, 3, 3} } },
	/* IGA2 Shadow Horizontal Blank End */
	{IGA2_SHADOW_HOR_BLANK_END_REG_NUM, {{CR6E, 0, 7} } },
	/* IGA2 Shadow Vertical Total */
	{IGA2_SHADOW_VER_TOTAL_REG_NUM, {{CR6F, 0, 7}, {CR71, 0, 2} } },
	/* IGA2 Shadow Vertical Addressable Video */
	{IGA2_SHADOW_VER_ADDR_REG_NUM, {{CR70, 0, 7}, {CR71, 4, 6} } },
	/* IGA2 Shadow Vertical Blank Start */
	{IGA2_SHADOW_VER_BLANK_START_REG_NUM,
	 {{CR72, 0, 7}, {CR74, 4, 6} } },
	/* IGA2 Shadow Vertical Blank End */
	{IGA2_SHADOW_VER_BLANK_END_REG_NUM, {{CR73, 0, 7}, {CR74, 0, 2} } },
	/* IGA2 Shadow Vertical Sync Start */
	{IGA2_SHADOW_VER_SYNC_START_REG_NUM, {{CR75, 0, 7}, {CR76, 4, 6} } },
	/* IGA2 Shadow Vertical Sync End */
	{IGA2_SHADOW_VER_SYNC_END_REG_NUM, {{CR76, 0, 3} } }
};

static struct _lcd_scaling_factor lcd_scaling_factor = {
	/* LCD Horizontal Scaling Factor Register */
	{LCD_HOR_SCALING_FACTOR_REG_NUM,
	 {{CR9F, 0, 1}, {CR77, 0, 7}, {CR79, 4, 5} } },
	/* LCD Vertical Scaling Factor Register */
	{LCD_VER_SCALING_FACTOR_REG_NUM,
	 {{CR79, 3, 3}, {CR78, 0, 7}, {CR79, 6, 7} } }
};
static struct _lcd_scaling_factor lcd_scaling_factor_CLE = {
	/* LCD Horizontal Scaling Factor Register */
	{LCD_HOR_SCALING_FACTOR_REG_NUM_CLE, {{CR77, 0, 7}, {CR79, 4, 5} } },
	/* LCD Vertical Scaling Factor Register */
	{LCD_VER_SCALING_FACTOR_REG_NUM_CLE, {{CR78, 0, 7}, {CR79, 6, 7} } }
};

static int check_lvds_chip(int device_id_subaddr, int device_id);
static bool lvds_identify_integratedlvds(void);
static int fp_id_to_vindex(int panel_id);
static int lvds_register_read(int index);
static void load_lcd_scaling(int set_hres, int set_vres, int panel_hres,
		      int panel_vres);
static void load_lcd_k400_patch_tbl(int set_hres, int set_vres,
	int panel_id);
static void load_lcd_p880_patch_tbl(int set_hres, int set_vres,
	int panel_id);
static void load_lcd_patch_regs(int set_hres, int set_vres,
	int panel_id, int set_iga);
static void via_pitch_alignment_patch_lcd(
	struct lvds_setting_information *plvds_setting_info,
				   struct lvds_chip_information
				   *plvds_chip_info);
static void lcd_patch_skew_dvp0(struct lvds_setting_information
			 *plvds_setting_info,
			 struct lvds_chip_information *plvds_chip_info);
static void lcd_patch_skew_dvp1(struct lvds_setting_information
			 *plvds_setting_info,
			 struct lvds_chip_information *plvds_chip_info);
static void lcd_patch_skew(struct lvds_setting_information
	*plvds_setting_info, struct lvds_chip_information *plvds_chip_info);

static void integrated_lvds_disable(struct lvds_setting_information
			     *plvds_setting_info,
			     struct lvds_chip_information *plvds_chip_info);
static void integrated_lvds_enable(struct lvds_setting_information
			    *plvds_setting_info,
			    struct lvds_chip_information *plvds_chip_info);
static void lcd_powersequence_off(void);
static void lcd_powersequence_on(void);
static void fill_lcd_format(void);
static void check_diport_of_integrated_lvds(
	struct lvds_chip_information *plvds_chip_info,
				     struct lvds_setting_information
				     *plvds_setting_info);
static struct display_timing lcd_centering_timging(struct display_timing
					    mode_crt_reg,
					   struct display_timing panel_crt_reg);
static void load_crtc_shadow_timing(struct display_timing mode_timing,
			     struct display_timing panel_timing);
static void viafb_load_scaling_factor_for_p4m900(int set_hres,
	int set_vres, int panel_hres, int panel_vres);

static int check_lvds_chip(int device_id_subaddr, int device_id)
{
	if (lvds_register_read(device_id_subaddr) == device_id)
		return OK;
	else
		return FAIL;
}

void viafb_init_lcd_size(void)
{
	DEBUG_MSG(KERN_INFO "viafb_init_lcd_size()\n");
	DEBUG_MSG(KERN_INFO
		"viaparinfo->lvds_setting_info->get_lcd_size_method %d\n",
		viaparinfo->lvds_setting_info->get_lcd_size_method);

	switch (viaparinfo->lvds_setting_info->get_lcd_size_method) {
	case GET_LCD_SIZE_BY_SYSTEM_BIOS:
		break;
	case GET_LCD_SZIE_BY_HW_STRAPPING:
		break;
	case GET_LCD_SIZE_BY_VGA_BIOS:
		DEBUG_MSG(KERN_INFO "Get LCD Size method by VGA BIOS !!\n");
		viaparinfo->lvds_setting_info->lcd_panel_size =
		    fp_id_to_vindex(viafb_lcd_panel_id);
		DEBUG_MSG(KERN_INFO "LCD Panel_ID = %d\n",
			  viaparinfo->lvds_setting_info->lcd_panel_id);
		DEBUG_MSG(KERN_INFO "LCD Panel Size = %d\n",
			  viaparinfo->lvds_setting_info->lcd_panel_size);
		break;
	case GET_LCD_SIZE_BY_USER_SETTING:
		DEBUG_MSG(KERN_INFO "Get LCD Size method by user setting !!\n");
		viaparinfo->lvds_setting_info->lcd_panel_size =
		    fp_id_to_vindex(viafb_lcd_panel_id);
		DEBUG_MSG(KERN_INFO "LCD Panel_ID = %d\n",
			  viaparinfo->lvds_setting_info->lcd_panel_id);
		DEBUG_MSG(KERN_INFO "LCD Panel Size = %d\n",
			  viaparinfo->lvds_setting_info->lcd_panel_size);
		break;
	default:
		DEBUG_MSG(KERN_INFO "viafb_init_lcd_size fail\n");
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID1_800X600;
		viaparinfo->lvds_setting_info->lcd_panel_size =
		    fp_id_to_vindex(LCD_PANEL_ID1_800X600);
	}
	viaparinfo->lvds_setting_info2->lcd_panel_id =
		viaparinfo->lvds_setting_info->lcd_panel_id;
	viaparinfo->lvds_setting_info2->lcd_panel_size =
		viaparinfo->lvds_setting_info->lcd_panel_size;
	viaparinfo->lvds_setting_info2->lcd_panel_hres =
		viaparinfo->lvds_setting_info->lcd_panel_hres;
	viaparinfo->lvds_setting_info2->lcd_panel_vres =
		viaparinfo->lvds_setting_info->lcd_panel_vres;
	viaparinfo->lvds_setting_info2->device_lcd_dualedge =
	    viaparinfo->lvds_setting_info->device_lcd_dualedge;
	viaparinfo->lvds_setting_info2->LCDDithering =
		viaparinfo->lvds_setting_info->LCDDithering;
}

static bool lvds_identify_integratedlvds(void)
{
	if (viafb_display_hardware_layout == HW_LAYOUT_LCD_EXTERNAL_LCD2) {
		/* Two dual channel LCD (Internal LVDS + External LVDS): */
		/* If we have an external LVDS, such as VT1636, we should
		   have its chip ID already. */
		if (viaparinfo->chip_info->lvds_chip_info.lvds_chip_name) {
			viaparinfo->chip_info->lvds_chip_info2.lvds_chip_name =
			    INTEGRATED_LVDS;
			DEBUG_MSG(KERN_INFO "Support two dual channel LVDS!\
				  (Internal LVDS + External LVDS)\n");
		} else {
			viaparinfo->chip_info->lvds_chip_info.lvds_chip_name =
			    INTEGRATED_LVDS;
			DEBUG_MSG(KERN_INFO "Not found external LVDS,\
				  so can't support two dual channel LVDS!\n");
		}
	} else if (viafb_display_hardware_layout == HW_LAYOUT_LCD1_LCD2) {
		/* Two single channel LCD (Internal LVDS + Internal LVDS): */
		viaparinfo->chip_info->lvds_chip_info.lvds_chip_name =
		INTEGRATED_LVDS;
		viaparinfo->chip_info->lvds_chip_info2.lvds_chip_name =
			INTEGRATED_LVDS;
		DEBUG_MSG(KERN_INFO "Support two single channel LVDS!\
			  (Internal LVDS + Internal LVDS)\n");
	} else if (viafb_display_hardware_layout != HW_LAYOUT_DVI_ONLY) {
		/* If we have found external LVDS, just use it,
		   otherwise, we will use internal LVDS as default. */
		if (!viaparinfo->chip_info->lvds_chip_info.lvds_chip_name) {
			viaparinfo->chip_info->lvds_chip_info.lvds_chip_name =
			    INTEGRATED_LVDS;
			DEBUG_MSG(KERN_INFO "Found Integrated LVDS!\n");
		}
	} else {
		viaparinfo->chip_info->lvds_chip_info.lvds_chip_name =
			NON_LVDS_TRANSMITTER;
		DEBUG_MSG(KERN_INFO "Do not support LVDS!\n");
		return false;
	}

	return true;
}

int viafb_lvds_trasmitter_identify(void)
{
	viaparinfo->shared->i2c_stuff.i2c_port = I2CPORTINDEX;
	if (viafb_lvds_identify_vt1636()) {
		viaparinfo->chip_info->lvds_chip_info.i2c_port = I2CPORTINDEX;
		DEBUG_MSG(KERN_INFO
			  "Found VIA VT1636 LVDS on port i2c 0x31 \n");
	} else {
		viaparinfo->shared->i2c_stuff.i2c_port = GPIOPORTINDEX;
		if (viafb_lvds_identify_vt1636()) {
			viaparinfo->chip_info->lvds_chip_info.i2c_port =
				GPIOPORTINDEX;
			DEBUG_MSG(KERN_INFO
				  "Found VIA VT1636 LVDS on port gpio 0x2c \n");
		}
	}

	if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700)
		lvds_identify_integratedlvds();

	if (viaparinfo->chip_info->lvds_chip_info.lvds_chip_name)
		return true;
	/* Check for VT1631: */
	viaparinfo->chip_info->lvds_chip_info.lvds_chip_name = VT1631_LVDS;
	viaparinfo->chip_info->lvds_chip_info.lvds_chip_slave_addr =
		VT1631_LVDS_I2C_ADDR;

	if (check_lvds_chip(VT1631_DEVICE_ID_REG, VT1631_DEVICE_ID) != FAIL) {
		DEBUG_MSG(KERN_INFO "\n VT1631 LVDS ! \n");
		DEBUG_MSG(KERN_INFO "\n %2d",
			  viaparinfo->chip_info->lvds_chip_info.lvds_chip_name);
		DEBUG_MSG(KERN_INFO "\n %2d",
			  viaparinfo->chip_info->lvds_chip_info.lvds_chip_name);
		return OK;
	}

	viaparinfo->chip_info->lvds_chip_info.lvds_chip_name =
		NON_LVDS_TRANSMITTER;
	viaparinfo->chip_info->lvds_chip_info.lvds_chip_slave_addr =
		VT1631_LVDS_I2C_ADDR;
	return FAIL;
}

static int fp_id_to_vindex(int panel_id)
{
	DEBUG_MSG(KERN_INFO "fp_get_panel_id()\n");

	if (panel_id > LCD_PANEL_ID_MAXIMUM)
		viafb_lcd_panel_id = panel_id =
		viafb_read_reg(VIACR, CR3F) & 0x0F;

	switch (panel_id) {
	case 0x0:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 640;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 480;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID0_640X480;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_640X480;
		break;
	case 0x1:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 800;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 600;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID1_800X600;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_800X600;
		break;
	case 0x2:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1024;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID2_1024X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1024X768;
		break;
	case 0x3:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1280;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID3_1280X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1280X768;
		break;
	case 0x4:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1280;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 1024;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID4_1280X1024;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1280X1024;
		break;
	case 0x5:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1400;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 1050;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID5_1400X1050;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1400X1050;
		break;
	case 0x6:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1600;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 1200;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID6_1600X1200;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1600X1200;
		break;
	case 0x8:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 800;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 480;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_IDA_800X480;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_800X480;
		break;
	case 0x9:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1024;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID2_1024X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1024X768;
		break;
	case 0xA:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1024;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID2_1024X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1024X768;
		break;
	case 0xB:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1024;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID2_1024X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1024X768;
		break;
	case 0xC:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1280;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID3_1280X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1280X768;
		break;
	case 0xD:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1280;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 1024;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID4_1280X1024;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1280X1024;
		break;
	case 0xE:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1400;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 1050;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID5_1400X1050;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1400X1050;
		break;
	case 0xF:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1600;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 1200;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID6_1600X1200;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1600X1200;
		break;
	case 0x10:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1366;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID7_1366X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1368X768;
		break;
	case 0x11:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1024;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 600;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID8_1024X600;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1024X600;
		break;
	case 0x12:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1280;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID3_1280X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1280X768;
		break;
	case 0x13:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1280;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 800;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID9_1280X800;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_1280X800;
		break;
	case 0x14:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1360;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_IDB_1360X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1360X768;
		break;
	case 0x15:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 1280;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 768;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID3_1280X768;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 1;
		viaparinfo->lvds_setting_info->LCDDithering = 0;
		return VIA_RES_1280X768;
		break;
	case 0x16:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 480;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 640;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_IDC_480X640;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_480X640;
		break;
	default:
		viaparinfo->lvds_setting_info->lcd_panel_hres = 800;
		viaparinfo->lvds_setting_info->lcd_panel_vres = 600;
		viaparinfo->lvds_setting_info->lcd_panel_id =
			LCD_PANEL_ID1_800X600;
		viaparinfo->lvds_setting_info->device_lcd_dualedge = 0;
		viaparinfo->lvds_setting_info->LCDDithering = 1;
		return VIA_RES_800X600;
	}
}

static int lvds_register_read(int index)
{
	u8 data;

	viaparinfo->shared->i2c_stuff.i2c_port = GPIOPORTINDEX;
	viafb_i2c_readbyte((u8) viaparinfo->chip_info->
	    lvds_chip_info.lvds_chip_slave_addr,
			(u8) index, &data);
	return data;
}

static void load_lcd_scaling(int set_hres, int set_vres, int panel_hres,
		      int panel_vres)
{
	int reg_value = 0;
	int viafb_load_reg_num;
	struct io_register *reg = NULL;

	DEBUG_MSG(KERN_INFO "load_lcd_scaling()!!\n");

	/* LCD Scaling Enable */
	viafb_write_reg_mask(CR79, VIACR, 0x07, BIT0 + BIT1 + BIT2);
	if (UNICHROME_P4M900 == viaparinfo->chip_info->gfx_chip_name) {
		viafb_load_scaling_factor_for_p4m900(set_hres, set_vres,
					       panel_hres, panel_vres);
		return;
	}

	/* Check if expansion for horizontal */
	if (set_hres != panel_hres) {
		/* Load Horizontal Scaling Factor */
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_CLE266:
		case UNICHROME_K400:
			reg_value =
			    CLE266_LCD_HOR_SCF_FORMULA(set_hres, panel_hres);
			viafb_load_reg_num =
			    lcd_scaling_factor_CLE.lcd_hor_scaling_factor.
			    reg_num;
			reg = lcd_scaling_factor_CLE.lcd_hor_scaling_factor.reg;
			viafb_load_reg(reg_value,
				viafb_load_reg_num, reg, VIACR);
			break;
		case UNICHROME_K800:
		case UNICHROME_PM800:
		case UNICHROME_CN700:
		case UNICHROME_CX700:
		case UNICHROME_K8M890:
		case UNICHROME_P4M890:
			reg_value =
			    K800_LCD_HOR_SCF_FORMULA(set_hres, panel_hres);
			/* Horizontal scaling enabled */
			viafb_write_reg_mask(CRA2, VIACR, 0xC0, BIT7 + BIT6);
			viafb_load_reg_num =
			    lcd_scaling_factor.lcd_hor_scaling_factor.reg_num;
			reg = lcd_scaling_factor.lcd_hor_scaling_factor.reg;
			viafb_load_reg(reg_value,
				viafb_load_reg_num, reg, VIACR);
			break;
		}

		DEBUG_MSG(KERN_INFO "Horizontal Scaling value = %d", reg_value);
	} else {
		/* Horizontal scaling disabled */
		viafb_write_reg_mask(CRA2, VIACR, 0x00, BIT7);
	}

	/* Check if expansion for vertical */
	if (set_vres != panel_vres) {
		/* Load Vertical Scaling Factor */
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_CLE266:
		case UNICHROME_K400:
			reg_value =
			    CLE266_LCD_VER_SCF_FORMULA(set_vres, panel_vres);
			viafb_load_reg_num =
			    lcd_scaling_factor_CLE.lcd_ver_scaling_factor.
			    reg_num;
			reg = lcd_scaling_factor_CLE.lcd_ver_scaling_factor.reg;
			viafb_load_reg(reg_value,
				viafb_load_reg_num, reg, VIACR);
			break;
		case UNICHROME_K800:
		case UNICHROME_PM800:
		case UNICHROME_CN700:
		case UNICHROME_CX700:
		case UNICHROME_K8M890:
		case UNICHROME_P4M890:
			reg_value =
			    K800_LCD_VER_SCF_FORMULA(set_vres, panel_vres);
			/* Vertical scaling enabled */
			viafb_write_reg_mask(CRA2, VIACR, 0x08, BIT3);
			viafb_load_reg_num =
			    lcd_scaling_factor.lcd_ver_scaling_factor.reg_num;
			reg = lcd_scaling_factor.lcd_ver_scaling_factor.reg;
			viafb_load_reg(reg_value,
				viafb_load_reg_num, reg, VIACR);
			break;
		}

		DEBUG_MSG(KERN_INFO "Vertical Scaling value = %d", reg_value);
	} else {
		/* Vertical scaling disabled */
		viafb_write_reg_mask(CRA2, VIACR, 0x00, BIT3);
	}
}

static void load_lcd_k400_patch_tbl(int set_hres, int set_vres,
	int panel_id)
{
	int vmode_index;
	int reg_num = 0;
	struct io_reg *lcd_patch_reg = NULL;

	vmode_index = viafb_get_mode_index(set_hres, set_vres);
	switch (panel_id) {
		/* LCD 800x600 */
	case LCD_PANEL_ID1_800X600:
		switch (vmode_index) {
		case VIA_RES_640X400:
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_K400_LCD_RES_6X4_8X6;
			lcd_patch_reg = K400_LCD_RES_6X4_8X6;
			break;
		case VIA_RES_720X480:
		case VIA_RES_720X576:
			reg_num = NUM_TOTAL_K400_LCD_RES_7X4_8X6;
			lcd_patch_reg = K400_LCD_RES_7X4_8X6;
			break;
		}
		break;

		/* LCD 1024x768 */
	case LCD_PANEL_ID2_1024X768:
		switch (vmode_index) {
		case VIA_RES_640X400:
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_K400_LCD_RES_6X4_10X7;
			lcd_patch_reg = K400_LCD_RES_6X4_10X7;
			break;
		case VIA_RES_720X480:
		case VIA_RES_720X576:
			reg_num = NUM_TOTAL_K400_LCD_RES_7X4_10X7;
			lcd_patch_reg = K400_LCD_RES_7X4_10X7;
			break;
		case VIA_RES_800X600:
			reg_num = NUM_TOTAL_K400_LCD_RES_8X6_10X7;
			lcd_patch_reg = K400_LCD_RES_8X6_10X7;
			break;
		}
		break;

		/* LCD 1280x1024 */
	case LCD_PANEL_ID4_1280X1024:
		switch (vmode_index) {
		case VIA_RES_640X400:
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_K400_LCD_RES_6X4_12X10;
			lcd_patch_reg = K400_LCD_RES_6X4_12X10;
			break;
		case VIA_RES_720X480:
		case VIA_RES_720X576:
			reg_num = NUM_TOTAL_K400_LCD_RES_7X4_12X10;
			lcd_patch_reg = K400_LCD_RES_7X4_12X10;
			break;
		case VIA_RES_800X600:
			reg_num = NUM_TOTAL_K400_LCD_RES_8X6_12X10;
			lcd_patch_reg = K400_LCD_RES_8X6_12X10;
			break;
		case VIA_RES_1024X768:
			reg_num = NUM_TOTAL_K400_LCD_RES_10X7_12X10;
			lcd_patch_reg = K400_LCD_RES_10X7_12X10;
			break;

		}
		break;

		/* LCD 1400x1050 */
	case LCD_PANEL_ID5_1400X1050:
		switch (vmode_index) {
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_K400_LCD_RES_6X4_14X10;
			lcd_patch_reg = K400_LCD_RES_6X4_14X10;
			break;
		case VIA_RES_800X600:
			reg_num = NUM_TOTAL_K400_LCD_RES_8X6_14X10;
			lcd_patch_reg = K400_LCD_RES_8X6_14X10;
			break;
		case VIA_RES_1024X768:
			reg_num = NUM_TOTAL_K400_LCD_RES_10X7_14X10;
			lcd_patch_reg = K400_LCD_RES_10X7_14X10;
			break;
		case VIA_RES_1280X768:
		case VIA_RES_1280X800:
		case VIA_RES_1280X960:
		case VIA_RES_1280X1024:
			reg_num = NUM_TOTAL_K400_LCD_RES_12X10_14X10;
			lcd_patch_reg = K400_LCD_RES_12X10_14X10;
			break;
		}
		break;

		/* LCD 1600x1200 */
	case LCD_PANEL_ID6_1600X1200:
		switch (vmode_index) {
		case VIA_RES_640X400:
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_K400_LCD_RES_6X4_16X12;
			lcd_patch_reg = K400_LCD_RES_6X4_16X12;
			break;
		case VIA_RES_720X480:
		case VIA_RES_720X576:
			reg_num = NUM_TOTAL_K400_LCD_RES_7X4_16X12;
			lcd_patch_reg = K400_LCD_RES_7X4_16X12;
			break;
		case VIA_RES_800X600:
			reg_num = NUM_TOTAL_K400_LCD_RES_8X6_16X12;
			lcd_patch_reg = K400_LCD_RES_8X6_16X12;
			break;
		case VIA_RES_1024X768:
			reg_num = NUM_TOTAL_K400_LCD_RES_10X7_16X12;
			lcd_patch_reg = K400_LCD_RES_10X7_16X12;
			break;
		case VIA_RES_1280X768:
		case VIA_RES_1280X800:
		case VIA_RES_1280X960:
		case VIA_RES_1280X1024:
			reg_num = NUM_TOTAL_K400_LCD_RES_12X10_16X12;
			lcd_patch_reg = K400_LCD_RES_12X10_16X12;
			break;
		}
		break;

		/* LCD 1366x768 */
	case LCD_PANEL_ID7_1366X768:
		switch (vmode_index) {
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_K400_LCD_RES_6X4_1366X7;
			lcd_patch_reg = K400_LCD_RES_6X4_1366X7;
			break;
		case VIA_RES_720X480:
		case VIA_RES_720X576:
			reg_num = NUM_TOTAL_K400_LCD_RES_7X4_1366X7;
			lcd_patch_reg = K400_LCD_RES_7X4_1366X7;
			break;
		case VIA_RES_800X600:
			reg_num = NUM_TOTAL_K400_LCD_RES_8X6_1366X7;
			lcd_patch_reg = K400_LCD_RES_8X6_1366X7;
			break;
		case VIA_RES_1024X768:
			reg_num = NUM_TOTAL_K400_LCD_RES_10X7_1366X7;
			lcd_patch_reg = K400_LCD_RES_10X7_1366X7;
			break;
		case VIA_RES_1280X768:
		case VIA_RES_1280X800:
		case VIA_RES_1280X960:
		case VIA_RES_1280X1024:
			reg_num = NUM_TOTAL_K400_LCD_RES_12X10_1366X7;
			lcd_patch_reg = K400_LCD_RES_12X10_1366X7;
			break;
		}
		break;

		/* LCD 1360x768 */
	case LCD_PANEL_IDB_1360X768:
		break;
	}
	if (reg_num != 0) {
		/* H.W. Reset : ON */
		viafb_write_reg_mask(CR17, VIACR, 0x00, BIT7);

		viafb_write_regx(lcd_patch_reg, reg_num);

		/* H.W. Reset : OFF */
		viafb_write_reg_mask(CR17, VIACR, 0x80, BIT7);

		/* Reset PLL */
		viafb_write_reg_mask(SR40, VIASR, 0x02, BIT1);
		viafb_write_reg_mask(SR40, VIASR, 0x00, BIT1);

		/* Fire! */
		outb(inb(VIARMisc) | (BIT2 + BIT3), VIAWMisc);
	}
}

static void load_lcd_p880_patch_tbl(int set_hres, int set_vres,
	int panel_id)
{
	int vmode_index;
	int reg_num = 0;
	struct io_reg *lcd_patch_reg = NULL;

	vmode_index = viafb_get_mode_index(set_hres, set_vres);

	switch (panel_id) {
	case LCD_PANEL_ID5_1400X1050:
		switch (vmode_index) {
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_P880_LCD_RES_6X4_14X10;
			lcd_patch_reg = P880_LCD_RES_6X4_14X10;
			break;
		case VIA_RES_800X600:
			reg_num = NUM_TOTAL_P880_LCD_RES_8X6_14X10;
			lcd_patch_reg = P880_LCD_RES_8X6_14X10;
			break;
		}
		break;
	case LCD_PANEL_ID6_1600X1200:
		switch (vmode_index) {
		case VIA_RES_640X400:
		case VIA_RES_640X480:
			reg_num = NUM_TOTAL_P880_LCD_RES_6X4_16X12;
			lcd_patch_reg = P880_LCD_RES_6X4_16X12;
			break;
		case VIA_RES_720X480:
		case VIA_RES_720X576:
			reg_num = NUM_TOTAL_P880_LCD_RES_7X4_16X12;
			lcd_patch_reg = P880_LCD_RES_7X4_16X12;
			break;
		case VIA_RES_800X600:
			reg_num = NUM_TOTAL_P880_LCD_RES_8X6_16X12;
			lcd_patch_reg = P880_LCD_RES_8X6_16X12;
			break;
		case VIA_RES_1024X768:
			reg_num = NUM_TOTAL_P880_LCD_RES_10X7_16X12;
			lcd_patch_reg = P880_LCD_RES_10X7_16X12;
			break;
		case VIA_RES_1280X768:
		case VIA_RES_1280X960:
		case VIA_RES_1280X1024:
			reg_num = NUM_TOTAL_P880_LCD_RES_12X10_16X12;
			lcd_patch_reg = P880_LCD_RES_12X10_16X12;
			break;
		}
		break;

	}
	if (reg_num != 0) {
		/* H.W. Reset : ON */
		viafb_write_reg_mask(CR17, VIACR, 0x00, BIT7);

		viafb_write_regx(lcd_patch_reg, reg_num);

		/* H.W. Reset : OFF */
		viafb_write_reg_mask(CR17, VIACR, 0x80, BIT7);

		/* Reset PLL */
		viafb_write_reg_mask(SR40, VIASR, 0x02, BIT1);
		viafb_write_reg_mask(SR40, VIASR, 0x00, BIT1);

		/* Fire! */
		outb(inb(VIARMisc) | (BIT2 + BIT3), VIAWMisc);
	}
}

static void load_lcd_patch_regs(int set_hres, int set_vres,
	int panel_id, int set_iga)
{
	int vmode_index;

	vmode_index = viafb_get_mode_index(set_hres, set_vres);

	viafb_unlock_crt();

	/* Patch for simultaneous & Expansion */
	if ((set_iga == IGA1_IGA2) &&
		(viaparinfo->lvds_setting_info->display_method ==
	    LCD_EXPANDSION)) {
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_CLE266:
		case UNICHROME_K400:
			load_lcd_k400_patch_tbl(set_hres, set_vres, panel_id);
			break;
		case UNICHROME_K800:
			break;
		case UNICHROME_PM800:
		case UNICHROME_CN700:
		case UNICHROME_CX700:
			load_lcd_p880_patch_tbl(set_hres, set_vres, panel_id);
		}
	}

	viafb_lock_crt();
}

static void via_pitch_alignment_patch_lcd(
	struct lvds_setting_information *plvds_setting_info,
				   struct lvds_chip_information
				   *plvds_chip_info)
{
	unsigned char cr13, cr35, cr65, cr66, cr67;
	unsigned long dwScreenPitch = 0;
	unsigned long dwPitch;

	dwPitch = plvds_setting_info->h_active * (plvds_setting_info->bpp >> 3);
	if (dwPitch & 0x1F) {
		dwScreenPitch = ((dwPitch + 31) & ~31) >> 3;
		if (plvds_setting_info->iga_path == IGA2) {
			if (plvds_setting_info->bpp > 8) {
				cr66 = (unsigned char)(dwScreenPitch & 0xFF);
				viafb_write_reg(CR66, VIACR, cr66);
				cr67 = viafb_read_reg(VIACR, CR67) & 0xFC;
				cr67 |=
				    (unsigned
				     char)((dwScreenPitch & 0x300) >> 8);
				viafb_write_reg(CR67, VIACR, cr67);
			}

			/* Fetch Count */
			cr67 = viafb_read_reg(VIACR, CR67) & 0xF3;
			cr67 |= (unsigned char)((dwScreenPitch & 0x600) >> 7);
			viafb_write_reg(CR67, VIACR, cr67);
			cr65 = (unsigned char)((dwScreenPitch >> 1) & 0xFF);
			cr65 += 2;
			viafb_write_reg(CR65, VIACR, cr65);
		} else {
			if (plvds_setting_info->bpp > 8) {
				cr13 = (unsigned char)(dwScreenPitch & 0xFF);
				viafb_write_reg(CR13, VIACR, cr13);
				cr35 = viafb_read_reg(VIACR, CR35) & 0x1F;
				cr35 |=
				    (unsigned
				     char)((dwScreenPitch & 0x700) >> 3);
				viafb_write_reg(CR35, VIACR, cr35);
			}
		}
	}
}
static void lcd_patch_skew_dvp0(struct lvds_setting_information
			 *plvds_setting_info,
			 struct lvds_chip_information *plvds_chip_info)
{
	if (VT1636_LVDS == plvds_chip_info->lvds_chip_name) {
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_P4M900:
			viafb_vt1636_patch_skew_on_vt3364(plvds_setting_info,
						    plvds_chip_info);
			break;
		case UNICHROME_P4M890:
			viafb_vt1636_patch_skew_on_vt3327(plvds_setting_info,
						    plvds_chip_info);
			break;
		}
	}
}
static void lcd_patch_skew_dvp1(struct lvds_setting_information
			 *plvds_setting_info,
			 struct lvds_chip_information *plvds_chip_info)
{
	if (VT1636_LVDS == plvds_chip_info->lvds_chip_name) {
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_CX700:
			viafb_vt1636_patch_skew_on_vt3324(plvds_setting_info,
						    plvds_chip_info);
			break;
		}
	}
}
static void lcd_patch_skew(struct lvds_setting_information
	*plvds_setting_info, struct lvds_chip_information *plvds_chip_info)
{
	DEBUG_MSG(KERN_INFO "lcd_patch_skew\n");
	switch (plvds_chip_info->output_interface) {
	case INTERFACE_DVP0:
		lcd_patch_skew_dvp0(plvds_setting_info, plvds_chip_info);
		break;
	case INTERFACE_DVP1:
		lcd_patch_skew_dvp1(plvds_setting_info, plvds_chip_info);
		break;
	case INTERFACE_DFP_LOW:
		if (UNICHROME_P4M900 == viaparinfo->chip_info->gfx_chip_name) {
			viafb_write_reg_mask(CR99, VIACR, 0x08,
				       BIT0 + BIT1 + BIT2 + BIT3);
		}
		break;
	}
}

/* LCD Set Mode */
void viafb_lcd_set_mode(struct crt_mode_table *mode_crt_table,
		  struct lvds_setting_information *plvds_setting_info,
		  struct lvds_chip_information *plvds_chip_info)
{
	int video_index = plvds_setting_info->lcd_panel_size;
	int set_iga = plvds_setting_info->iga_path;
	int mode_bpp = plvds_setting_info->bpp;
	int set_hres, set_vres;
	int panel_hres, panel_vres;
	u32 pll_D_N;
	int offset;
	struct display_timing mode_crt_reg, panel_crt_reg;
	struct crt_mode_table *panel_crt_table = NULL;
	struct VideoModeTable *vmode_tbl = NULL;

	DEBUG_MSG(KERN_INFO "viafb_lcd_set_mode!!\n");
	/* Get mode table */
	mode_crt_reg = mode_crt_table->crtc;
	/* Get panel table Pointer */
	vmode_tbl = viafb_get_modetbl_pointer(video_index);
	panel_crt_table = vmode_tbl->crtc;
	panel_crt_reg = panel_crt_table->crtc;
	DEBUG_MSG(KERN_INFO "bellow viafb_lcd_set_mode!!\n");
	set_hres = plvds_setting_info->h_active;
	set_vres = plvds_setting_info->v_active;
	panel_hres = plvds_setting_info->lcd_panel_hres;
	panel_vres = plvds_setting_info->lcd_panel_vres;
	if (VT1636_LVDS == plvds_chip_info->lvds_chip_name)
		viafb_init_lvds_vt1636(plvds_setting_info, plvds_chip_info);
	plvds_setting_info->vclk = panel_crt_table->clk;
	if (set_iga == IGA1) {
		/* IGA1 doesn't have LCD scaling, so set it as centering. */
		viafb_load_crtc_timing(lcd_centering_timging
				 (mode_crt_reg, panel_crt_reg), IGA1);
	} else {
		/* Expansion */
		if ((plvds_setting_info->display_method ==
		     LCD_EXPANDSION) & ((set_hres != panel_hres)
					|| (set_vres != panel_vres))) {
			/* expansion timing IGA2 loaded panel set timing*/
			viafb_load_crtc_timing(panel_crt_reg, IGA2);
			DEBUG_MSG(KERN_INFO "viafb_load_crtc_timing!!\n");
			load_lcd_scaling(set_hres, set_vres, panel_hres,
					 panel_vres);
			DEBUG_MSG(KERN_INFO "load_lcd_scaling!!\n");
		} else {	/* Centering */
			/* centering timing IGA2 always loaded panel
			   and mode releative timing */
			viafb_load_crtc_timing(lcd_centering_timging
					 (mode_crt_reg, panel_crt_reg), IGA2);
			viafb_write_reg_mask(CR79, VIACR, 0x00,
				BIT0 + BIT1 + BIT2);
			/* LCD scaling disabled */
		}
	}

	if (set_iga == IGA1_IGA2) {
		load_crtc_shadow_timing(mode_crt_reg, panel_crt_reg);
		/* Fill shadow registers */

		switch (plvds_setting_info->lcd_panel_id) {
		case LCD_PANEL_ID0_640X480:
			offset = 80;
			break;
		case LCD_PANEL_ID1_800X600:
		case LCD_PANEL_IDA_800X480:
			offset = 110;
			break;
		case LCD_PANEL_ID2_1024X768:
			offset = 150;
			break;
		case LCD_PANEL_ID3_1280X768:
		case LCD_PANEL_ID4_1280X1024:
		case LCD_PANEL_ID5_1400X1050:
		case LCD_PANEL_ID9_1280X800:
			offset = 190;
			break;
		case LCD_PANEL_ID6_1600X1200:
			offset = 250;
			break;
		case LCD_PANEL_ID7_1366X768:
		case LCD_PANEL_IDB_1360X768:
			offset = 212;
			break;
		default:
			offset = 140;
			break;
		}

		/* Offset for simultaneous */
		viafb_set_secondary_pitch(offset << 3);
		DEBUG_MSG(KERN_INFO "viafb_load_reg!!\n");
		viafb_load_fetch_count_reg(set_hres, 4, IGA2);
		/* Fetch count for simultaneous */
	} else {		/* SAMM */
		/* Fetch count for IGA2 only */
		viafb_load_fetch_count_reg(set_hres, mode_bpp / 8, set_iga);

		if ((viaparinfo->chip_info->gfx_chip_name != UNICHROME_CLE266)
		    && (viaparinfo->chip_info->gfx_chip_name != UNICHROME_K400))
			viafb_load_FIFO_reg(set_iga, set_hres, set_vres);

		viafb_set_color_depth(mode_bpp / 8, set_iga);
	}

	fill_lcd_format();

	pll_D_N = viafb_get_clk_value(panel_crt_table[0].clk);
	DEBUG_MSG(KERN_INFO "PLL=0x%x", pll_D_N);
	viafb_set_vclock(pll_D_N, set_iga);

	viafb_set_output_path(DEVICE_LCD, set_iga,
		plvds_chip_info->output_interface);
	lcd_patch_skew(plvds_setting_info, plvds_chip_info);

	/* If K8M800, enable LCD Prefetch Mode. */
	if ((viaparinfo->chip_info->gfx_chip_name == UNICHROME_K800)
	    || (UNICHROME_K8M890 == viaparinfo->chip_info->gfx_chip_name))
		viafb_write_reg_mask(CR6A, VIACR, 0x01, BIT0);

	load_lcd_patch_regs(set_hres, set_vres,
			    plvds_setting_info->lcd_panel_id, set_iga);

	DEBUG_MSG(KERN_INFO "load_lcd_patch_regs!!\n");

	/* Patch for non 32bit alignment mode */
	via_pitch_alignment_patch_lcd(plvds_setting_info, plvds_chip_info);
}

static void integrated_lvds_disable(struct lvds_setting_information
			     *plvds_setting_info,
			     struct lvds_chip_information *plvds_chip_info)
{
	bool turn_off_first_powersequence = false;
	bool turn_off_second_powersequence = false;
	if (INTERFACE_LVDS0LVDS1 == plvds_chip_info->output_interface)
		turn_off_first_powersequence = true;
	if (INTERFACE_LVDS0 == plvds_chip_info->output_interface)
		turn_off_first_powersequence = true;
	if (INTERFACE_LVDS1 == plvds_chip_info->output_interface)
		turn_off_second_powersequence = true;
	if (turn_off_second_powersequence) {
		/* Use second power sequence control: */

		/* Turn off power sequence. */
		viafb_write_reg_mask(CRD4, VIACR, 0, BIT1);

		/* Turn off back light. */
		viafb_write_reg_mask(CRD3, VIACR, 0xC0, BIT6 + BIT7);
	}
	if (turn_off_first_powersequence) {
		/* Use first power sequence control: */

		/* Turn off power sequence. */
		viafb_write_reg_mask(CR6A, VIACR, 0, BIT3);

		/* Turn off back light. */
		viafb_write_reg_mask(CR91, VIACR, 0xC0, BIT6 + BIT7);
	}

	/* Turn DFP High/Low Pad off. */
	viafb_write_reg_mask(SR2A, VIASR, 0, BIT0 + BIT1 + BIT2 + BIT3);

	/* Power off LVDS channel. */
	switch (plvds_chip_info->output_interface) {
	case INTERFACE_LVDS0:
		{
			viafb_write_reg_mask(CRD2, VIACR, 0x80, BIT7);
			break;
		}

	case INTERFACE_LVDS1:
		{
			viafb_write_reg_mask(CRD2, VIACR, 0x40, BIT6);
			break;
		}

	case INTERFACE_LVDS0LVDS1:
		{
			viafb_write_reg_mask(CRD2, VIACR, 0xC0, BIT6 + BIT7);
			break;
		}
	}
}

static void integrated_lvds_enable(struct lvds_setting_information
			    *plvds_setting_info,
			    struct lvds_chip_information *plvds_chip_info)
{
	bool turn_on_first_powersequence = false;
	bool turn_on_second_powersequence = false;

	DEBUG_MSG(KERN_INFO "integrated_lvds_enable, out_interface:%d\n",
		  plvds_chip_info->output_interface);
	if (plvds_setting_info->lcd_mode == LCD_SPWG)
		viafb_write_reg_mask(CRD2, VIACR, 0x00, BIT0 + BIT1);
	 else
		viafb_write_reg_mask(CRD2, VIACR, 0x03, BIT0 + BIT1);
	if (INTERFACE_LVDS0LVDS1 == plvds_chip_info->output_interface)
		turn_on_first_powersequence = true;
	if (INTERFACE_LVDS0 == plvds_chip_info->output_interface)
		turn_on_first_powersequence = true;
	if (INTERFACE_LVDS1 == plvds_chip_info->output_interface)
		turn_on_second_powersequence = true;

	if (turn_on_second_powersequence) {
		/* Use second power sequence control: */

		/* Use hardware control power sequence. */
		viafb_write_reg_mask(CRD3, VIACR, 0, BIT0);

		/* Turn on back light. */
		viafb_write_reg_mask(CRD3, VIACR, 0, BIT6 + BIT7);

		/* Turn on hardware power sequence. */
		viafb_write_reg_mask(CRD4, VIACR, 0x02, BIT1);
	}
	if (turn_on_first_powersequence) {
		/* Use first power sequence control: */

		/* Use hardware control power sequence. */
		viafb_write_reg_mask(CR91, VIACR, 0, BIT0);

		/* Turn on back light. */
		viafb_write_reg_mask(CR91, VIACR, 0, BIT6 + BIT7);

		/* Turn on hardware power sequence. */
		viafb_write_reg_mask(CR6A, VIACR, 0x08, BIT3);
	}

	/* Turn DFP High/Low pad on. */
	viafb_write_reg_mask(SR2A, VIASR, 0x0F, BIT0 + BIT1 + BIT2 + BIT3);

	/* Power on LVDS channel. */
	switch (plvds_chip_info->output_interface) {
	case INTERFACE_LVDS0:
		{
			viafb_write_reg_mask(CRD2, VIACR, 0, BIT7);
			break;
		}

	case INTERFACE_LVDS1:
		{
			viafb_write_reg_mask(CRD2, VIACR, 0, BIT6);
			break;
		}

	case INTERFACE_LVDS0LVDS1:
		{
			viafb_write_reg_mask(CRD2, VIACR, 0, BIT6 + BIT7);
			break;
		}
	}
}

void viafb_lcd_disable(void)
{

	if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266) {
		lcd_powersequence_off();
		/* DI1 pad off */
		viafb_write_reg_mask(SR1E, VIASR, 0x00, 0x30);
	} else if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700) {
		if (viafb_LCD2_ON
		    && (INTEGRATED_LVDS ==
			viaparinfo->chip_info->lvds_chip_info2.lvds_chip_name))
			integrated_lvds_disable(viaparinfo->lvds_setting_info,
				&viaparinfo->chip_info->lvds_chip_info2);
		if (INTEGRATED_LVDS ==
			viaparinfo->chip_info->lvds_chip_info.lvds_chip_name)
			integrated_lvds_disable(viaparinfo->lvds_setting_info,
				&viaparinfo->chip_info->lvds_chip_info);
		if (VT1636_LVDS == viaparinfo->chip_info->
			lvds_chip_info.lvds_chip_name)
			viafb_disable_lvds_vt1636(viaparinfo->lvds_setting_info,
				&viaparinfo->chip_info->lvds_chip_info);
	} else if (VT1636_LVDS ==
	viaparinfo->chip_info->lvds_chip_info.lvds_chip_name) {
		viafb_disable_lvds_vt1636(viaparinfo->lvds_setting_info,
				    &viaparinfo->chip_info->lvds_chip_info);
	} else {
		/* DFP-HL pad off          */
		viafb_write_reg_mask(SR2A, VIASR, 0x00, 0x0F);
		/* Backlight off           */
		viafb_write_reg_mask(SR3D, VIASR, 0x00, 0x20);
		/* 24 bit DI data paht off */
		viafb_write_reg_mask(CR91, VIACR, 0x80, 0x80);
		/* Simultaneout disabled   */
		viafb_write_reg_mask(CR6B, VIACR, 0x00, 0x08);
	}

	/* Disable expansion bit   */
	viafb_write_reg_mask(CR79, VIACR, 0x00, 0x01);
	/* CRT path set to IGA1    */
	viafb_write_reg_mask(SR16, VIASR, 0x00, 0x40);
	/* Simultaneout disabled   */
	viafb_write_reg_mask(CR6B, VIACR, 0x00, 0x08);
	/* IGA2 path disabled      */
	viafb_write_reg_mask(CR6A, VIACR, 0x00, 0x80);

}

void viafb_lcd_enable(void)
{
	if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CLE266) {
		/* DI1 pad on */
		viafb_write_reg_mask(SR1E, VIASR, 0x30, 0x30);
		lcd_powersequence_on();
	} else if (viaparinfo->chip_info->gfx_chip_name == UNICHROME_CX700) {
		if (viafb_LCD2_ON && (INTEGRATED_LVDS ==
			viaparinfo->chip_info->lvds_chip_info2.lvds_chip_name))
			integrated_lvds_enable(viaparinfo->lvds_setting_info2, \
				&viaparinfo->chip_info->lvds_chip_info2);
		if (INTEGRATED_LVDS ==
			viaparinfo->chip_info->lvds_chip_info.lvds_chip_name)
			integrated_lvds_enable(viaparinfo->lvds_setting_info,
				&viaparinfo->chip_info->lvds_chip_info);
		if (VT1636_LVDS == viaparinfo->chip_info->
			lvds_chip_info.lvds_chip_name)
			viafb_enable_lvds_vt1636(viaparinfo->
			lvds_setting_info, &viaparinfo->chip_info->
			lvds_chip_info);
	} else if (VT1636_LVDS ==
	viaparinfo->chip_info->lvds_chip_info.lvds_chip_name) {
		viafb_enable_lvds_vt1636(viaparinfo->lvds_setting_info,
				   &viaparinfo->chip_info->lvds_chip_info);
	} else {
		/* DFP-HL pad on           */
		viafb_write_reg_mask(SR2A, VIASR, 0x0F, 0x0F);
		/* Backlight on            */
		viafb_write_reg_mask(SR3D, VIASR, 0x20, 0x20);
		/* 24 bit DI data paht on  */
		viafb_write_reg_mask(CR91, VIACR, 0x00, 0x80);

		/* Set data source selection bit by iga path */
		if (viaparinfo->lvds_setting_info->iga_path == IGA1) {
			/* DFP-H set to IGA1       */
			viafb_write_reg_mask(CR97, VIACR, 0x00, 0x10);
			/* DFP-L set to IGA1       */
			viafb_write_reg_mask(CR99, VIACR, 0x00, 0x10);
		} else {
			/* DFP-H set to IGA2       */
			viafb_write_reg_mask(CR97, VIACR, 0x10, 0x10);
			/* DFP-L set to IGA2       */
			viafb_write_reg_mask(CR99, VIACR, 0x10, 0x10);
		}
		/* LCD enabled             */
		viafb_write_reg_mask(CR6A, VIACR, 0x48, 0x48);
	}

	if ((viaparinfo->lvds_setting_info->iga_path == IGA1)
	    || (viaparinfo->lvds_setting_info->iga_path == IGA1_IGA2)) {
		/* CRT path set to IGA2    */
		viafb_write_reg_mask(SR16, VIASR, 0x40, 0x40);
		/* IGA2 path disabled      */
		viafb_write_reg_mask(CR6A, VIACR, 0x00, 0x80);
		/* IGA2 path enabled       */
	} else {		/* IGA2 */
		viafb_write_reg_mask(CR6A, VIACR, 0x80, 0x80);
	}

}

static void lcd_powersequence_off(void)
{
	int i, mask, data;

	/* Software control power sequence */
	viafb_write_reg_mask(CR91, VIACR, 0x11, 0x11);

	for (i = 0; i < 3; i++) {
		mask = PowerSequenceOff[0][i];
		data = PowerSequenceOff[1][i] & mask;
		viafb_write_reg_mask(CR91, VIACR, (u8) data, (u8) mask);
		udelay(PowerSequenceOff[2][i]);
	}

	/* Disable LCD */
	viafb_write_reg_mask(CR6A, VIACR, 0x00, 0x08);
}

static void lcd_powersequence_on(void)
{
	int i, mask, data;

	/* Software control power sequence */
	viafb_write_reg_mask(CR91, VIACR, 0x11, 0x11);

	/* Enable LCD */
	viafb_write_reg_mask(CR6A, VIACR, 0x08, 0x08);

	for (i = 0; i < 3; i++) {
		mask = PowerSequenceOn[0][i];
		data = PowerSequenceOn[1][i] & mask;
		viafb_write_reg_mask(CR91, VIACR, (u8) data, (u8) mask);
		udelay(PowerSequenceOn[2][i]);
	}

	udelay(1);
}

static void fill_lcd_format(void)
{
	u8 bdithering = 0, bdual = 0;

	if (viaparinfo->lvds_setting_info->device_lcd_dualedge)
		bdual = BIT4;
	if (viaparinfo->lvds_setting_info->LCDDithering)
		bdithering = BIT0;
	/* Dual & Dithering */
	viafb_write_reg_mask(CR88, VIACR, (bdithering | bdual), BIT4 + BIT0);
}

static void check_diport_of_integrated_lvds(
	struct lvds_chip_information *plvds_chip_info,
				     struct lvds_setting_information
				     *plvds_setting_info)
{
	/* Determine LCD DI Port by hardware layout. */
	switch (viafb_display_hardware_layout) {
	case HW_LAYOUT_LCD_ONLY:
		{
			if (plvds_setting_info->device_lcd_dualedge) {
				plvds_chip_info->output_interface =
				    INTERFACE_LVDS0LVDS1;
			} else {
				plvds_chip_info->output_interface =
				    INTERFACE_LVDS0;
			}

			break;
		}

	case HW_LAYOUT_DVI_ONLY:
		{
			plvds_chip_info->output_interface = INTERFACE_NONE;
			break;
		}

	case HW_LAYOUT_LCD1_LCD2:
	case HW_LAYOUT_LCD_EXTERNAL_LCD2:
		{
			plvds_chip_info->output_interface =
			    INTERFACE_LVDS0LVDS1;
			break;
		}

	case HW_LAYOUT_LCD_DVI:
		{
			plvds_chip_info->output_interface = INTERFACE_LVDS1;
			break;
		}

	default:
		{
			plvds_chip_info->output_interface = INTERFACE_LVDS1;
			break;
		}
	}

	DEBUG_MSG(KERN_INFO
		  "Display Hardware Layout: 0x%x, LCD DI Port: 0x%x\n",
		  viafb_display_hardware_layout,
		  plvds_chip_info->output_interface);
}

void viafb_init_lvds_output_interface(struct lvds_chip_information
				*plvds_chip_info,
				struct lvds_setting_information
				*plvds_setting_info)
{
	if (INTERFACE_NONE != plvds_chip_info->output_interface) {
		/*Do nothing, lcd port is specified by module parameter */
		return;
	}

	switch (plvds_chip_info->lvds_chip_name) {

	case VT1636_LVDS:
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_CX700:
			plvds_chip_info->output_interface = INTERFACE_DVP1;
			break;
		case UNICHROME_CN700:
			plvds_chip_info->output_interface = INTERFACE_DFP_LOW;
			break;
		default:
			plvds_chip_info->output_interface = INTERFACE_DVP0;
			break;
		}
		break;

	case INTEGRATED_LVDS:
		check_diport_of_integrated_lvds(plvds_chip_info,
						plvds_setting_info);
		break;

	default:
		switch (viaparinfo->chip_info->gfx_chip_name) {
		case UNICHROME_K8M890:
		case UNICHROME_P4M900:
		case UNICHROME_P4M890:
			plvds_chip_info->output_interface = INTERFACE_DFP_LOW;
			break;
		default:
			plvds_chip_info->output_interface = INTERFACE_DFP;
			break;
		}
		break;
	}
}

static struct display_timing lcd_centering_timging(struct display_timing
					    mode_crt_reg,
					    struct display_timing panel_crt_reg)
{
	struct display_timing crt_reg;

	crt_reg.hor_total = panel_crt_reg.hor_total;
	crt_reg.hor_addr = mode_crt_reg.hor_addr;
	crt_reg.hor_blank_start =
	    (panel_crt_reg.hor_addr - mode_crt_reg.hor_addr) / 2 +
	    crt_reg.hor_addr;
	crt_reg.hor_blank_end = panel_crt_reg.hor_blank_end;
	crt_reg.hor_sync_start =
	    (panel_crt_reg.hor_sync_start -
	     panel_crt_reg.hor_blank_start) + crt_reg.hor_blank_start;
	crt_reg.hor_sync_end = panel_crt_reg.hor_sync_end;

	crt_reg.ver_total = panel_crt_reg.ver_total;
	crt_reg.ver_addr = mode_crt_reg.ver_addr;
	crt_reg.ver_blank_start =
	    (panel_crt_reg.ver_addr - mode_crt_reg.ver_addr) / 2 +
	    crt_reg.ver_addr;
	crt_reg.ver_blank_end = panel_crt_reg.ver_blank_end;
	crt_reg.ver_sync_start =
	    (panel_crt_reg.ver_sync_start -
	     panel_crt_reg.ver_blank_start) + crt_reg.ver_blank_start;
	crt_reg.ver_sync_end = panel_crt_reg.ver_sync_end;

	return crt_reg;
}

static void load_crtc_shadow_timing(struct display_timing mode_timing,
			     struct display_timing panel_timing)
{
	struct io_register *reg = NULL;
	int i;
	int viafb_load_reg_Num = 0;
	int reg_value = 0;

	if (viaparinfo->lvds_setting_info->display_method == LCD_EXPANDSION) {
		/* Expansion */
		for (i = 12; i < 20; i++) {
			switch (i) {
			case H_TOTAL_SHADOW_INDEX:
				reg_value =
				    IGA2_HOR_TOTAL_SHADOW_FORMULA
				    (panel_timing.hor_total);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.hor_total_shadow.
				    reg_num;
				reg = iga2_shadow_crtc_reg.hor_total_shadow.reg;
				break;
			case H_BLANK_END_SHADOW_INDEX:
				reg_value =
				    IGA2_HOR_BLANK_END_SHADOW_FORMULA
				    (panel_timing.hor_blank_start,
				     panel_timing.hor_blank_end);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    hor_blank_end_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    hor_blank_end_shadow.reg;
				break;
			case V_TOTAL_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_TOTAL_SHADOW_FORMULA
				    (panel_timing.ver_total);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.ver_total_shadow.
				    reg_num;
				reg = iga2_shadow_crtc_reg.ver_total_shadow.reg;
				break;
			case V_ADDR_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_ADDR_SHADOW_FORMULA
				    (panel_timing.ver_addr);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.ver_addr_shadow.
				    reg_num;
				reg = iga2_shadow_crtc_reg.ver_addr_shadow.reg;
				break;
			case V_BLANK_SATRT_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_BLANK_START_SHADOW_FORMULA
				    (panel_timing.ver_blank_start);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    ver_blank_start_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    ver_blank_start_shadow.reg;
				break;
			case V_BLANK_END_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_BLANK_END_SHADOW_FORMULA
				    (panel_timing.ver_blank_start,
				     panel_timing.ver_blank_end);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    ver_blank_end_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    ver_blank_end_shadow.reg;
				break;
			case V_SYNC_SATRT_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_SYNC_START_SHADOW_FORMULA
				    (panel_timing.ver_sync_start);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    ver_sync_start_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    ver_sync_start_shadow.reg;
				break;
			case V_SYNC_END_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_SYNC_END_SHADOW_FORMULA
				    (panel_timing.ver_sync_start,
				     panel_timing.ver_sync_end);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    ver_sync_end_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    ver_sync_end_shadow.reg;
				break;
			}
			viafb_load_reg(reg_value,
				viafb_load_reg_Num, reg, VIACR);
		}
	} else {		/* Centering */
		for (i = 12; i < 20; i++) {
			switch (i) {
			case H_TOTAL_SHADOW_INDEX:
				reg_value =
				    IGA2_HOR_TOTAL_SHADOW_FORMULA
				    (panel_timing.hor_total);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.hor_total_shadow.
				    reg_num;
				reg = iga2_shadow_crtc_reg.hor_total_shadow.reg;
				break;
			case H_BLANK_END_SHADOW_INDEX:
				reg_value =
				    IGA2_HOR_BLANK_END_SHADOW_FORMULA
				    (panel_timing.hor_blank_start,
				     panel_timing.hor_blank_end);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    hor_blank_end_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    hor_blank_end_shadow.reg;
				break;
			case V_TOTAL_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_TOTAL_SHADOW_FORMULA
				    (panel_timing.ver_total);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.ver_total_shadow.
				    reg_num;
				reg = iga2_shadow_crtc_reg.ver_total_shadow.reg;
				break;
			case V_ADDR_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_ADDR_SHADOW_FORMULA
				    (mode_timing.ver_addr);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.ver_addr_shadow.
				    reg_num;
				reg = iga2_shadow_crtc_reg.ver_addr_shadow.reg;
				break;
			case V_BLANK_SATRT_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_BLANK_START_SHADOW_FORMULA
				    (mode_timing.ver_blank_start);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    ver_blank_start_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    ver_blank_start_shadow.reg;
				break;
			case V_BLANK_END_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_BLANK_END_SHADOW_FORMULA
				    (panel_timing.ver_blank_start,
				     panel_timing.ver_blank_end);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.
				    ver_blank_end_shadow.reg_num;
				reg =
				    iga2_shadow_crtc_reg.
				    ver_blank_end_shadow.reg;
				break;
			case V_SYNC_SATRT_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_SYNC_START_SHADOW_FORMULA(
				    (panel_timing.ver_sync_start -
				    panel_timing.ver_blank_start) +
				    (panel_timing.ver_addr -
				    mode_timing.ver_addr) / 2 +
				    mode_timing.ver_addr);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.ver_sync_start_shadow.
				    reg_num;
				reg =
				    iga2_shadow_crtc_reg.ver_sync_start_shadow.
				    reg;
				break;
			case V_SYNC_END_SHADOW_INDEX:
				reg_value =
				    IGA2_VER_SYNC_END_SHADOW_FORMULA(
				    (panel_timing.ver_sync_start -
				    panel_timing.ver_blank_start) +
				    (panel_timing.ver_addr -
				    mode_timing.ver_addr) / 2 +
				    mode_timing.ver_addr,
				    panel_timing.ver_sync_end);
				viafb_load_reg_Num =
				    iga2_shadow_crtc_reg.ver_sync_end_shadow.
				    reg_num;
				reg =
				    iga2_shadow_crtc_reg.ver_sync_end_shadow.
				    reg;
				break;
			}
			viafb_load_reg(reg_value,
				viafb_load_reg_Num, reg, VIACR);
		}
	}
}

bool viafb_lcd_get_mobile_state(bool *mobile)
{
	unsigned char *romptr, *tableptr;
	u8 core_base;
	unsigned char *biosptr;
	/* Rom address */
	u32 romaddr = 0x000C0000;
	u16 start_pattern = 0;

	biosptr = ioremap(romaddr, 0x10000);

	memcpy(&start_pattern, biosptr, 2);
	/* Compare pattern */
	if (start_pattern == 0xAA55) {
		/* Get the start of Table */
		/* 0x1B means BIOS offset position */
		romptr = biosptr + 0x1B;
		tableptr = biosptr + *((u16 *) romptr);

		/* Get the start of biosver structure */
		/* 18 means BIOS version position. */
		romptr = tableptr + 18;
		romptr = biosptr + *((u16 *) romptr);

		/* The offset should be 44, but the
		   actual image is less three char. */
		/* pRom += 44; */
		romptr += 41;

		core_base = *romptr++;

		if (core_base & 0x8)
			*mobile = false;
		else
			*mobile = true;
		/* release memory */
		iounmap(biosptr);

		return true;
	} else {
		iounmap(biosptr);
		return false;
	}
}

static void viafb_load_scaling_factor_for_p4m900(int set_hres,
	int set_vres, int panel_hres, int panel_vres)
{
	int h_scaling_factor;
	int v_scaling_factor;
	u8 cra2 = 0;
	u8 cr77 = 0;
	u8 cr78 = 0;
	u8 cr79 = 0;
	u8 cr9f = 0;
	/* Check if expansion for horizontal */
	if (set_hres < panel_hres) {
		/* Load Horizontal Scaling Factor */

		/* For VIA_K8M800 or later chipsets. */
		h_scaling_factor =
		    K800_LCD_HOR_SCF_FORMULA(set_hres, panel_hres);
		/* HSCaleFactor[1:0] at CR9F[1:0] */
		cr9f = h_scaling_factor & 0x0003;
		/* HSCaleFactor[9:2] at CR77[7:0] */
		cr77 = (h_scaling_factor & 0x03FC) >> 2;
		/* HSCaleFactor[11:10] at CR79[5:4] */
		cr79 = (h_scaling_factor & 0x0C00) >> 10;
		cr79 <<= 4;

		/* Horizontal scaling enabled */
		cra2 = 0xC0;

		DEBUG_MSG(KERN_INFO "Horizontal Scaling value = %d\n",
			  h_scaling_factor);
	} else {
		/* Horizontal scaling disabled */
		cra2 = 0x00;
	}

	/* Check if expansion for vertical */
	if (set_vres < panel_vres) {
		/* Load Vertical Scaling Factor */

		/* For VIA_K8M800 or later chipsets. */
		v_scaling_factor =
		    K800_LCD_VER_SCF_FORMULA(set_vres, panel_vres);

		/* Vertical scaling enabled */
		cra2 |= 0x08;
		/* VSCaleFactor[0] at CR79[3] */
		cr79 |= ((v_scaling_factor & 0x0001) << 3);
		/* VSCaleFactor[8:1] at CR78[7:0] */
		cr78 |= (v_scaling_factor & 0x01FE) >> 1;
		/* VSCaleFactor[10:9] at CR79[7:6] */
		cr79 |= ((v_scaling_factor & 0x0600) >> 9) << 6;

		DEBUG_MSG(KERN_INFO "Vertical Scaling value = %d\n",
			  v_scaling_factor);
	} else {
		/* Vertical scaling disabled */
		cra2 |= 0x00;
	}

	viafb_write_reg_mask(CRA2, VIACR, cra2, BIT3 + BIT6 + BIT7);
	viafb_write_reg_mask(CR77, VIACR, cr77, 0xFF);
	viafb_write_reg_mask(CR78, VIACR, cr78, 0xFF);
	viafb_write_reg_mask(CR79, VIACR, cr79, 0xF8);
	viafb_write_reg_mask(CR9F, VIACR, cr9f, BIT0 + BIT1);
}

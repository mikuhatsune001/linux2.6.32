/*
 *		sonix sn9c102 (bayer) library
 *		Copyright (C) 2003 2004 Michel Xhaard mxhaard@magic.fr
 * Add Pas106 Stefano Mozzi (C) 2004
 *
 * V4L2 by Jean-Francois Moine <http://moinejf.free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* Some documentation on known sonixb registers:

Reg	Use
0x10	high nibble red gain low nibble blue gain
0x11	low nibble green gain
0x12	hstart
0x13	vstart
0x15	hsize (hsize = register-value * 16)
0x16	vsize (vsize = register-value * 16)
0x17	bit 0 toggle compression quality (according to sn9c102 driver)
0x18	bit 7 enables compression, bit 4-5 set image down scaling:
	00 scale 1, 01 scale 1/2, 10, scale 1/4
0x19	high-nibble is sensor clock divider, changes exposure on sensors which
	use a clock generated by the bridge. Some sensors have their own clock.
0x1c	auto_exposure area (for avg_lum) startx (startx = register-value * 32)
0x1d	auto_exposure area (for avg_lum) starty (starty = register-value * 32)
0x1e	auto_exposure area (for avg_lum) stopx (hsize = (0x1e - 0x1c) * 32)
0x1f	auto_exposure area (for avg_lum) stopy (vsize = (0x1f - 0x1d) * 32)
*/

#define MODULE_NAME "sonixb"

#include "gspca.h"

MODULE_AUTHOR("Michel Xhaard <mxhaard@users.sourceforge.net>");
MODULE_DESCRIPTION("GSPCA/SN9C102 USB Camera Driver");
MODULE_LICENSE("GPL");

/* specific webcam descriptor */
struct sd {
	struct gspca_dev gspca_dev;	/* !! must be the first item */
	atomic_t avg_lum;
	int prev_avg_lum;

	unsigned char gain;
	unsigned char exposure;
	unsigned char brightness;
	unsigned char autogain;
	unsigned char autogain_ignore_frames;
	unsigned char frames_to_drop;
	unsigned char freq;		/* light freq filter setting */

	__u8 bridge;			/* Type of bridge */
#define BRIDGE_101 0
#define BRIDGE_102 0 /* We make no difference between 101 and 102 */
#define BRIDGE_103 1

	__u8 sensor;			/* Type of image sensor chip */
#define SENSOR_HV7131R 0
#define SENSOR_OV6650 1
#define SENSOR_OV7630 2
#define SENSOR_PAS106 3
#define SENSOR_PAS202 4
#define SENSOR_TAS5110 5
#define SENSOR_TAS5130CXX 6
	__u8 reg11;
};

typedef const __u8 sensor_init_t[8];

struct sensor_data {
	const __u8 *bridge_init[2];
	int bridge_init_size[2];
	sensor_init_t *sensor_init;
	int sensor_init_size;
	sensor_init_t *sensor_bridge_init[2];
	int sensor_bridge_init_size[2];
	int flags;
	unsigned ctrl_dis;
	__u8 sensor_addr;
};

/* sensor_data flags */
#define F_GAIN 0x01		/* has gain */
#define F_SIF  0x02		/* sif or vga */

/* priv field of struct v4l2_pix_format flags (do not use low nibble!) */
#define MODE_RAW 0x10		/* raw bayer mode */
#define MODE_REDUCED_SIF 0x20	/* vga mode (320x240 / 160x120) on sif cam */

/* ctrl_dis helper macros */
#define NO_EXPO ((1 << EXPOSURE_IDX) | (1 << AUTOGAIN_IDX))
#define NO_FREQ (1 << FREQ_IDX)
#define NO_BRIGHTNESS (1 << BRIGHTNESS_IDX)

#define COMP2 0x8f
#define COMP 0xc7		/* 0x87 //0x07 */
#define COMP1 0xc9		/* 0x89 //0x09 */

#define MCK_INIT 0x63
#define MCK_INIT1 0x20		/*fixme: Bayer - 0x50 for JPEG ??*/

#define SYS_CLK 0x04

#define SENS(bridge_1, bridge_3, sensor, sensor_1, \
	sensor_3, _flags, _ctrl_dis, _sensor_addr) \
{ \
	.bridge_init = { bridge_1, bridge_3 }, \
	.bridge_init_size = { sizeof(bridge_1), sizeof(bridge_3) }, \
	.sensor_init = sensor, \
	.sensor_init_size = sizeof(sensor), \
	.sensor_bridge_init = { sensor_1, sensor_3,}, \
	.sensor_bridge_init_size = { sizeof(sensor_1), sizeof(sensor_3)}, \
	.flags = _flags, .ctrl_dis = _ctrl_dis, .sensor_addr = _sensor_addr \
}

/* We calculate the autogain at the end of the transfer of a frame, at this
   moment a frame with the old settings is being transmitted, and a frame is
   being captured with the old settings. So if we adjust the autogain we must
   ignore atleast the 2 next frames for the new settings to come into effect
   before doing any other adjustments */
#define AUTOGAIN_IGNORE_FRAMES 3

/* V4L2 controls supported by the driver */
static int sd_setbrightness(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getbrightness(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setgain(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getgain(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setexposure(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getexposure(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setautogain(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getautogain(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setfreq(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getfreq(struct gspca_dev *gspca_dev, __s32 *val);

static struct ctrl sd_ctrls[] = {
#define BRIGHTNESS_IDX 0
	{
	    {
		.id      = V4L2_CID_BRIGHTNESS,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Brightness",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
#define BRIGHTNESS_DEF 127
		.default_value = BRIGHTNESS_DEF,
	    },
	    .set = sd_setbrightness,
	    .get = sd_getbrightness,
	},
#define GAIN_IDX 1
	{
	    {
		.id      = V4L2_CID_GAIN,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Gain",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
#define GAIN_DEF 127
#define GAIN_KNEE 200
		.default_value = GAIN_DEF,
	    },
	    .set = sd_setgain,
	    .get = sd_getgain,
	},
#define EXPOSURE_IDX 2
	{
		{
			.id = V4L2_CID_EXPOSURE,
			.type = V4L2_CTRL_TYPE_INTEGER,
			.name = "Exposure",
#define EXPOSURE_DEF  16 /*  32 ms / 30 fps */
#define EXPOSURE_KNEE 50 /* 100 ms / 10 fps */
			.minimum = 0,
			.maximum = 255,
			.step = 1,
			.default_value = EXPOSURE_DEF,
			.flags = 0,
		},
		.set = sd_setexposure,
		.get = sd_getexposure,
	},
#define AUTOGAIN_IDX 3
	{
		{
			.id = V4L2_CID_AUTOGAIN,
			.type = V4L2_CTRL_TYPE_BOOLEAN,
			.name = "Automatic Gain (and Exposure)",
			.minimum = 0,
			.maximum = 1,
			.step = 1,
#define AUTOGAIN_DEF 1
			.default_value = AUTOGAIN_DEF,
			.flags = 0,
		},
		.set = sd_setautogain,
		.get = sd_getautogain,
	},
#define FREQ_IDX 4
	{
		{
			.id	 = V4L2_CID_POWER_LINE_FREQUENCY,
			.type    = V4L2_CTRL_TYPE_MENU,
			.name    = "Light frequency filter",
			.minimum = 0,
			.maximum = 2,	/* 0: 0, 1: 50Hz, 2:60Hz */
			.step    = 1,
#define FREQ_DEF 1
			.default_value = FREQ_DEF,
		},
		.set = sd_setfreq,
		.get = sd_getfreq,
	},
};

static const struct v4l2_pix_format vga_mode[] = {
	{160, 120, V4L2_PIX_FMT_SBGGR8, V4L2_FIELD_NONE,
		.bytesperline = 160,
		.sizeimage = 160 * 120,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 2 | MODE_RAW},
	{160, 120, V4L2_PIX_FMT_SN9C10X, V4L2_FIELD_NONE,
		.bytesperline = 160,
		.sizeimage = 160 * 120 * 5 / 4,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 2},
	{320, 240, V4L2_PIX_FMT_SN9C10X, V4L2_FIELD_NONE,
		.bytesperline = 320,
		.sizeimage = 320 * 240 * 5 / 4,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 1},
	{640, 480, V4L2_PIX_FMT_SN9C10X, V4L2_FIELD_NONE,
		.bytesperline = 640,
		.sizeimage = 640 * 480 * 5 / 4,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0},
};
static const struct v4l2_pix_format sif_mode[] = {
	{160, 120, V4L2_PIX_FMT_SBGGR8, V4L2_FIELD_NONE,
		.bytesperline = 160,
		.sizeimage = 160 * 120,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 1 | MODE_RAW | MODE_REDUCED_SIF},
	{160, 120, V4L2_PIX_FMT_SN9C10X, V4L2_FIELD_NONE,
		.bytesperline = 160,
		.sizeimage = 160 * 120 * 5 / 4,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 1 | MODE_REDUCED_SIF},
	{176, 144, V4L2_PIX_FMT_SBGGR8, V4L2_FIELD_NONE,
		.bytesperline = 176,
		.sizeimage = 176 * 144,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 1 | MODE_RAW},
	{176, 144, V4L2_PIX_FMT_SN9C10X, V4L2_FIELD_NONE,
		.bytesperline = 176,
		.sizeimage = 176 * 144 * 5 / 4,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 1},
	{320, 240, V4L2_PIX_FMT_SN9C10X, V4L2_FIELD_NONE,
		.bytesperline = 320,
		.sizeimage = 320 * 240 * 5 / 4,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0 | MODE_REDUCED_SIF},
	{352, 288, V4L2_PIX_FMT_SN9C10X, V4L2_FIELD_NONE,
		.bytesperline = 352,
		.sizeimage = 352 * 288 * 5 / 4,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0},
};

static const __u8 initHv7131[] = {
	0x46, 0x77, 0x00, 0x04, 0x00, 0x00, 0x00, 0x80, 0x11, 0x00, 0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x01, 0x00,
	0x28, 0x1e, 0x60, 0x8a, 0x20,
	0x1d, 0x10, 0x02, 0x03, 0x0f, 0x0c
};
static const __u8 hv7131_sensor_init[][8] = {
	{0xc0, 0x11, 0x31, 0x38, 0x2a, 0x2e, 0x00, 0x10},
	{0xa0, 0x11, 0x01, 0x08, 0x2a, 0x2e, 0x00, 0x10},
	{0xb0, 0x11, 0x20, 0x00, 0xd0, 0x2e, 0x00, 0x10},
	{0xc0, 0x11, 0x25, 0x03, 0x0e, 0x28, 0x00, 0x16},
	{0xa0, 0x11, 0x30, 0x10, 0x0e, 0x28, 0x00, 0x15},
};
static const __u8 initOv6650[] = {
	0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x01, 0x0a, 0x16, 0x12, 0x68, 0x8b,
	0x10, 0x1d, 0x10, 0x02, 0x02, 0x09, 0x07
};
static const __u8 ov6650_sensor_init[][8] =
{
	/* Bright, contrast, etc are set througth SCBB interface.
	 * AVCAP on win2 do not send any data on this 	controls. */
	/* Anyway, some registers appears to alter bright and constrat */

	/* Reset sensor */
	{0xa0, 0x60, 0x12, 0x80, 0x00, 0x00, 0x00, 0x10},
	/* Set clock register 0x11 low nibble is clock divider */
	{0xd0, 0x60, 0x11, 0xc0, 0x1b, 0x18, 0xc1, 0x10},
	/* Next some unknown stuff */
	{0xb0, 0x60, 0x15, 0x00, 0x02, 0x18, 0xc1, 0x10},
/*	{0xa0, 0x60, 0x1b, 0x01, 0x02, 0x18, 0xc1, 0x10},
		 * THIS SET GREEN SCREEN
		 * (pixels could be innverted in decode kind of "brg",
		 * but blue wont be there. Avoid this data ... */
	{0xd0, 0x60, 0x26, 0x01, 0x14, 0xd8, 0xa4, 0x10}, /* format out? */
	{0xd0, 0x60, 0x26, 0x01, 0x14, 0xd8, 0xa4, 0x10},
	{0xa0, 0x60, 0x30, 0x3d, 0x0A, 0xd8, 0xa4, 0x10},
	/* Enable rgb brightness control */
	{0xa0, 0x60, 0x61, 0x08, 0x00, 0x00, 0x00, 0x10},
	/* HDG: Note windows uses the line below, which sets both register 0x60
	   and 0x61 I believe these registers of the ov6650 are identical as
	   those of the ov7630, because if this is true the windows settings
	   add a bit additional red gain and a lot additional blue gain, which
	   matches my findings that the windows settings make blue much too
	   blue and red a little too red.
	{0xb0, 0x60, 0x60, 0x66, 0x68, 0xd8, 0xa4, 0x10}, */
	/* Some more unknown stuff */
	{0xa0, 0x60, 0x68, 0x04, 0x68, 0xd8, 0xa4, 0x10},
	{0xd0, 0x60, 0x17, 0x24, 0xd6, 0x04, 0x94, 0x10}, /* Clipreg */
};

static const __u8 initOv7630[] = {
	0x04, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,	/* r01 .. r08 */
	0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/* r09 .. r10 */
	0x00, 0x01, 0x01, 0x0a,				/* r11 .. r14 */
	0x28, 0x1e,			/* H & V sizes     r15 .. r16 */
	0x68, COMP2, MCK_INIT1,				/* r17 .. r19 */
	0x1d, 0x10, 0x02, 0x03, 0x0f, 0x0c		/* r1a .. r1f */
};
static const __u8 initOv7630_3[] = {
	0x44, 0x44, 0x00, 0x1a, 0x20, 0x20, 0x20, 0x80,	/* r01 .. r08 */
	0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,	/* r09 .. r10 */
	0x00, 0x02, 0x01, 0x0a,				/* r11 .. r14 */
	0x28, 0x1e,			/* H & V sizes     r15 .. r16 */
	0x68, 0x8f, MCK_INIT1,				/* r17 .. r19 */
	0x1d, 0x10, 0x02, 0x03, 0x0f, 0x0c, 0x00,	/* r1a .. r20 */
	0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, /* r21 .. r28 */
	0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0, 0xff  /* r29 .. r30 */
};
static const __u8 ov7630_sensor_init[][8] = {
	{0xa0, 0x21, 0x12, 0x80, 0x00, 0x00, 0x00, 0x10},
	{0xb0, 0x21, 0x01, 0x77, 0x3a, 0x00, 0x00, 0x10},
/*	{0xd0, 0x21, 0x12, 0x7c, 0x01, 0x80, 0x34, 0x10},	   jfm */
	{0xd0, 0x21, 0x12, 0x1c, 0x00, 0x80, 0x34, 0x10},	/* jfm */
	{0xa0, 0x21, 0x1b, 0x04, 0x00, 0x80, 0x34, 0x10},
	{0xa0, 0x21, 0x20, 0x44, 0x00, 0x80, 0x34, 0x10},
	{0xa0, 0x21, 0x23, 0xee, 0x00, 0x80, 0x34, 0x10},
	{0xd0, 0x21, 0x26, 0xa0, 0x9a, 0xa0, 0x30, 0x10},
	{0xb0, 0x21, 0x2a, 0x80, 0x00, 0xa0, 0x30, 0x10},
	{0xb0, 0x21, 0x2f, 0x3d, 0x24, 0xa0, 0x30, 0x10},
	{0xa0, 0x21, 0x32, 0x86, 0x24, 0xa0, 0x30, 0x10},
	{0xb0, 0x21, 0x60, 0xa9, 0x4a, 0xa0, 0x30, 0x10},
/*	{0xb0, 0x21, 0x60, 0xa9, 0x42, 0xa0, 0x30, 0x10},	 * jfm */
	{0xa0, 0x21, 0x65, 0x00, 0x42, 0xa0, 0x30, 0x10},
	{0xa0, 0x21, 0x69, 0x38, 0x42, 0xa0, 0x30, 0x10},
	{0xc0, 0x21, 0x6f, 0x88, 0x0b, 0x00, 0x30, 0x10},
	{0xc0, 0x21, 0x74, 0x21, 0x8e, 0x00, 0x30, 0x10},
	{0xa0, 0x21, 0x7d, 0xf7, 0x8e, 0x00, 0x30, 0x10},
	{0xd0, 0x21, 0x17, 0x1c, 0xbd, 0x06, 0xf6, 0x10},
};

static const __u8 ov7630_sensor_init_3[][8] = {
	{0xa0, 0x21, 0x13, 0x80, 0x00,	0x00, 0x00, 0x10},
};

static const __u8 initPas106[] = {
	0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x40, 0x00, 0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x01, 0x00,
	0x16, 0x12, 0x24, COMP1, MCK_INIT1,
	0x18, 0x10, 0x02, 0x02, 0x09, 0x07
};
/* compression 0x86 mckinit1 0x2b */
static const __u8 pas106_sensor_init[][8] = {
	/* Pixel Clock Divider 6 */
	{ 0xa1, 0x40, 0x02, 0x04, 0x00, 0x00, 0x00, 0x14 },
	/* Frame Time MSB (also seen as 0x12) */
	{ 0xa1, 0x40, 0x03, 0x13, 0x00, 0x00, 0x00, 0x14 },
	/* Frame Time LSB (also seen as 0x05) */
	{ 0xa1, 0x40, 0x04, 0x06, 0x00, 0x00, 0x00, 0x14 },
	/* Shutter Time Line Offset (also seen as 0x6d) */
	{ 0xa1, 0x40, 0x05, 0x65, 0x00, 0x00, 0x00, 0x14 },
	/* Shutter Time Pixel Offset (also seen as 0xb1) */
	{ 0xa1, 0x40, 0x06, 0xcd, 0x00, 0x00, 0x00, 0x14 },
	/* Black Level Subtract Sign (also seen 0x00) */
	{ 0xa1, 0x40, 0x07, 0xc1, 0x00, 0x00, 0x00, 0x14 },
	/* Black Level Subtract Level (also seen 0x01) */
	{ 0xa1, 0x40, 0x08, 0x06, 0x00, 0x00, 0x00, 0x14 },
	{ 0xa1, 0x40, 0x08, 0x06, 0x00, 0x00, 0x00, 0x14 },
	/* Color Gain B Pixel 5 a */
	{ 0xa1, 0x40, 0x09, 0x05, 0x00, 0x00, 0x00, 0x14 },
	/* Color Gain G1 Pixel 1 5 */
	{ 0xa1, 0x40, 0x0a, 0x04, 0x00, 0x00, 0x00, 0x14 },
	/* Color Gain G2 Pixel 1 0 5 */
	{ 0xa1, 0x40, 0x0b, 0x04, 0x00, 0x00, 0x00, 0x14 },
	/* Color Gain R Pixel 3 1 */
	{ 0xa1, 0x40, 0x0c, 0x05, 0x00, 0x00, 0x00, 0x14 },
	/* Color GainH  Pixel */
	{ 0xa1, 0x40, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x14 },
	/* Global Gain */
	{ 0xa1, 0x40, 0x0e, 0x0e, 0x00, 0x00, 0x00, 0x14 },
	/* Contrast */
	{ 0xa1, 0x40, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x14 },
	/* H&V synchro polarity */
	{ 0xa1, 0x40, 0x10, 0x06, 0x00, 0x00, 0x00, 0x14 },
	/* ?default */
	{ 0xa1, 0x40, 0x11, 0x06, 0x00, 0x00, 0x00, 0x14 },
	/* DAC scale */
	{ 0xa1, 0x40, 0x12, 0x06, 0x00, 0x00, 0x00, 0x14 },
	/* ?default */
	{ 0xa1, 0x40, 0x14, 0x02, 0x00, 0x00, 0x00, 0x14 },
	/* Validate Settings */
	{ 0xa1, 0x40, 0x13, 0x01, 0x00, 0x00, 0x00, 0x14 },
};

static const __u8 initPas202[] = {
	0x44, 0x44, 0x21, 0x30, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x03, 0x0a,
	0x28, 0x1e, 0x28, 0x89, 0x20,
	0x00, 0x00, 0x02, 0x03, 0x0f, 0x0c
};
static const __u8 pas202_sensor_init[][8] = {
	{0xa0, 0x40, 0x02, 0x03, 0x00, 0x00, 0x00, 0x10},
	{0xd0, 0x40, 0x04, 0x07, 0x34, 0x00, 0x09, 0x10},
	{0xd0, 0x40, 0x08, 0x01, 0x00, 0x00, 0x01, 0x10},
	{0xd0, 0x40, 0x0C, 0x00, 0x0C, 0x00, 0x32, 0x10},
	{0xd0, 0x40, 0x10, 0x00, 0x01, 0x00, 0x63, 0x10},
	{0xa0, 0x40, 0x15, 0x70, 0x01, 0x00, 0x63, 0x10},
	{0xa0, 0x40, 0x18, 0x00, 0x01, 0x00, 0x63, 0x10},
	{0xa0, 0x40, 0x11, 0x01, 0x01, 0x00, 0x63, 0x10},
	{0xa0, 0x40, 0x03, 0x56, 0x01, 0x00, 0x63, 0x10},
	{0xa0, 0x40, 0x11, 0x01, 0x01, 0x00, 0x63, 0x10},
	{0xb0, 0x40, 0x04, 0x07, 0x2a, 0x00, 0x63, 0x10},
	{0xb0, 0x40, 0x0e, 0x00, 0x3d, 0x00, 0x63, 0x10},

	{0xa0, 0x40, 0x11, 0x01, 0x3d, 0x00, 0x63, 0x16},
	{0xa0, 0x40, 0x10, 0x08, 0x3d, 0x00, 0x63, 0x15},
	{0xa0, 0x40, 0x02, 0x04, 0x3d, 0x00, 0x63, 0x16},
	{0xa0, 0x40, 0x11, 0x01, 0x3d, 0x00, 0x63, 0x16},
	{0xb0, 0x40, 0x0e, 0x00, 0x31, 0x00, 0x63, 0x16},
	{0xa0, 0x40, 0x11, 0x01, 0x31, 0x00, 0x63, 0x16},
	{0xa0, 0x40, 0x10, 0x0e, 0x31, 0x00, 0x63, 0x15},
	{0xa0, 0x40, 0x11, 0x01, 0x31, 0x00, 0x63, 0x16},
};

static const __u8 initTas5110[] = {
	0x44, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x11, 0x00, 0x00, 0x00,
	0x00, 0x00,
	0x00, 0x01, 0x00, 0x45, 0x09, 0x0a,
	0x16, 0x12, 0x60, 0x86, 0x2b,
	0x14, 0x0a, 0x02, 0x02, 0x09, 0x07
};
static const __u8 tas5110_sensor_init[][8] = {
	{0x30, 0x11, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x10},
	{0x30, 0x11, 0x02, 0x20, 0xa9, 0x00, 0x00, 0x10},
	{0xa0, 0x61, 0x9a, 0xca, 0x00, 0x00, 0x00, 0x17},
};

static const __u8 initTas5130[] = {
	0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x11, 0x00, 0x00, 0x00,
	0x00, 0x00,
	0x00, 0x01, 0x00, 0x68, 0x0c, 0x0a,
	0x28, 0x1e, 0x60, COMP, MCK_INIT,
	0x18, 0x10, 0x04, 0x03, 0x11, 0x0c
};
static const __u8 tas5130_sensor_init[][8] = {
/* 	{0x30, 0x11, 0x00, 0x40, 0x47, 0x00, 0x00, 0x10},
					* shutter 0x47 short exposure? */
	{0x30, 0x11, 0x00, 0x40, 0x01, 0x00, 0x00, 0x10},
					/* shutter 0x01 long exposure */
	{0x30, 0x11, 0x02, 0x20, 0x70, 0x00, 0x00, 0x10},
};

static struct sensor_data sensor_data[] = {
SENS(initHv7131, NULL, hv7131_sensor_init, NULL, NULL, 0, NO_EXPO|NO_FREQ, 0),
SENS(initOv6650, NULL, ov6650_sensor_init, NULL, NULL, F_GAIN|F_SIF, 0, 0x60),
SENS(initOv7630, initOv7630_3, ov7630_sensor_init, NULL, ov7630_sensor_init_3,
	F_GAIN, 0, 0x21),
SENS(initPas106, NULL, pas106_sensor_init, NULL, NULL, F_SIF, NO_EXPO|NO_FREQ,
	0),
SENS(initPas202, initPas202, pas202_sensor_init, NULL, NULL, 0,
	NO_EXPO|NO_FREQ, 0),
SENS(initTas5110, NULL, tas5110_sensor_init, NULL, NULL, F_GAIN|F_SIF,
	NO_BRIGHTNESS|NO_FREQ, 0),
SENS(initTas5130, NULL, tas5130_sensor_init, NULL, NULL, 0, NO_EXPO|NO_FREQ,
	0),
};

/* get one byte in gspca_dev->usb_buf */
static void reg_r(struct gspca_dev *gspca_dev,
		  __u16 value)
{
	usb_control_msg(gspca_dev->dev,
			usb_rcvctrlpipe(gspca_dev->dev, 0),
			0,			/* request */
			USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE,
			value,
			0,			/* index */
			gspca_dev->usb_buf, 1,
			500);
}

static void reg_w(struct gspca_dev *gspca_dev,
		  __u16 value,
		  const __u8 *buffer,
		  int len)
{
#ifdef GSPCA_DEBUG
	if (len > USB_BUF_SZ) {
		PDEBUG(D_ERR|D_PACK, "reg_w: buffer overflow");
		return;
	}
#endif
	memcpy(gspca_dev->usb_buf, buffer, len);
	usb_control_msg(gspca_dev->dev,
			usb_sndctrlpipe(gspca_dev->dev, 0),
			0x08,			/* request */
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_INTERFACE,
			value,
			0,			/* index */
			gspca_dev->usb_buf, len,
			500);
}

static int i2c_w(struct gspca_dev *gspca_dev, const __u8 *buffer)
{
	int retry = 60;

	/* is i2c ready */
	reg_w(gspca_dev, 0x08, buffer, 8);
	while (retry--) {
		msleep(10);
		reg_r(gspca_dev, 0x08);
		if (gspca_dev->usb_buf[0] & 0x04) {
			if (gspca_dev->usb_buf[0] & 0x08)
				return -1;
			return 0;
		}
	}
	return -1;
}

static void i2c_w_vector(struct gspca_dev *gspca_dev,
			const __u8 buffer[][8], int len)
{
	for (;;) {
		reg_w(gspca_dev, 0x08, *buffer, 8);
		len -= 8;
		if (len <= 0)
			break;
		buffer++;
	}
}

static void setbrightness(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;
	__u8 value;

	switch (sd->sensor) {
	case  SENSOR_OV6650:
	case  SENSOR_OV7630: {
		__u8 i2cOV[] =
			{0xa0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x10};

		/* change reg 0x06 */
		i2cOV[1] = sensor_data[sd->sensor].sensor_addr;
		i2cOV[3] = sd->brightness;
		if (i2c_w(gspca_dev, i2cOV) < 0)
			goto err;
		break;
	    }
	case SENSOR_PAS106: {
		__u8 i2c1[] =
			{0xa1, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14};

		i2c1[3] = sd->brightness >> 3;
		i2c1[2] = 0x0e;
		if (i2c_w(gspca_dev, i2c1) < 0)
			goto err;
		i2c1[3] = 0x01;
		i2c1[2] = 0x13;
		if (i2c_w(gspca_dev, i2c1) < 0)
			goto err;
		break;
	    }
	case SENSOR_PAS202: {
		/* __u8 i2cpexpo1[] =
			{0xb0, 0x40, 0x04, 0x07, 0x2a, 0x00, 0x63, 0x16}; */
		__u8 i2cpexpo[] =
			{0xb0, 0x40, 0x0e, 0x01, 0xab, 0x00, 0x63, 0x16};
		__u8 i2cp202[] =
			{0xa0, 0x40, 0x10, 0x0e, 0x31, 0x00, 0x63, 0x15};
		static __u8 i2cpdoit[] =
			{0xa0, 0x40, 0x11, 0x01, 0x31, 0x00, 0x63, 0x16};

		/* change reg 0x10 */
		i2cpexpo[4] = 0xff - sd->brightness;
/*		if(i2c_w(gspca_dev,i2cpexpo1) < 0)
			goto err; */
/*		if(i2c_w(gspca_dev,i2cpdoit) < 0)
			goto err; */
		if (i2c_w(gspca_dev, i2cpexpo) < 0)
			goto err;
		if (i2c_w(gspca_dev, i2cpdoit) < 0)
			goto err;
		i2cp202[3] = sd->brightness >> 3;
		if (i2c_w(gspca_dev, i2cp202) < 0)
			goto err;
		if (i2c_w(gspca_dev, i2cpdoit) < 0)
			goto err;
		break;
	    }
	case SENSOR_TAS5130CXX: {
		__u8 i2c[] =
			{0x30, 0x11, 0x02, 0x20, 0x70, 0x00, 0x00, 0x10};

		value = 0xff - sd->brightness;
		i2c[4] = value;
		PDEBUG(D_CONF, "brightness %d : %d", value, i2c[4]);
		if (i2c_w(gspca_dev, i2c) < 0)
			goto err;
		break;
	    }
	}
	return;
err:
	PDEBUG(D_ERR, "i2c error brightness");
}

static void setsensorgain(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;
	unsigned char gain = sd->gain;

	switch (sd->sensor) {

	case SENSOR_TAS5110: {
		__u8 i2c[] =
			{0x30, 0x11, 0x02, 0x20, 0x70, 0x00, 0x00, 0x10};

		i2c[4] = 255 - gain;
		if (i2c_w(gspca_dev, i2c) < 0)
			goto err;
		break;
	    }

	case SENSOR_OV6650:
		gain >>= 1;
		/* fall thru */
	case SENSOR_OV7630: {
		__u8 i2c[] = {0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};

		i2c[1] = sensor_data[sd->sensor].sensor_addr;
		i2c[3] = gain >> 2;
		if (i2c_w(gspca_dev, i2c) < 0)
			goto err;
		break;
	    }
	}
	return;
err:
	PDEBUG(D_ERR, "i2c error gain");
}

static void setgain(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;
	__u8 gain;
	__u8 rgb_value;

	gain = sd->gain >> 4;

	/* red and blue gain */
	rgb_value = gain << 4 | gain;
	reg_w(gspca_dev, 0x10, &rgb_value, 1);
	/* green gain */
	rgb_value = gain;
	reg_w(gspca_dev, 0x11, &rgb_value, 1);

	if (sensor_data[sd->sensor].flags & F_GAIN)
		setsensorgain(gspca_dev);
}

static void setexposure(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	switch (sd->sensor) {
	case SENSOR_TAS5110: {
		__u8 reg;

		/* register 19's high nibble contains the sn9c10x clock divider
		   The high nibble configures the no fps according to the
		   formula: 60 / high_nibble. With a maximum of 30 fps */
		reg = 120 * sd->exposure / 1000;
		if (reg < 2)
			reg = 2;
		else if (reg > 15)
			reg = 15;
		reg = (reg << 4) | 0x0b;
		reg_w(gspca_dev, 0x19, &reg, 1);
		break;
	    }
	case SENSOR_OV6650:
	case SENSOR_OV7630: {
		/* The ov6650 / ov7630 have 2 registers which both influence
		   exposure, register 11, whose low nibble sets the nr off fps
		   according to: fps = 30 / (low_nibble + 1)

		   The fps configures the maximum exposure setting, but it is
		   possible to use less exposure then what the fps maximum
		   allows by setting register 10. register 10 configures the
		   actual exposure as quotient of the full exposure, with 0
		   being no exposure at all (not very usefull) and reg10_max
		   being max exposure possible at that framerate.

		   The code maps our 0 - 510 ms exposure ctrl to these 2
		   registers, trying to keep fps as high as possible.
		*/
		__u8 i2c[] = {0xb0, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10};
		int reg10, reg11, reg10_max;

		/* ov6645 datasheet says reg10_max is 9a, but that uses
		   tline * 2 * reg10 as formula for calculating texpo, the
		   ov6650 probably uses the same formula as the 7730 which uses
		   tline * 4 * reg10, which explains why the reg10max we've
		   found experimentally for the ov6650 is exactly half that of
		   the ov6645. The ov7630 datasheet says the max is 0x41. */
		if (sd->sensor == SENSOR_OV6650) {
			reg10_max = 0x4d;
			i2c[4] = 0xc0; /* OV6650 needs non default vsync pol */
		} else
			reg10_max = 0x41;

		reg11 = (60 * sd->exposure + 999) / 1000;
		if (reg11 < 1)
			reg11 = 1;
		else if (reg11 > 16)
			reg11 = 16;

		/* In 640x480, if the reg11 has less than 3, the image is
		   unstable (not enough bandwidth). */
		if (gspca_dev->width == 640 && reg11 < 3)
			reg11 = 3;

		/* frame exposure time in ms = 1000 * reg11 / 30    ->
		reg10 = sd->exposure * 2 * reg10_max / (1000 * reg11 / 30) */
		reg10 = (sd->exposure * 60 * reg10_max) / (1000 * reg11);

		/* Don't allow this to get below 10 when using autogain, the
		   steps become very large (relatively) when below 10 causing
		   the image to oscilate from much too dark, to much too bright
		   and back again. */
		if (sd->autogain && reg10 < 10)
			reg10 = 10;
		else if (reg10 > reg10_max)
			reg10 = reg10_max;

		/* Write reg 10 and reg11 low nibble */
		i2c[1] = sensor_data[sd->sensor].sensor_addr;
		i2c[3] = reg10;
		i2c[4] |= reg11 - 1;

		/* If register 11 didn't change, don't change it */
		if (sd->reg11 == reg11 )
			i2c[0] = 0xa0;

		if (i2c_w(gspca_dev, i2c) == 0)
			sd->reg11 = reg11;
		else
			PDEBUG(D_ERR, "i2c error exposure");
		break;
	    }
	}
}

static void setfreq(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;

	switch (sd->sensor) {
	case SENSOR_OV6650:
	case SENSOR_OV7630: {
		/* Framerate adjust register for artificial light 50 hz flicker
		   compensation, for the ov6650 this is identical to ov6630
		   0x2b register, see ov6630 datasheet.
		   0x4f / 0x8a -> (30 fps -> 25 fps), 0x00 -> no adjustment */
		__u8 i2c[] = {0xa0, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x10};
		switch (sd->freq) {
		default:
/*		case 0:			 * no filter*/
/*		case 2:			 * 60 hz */
			i2c[3] = 0;
			break;
		case 1:			/* 50 hz */
			i2c[3] = (sd->sensor == SENSOR_OV6650)
					? 0x4f : 0x8a;
			break;
		}
		i2c[1] = sensor_data[sd->sensor].sensor_addr;
		if (i2c_w(gspca_dev, i2c) < 0)
			PDEBUG(D_ERR, "i2c error setfreq");
		break;
	    }
	}
}

static void do_autogain(struct gspca_dev *gspca_dev)
{
	int deadzone, desired_avg_lum;
	struct sd *sd = (struct sd *) gspca_dev;
	int avg_lum = atomic_read(&sd->avg_lum);

	if (avg_lum == -1)
		return;

	/* SIF / VGA sensors have a different autoexposure area and thus
	   different avg_lum values for the same picture brightness */
	if (sensor_data[sd->sensor].flags & F_SIF) {
		deadzone = 1000;
		desired_avg_lum = 7000;
	} else {
		deadzone = 3000;
		desired_avg_lum = 23000;
	}

	if (sd->autogain_ignore_frames > 0)
		sd->autogain_ignore_frames--;
	else if (gspca_auto_gain_n_exposure(gspca_dev, avg_lum,
			sd->brightness * desired_avg_lum / 127,
			deadzone, GAIN_KNEE, EXPOSURE_KNEE)) {
		PDEBUG(D_FRAM, "autogain: gain changed: gain: %d expo: %d",
			(int)sd->gain, (int)sd->exposure);
		sd->autogain_ignore_frames = AUTOGAIN_IGNORE_FRAMES;
	}
}

/* this function is called at probe time */
static int sd_config(struct gspca_dev *gspca_dev,
			const struct usb_device_id *id)
{
	struct sd *sd = (struct sd *) gspca_dev;
	struct cam *cam;

	reg_r(gspca_dev, 0x00);
	if (gspca_dev->usb_buf[0] != 0x10)
		return -ENODEV;

	/* copy the webcam info from the device id */
	sd->sensor = id->driver_info >> 8;
	sd->bridge = id->driver_info & 0xff;
	gspca_dev->ctrl_dis = sensor_data[sd->sensor].ctrl_dis;

	cam = &gspca_dev->cam;
	if (!(sensor_data[sd->sensor].flags & F_SIF)) {
		cam->cam_mode = vga_mode;
		cam->nmodes = ARRAY_SIZE(vga_mode);
	} else {
		cam->cam_mode = sif_mode;
		cam->nmodes = ARRAY_SIZE(sif_mode);
	}
	cam->npkt = 36;			/* 36 packets per ISOC message */

	sd->brightness = BRIGHTNESS_DEF;
	sd->gain = GAIN_DEF;
	sd->exposure = EXPOSURE_DEF;
	if (gspca_dev->ctrl_dis & (1 << AUTOGAIN_IDX))
		sd->autogain = 0; /* Disable do_autogain callback */
	else
		sd->autogain = AUTOGAIN_DEF;
	sd->freq = FREQ_DEF;

	return 0;
}

/* this function is called at probe and resume time */
static int sd_init(struct gspca_dev *gspca_dev)
{
	const __u8 stop = 0x09; /* Disable stream turn of LED */

	reg_w(gspca_dev, 0x01, &stop, 1);

	return 0;
}

/* -- start the camera -- */
static int sd_start(struct gspca_dev *gspca_dev)
{
	struct sd *sd = (struct sd *) gspca_dev;
	struct cam *cam = &gspca_dev->cam;
	int mode, l;
	const __u8 *sn9c10x;
	__u8 reg12_19[8];

	mode = cam->cam_mode[gspca_dev->curr_mode].priv & 0x07;
	sn9c10x = sensor_data[sd->sensor].bridge_init[sd->bridge];
	l = sensor_data[sd->sensor].bridge_init_size[sd->bridge];
	memcpy(reg12_19, &sn9c10x[0x12 - 1], 8);
	reg12_19[6] = sn9c10x[0x18 - 1] | (mode << 4);
	/* Special cases where reg 17 and or 19 value depends on mode */
	switch (sd->sensor) {
	case SENSOR_PAS202:
		reg12_19[5] = mode ? 0x24 : 0x20;
		break;
	case SENSOR_TAS5130CXX:
		/* probably not mode specific at all most likely the upper
		   nibble of 0x19 is exposure (clock divider) just as with
		   the tas5110, we need someone to test this. */
		reg12_19[7] = mode ? 0x23 : 0x43;
		break;
	}
	/* Disable compression when the raw bayer format has been selected */
	if (cam->cam_mode[gspca_dev->curr_mode].priv & MODE_RAW)
		reg12_19[6] &= ~0x80;

	/* Vga mode emulation on SIF sensor? */
	if (cam->cam_mode[gspca_dev->curr_mode].priv & MODE_REDUCED_SIF) {
		reg12_19[0] += 16; /* 0x12: hstart adjust */
		reg12_19[1] += 24; /* 0x13: vstart adjust */
		reg12_19[3] = 320 / 16; /* 0x15: hsize */
		reg12_19[4] = 240 / 16; /* 0x16: vsize */
	}

	/* reg 0x01 bit 2 video transfert on */
	reg_w(gspca_dev, 0x01, &sn9c10x[0x01 - 1], 1);
	/* reg 0x17 SensorClk enable inv Clk 0x60 */
	reg_w(gspca_dev, 0x17, &sn9c10x[0x17 - 1], 1);
	/* Set the registers from the template */
	reg_w(gspca_dev, 0x01, sn9c10x, l);

	/* Init the sensor */
	i2c_w_vector(gspca_dev, sensor_data[sd->sensor].sensor_init,
			sensor_data[sd->sensor].sensor_init_size);
	if (sensor_data[sd->sensor].sensor_bridge_init[sd->bridge])
		i2c_w_vector(gspca_dev,
			sensor_data[sd->sensor].sensor_bridge_init[sd->bridge],
			sensor_data[sd->sensor].sensor_bridge_init_size[
				sd->bridge]);

	/* H_size V_size 0x28, 0x1e -> 640x480. 0x16, 0x12 -> 352x288 */
	reg_w(gspca_dev, 0x15, &reg12_19[3], 2);
	/* compression register */
	reg_w(gspca_dev, 0x18, &reg12_19[6], 1);
	/* H_start */
	reg_w(gspca_dev, 0x12, &reg12_19[0], 1);
	/* V_START */
	reg_w(gspca_dev, 0x13, &reg12_19[1], 1);
	/* reset 0x17 SensorClk enable inv Clk 0x60 */
				/*fixme: ov7630 [17]=68 8f (+20 if 102)*/
	reg_w(gspca_dev, 0x17, &reg12_19[5], 1);
	/*MCKSIZE ->3 */	/*fixme: not ov7630*/
	reg_w(gspca_dev, 0x19, &reg12_19[7], 1);
	/* AE_STRX AE_STRY AE_ENDX AE_ENDY */
	reg_w(gspca_dev, 0x1c, &sn9c10x[0x1c - 1], 4);
	/* Enable video transfert */
	reg_w(gspca_dev, 0x01, &sn9c10x[0], 1);
	/* Compression */
	reg_w(gspca_dev, 0x18, &reg12_19[6], 2);
	msleep(20);

	sd->reg11 = -1;

	setgain(gspca_dev);
	setbrightness(gspca_dev);
	setexposure(gspca_dev);
	setfreq(gspca_dev);

	sd->frames_to_drop = 0;
	sd->autogain_ignore_frames = 0;
	atomic_set(&sd->avg_lum, -1);
	return 0;
}

static void sd_stopN(struct gspca_dev *gspca_dev)
{
	sd_init(gspca_dev);
}

static void sd_pkt_scan(struct gspca_dev *gspca_dev,
			struct gspca_frame *frame,	/* target */
			unsigned char *data,		/* isoc packet */
			int len)			/* iso packet length */
{
	int i;
	struct sd *sd = (struct sd *) gspca_dev;
	struct cam *cam = &gspca_dev->cam;

	/* frames start with:
	 *	ff ff 00 c4 c4 96	synchro
	 *	00		(unknown)
	 *	xx		(frame sequence / size / compression)
	 *	(xx)		(idem - extra byte for sn9c103)
	 *	ll mm		brightness sum inside auto exposure
	 *	ll mm		brightness sum outside auto exposure
	 *	(xx xx xx xx xx)	audio values for snc103
	 */
	if (len > 6 && len < 24) {
		for (i = 0; i < len - 6; i++) {
			if (data[0 + i] == 0xff
			    && data[1 + i] == 0xff
			    && data[2 + i] == 0x00
			    && data[3 + i] == 0xc4
			    && data[4 + i] == 0xc4
			    && data[5 + i] == 0x96) {	/* start of frame */
				int lum = -1;
				int pkt_type = LAST_PACKET;
				int fr_h_sz = (sd->bridge == BRIDGE_103) ?
					18 : 12;

				if (len - i < fr_h_sz) {
					PDEBUG(D_STREAM, "packet too short to"
						" get avg brightness");
				} else if (sd->bridge == BRIDGE_103) {
					lum = data[i + 9] +
						(data[i + 10] << 8);
				} else {
					lum = data[i + 8] + (data[i + 9] << 8);
				}
				/* When exposure changes midway a frame we
				   get a lum of 0 in this case drop 2 frames
				   as the frames directly after an exposure
				   change have an unstable image. Sometimes lum
				   *really* is 0 (cam used in low light with
				   low exposure setting), so do not drop frames
				   if the previous lum was 0 too. */
				if (lum == 0 && sd->prev_avg_lum != 0) {
					lum = -1;
					sd->frames_to_drop = 2;
					sd->prev_avg_lum = 0;
				} else
					sd->prev_avg_lum = lum;
				atomic_set(&sd->avg_lum, lum);

				if (sd->frames_to_drop) {
					sd->frames_to_drop--;
					pkt_type = DISCARD_PACKET;
				}

				frame = gspca_frame_add(gspca_dev, pkt_type,
							frame, data, 0);
				data += i + fr_h_sz;
				len -= i + fr_h_sz;
				gspca_frame_add(gspca_dev, FIRST_PACKET,
						frame, data, len);
				return;
			}
		}
	}

	if (cam->cam_mode[gspca_dev->curr_mode].priv & MODE_RAW) {
		/* In raw mode we sometimes get some garbage after the frame
		   ignore this */
		int used = frame->data_end - frame->data;
		int size = cam->cam_mode[gspca_dev->curr_mode].sizeimage;

		if (used + len > size)
			len = size - used;
	}

	gspca_frame_add(gspca_dev, INTER_PACKET,
			frame, data, len);
}

static int sd_setbrightness(struct gspca_dev *gspca_dev, __s32 val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->brightness = val;
	if (gspca_dev->streaming)
		setbrightness(gspca_dev);
	return 0;
}

static int sd_getbrightness(struct gspca_dev *gspca_dev, __s32 *val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	*val = sd->brightness;
	return 0;
}

static int sd_setgain(struct gspca_dev *gspca_dev, __s32 val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->gain = val;
	if (gspca_dev->streaming)
		setgain(gspca_dev);
	return 0;
}

static int sd_getgain(struct gspca_dev *gspca_dev, __s32 *val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	*val = sd->gain;
	return 0;
}

static int sd_setexposure(struct gspca_dev *gspca_dev, __s32 val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->exposure = val;
	if (gspca_dev->streaming)
		setexposure(gspca_dev);
	return 0;
}

static int sd_getexposure(struct gspca_dev *gspca_dev, __s32 *val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	*val = sd->exposure;
	return 0;
}

static int sd_setautogain(struct gspca_dev *gspca_dev, __s32 val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->autogain = val;
	/* when switching to autogain set defaults to make sure
	   we are on a valid point of the autogain gain /
	   exposure knee graph, and give this change time to
	   take effect before doing autogain. */
	if (sd->autogain) {
		sd->exposure = EXPOSURE_DEF;
		sd->gain = GAIN_DEF;
		if (gspca_dev->streaming) {
			sd->autogain_ignore_frames = AUTOGAIN_IGNORE_FRAMES;
			setexposure(gspca_dev);
			setgain(gspca_dev);
		}
	}

	return 0;
}

static int sd_getautogain(struct gspca_dev *gspca_dev, __s32 *val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	*val = sd->autogain;
	return 0;
}

static int sd_setfreq(struct gspca_dev *gspca_dev, __s32 val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	sd->freq = val;
	if (gspca_dev->streaming)
		setfreq(gspca_dev);
	return 0;
}

static int sd_getfreq(struct gspca_dev *gspca_dev, __s32 *val)
{
	struct sd *sd = (struct sd *) gspca_dev;

	*val = sd->freq;
	return 0;
}

static int sd_querymenu(struct gspca_dev *gspca_dev,
			struct v4l2_querymenu *menu)
{
	switch (menu->id) {
	case V4L2_CID_POWER_LINE_FREQUENCY:
		switch (menu->index) {
		case 0:		/* V4L2_CID_POWER_LINE_FREQUENCY_DISABLED */
			strcpy((char *) menu->name, "NoFliker");
			return 0;
		case 1:		/* V4L2_CID_POWER_LINE_FREQUENCY_50HZ */
			strcpy((char *) menu->name, "50 Hz");
			return 0;
		case 2:		/* V4L2_CID_POWER_LINE_FREQUENCY_60HZ */
			strcpy((char *) menu->name, "60 Hz");
			return 0;
		}
		break;
	}
	return -EINVAL;
}

/* sub-driver description */
static const struct sd_desc sd_desc = {
	.name = MODULE_NAME,
	.ctrls = sd_ctrls,
	.nctrls = ARRAY_SIZE(sd_ctrls),
	.config = sd_config,
	.init = sd_init,
	.start = sd_start,
	.stopN = sd_stopN,
	.pkt_scan = sd_pkt_scan,
	.querymenu = sd_querymenu,
	.dq_callback = do_autogain,
};

/* -- module initialisation -- */
#define SB(sensor, bridge) \
	.driver_info = (SENSOR_ ## sensor << 8) | BRIDGE_ ## bridge


static __devinitdata struct usb_device_id device_table[] = {
	{USB_DEVICE(0x0c45, 0x6001), SB(TAS5110, 102)}, /* TAS5110C1B */
	{USB_DEVICE(0x0c45, 0x6005), SB(TAS5110, 101)}, /* TAS5110C1B */
#if !defined CONFIG_USB_SN9C102 && !defined CONFIG_USB_SN9C102_MODULE
	{USB_DEVICE(0x0c45, 0x6007), SB(TAS5110, 101)}, /* TAS5110D */
	{USB_DEVICE(0x0c45, 0x6009), SB(PAS106, 101)},
	{USB_DEVICE(0x0c45, 0x600d), SB(PAS106, 101)},
#endif
	{USB_DEVICE(0x0c45, 0x6011), SB(OV6650, 101)},
#if !defined CONFIG_USB_SN9C102 && !defined CONFIG_USB_SN9C102_MODULE
	{USB_DEVICE(0x0c45, 0x6019), SB(OV7630, 101)},
	{USB_DEVICE(0x0c45, 0x6024), SB(TAS5130CXX, 102)},
	{USB_DEVICE(0x0c45, 0x6025), SB(TAS5130CXX, 102)},
	{USB_DEVICE(0x0c45, 0x6028), SB(PAS202, 102)},
	{USB_DEVICE(0x0c45, 0x6029), SB(PAS106, 102)},
#endif
	{USB_DEVICE(0x0c45, 0x602c), SB(OV7630, 102)},
	{USB_DEVICE(0x0c45, 0x602d), SB(HV7131R, 102)},
#if !defined CONFIG_USB_SN9C102 && !defined CONFIG_USB_SN9C102_MODULE
	{USB_DEVICE(0x0c45, 0x602e), SB(OV7630, 102)},
#endif
	{USB_DEVICE(0x0c45, 0x608f), SB(OV7630, 103)},
#if !defined CONFIG_USB_SN9C102 && !defined CONFIG_USB_SN9C102_MODULE
	{USB_DEVICE(0x0c45, 0x60af), SB(PAS202, 103)},
#endif
	{USB_DEVICE(0x0c45, 0x60b0), SB(OV7630, 103)},
	{}
};
MODULE_DEVICE_TABLE(usb, device_table);

/* -- device connect -- */
static int sd_probe(struct usb_interface *intf,
			const struct usb_device_id *id)
{
	return gspca_dev_probe(intf, id, &sd_desc, sizeof(struct sd),
				THIS_MODULE);
}

static struct usb_driver sd_driver = {
	.name = MODULE_NAME,
	.id_table = device_table,
	.probe = sd_probe,
	.disconnect = gspca_disconnect,
#ifdef CONFIG_PM
	.suspend = gspca_suspend,
	.resume = gspca_resume,
#endif
};

/* -- module insert / remove -- */
static int __init sd_mod_init(void)
{
	int ret;
	ret = usb_register(&sd_driver);
	if (ret < 0)
		return ret;
	PDEBUG(D_PROBE, "registered");
	return 0;
}
static void __exit sd_mod_exit(void)
{
	usb_deregister(&sd_driver);
	PDEBUG(D_PROBE, "deregistered");
}

module_init(sd_mod_init);
module_exit(sd_mod_exit);

/*
 *  Driver for the Conexant CX25821 PCIe bridge
 *
 *  Copyright (C) 2009 Conexant Systems Inc.
 *  Authors  <shu.lin@conexant.com>, <hiep.huynh@conexant.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _MEDUSA_DEF_H_
#define _MEDUSA_DEF_H_

// Video deocder that we supported
#define VDEC_A		0
#define VDEC_B		1
#define VDEC_C		2
#define VDEC_D		3
#define VDEC_E		4
#define VDEC_F		5
#define VDEC_G		6
#define VDEC_H		7

//#define AUTO_SWITCH_BIT[]  = { 8, 9, 10, 11, 12, 13, 14, 15 };

// The following bit position enables automatic source switching for decoder A-H.
// Display index per camera.
//#define VDEC_INDEX[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};

// Select input bit to video decoder A-H.
//#define CH_SRC_SEL_BIT[] = {24, 25, 26, 27, 28, 29, 30, 31};

// end of display sequence
#define END_OF_SEQ					0xF;

// registry string size
#define MAX_REGISTRY_SZ					40;

#endif

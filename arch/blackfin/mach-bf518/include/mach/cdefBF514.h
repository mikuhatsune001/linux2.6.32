/*
 * Copyright 2008-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later
 */

#ifndef _CDEF_BF514_H
#define _CDEF_BF514_H

/* include all Core registers and bit definitions */
#include "defBF514.h"

/* include core specific register pointer definitions */
#include <asm/cdef_LPBlackfin.h>

/* SYSTEM & MMR ADDRESS DEFINITIONS FOR ADSP-BF514 */

/* include cdefBF51x_base.h for the set of #defines that are common to all ADSP-BF51x processors */
#include "cdefBF51x_base.h"

/* The following are the #defines needed by ADSP-BF514 that are not in the common header */

/* Removable Storage Interface Registers */

#define bfin_read_RSI_PWR_CTL()        bfin_read16(RSI_PWR_CONTROL)
#define bfin_write_RSI_PWR_CTL(val)    bfin_write16(RSI_PWR_CONTROL, val)
#define bfin_read_RSI_CLK_CTL()	       bfin_read16(RSI_CLK_CONTROL)
#define bfin_write_RSI_CLK_CTL(val)    bfin_write16(RSI_CLK_CONTROL, val)
#define bfin_read_RSI_ARGUMENT()       bfin_read32(RSI_ARGUMENT)
#define bfin_write_RSI_ARGUMENT(val)   bfin_write32(RSI_ARGUMENT, val)
#define bfin_read_RSI_COMMAND()        bfin_read16(RSI_COMMAND)
#define bfin_write_RSI_COMMAND(val)    bfin_write16(RSI_COMMAND, val)
#define bfin_read_RSI_RESP_CMD()       bfin_read16(RSI_RESP_CMD)
#define bfin_write_RSI_RESP_CMD(val)   bfin_write16(RSI_RESP_CMD, val)
#define bfin_read_RSI_RESPONSE0()      bfin_read32(RSI_RESPONSE0)
#define bfin_write_RSI_RESPONSE0(val)  bfin_write32(RSI_RESPONSE0, val)
#define bfin_read_RSI_RESPONSE1()      bfin_read32(RSI_RESPONSE1)
#define bfin_write_RSI_RESPONSE1(val)  bfin_write32(RSI_RESPONSE1, val)
#define bfin_read_RSI_RESPONSE2()      bfin_read32(RSI_RESPONSE2)
#define bfin_write_RSI_RESPONSE2(val)  bfin_write32(RSI_RESPONSE2, val)
#define bfin_read_RSI_RESPONSE3()      bfin_read32(RSI_RESPONSE3)
#define bfin_write_RSI_RESPONSE3(val)  bfin_write32(RSI_RESPONSE3, val)
#define bfin_read_RSI_DATA_TIMER()     bfin_read32(RSI_DATA_TIMER)
#define bfin_write_RSI_DATA_TIMER(val) bfin_write32(RSI_DATA_TIMER, val)
#define bfin_read_RSI_DATA_LGTH()      bfin_read16(RSI_DATA_LGTH)
#define bfin_write_RSI_DATA_LGTH(val)  bfin_write16(RSI_DATA_LGTH, val)
#define bfin_read_RSI_DATA_CTL()       bfin_read16(RSI_DATA_CONTROL)
#define bfin_write_RSI_DATA_CTL(val)   bfin_write16(RSI_DATA_CONTROL, val)
#define bfin_read_RSI_DATA_CNT()       bfin_read16(RSI_DATA_CNT)
#define bfin_write_RSI_DATA_CNT(val)   bfin_write16(RSI_DATA_CNT, val)
#define bfin_read_RSI_STATUS()         bfin_read32(RSI_STATUS)
#define bfin_write_RSI_STATUS(val)     bfin_write32(RSI_STATUS, val)
#define bfin_read_RSI_STATUS_CLR()     bfin_read16(RSI_STATUSCL)
#define bfin_write_RSI_STATUS_CLR(val) bfin_write16(RSI_STATUSCL, val)
#define bfin_read_RSI_MASK0()          bfin_read32(RSI_MASK0)
#define bfin_write_RSI_MASK0(val)      bfin_write32(RSI_MASK0, val)
#define bfin_read_RSI_MASK1()          bfin_read32(RSI_MASK1)
#define bfin_write_RSI_MASK1(val)      bfin_write32(RSI_MASK1, val)
#define bfin_read_RSI_FIFO_CNT()       bfin_read16(RSI_FIFO_CNT)
#define bfin_write_RSI_FIFO_CNT(val)   bfin_write16(RSI_FIFO_CNT, val)
#define bfin_read_RSI_CEATA_CTL()      bfin_read16(RSI_CEATA_CONTROL)
#define bfin_write_RSI_CEATA_CTL(val)  bfin_write16(RSI_CEATA_CONTROL, val)
#define bfin_read_RSI_FIFO()           bfin_read32(RSI_FIFO)
#define bfin_write_RSI_FIFO(val)       bfin_write32(RSI_FIFO, val)
#define bfin_read_RSI_E_STATUS()       bfin_read16(RSI_ESTAT)
#define bfin_write_RSI_E_STATUS(val)   bfin_write16(RSI_ESTAT, val)
#define bfin_read_RSI_E_MASK()         bfin_read16(RSI_EMASK)
#define bfin_write_RSI_E_MASK(val)     bfin_write16(RSI_EMASK, val)
#define bfin_read_RSI_CFG()            bfin_read16(RSI_CONFIG)
#define bfin_write_RSI_CFG(val)        bfin_write16(RSI_CONFIG, val)
#define bfin_read_RSI_RD_WAIT_EN()     bfin_read16(RSI_RD_WAIT_EN)
#define bfin_write_RSI_RD_WAIT_EN(val) bfin_write16(RSI_RD_WAIT_EN, val)
#define bfin_read_RSI_PID0()           bfin_read16(RSI_PID0)
#define bfin_write_RSI_PID0(val)       bfin_write16(RSI_PID0, val)
#define bfin_read_RSI_PID1()           bfin_read16(RSI_PID1)
#define bfin_write_RSI_PID1(val)       bfin_write16(RSI_PID1, val)
#define bfin_read_RSI_PID2()           bfin_read16(RSI_PID2)
#define bfin_write_RSI_PID2(val)       bfin_write16(RSI_PID2, val)
#define bfin_read_RSI_PID3()           bfin_read16(RSI_PID3)
#define bfin_write_RSI_PID3(val)       bfin_write16(RSI_PID3, val)
#define bfin_read_RSI_PID4()           bfin_read16(RSI_PID4)
#define bfin_write_RSI_PID4(val)       bfin_write16(RSI_PID4, val)
#define bfin_read_RSI_PID5()           bfin_read16(RSI_PID5)
#define bfin_write_RSI_PID5(val)       bfin_write16(RSI_PID5, val)
#define bfin_read_RSI_PID6()           bfin_read16(RSI_PID6)
#define bfin_write_RSI_PID6(val)       bfin_write16(RSI_PID6, val)
#define bfin_read_RSI_PID7()           bfin_read16(RSI_PID7)
#define bfin_write_RSI_PID7(val)       bfin_write16(RSI_PID7, val)

#endif /* _CDEF_BF514_H */

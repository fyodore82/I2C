/* 
 * File:   i2cCmds.h
 * Author: fedor
 *
 * Created on 21 ???? 2014 ?., 12:31
 */

#ifndef I2CCMDS_H
#define	I2CCMDS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define _I2C_NOCMD  0x0
#define _I2C_DAY    0x1
#define _I2C_HOUR   0x2
#define _I2C_MIN    0x3
#define _I2C_SEC    0x4
#define _I2C_ADDRH  0x5
#define _I2C_ADDRL  0x6
#define _I2C_UBATT  0x7
#define _I2C_TENG   0x8
#define _I2C_PINSTATE   0x9
#define _I2C_BINST      0x1A
#define _I2C_ENGST      0x1B
#define _I2C_INTSVCSYNC 0x1C
#define _I2C_NEWBINTMR  0x1D
#define _I2C_NEWASRTMR  0x1E
#define _I2C_ASRONTMR   0x1F
#define _I2C_ENGSTARTTMR    0x20
#define _I2C_UBATTTMR   0x21
#define _I2C_TEB        0x22
#define _I2C_DI21       0x23
#define _I2C_DI23       0x24
#define _I2C_DI24       0x25
#define _I2C_DI25       0x26
#define _I2C_DO26       0x27
#define _I2C_DO27       0x28
#define _I2C_AI28       0x29
#define _I2C_AI29       0x2A
#define _I2C_DI210      0x2B
#define _I2C_DI1416     0x2C
#define _I2C_DO1819     0x2D
#define _I2C_UTRHLD     0x2E
#define _I2C_TTRHLD     0x2F
#define _I2C_CMDREBOOT  0x31
#define _I2C_EEPROMRW   0x32
#define _I2C_HOLDTMR    0x33    // Hold all timers for some time



#ifdef	__cplusplus
}
#endif

#endif	/* I2CCMDS_H */


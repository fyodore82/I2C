/* 
 * File:   i2c_my.h
 * Author: fedor
 *
 * Created on 9 ??????? 2013 ?., 19:39
 */

#ifndef I2C_MY_H
#define	I2C_MY_H

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(__18F4550)
    #define SCL TRISBbits.TRISB1
    #define SDA TRISBbits.TRISB0
    #define SCL_IN PORTBbits.RB1
    #define SDA_IN PORTBbits.RB0
#elif defined(__16F819)
    #define SCL TRISBbits.TRISB4
    #define SDA TRISBbits.TRISB1
    #define SCL_IN PORTBbits.RB4
    #define SDA_IN PORTBbits.RB1
    #define _XTAL_FREQ 20000000
#elif defined(__16F690)
    #define SCL TRISBbits.TRISB6
    #define SDA TRISBbits.TRISB4
    #define SCL_IN PORTBbits.RB6
    #define SDA_IN PORTBbits.RB4
    #define _XTAL_FREQ 4000000
#endif

/*
#define I2CND    0   // 0 - not detected
#define I2CBUSY  1   // 1 - Busy
#define I2CBUSYME 2  // Set busy by me
#define I2CFREE  3   // 2 - Free
*/
#define I2C_7_STARTSETUPUS 6       // 90 in P16F690 spec, (7) Start condition setup time (when SDA and SCL are high), 4700 ns min
#define I2C_6_STARTHOLDUS  5       // 91, (6) Start condition hold time (when SDA = 0, SCL = 1), 4000 ns min

#define I2C_8_DATAHOLDUS 1         // 106, (8) Data input hold time (SCL = 0, SDA - not set), 0 ns min
#define I2C_9_DATASETUPUS  1       // 107, (9) Data input setup time (SDA = <data>, SCL = 0), 250 ns min

#define I2C_2_CLOCKHIGHUS  5       // 100, (2) Clock high for SCL for 100 kHz transfer rate, 4000 us min
#define I2C_3_CLOCKLOWUS   6       // 101, (3) Clock low time (SCL = 0), 4700 us min

#define I2C_10_STOPSETUPUS  5       // 92, (10) Stop condition setup time (SDA = 0, SCL = 1), 4000 ns

#define I2C_BUSFREETIMEUS   5       // 110, Bus free time (SCL = SDA = 1), 4700 ns min

#define I2CSTOPTRYTIMES 3

#define I2CCLOCKSTRTMS  200       // Clock stretch times we wait

#define I2CERRSTART    1    // I2C error on Start
#define I2CERRSTOP     2
#define I2CERRSTARTBYTE 3
#define I2CERRADDRH     4
#define I2CERRADDRL     5
#define I2CERRADDR      0x10
#define I2CERRRX        6
#define I2CERRTX        7
#define I2CERRCLCKSTR     0xA
#define I2CERRTXCOLLIGION   0xB
#define I2CERRRXSTART   8
#define I2CERRTXWAITACK 9
#define I2CERRNOACK     0xC     // No ACK bit has been received
    
#define I2CERRACKRECEIVED 0xD   // Not an error. It states, that ACK has been received

#define I2CERRSIZEINCORRECT 0x1A

#if defined(__18F4550)
    #define _XTAL_FREQ  48000000
#endif

void i2c_init (unsigned char slaveaddr);
unsigned char i2c_start (char);
unsigned char i2c_stop (char Force);
unsigned char i2c_tx (unsigned char dt, unsigned char);
unsigned char i2c_rx (unsigned char ack, unsigned char *d, unsigned char ClkStrRetryTimes);
unsigned char TXbyte (unsigned char addrh, unsigned char addrl, unsigned char data);
unsigned char TXWaitACK ( unsigned char cyclestowait);
unsigned char RXbyte (unsigned char addrH, unsigned char addrL, unsigned char *d);
unsigned char RX16bytes (unsigned char addrH, unsigned char addrL, unsigned char *d);    // d is 16 bytes array
unsigned char RXNbytes (unsigned char addrH, unsigned char addrL, unsigned char N, unsigned char *d);
unsigned char TXNbytes (unsigned char addrh, unsigned char addrl, unsigned char N, unsigned char *d);     // TX N bytes, d is N bytes array

unsigned char TXCmdToPIC16 (unsigned char* cmd, unsigned char size, unsigned char WaitBtwnCmdms);
unsigned char RXfromPIC16 (unsigned char *cmd, unsigned char size, unsigned char WaitBtwnCmdms);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_MY_H */


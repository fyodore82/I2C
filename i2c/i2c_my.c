#include "i2c_my.h"

#include <htc.h>

// Stack = 0
void i2c_init (unsigned char slaveaddr)
{
    // I2C Configuration

#if defined(__18F4550)
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB0 = 1;
    SSPBUF = 0x0;
    SSPCON1 = 0b00001011;    // Master mode
    SSPCON2 = 0x0;


//    SSPSTAT = 0b10000000;      // SSPSTAT<7> = 1: Slew rate control disabled for Standard Speed mode (100 kHz and 1 MHz)
//    PIE1bits.SSPIE = 0b0;       // Disable SSP interrupt

//    SSPADD = 0x77;      // In Master clock will be I2C clock = Fosc / (4*(SSPADD + 1)) = 100 kHz (Fosc = 48 Mhz)

    SSPCON1bits.SSPEN = 0b1;     // Enable SSP module
#elif defined(__16F819)
    // RB4 = 1 (SCL)
    // RB1 = 1 (SDA)
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB1 = 1;

    SSPSTAT = 0x0;      // Clear for I2C = 0
    SSPADD = slaveaddr;
/*    SSPCON = 0b00001001;    // Load SSPMASK
    
*/

    SSPCONbits.SSPM = 0b1011;   // Master
    SSPCONbits.CKP = 0x1;         // Enable I2C Clock
    SSPCONbits.SSPEN = 0x1;     // Enable SSP module

#elif defined(__16F690)
    // V 11 - RB6 = 1 (SCL)
    // V 13 - RB4 = 1 (SDA)
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB6 = 1;

    SSPSTAT = 0x0;      // Clear for I2C = 0

    SSPCON = 0b00001001;    // Load SSPMASK
    SSPADD = slaveaddr;
    SSPMSK = slaveaddr;


    SSPCONbits.SSPM = 0b1110;        
    SSPCONbits.CKP = 0x1;         // Enable I2C Clock
    SSPMSK = 0xFF;
    SSPCONbits.SSPEN = 0x1;     // Enable SSP module
#endif
}

// Stack = 1
unsigned char i2c_start (char force)   //  Start I2C. Return - 0 started, 1 - failed to start:
                        //    - I2C bus busy (multi master)
{
    if (force || SSPSTATbits.P || !(SSPSTATbits.P || SSPSTATbits.S))  // Ether STOP detected or both START & STOP = 0
    {
//        SSPCON2bits.SEN = 1;    // Generate Start
        SDA = 1;
        SCL = 1;
        __delay_us (I2C_7_STARTSETUPUS);
        SDA = 0b0;
        SDA_IN = 0;   // This MAGIC MUST BE HERE
        __delay_us (I2C_6_STARTHOLDUS);
        SCL = 0b0;
        SCL_IN = 0;   // This MAGIC MUST BE HERE
        PIR1bits.SSPIF = 0;
        return (0);
    }
    return(1);
}

// Stack = 1
unsigned char i2c_stop (char Force)    // STOPS i2c transmission 0 - success
{
    char sttries = I2CSTOPTRYTIMES;
    PIR1bits.SSPIF = 0;

    SDA = 0;
    __delay_us (I2C_9_DATASETUPUS); // This time is not defined
    SCL = 1;

    while (SCL_IN == 0 && sttries > 0)    // Is SCL high? (No slave holds SCL down for write to com1plete)
    {
        sttries--;
        __delay_us(I2C_10_STOPSETUPUS);
    }
    __delay_us(I2C_10_STOPSETUPUS);

    if (SCL_IN)
    {
        SDA = 1;
        PIR1bits.SSPIF = 0;
        return (0);
    }
    if (Force)        // Slave holds SCL low, but send stop as we Forced
    {
        SDA = 1;
        PIR1bits.SSPIF = 0;
        return (1);
    }
    PIR1bits.SSPIF = 0;
    return (1);     // No success. Someone holds SCL low and we are not forced to send stop anyway

}

// Stack = 1
unsigned char i2c_tx (unsigned char dt, unsigned char CareAboutAck)
{
//    SSPBUF = dt;
//    return 0;
    unsigned char i;
    unsigned char ClckStretchus;
    PIR1bits.SSPIF = 0;

    SCL = 0;
    __delay_us (I2C_8_DATAHOLDUS);

    for (i = 8; i; i--)
    {
        if (dt&0x80)
            SDA = 1;
        else
            SDA = 0;

        __delay_us (I2C_9_DATASETUPUS);

        SCL = 1;
        __delay_us (I2C_2_CLOCKHIGHUS);

        for (ClckStretchus = I2CCLOCKSTRTMS; !SCL_IN && ClckStretchus > 0; ClckStretchus--)
        {
            SCL = 1;
            __delay_us (I2C_2_CLOCKHIGHUS);
        }
        if (!SCL_IN)     // Clock still stretched, return error
        {
            SCL = 0;
            return (I2CERRCLCKSTR);
        }

        if ((dt&0x80) && !SDA_IN)      // Collision
        {
            SCL = 0;
            PIR1bits.SSPIF = 0;
            return (I2CERRTXCOLLIGION);
        }

        SCL = 0;
        dt <<= 1;
        __delay_us (I2C_8_DATAHOLDUS);
    }
    SDA = 1;
    __delay_us (I2C_9_DATASETUPUS);
    SCL = 1;
    __delay_us (I2C_2_CLOCKHIGHUS);
    i = (char)SDA_IN;     // Possible ACK bit
    SCL = 0;
    PIR1bits.SSPIF = 0;
    if (CareAboutAck)
    {
        return (i == 0 ? I2CERRACKRECEIVED : I2CERRNOACK);  // ACK is when SDA is low
    }
    else
    {
        return 0;
    }

}

// Stack = 1
unsigned char i2c_rx (unsigned char ack, unsigned char *d, unsigned char ClkStrRetryTimes)
{
    unsigned char i, j;
    *d = 0;
    SCL = 0;
    SDA = 1;
    for(i = 0; i < 8; i++)
    {
        __delay_us (I2C_3_CLOCKLOWUS);
        *d <<= 1;
        j = ClkStrRetryTimes;
        do {
            SCL = 1;
            __delay_us (I2C_2_CLOCKHIGHUS);
            j--;
        }
        while(SCL_IN == 0 && j != 0);    // wait for any SCL clock stretching
                                            // no more than j - 1 times
        if (j == 0)
        {
            PIR1bits.SSPIF = 0;
            return (I2CERRCLCKSTR);
        }

        if (SDA_IN)
            *d |= 1;
        SCL = 0;
   }
   __delay_us (I2C_3_CLOCKLOWUS);
   __delay_us (I2C_8_DATAHOLDUS);
   if (ack)
       SDA = 0;
   else
       SDA = 1;
   __delay_us (I2C_9_DATASETUPUS);
   SCL = 1;
   __delay_us(I2C_2_CLOCKHIGHUS);             // send (N)ACK bit
   SCL = 0;
   __delay_us (I2C_8_DATAHOLDUS);
   SDA = 1;
   PIR1bits.SSPIF = 0;
   return 0;
}

// Stack = 2
unsigned char TXbyte (unsigned char addrh, unsigned char addrl, unsigned char data)     // Transmit 1 byte of data
{
    unsigned char ret = 0;
    unsigned char i = I2CSTOPTRYTIMES;

    if (i2c_start (0))      // Cannot start
    {
        return (I2CERRSTART);
    }
    if (i2c_tx(0b10100000, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }
    if (i2c_tx(addrh, 0))
    {
        ret = I2CERRADDRH;
        goto end;
    }
    if (i2c_tx(addrl, 0))
    {
        ret = I2CERRADDRL;
        goto end;
    }
    if (i2c_tx(data, 0))
    {
        ret = I2CERRTX;
        goto end;
    }
    end:
    while (i2c_stop(0) && --i != 0)
        __delay_us (I2C_10_STOPSETUPUS);
    if (i == 0)
    {
        i2c_stop(1);
        ret = I2CERRSTOP;
    }
    return (ret);
}

// Stack = 2
unsigned char TXWaitACK (unsigned char cyclestowait)
{
    unsigned char ret = 0;
    unsigned char i = I2CSTOPTRYTIMES;

    do
    {
        if (i2c_start(1))
        {
            return (I2CERRSTART);
        }
        ret = i2c_tx(0b10100000, 1);    // Care about ack = 1. Ret will be == 0 in case of ACK received
        __delay_us (100);
    }
    while (cyclestowait-- > 0 && ret != I2CERRACKRECEIVED);

    while (i2c_stop(0) && --i > 0)
        __delay_us (I2C_10_STOPSETUPUS);
    if (i == 0)
    {
        i2c_stop(1);
        //if (ret == 0)      
            return (I2CERRSTOP);  //  if ret is zero (we got ACK from tx), then set ret to I2CERRSTOP
    }
    return ret != I2CERRACKRECEIVED ? ret : 0;  // If we finally got ACK, return 0 to indicate success

}

// Stack = 2
unsigned char RXbyte (unsigned char addrH, unsigned char addrL, unsigned char *d)
{
    unsigned char ret = 0;
    unsigned char i = I2CSTOPTRYTIMES;

    if (i2c_start(0))
    {
        return (I2CERRSTART);
    }
    if (i2c_tx(0b10100000, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }
    if (i2c_tx(addrH, 0))
    {
        ret = I2CERRADDRH;
        goto end;
    }
    if (i2c_tx(addrL, 0))
    {
        ret = I2CERRADDRL;
        goto end;
    }
    if (i2c_start(1))
    {
        ret = I2CERRRXSTART;
        goto end;
    }
    if (i2c_tx(0b10100001, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }
    if (i2c_rx(0, d, I2CCLOCKSTRTMS))
    {
        ret = I2CERRRX;
        goto end;
    }
    end:
    while (i2c_stop(0) && --i != 0)
        __delay_us (I2C_10_STOPSETUPUS);
    if (i == 0)
    {
        i2c_stop(1);
        ret = I2CERRSTOP;
    }
    return ret;
}

// Stack = 2
unsigned char RX16bytes (unsigned char addrH, unsigned char addrL, unsigned char *d)    // d is 16 bytes array
{
    unsigned char ret = 0;
    unsigned char i;

    if (i2c_start(0))
    {
        return (I2CERRSTART);
    }
    if (i2c_tx(0b10100000, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }
    if (i2c_tx(addrH, 0))
    {
        ret = I2CERRADDRH;
        goto end;
    }
    if (i2c_tx(addrL, 0))
    {
        ret = I2CERRADDRL;
        goto end;
    }
    if (i2c_start(1))
    {
        ret = I2CERRRXSTART;
        goto end;
    }
    if (i2c_tx(0b10100001, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }

    for (i = 0; i < 15; i++)
    {
        if (i2c_rx(1, &d[i], I2CCLOCKSTRTMS))
        {
            ret = (i<<4) & I2CERRRX;
            goto end;
        }
    }

    if (i2c_rx(0, &d[15], I2CCLOCKSTRTMS))
    {
        ret = 0xF0 & I2CERRRX;
        goto end;
    }

    end:
    i = I2CSTOPTRYTIMES;
    while (i2c_stop(0) && --i != 0)
        __delay_us (I2C_10_STOPSETUPUS);
    if (i == 0)
    {
        i2c_stop(1);
        if (ret == 0)
            ret = I2CERRSTOP;
    }
    return ret;
}

unsigned char RXNbytes (unsigned char addrH, unsigned char addrL, unsigned char N, unsigned char *d)    // d is N bytes array
{
    unsigned char ret = 0;
    unsigned char i;

    if (i2c_start(0))
    {
        return (I2CERRSTART);
    }
    if (i2c_tx(0b10100000, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }
    if (i2c_tx(addrH, 0))
    {
        ret = I2CERRADDRH;
        goto end;
    }
    if (i2c_tx(addrL, 0))
    {
        ret = I2CERRADDRL;
        goto end;
    }
    if (i2c_start(1))
    {
        ret = I2CERRRXSTART;
        goto end;
    }
    if (i2c_tx(0b10100001, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }

    for (i = 0; i <= N - 2; i++)
    {
        if (i2c_rx(1, &d[i], I2CCLOCKSTRTMS))
        {
            ret = (i<<4) & I2CERRRX;
            goto end;
        }
    }

    if (i2c_rx(0, &d[N - 1], I2CCLOCKSTRTMS))
    {
        ret = 0xF0 & I2CERRRX;
        goto end;
    }

    end:
    i = I2CSTOPTRYTIMES;
    while (i2c_stop(0) && --i > 0)
        __delay_us (I2C_10_STOPSETUPUS);
    if (i == 0)
    {
        i2c_stop(1);
        if (ret == 0)
            ret = I2CERRSTOP;
    }
    return ret;
}


// Stack = 2
unsigned char TXNbytes (unsigned char addrh, unsigned char addrl, unsigned char N, unsigned char *d)     // TX N bytes, d is N bytes array
{
    unsigned char ret = 0;
    unsigned char i, j;

    startTXNBytes:;
        
    if (i2c_start (0))      // Cannot start
    {
        return (I2CERRSTART);
    }
    if (i2c_tx(0b10100000, 0))
    {
        ret = I2CERRSTARTBYTE;
        goto end;
    }
    if (i2c_tx(addrh, 0))
    {
        ret = I2CERRADDRH;
        goto end;
    }
    if (i2c_tx(addrl, 0))
    {
        ret = I2CERRADDRL;
        goto end;
    }

    j = I2CSTOPTRYTIMES;
    // Checking here if we approach PAGE end (0x7F OR 0xFF). If so, we stop writing and restart this routine from new page
    for (i = 0; i < N && ((addrl - 1 + i != 0x7F && addrl - 1 + i != 0xFF) || addrl == 0 || addrl == 0x80); i++)
    {
        if (i2c_tx(d[i], 0))
        {
            ret = (i<<4) | I2CERRTX;
            goto end;
        }
    }

    end:

    while (i2c_stop(0) && --j > 0)
        __delay_us (I2C_10_STOPSETUPUS);

    if (j == 0)
    {
        i2c_stop(1);
        ret = I2CERRSTOP;
    }


    if (!ret && i != N)
    {
        if ((ret = TXWaitACK (20)))
            return ((ret << 4) | I2CERRTXWAITACK);

        d += i;
        N -= i;
        if (addrl + i == 0x80)
            addrl = 0x80;
            //ret = TXNbytes (addrh, 0x80, N - i, &d[i]);
        else
        {
            addrh += 1;
            addrl = 0x0;
        }
            //ret = TXNbytes (addrh + 1, 0x0, N - i, &d[i]);
        goto startTXNBytes;


        /*if (ret)
            ret = (ret << 4) | I2CERRTX | 0x08;*/
    }

    return (ret);
}


unsigned char TXCmdToPIC16 (unsigned char* cmd, unsigned char size, unsigned char WaitBtwnCmdms)
{
    unsigned char x = 0;
    unsigned char wt = WaitBtwnCmdms;
    unsigned char ret;
    if (i2c_start(0))
        return (I2CERRSTART);

    for (; wt != 0; wt--)
        __delay_ms(1);

    while (size--)
    {
        
        if ((ret = i2c_tx(cmd[x++], 0)))      // If We Don't get ACK, it's error
        {
            i2c_stop(1);
            return ((x << 4) | ret);
        }
        
        wt = WaitBtwnCmdms;
        for (; wt != 0; wt--)
            __delay_us(I2C_BUSFREETIMEUS);
    }
    i2c_stop(1);
    return (0);


}

unsigned char RXfromPIC16 (unsigned char *cmd, unsigned char size, unsigned char WaitBtwnCmdms)
{
    unsigned char d;
    unsigned char wt;
    unsigned char ret;

    if (size != 2)
        return (I2CERRSIZEINCORRECT);

    if (i2c_start(0))
        return (I2CERRSTART);

    wt = WaitBtwnCmdms;
    for (; wt != 0; wt--)
        __delay_us(I2C_BUSFREETIMEUS);

    if ((ret = i2c_tx (cmd[0], 0)))
    {
        i2c_stop(1);
        return (0x10 | I2CERRNOACK);
    }

    wt = WaitBtwnCmdms;
    for (; wt != 0; wt--)
        __delay_us(I2C_BUSFREETIMEUS);
    
    if ((ret = i2c_rx (0, &cmd[1], WaitBtwnCmdms)))
    {
        i2c_stop(1);
        return (0x20 | ret);
    }

    wt = WaitBtwnCmdms;
    for (; wt != 0; wt--)
        __delay_us(I2C_BUSFREETIMEUS);

    i2c_stop(1);
    return 0;
}

/* 
 * File:   PrgData.h
 * Author: fedor
 *
 * Created on 7 ???????? 2015 ?., 13:41
 */

#ifndef PRGDATA_H
#define	PRGDATA_H

#ifdef	__cplusplus
extern "C" {
#endif

/*    PrgData[0] = 0x83;
    PrgData[1] = 0x01;
    PrgData[2] = 0x43;
    PrgData[3] = 0x2F;
    PrgData[4] = 0xF5;
    PrgData[5] = 0x00;
    PrgData[6] = 0x00;
    PrgData[7] = 0x30;
    PrgData[8] = 0x8B;
    PrgData[9] = 0x1B;
    PrgData[10] = 0x01;
    PrgData[11] = 0x30;
    PrgData[12] = 0xF6;
    PrgData[13] = 0x00;
    PrgData[14] = 0x8B;
    PrgData[15] = 0x13;
    PrgData[16] = 0x75;
    PrgData[17] = 0x08;
    PrgData[18] = 0x83;
    PrgData[19] = 0x12;
    PrgData[20] = 0x03;
    PrgData[21] = 0x17;
    PrgData[22] = 0x8D;
    PrgData[23] = 0x00;
    PrgData[24] = 0x83;
    PrgData[25] = 0x16;
    PrgData[26] = 0x03;
    PrgData[27] = 0x17;
    PrgData[28] = 0x8C;
    PrgData[29] = 0x13;
    PrgData[30] = 0x71;
    PrgData[31] = 0x08;
    PrgData[32] = 0x03;
    PrgData[33] = 0x1D;
    PrgData[34] = 0x13;
    PrgData[35] = 0x2F;
    PrgData[36] = 0x1D;
    PrgData[37] = 0x2F;
    PrgData[38] = 0x0C;
    PrgData[39] = 0x14;
    PrgData[40] = 0x83;
    PrgData[41] = 0x12;
    PrgData[42] = 0x03;
    PrgData[43] = 0x17;
    PrgData[44] = 0x0C;
    PrgData[45] = 0x08;
    PrgData[46] = 0xF2;
    PrgData[47] = 0x00;
    PrgData[48] = 0x70;
    PrgData[49] = 0x08;
    PrgData[50] = 0x84;
    PrgData[51] = 0x00;
    PrgData[52] = 0x72;
    PrgData[53] = 0x08;
    PrgData[54] = 0x80;
    PrgData[55] = 0x00;
    PrgData[56] = 0x3D;
    PrgData[57] = 0x2F;
    PrgData[58] = 0x70;
    PrgData[59] = 0x08;
    PrgData[60] = 0x84;
    PrgData[61] = 0x00;
    PrgData[62] = 0x00;
    PrgData[63] = 0x08;
    EraseDataMem (0x0E / 2, 0x00);
    WriteDataMem (0x0E / 2, 0x00);

    PrgData[0] = 0x83;
    PrgData[1] = 0x12;
    PrgData[2] = 0x8C;
    PrgData[3] = 0x00;
    PrgData[4] = 0x83;
    PrgData[5] = 0x16;
    PrgData[6] = 0x03;
    PrgData[7] = 0x17;
    PrgData[8] = 0x0C;
    PrgData[9] = 0x15;
    PrgData[10] = 0x55;
    PrgData[11] = 0x30;
    PrgData[12] = 0x8D;
    PrgData[13] = 0x00;
    PrgData[14] = 0xAA;
    PrgData[15] = 0x30;
    PrgData[16] = 0x8D;
    PrgData[17] = 0x00;
    PrgData[18] = 0x8C;
    PrgData[19] = 0x14;
    PrgData[20] = 0x2B;
    PrgData[21] = 0x2F;
    PrgData[22] = 0x8C;
    PrgData[23] = 0x18;
    PrgData[24] = 0x2E;
    PrgData[25] = 0x2F;
    PrgData[26] = 0x2F;
    PrgData[27] = 0x2F;
    PrgData[28] = 0x2B;
    PrgData[29] = 0x2F;
    PrgData[30] = 0x30;
    PrgData[31] = 0x2F;
    PrgData[32] = 0x03;
    PrgData[33] = 0x30;
    PrgData[34] = 0xF4;
    PrgData[35] = 0x00;
    PrgData[36] = 0x8A;
    PrgData[37] = 0x30;
    PrgData[38] = 0xF3;
    PrgData[39] = 0x00;
    PrgData[40] = 0x56;
    PrgData[41] = 0x30;
    PrgData[42] = 0xF2;
    PrgData[43] = 0x00;
    PrgData[44] = 0xF2;
    PrgData[45] = 0x0B;
    PrgData[46] = 0x36;
    PrgData[47] = 0x2F;
    PrgData[48] = 0xF3;
    PrgData[49] = 0x0B;
    PrgData[50] = 0x36;
    PrgData[51] = 0x2F;
    PrgData[52] = 0xF4;
    PrgData[53] = 0x0B;
    PrgData[54] = 0x36;
    PrgData[55] = 0x2F;
    PrgData[56] = 0x3D;
    PrgData[57] = 0x2F;
    PrgData[58] = 0x76;
    PrgData[59] = 0x08;
    PrgData[60] = 0x03;
    PrgData[61] = 0x1D;
    PrgData[62] = 0x8B;
    PrgData[63] = 0x17;

    EraseDataMem (0x0E / 2, 0x40/2);
    WriteDataMem (0x0E / 2, 0x40/2);


    PrgData[0] = 0x03;
    PrgData[1] = 0x19;
    PrgData[2] = 0x8B;
    PrgData[3] = 0x13;
    PrgData[4] = 0x08;
    PrgData[5] = 0x00;
    PrgData[6] = 0x33;
    PrgData[7] = 0x30;
    PrgData[8] = 0xF7;
    PrgData[9] = 0x00;
    PrgData[10] = 0x77;
    PrgData[11] = 0x08;
    PrgData[12] = 0xF8;
    PrgData[13] = 0x00;
    PrgData[14] = 0x78;
    PrgData[15] = 0x30;
    PrgData[16] = 0xF7;
    PrgData[17] = 0x00;
    PrgData[18] = 0x77;
    PrgData[19] = 0x08;
    PrgData[20] = 0xF0;
    PrgData[21] = 0x00;
    PrgData[22] = 0xF1;
    PrgData[23] = 0x01;
    PrgData[24] = 0x1A;
    PrgData[25] = 0x30;
    PrgData[26] = 0x02;
    PrgData[27] = 0x27;
    PrgData[28] = 0x01;
    PrgData[29] = 0x28;
    PrgData[30] = 0x00;
    PrgData[31] = 0x00;
    PrgData[32] = 0x00;
    PrgData[33] = 0x00;
    PrgData[34] = 0x00;
    PrgData[35] = 0x00;
    PrgData[36] = 0x00;
    PrgData[37] = 0x00;
    PrgData[38] = 0x00;
    PrgData[39] = 0x00;
    PrgData[40] = 0x00;
    PrgData[41] = 0x00;
    PrgData[42] = 0x00;
    PrgData[43] = 0x00;
    PrgData[44] = 0x00;
    PrgData[45] = 0x00;
    PrgData[46] = 0x00;
    PrgData[47] = 0x00;
    PrgData[48] = 0x00;
    PrgData[49] = 0x00;
    PrgData[50] = 0x00;
    PrgData[51] = 0x00;
    PrgData[52] = 0x00;
    PrgData[53] = 0x00;
    PrgData[54] = 0x00;
    PrgData[55] = 0x00;
    PrgData[56] = 0x00;
    PrgData[57] = 0x00;
    PrgData[58] = 0x00;
    PrgData[59] = 0x00;
    PrgData[60] = 0x00;
    PrgData[61] = 0x00;
    PrgData[62] = 0x00;
    PrgData[63] = 0x00;

    EraseDataMem (0x0E / 2, 0x80/2);
    WriteDataMem (0x0E / 2, 0x80/2);
 * */


#ifdef	__cplusplus
}
#endif

#endif	/* PRGDATA_H */


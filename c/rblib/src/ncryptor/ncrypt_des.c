#include <stdio.h>
#include <memory.h>
#include "ncrypt_des.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////                                                        ////
//// [ Ncrypt DES Libaray from KSNET ]                      ////
////                                                        ////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static const unsigned char S_Box[8][64] =
        {{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
           0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
           4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
          15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 },
        { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
           3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
           0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
          13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9 },
        { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
          13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
          13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
           1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12 },
        {  7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
          13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
          10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
           3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14 },
        {  2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
          14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
           4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
          11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 },
        { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
          10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
           9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
           4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 },
        {  4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
          13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
           1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
           6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 },
        { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
           1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
           7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
           2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 }};

static unsigned char Ki2[16][8];
static const unsigned char LS[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
static unsigned char Li[4], Ri[4], Ri_1[4], Ki[7];

static void function_des(char cr_decr, char *Data, char *Key, char *Res)
{
	unsigned char buff[16];
	unsigned char i, k, Datan;
	unsigned char s1, s2;

    Li[0] = Li[1] = Li[2] = Li[3] = 0;
    Ri_1[0] = Ri_1[1] = Ri_1[2] = Ri_1[3] = 0;

    memcpy(buff, Key, 8);
    Ki[0] = buff[7] & 0x80;
    Ki[0] += (buff[0] >> 7);
    if(buff[6] & (0x80)) Ki[0] += 0x40;
    if(buff[5] & (0x80)) Ki[0] += 0x20;
    if(buff[4] & (0x80)) Ki[0] += 0x10;
    if(buff[3] & (0x80)) Ki[0] += 0x08;
    if(buff[2] & (0x80)) Ki[0] += 0x04;
    if(buff[1] & (0x80)) Ki[0] += 0x02;

    Ki[1] = buff[6] & 0x40;
    if(buff[7] & (0x40)) Ki[1] += 0x80;
    if(buff[5] & (0x40)) Ki[1] += 0x20;
    if(buff[4] & (0x40)) Ki[1] += 0x10;
    if(buff[3] & (0x40)) Ki[1] += 0x08;
    if(buff[2] & (0x40)) Ki[1] += 0x04;
    if(buff[1] & (0x40)) Ki[1] += 0x02;
    if(buff[0] & (0x40)) Ki[1] += 0x01;

    Ki[2] = buff[5] & 0x20;
    if(buff[7] & (0x20)) Ki[2] += 0x80;
    if(buff[6] & (0x20)) Ki[2] += 0x40;
    if(buff[4] & (0x20)) Ki[2] += 0x10;
    if(buff[3] & (0x20)) Ki[2] += 0x08;
    if(buff[2] & (0x20)) Ki[2] += 0x04;
    if(buff[1] & (0x20)) Ki[2] += 0x02;
    if(buff[0] & (0x20)) Ki[2] += 0x01;

    Ki[3] = (buff[4] & 0x10);
    Ki[3] += (buff[5] & 0x02);
    if(buff[7] & (0x10)) Ki[3] += 0x80;
    if(buff[6] & (0x10)) Ki[3] += 0x40;
    if(buff[5] & (0x10)) Ki[3] += 0x20;
    if(buff[7] & (0x02)) Ki[3] += 0x08;
    if(buff[6] & (0x02)) Ki[3] += 0x04;
    if(buff[4] & (0x02)) Ki[3] += 0x01;

    Ki[4] = buff[6] & 0x04;
    if(buff[3] & (0x02)) Ki[4] += 0x80;
    if(buff[2] & (0x02)) Ki[4] += 0x40;
    if(buff[1] & (0x02)) Ki[4] += 0x20;
    if(buff[0] & (0x02)) Ki[4] += 0x10;
    if(buff[7] & (0x04)) Ki[4] += 0x08;
    if(buff[5] & (0x04)) Ki[4] += 0x02;
    if(buff[4] & (0x04)) Ki[4] += 0x01;

    Ki[5] = buff[7] & 0x08;
    if(buff[3] & (0x04)) Ki[5] += 0x80;
    if(buff[2] & (0x04)) Ki[5] += 0x40;
    if(buff[1] & (0x04)) Ki[5] += 0x20;
    if(buff[0] & (0x04)) Ki[5] += 0x10;
    if(buff[6] & (0x08)) Ki[5] += 0x04;
    if(buff[5] & (0x08)) Ki[5] += 0x02;
    if(buff[4] & (0x08)) Ki[5] += 0x01;

    Ki[6] = (buff[3] & 0x08) << 4;
    if(buff[2] & (0x08)) Ki[6] += 0x40;
    if(buff[1] & (0x08)) Ki[6] += 0x20;
    if(buff[0] & (0x08)) Ki[6] += 0x10;
    if(buff[3] & (0x10)) Ki[6] += 0x08;
    if(buff[2] & (0x10)) Ki[6] += 0x04;
    if(buff[1] & (0x10)) Ki[6] += 0x02;
    if(buff[0] & (0x10)) Ki[6] += 0x01;

    for(i = 0; i < 16; i++){
            s1 = ((Ki[0] & 0x80) >> 3);
            s2 = ((Ki[3] & 0x08) >> 3);
            Ki[0] <<= 1;
            Ki[0] += Ki[1] >> 7;
            Ki[1] <<= 1;
            Ki[1] += Ki[2] >> 7;
            Ki[2] <<= 1;
            Ki[2] += Ki[3] >> 7;
            Ki[3] <<= 1;
            Ki[3] += Ki[4] >> 7;
            Ki[4] <<= 1;
            Ki[4] += Ki[5] >> 7;
            Ki[5] <<= 1;
            Ki[5] += Ki[6] >> 7;
            Ki[6] <<= 1;
            Ki[3] &= 0xEF;
            Ki[3] |= s1;
            Ki[6] += s2;
            if(LS[i] == 2){
                    s1 = ((Ki[0] & 0x80) >> 3);
                    s2 = ((Ki[3] & 0x08) >> 3);
                    Ki[0] <<= 1;
                    Ki[0] += (Ki[1]) >> 7;
                    Ki[1] <<= 1;
                    Ki[1] += (Ki[2]) >> 7;
                    Ki[2] <<= 1;
                    Ki[2] += (Ki[3]) >> 7;
                    Ki[3] <<= 1;
                    Ki[3] += (Ki[4]) >> 7;
                    Ki[4] <<= 1;
                    Ki[4] += (Ki[5]) >> 7;
                    Ki[5] <<= 1;
                    Ki[5] += (Ki[6]) >> 7;
                    Ki[6] <<= 1;
                    Ki[3] &= 0xEF;
                    Ki[3] |= s1;
                    Ki[6] += s2;
            }

            Ki2[i][0] = (Ki[1] & 0x04) << 3;
            if(Ki[2] & 0x80) Ki2[i][0] += 0x10;
            if(Ki[1] & 0x20) Ki2[i][0] += 0x08;
            if(Ki[2] & 0x01) Ki2[i][0] += 0x04;
            if(Ki[0] & 0x80) Ki2[i][0] += 0x02;
            if(Ki[0] & 0x08) Ki2[i][0] += 0x01;

            Ki2[i][1] = (Ki[0] & 0x20);
            Ki2[i][1] += (Ki[3] & 0x10);
            Ki2[i][1] += (Ki[0] & 0x04);
            if(Ki[1] & 0x02) Ki2[i][1] += 0x08;
            if(Ki[2] & 0x08) Ki2[i][1] += 0x02;
            if(Ki[1] & 0x40) Ki2[i][1] += 0x01;

            Ki2[i][2] = Ki[0] & 0x01;
            if(Ki[2] & 0x02) Ki2[i][2] += 0x20;
            if(Ki[2] & 0x20) Ki2[i][2] += 0x10;
            if(Ki[1] & 0x10) Ki2[i][2] += 0x08;
            if(Ki[0] & 0x10) Ki2[i][2] += 0x04;
            if(Ki[3] & 0x40) Ki2[i][2] += 0x02;

            Ki2[i][3] = (Ki[1] & 0x01) << 5;
            if(Ki[0] & 0x02) Ki2[i][3] += 0x10;
            if(Ki[3] & 0x20) Ki2[i][3] += 0x08;
            if(Ki[2] & 0x10) Ki2[i][3] += 0x04;
            if(Ki[1] & 0x08) Ki2[i][3] += 0x02;
            if(Ki[0] & 0x40) Ki2[i][3] += 0x01;

            Ki2[i][4] = (Ki[6] & 0x10);
            Ki2[i][4] += (Ki[5] & 0x02);
            if(Ki[5] & 0x80) Ki2[i][4] += 0x20;
            if(Ki[3] & 0x02) Ki2[i][4] += 0x08;
            if(Ki[4] & 0x08) Ki2[i][4] += 0x04;
            if(Ki[6] & 0x02) Ki2[i][4] += 0x01;

            Ki2[i][5] = Ki[5] & 0x01;
            if(Ki[3] & 0x04) Ki2[i][5] += 0x20;
            if(Ki[4] & 0x01) Ki2[i][5] += 0x10;
            if(Ki[6] & 0x20) Ki2[i][5] += 0x08;
            if(Ki[5] & 0x08) Ki2[i][5] += 0x04;
            if(Ki[4] & 0x80) Ki2[i][5] += 0x02;

            Ki2[i][6] = (Ki[5] & 0x10) << 1;
            if(Ki[6] & 0x80) Ki2[i][6] += 0x10;
            if(Ki[4] & 0x02) Ki2[i][6] += 0x08;
            if(Ki[6] & 0x01) Ki2[i][6] += 0x04;
            if(Ki[4] & 0x40) Ki2[i][6] += 0x02;
            if(Ki[6] & 0x08) Ki2[i][6] += 0x01;

            Ki2[i][7] = Ki[3] & 0x01;
            if(Ki[5] & 0x04) Ki2[i][7] += 0x20;
            if(Ki[5] & 0x40) Ki2[i][7] += 0x10;
            if(Ki[6] & 0x40) Ki2[i][7] += 0x08;
            if(Ki[4] & 0x10) Ki2[i][7] += 0x04;
            if(Ki[3] & 0x08) Ki2[i][7] += 0x02;
    }

    for(i = 0; i < 8; i++){
            k = (1 << i);
            Datan = Data[i];
            if(Datan & 0x40) Li[0] += k;
            if(Datan & 0x10) Li[1] += k;
            if(Datan & 0x04) Li[2] += k;
            if(Datan & 0x01) Li[3] += k;
            if(Datan & 0x80) Ri_1[0] += k;
            if(Datan & 0x20) Ri_1[1] += k;
            if(Datan & 0x08) Ri_1[2] += k;
            if(Datan & 0x02) Ri_1[3] += k;
            Data[i] = Datan;
    }

    for(i = 0; i < 16; i++){
            buff[0] = ((Ri_1[3] & 0x01) << 5) | ((Ri_1[0]) >> 3);
            buff[1] = ((Ri_1[0] & 0x1F) << 1) | ((Ri_1[1]) >> 7);
            buff[2] = ((Ri_1[0] & 0x01) << 5) | ((Ri_1[1]) >> 3);
            buff[3] = ((Ri_1[1] & 0x1F) << 1) | ((Ri_1[2]) >> 7);
            buff[4] = ((Ri_1[1] & 0x01) << 5) | ((Ri_1[2]) >> 3);
            buff[5] = ((Ri_1[2] & 0x1F) << 1) | ((Ri_1[3]) >> 7);
            buff[6] = ((Ri_1[2] & 0x01) << 5) | ((Ri_1[3]) >> 3);
            buff[7] = ((Ri_1[3] & 0x1F) << 1) | ((Ri_1[0]) >> 7);

            if(cr_decr == 1){
                    buff[0] ^= Ki2[i][0];
                    s1 = (buff[0] & 0x20) + ((buff[0] & 0x01) << 4);
                    s2 = (buff[0] & 0x1E) >> 1;
                    buff[0] = S_Box[0][s1 + s2];

                    buff[1] ^= Ki2[i][1];
                    s1 = (buff[1] & 0x20) + ((buff[1] & 0x01) << 4);
                    s2 = (buff[1] & 0x1E) >> 1;
                    buff[1] = S_Box[1][s1 + s2];

                    buff[2] ^= Ki2[i][2];
                    s1 = (buff[2] & 0x20) + ((buff[2] & 0x01) << 4);
                    s2 = (buff[2] & 0x1E) >> 1;
                    buff[2] = S_Box[2][s1 + s2];

                    buff[3] ^= Ki2[i][3];
                    s1 = (buff[3] & 0x20) + ((buff[3] & 0x01) << 4);
                    s2 = (buff[3] & 0x1E) >> 1;
                    buff[3] = S_Box[3][s1 + s2];

                    buff[4] ^= Ki2[i][4];
                    s1 = (buff[4] & 0x20) + ((buff[4] & 0x01) << 4);
                    s2 = (buff[4] & 0x1E) >> 1;
                    buff[4] = S_Box[4][s1 + s2];

                    buff[5] ^= Ki2[i][5];
                    s1 = (buff[5] & 0x20) + ((buff[5] & 0x01) << 4);
                    s2 = (buff[5] & 0x1E) >> 1;
                    buff[5] = S_Box[5][s1 + s2];

                    buff[6] ^= Ki2[i][6];
                    s1 = (buff[6] & 0x20) + ((buff[6] & 0x01) << 4);
                    s2 = (buff[6] & 0x1E) >> 1;
                    buff[6] = S_Box[6][s1 + s2];

                    buff[7] ^= Ki2[i][7];
                    s1 = (buff[7] & 0x20) + ((buff[7] & 0x01) << 4);
                    s2 = (buff[7] & 0x1E) >> 1;
                    buff[7] = S_Box[7][s1 + s2];
            }else{
                    buff[0] ^= Ki2[15-i][0];
                    s1 = (buff[0] & 0x20) + ((buff[0] & 0x01) << 4);
                    s2 = (buff[0] & 0x1E) >> 1;
                    buff[0] = S_Box[0][s1 + s2];

                    buff[1] ^= Ki2[15-i][1];
                    s1 = (buff[1] & 0x20) + ((buff[1] & 0x01) << 4);
                    s2 = (buff[1] & 0x1E) >> 1;
                    buff[1] = S_Box[1][s1 + s2];

                    buff[2] ^= Ki2[15-i][2];
                    s1 = (buff[2] & 0x20) + ((buff[2] & 0x01) << 4);
                    s2 = (buff[2] & 0x1E) >> 1;
                    buff[2] = S_Box[2][s1 + s2];

                    buff[3] ^= Ki2[15-i][3];
                    s1 = (buff[3] & 0x20) + ((buff[3] & 0x01) << 4);
                    s2 = (buff[3] & 0x1E) >> 1;
                    buff[3] = S_Box[3][s1 + s2];

                    buff[4] ^= Ki2[15-i][4];
                    s1 = (buff[4] & 0x20) + ((buff[4] & 0x01) << 4);
                    s2 = (buff[4] & 0x1E) >> 1;
                    buff[4] = S_Box[4][s1 + s2];

                    buff[5] ^= Ki2[15-i][5];
                    s1 = (buff[5] & 0x20) + ((buff[5] & 0x01) << 4);
                    s2 = (buff[5] & 0x1E) >> 1;
                    buff[5] = S_Box[5][s1 + s2];

                    buff[6] ^= Ki2[15-i][6];
                    s1 = (buff[6] & 0x20) + ((buff[6] & 0x01) << 4);
                    s2 = (buff[6] & 0x1E) >> 1;
                    buff[6] = S_Box[6][s1 + s2];

                    buff[7] ^= Ki2[15-i][7];
                    s1 = (buff[7] & 0x20) + ((buff[7] & 0x01) << 4);
                    s2 = (buff[7] & 0x1E) >> 1;
                    buff[7] = S_Box[7][s1 + s2];
            }

            buff[0] = (buff[0] << 4) + buff[1];
            buff[1] = (buff[2] << 4) + buff[3];
            buff[2] = (buff[4] << 4) + buff[5];
            buff[3] = (buff[6] << 4) + buff[7];

            Ri[0] = (buff[3] & 0x08);
            Ri[0] += (buff[2] & 0x18) << 1;
            Ri[0] += buff[1] << 7;
            Ri[0] += buff[2] >> 7;
            if(buff[0] & (0x02)) Ri[0] += 0x40;
            if(buff[1] & (0x10)) Ri[0] += 0x04;
            if(buff[3] & (0x10)) Ri[0] += 0x02;

            Ri[1] = (buff[0] & 0x80);
            Ri[1] += (buff[0] & 0x08);
            Ri[1] += (buff[3] & 0x02);
            if(buff[1] & (0x02)) Ri[1] += 0x40;
            if(buff[2] & (0x02)) Ri[1] += 0x20;
            if(buff[3] & (0x40)) Ri[1] += 0x10;
            if(buff[2] & (0x40)) Ri[1] += 0x04;
            if(buff[1] & (0x40)) Ri[1] += 0x01;

            Ri[2] = buff[1] >> 7;
            if(buff[0] & (0x40)) Ri[2] += 0x80;
            if(buff[0] & (0x01)) Ri[2] += 0x40;
            if(buff[2] & (0x01)) Ri[2] += 0x20;
            if(buff[1] & (0x04)) Ri[2] += 0x10;
            if(buff[3] & (0x01)) Ri[2] += 0x08;
            if(buff[3] & (0x20)) Ri[2] += 0x04;
            if(buff[0] & (0x20)) Ri[2] += 0x02;

            Ri[3] = buff[3] >> 7;
            if(buff[2] & (0x20)) Ri[3] += 0x80;
            if(buff[1] & (0x08)) Ri[3] += 0x40;
            if(buff[3] & (0x04)) Ri[3] += 0x20;
            if(buff[0] & (0x04)) Ri[3] += 0x10;
            if(buff[2] & (0x04)) Ri[3] += 0x08;
            if(buff[1] & (0x20)) Ri[3] += 0x04;
            if(buff[0] & (0x10)) Ri[3] += 0x02;

            Ri[0] ^= Li[0];         Ri[1] ^= Li[1];
            Ri[2] ^= Li[2];         Ri[3] ^= Li[3];

            Li[0] = Ri_1[0];        Li[1] = Ri_1[1];
            Li[2] = Ri_1[2];        Li[3] = Ri_1[3];

            Ri_1[0] = Ri[0];        Ri_1[1] = Ri[1];
            Ri_1[2] = Ri[2];        Ri_1[3] = Ri[3];
    }

    for(i = 0; i < 8; i++){
            k = (1 << i);
            Datan = 0;
            if(Li[0] & k) Datan += 0x80;
            if(Li[1] & k) Datan += 0x20;
            if(Li[2] & k) Datan += 0x08;
            if(Li[3] & k) Datan += 0x02;
            if(Ri[0] & k) Datan += 0x40;
            if(Ri[1] & k) Datan += 0x10;
            if(Ri[2] & k) Datan += 0x04;
            if(Ri[3] & k) Datan += 0x01;
            Res[i] = Datan;
    }
}

/*
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////                                                        ////
//// [ Ncrypt DES Interface Source ]                        ////
////                                                        ////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
*/

#define N_ROUNDS 16
#define SZ_BLOCK 8
#define SZ_KEY_MAX 24 /* 8(Single DES), 24(Triple DES) */
#define SZ_ROUND_KEY_MAX (SZ_BLOCK * N_ROUNDS * 3)

static void EnDecrypt_DES(BYTE *pbBlock, int szBlock, BYTE *pbRoundKey, int szRoundKey, void *pstArgs)
{
    static const int     abKeyIdx[2][3] = { { 0, 8, 0 }, { 0, 8, 16 } }; /* 16,24byte key start idx */
    NCRYPT_ALGO_DES_ARGS *pstDES_Args = (NCRYPT_ALGO_DES_ARGS *)pstArgs;
	BYTE                 *pbKey = NULL;
    BYTE 				 rtBuf[szBlock];
    char                 ncryptOps = pstDES_Args->stArgs.isEncrypt;
    int                  nTripleCnt = pstDES_Args->isTripleDES == 1 ? 3 : 1;
    int                  szKey = pstDES_Args->stArgs.szKey;
    int                  idx, i;
    
    for (i = 0; i < nTripleCnt; ++i)
    {
        /*
		//              [Encryption] / [Decryption]
		// - Single DES:     ENC     /     DEC
		// - Triple DES: ENC-DEC-ENC / DEC-ENC-DEC
        */
        if (szKey == 8)
        {
            idx = 0;
        }
        else if (szKey == 16)
        {
            idx = abKeyIdx[0][i]; /* 0, 8, 0 */
        }
        else
        {
            idx = abKeyIdx[1][(pstDES_Args->stArgs.isEncrypt == 1 ? i : 2 - i)]; /* 0, 8, 16 or 16, 8, 0 */
        }
		
        pbKey = &(pstDES_Args->stArgs.pbKey[idx]);

        function_des(ncryptOps, pbBlock, pbKey, rtBuf);
		memcpy(pbBlock, rtBuf, sizeof(rtBuf));
		ncryptOps = !ncryptOps;
    }
}

int Ncrypt_DES(NCRYPT_ALGO_DES_ARGS *pstArgs)
{
    /* DES(Data Encryption Standard) 64bit(8byte) fixed-size block en/decrypt algorithm */
    /* - Use 8 or 24byte key(each are single, triple algorithm), 128/384byte round key, */
    /*   input 8byte stream block, output 8byte block(NULL padding) */
    static int			IsInitalized = 0;
	int                 szKey = pstArgs->stArgs.szKey;
    int					rtVal;

	if (pstArgs == NULL)
	{
		fprintf(stderr, "pstArgs == NULL\n");
		return -1;
	}

    if (pstArgs->isTripleDES == 1 && szKey != 16 && szKey != 24)
    {
        fprintf(stderr, "When DES mode is triple, szKey must be 16 or 24. (szKey:%d)\n", szKey);
        return -1;
    }
    else if (pstArgs->isTripleDES != 1 && szKey != 8)
    {
        fprintf(stderr, "When DES mode is single, szKey must be 8. (szKey:%d)\n", szKey);
        return -1;
    }

    if ((rtVal = Ncryption(&pstArgs->stArgs, SZ_BLOCK, pstArgs->stArgs.pbKey,
                           pstArgs->stArgs.szKey, EnDecrypt_DES)) < 0)
    {
        fprintf(stderr, "Ncryption() error. (return:%d)\n", rtVal);
        return -1;
    }

    return rtVal;
}
#pragma warning(disable : 6031)
#pragma warning(disable : 4996)
#include <stdio.h>

#define Nr 4
#define inBufSize (1 << 23)
#define outBufSize (1 << 22)

#define quickFread() (pIn == pInMax? (fread(inBuf, 1, inBufSize, stdin), pInMax = inBuf + inBufSize, *((pIn = inBuf)++)) : *(pIn++))

char inBuf[inBufSize], * pIn, * pInMax;
char outBuf[outBufSize], * pOut = outBuf;

// define substitution
const char substitution[16] = { 0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7 };
const char substitutionInv[16] = { 0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5 };
const unsigned short SubAndPerTable_En_1[0x100] = {
	0xccc0, 0x8c80, 0xcc84, 0x8884, 0x88c0, 0xccc4, 0xc8c4, 0xc880, 0x88c4, 0xc8c0, 0x8cc0, 0xcc80, 0x8c84, 0xc884, 0x8880, 0x8cc4,
	0x4c40, 0x0c00, 0x4c04, 0x0804, 0x0840, 0x4c44, 0x4844, 0x4800, 0x0844, 0x4840, 0x0c40, 0x4c00, 0x0c04, 0x4804, 0x0800, 0x0c44,
	0xcc48, 0x8c08, 0xcc0c, 0x880c, 0x8848, 0xcc4c, 0xc84c, 0xc808, 0x884c, 0xc848, 0x8c48, 0xcc08, 0x8c0c, 0xc80c, 0x8808, 0x8c4c,
	0x4448, 0x0408, 0x440c, 0x000c, 0x0048, 0x444c, 0x404c, 0x4008, 0x004c, 0x4048, 0x0448, 0x4408, 0x040c, 0x400c, 0x0008, 0x044c,
	0x44c0, 0x0480, 0x4484, 0x0084, 0x00c0, 0x44c4, 0x40c4, 0x4080, 0x00c4, 0x40c0, 0x04c0, 0x4480, 0x0484, 0x4084, 0x0080, 0x04c4,
	0xccc8, 0x8c88, 0xcc8c, 0x888c, 0x88c8, 0xcccc, 0xc8cc, 0xc888, 0x88cc, 0xc8c8, 0x8cc8, 0xcc88, 0x8c8c, 0xc88c, 0x8888, 0x8ccc,
	0xc4c8, 0x8488, 0xc48c, 0x808c, 0x80c8, 0xc4cc, 0xc0cc, 0xc088, 0x80cc, 0xc0c8, 0x84c8, 0xc488, 0x848c, 0xc08c, 0x8088, 0x84cc,
	0xc440, 0x8400, 0xc404, 0x8004, 0x8040, 0xc444, 0xc044, 0xc000, 0x8044, 0xc040, 0x8440, 0xc400, 0x8404, 0xc004, 0x8000, 0x8444,
	0x44c8, 0x0488, 0x448c, 0x008c, 0x00c8, 0x44cc, 0x40cc, 0x4088, 0x00cc, 0x40c8, 0x04c8, 0x4488, 0x048c, 0x408c, 0x0088, 0x04cc,
	0xc4c0, 0x8480, 0xc484, 0x8084, 0x80c0, 0xc4c4, 0xc0c4, 0xc080, 0x80c4, 0xc0c0, 0x84c0, 0xc480, 0x8484, 0xc084, 0x8080, 0x84c4,
	0x4cc0, 0x0c80, 0x4c84, 0x0884, 0x08c0, 0x4cc4, 0x48c4, 0x4880, 0x08c4, 0x48c0, 0x0cc0, 0x4c80, 0x0c84, 0x4884, 0x0880, 0x0cc4,
	0xcc40, 0x8c00, 0xcc04, 0x8804, 0x8840, 0xcc44, 0xc844, 0xc800, 0x8844, 0xc840, 0x8c40, 0xcc00, 0x8c04, 0xc804, 0x8800, 0x8c44,
	0x4c48, 0x0c08, 0x4c0c, 0x080c, 0x0848, 0x4c4c, 0x484c, 0x4808, 0x084c, 0x4848, 0x0c48, 0x4c08, 0x0c0c, 0x480c, 0x0808, 0x0c4c,
	0xc448, 0x8408, 0xc40c, 0x800c, 0x8048, 0xc44c, 0xc04c, 0xc008, 0x804c, 0xc048, 0x8448, 0xc408, 0x840c, 0xc00c, 0x8008, 0x844c,
	0x4440, 0x0400, 0x4404, 0x0004, 0x0040, 0x4444, 0x4044, 0x4000, 0x0044, 0x4040, 0x0440, 0x4400, 0x0404, 0x4004, 0x0000, 0x0444,
	0x4cc8, 0x0c88, 0x4c8c, 0x088c, 0x08c8, 0x4ccc, 0x48cc, 0x4888, 0x08cc, 0x48c8, 0x0cc8, 0x4c88, 0x0c8c, 0x488c, 0x0888, 0x0ccc
};
const unsigned short SubAndPerTable_En_2[0x100] = {
	0x3330, 0x2320, 0x3321, 0x2221, 0x2230, 0x3331, 0x3231, 0x3220, 0x2231, 0x3230, 0x2330, 0x3320, 0x2321, 0x3221, 0x2220, 0x2331,
	0x1310, 0x0300, 0x1301, 0x0201, 0x0210, 0x1311, 0x1211, 0x1200, 0x0211, 0x1210, 0x0310, 0x1300, 0x0301, 0x1201, 0x0200, 0x0311,
	0x3312, 0x2302, 0x3303, 0x2203, 0x2212, 0x3313, 0x3213, 0x3202, 0x2213, 0x3212, 0x2312, 0x3302, 0x2303, 0x3203, 0x2202, 0x2313,
	0x1112, 0x0102, 0x1103, 0x0003, 0x0012, 0x1113, 0x1013, 0x1002, 0x0013, 0x1012, 0x0112, 0x1102, 0x0103, 0x1003, 0x0002, 0x0113,
	0x1130, 0x0120, 0x1121, 0x0021, 0x0030, 0x1131, 0x1031, 0x1020, 0x0031, 0x1030, 0x0130, 0x1120, 0x0121, 0x1021, 0x0020, 0x0131,
	0x3332, 0x2322, 0x3323, 0x2223, 0x2232, 0x3333, 0x3233, 0x3222, 0x2233, 0x3232, 0x2332, 0x3322, 0x2323, 0x3223, 0x2222, 0x2333,
	0x3132, 0x2122, 0x3123, 0x2023, 0x2032, 0x3133, 0x3033, 0x3022, 0x2033, 0x3032, 0x2132, 0x3122, 0x2123, 0x3023, 0x2022, 0x2133,
	0x3110, 0x2100, 0x3101, 0x2001, 0x2010, 0x3111, 0x3011, 0x3000, 0x2011, 0x3010, 0x2110, 0x3100, 0x2101, 0x3001, 0x2000, 0x2111,
	0x1132, 0x0122, 0x1123, 0x0023, 0x0032, 0x1133, 0x1033, 0x1022, 0x0033, 0x1032, 0x0132, 0x1122, 0x0123, 0x1023, 0x0022, 0x0133,
	0x3130, 0x2120, 0x3121, 0x2021, 0x2030, 0x3131, 0x3031, 0x3020, 0x2031, 0x3030, 0x2130, 0x3120, 0x2121, 0x3021, 0x2020, 0x2131,
	0x1330, 0x0320, 0x1321, 0x0221, 0x0230, 0x1331, 0x1231, 0x1220, 0x0231, 0x1230, 0x0330, 0x1320, 0x0321, 0x1221, 0x0220, 0x0331,
	0x3310, 0x2300, 0x3301, 0x2201, 0x2210, 0x3311, 0x3211, 0x3200, 0x2211, 0x3210, 0x2310, 0x3300, 0x2301, 0x3201, 0x2200, 0x2311,
	0x1312, 0x0302, 0x1303, 0x0203, 0x0212, 0x1313, 0x1213, 0x1202, 0x0213, 0x1212, 0x0312, 0x1302, 0x0303, 0x1203, 0x0202, 0x0313,
	0x3112, 0x2102, 0x3103, 0x2003, 0x2012, 0x3113, 0x3013, 0x3002, 0x2013, 0x3012, 0x2112, 0x3102, 0x2103, 0x3003, 0x2002, 0x2113,
	0x1110, 0x0100, 0x1101, 0x0001, 0x0010, 0x1111, 0x1011, 0x1000, 0x0011, 0x1010, 0x0110, 0x1100, 0x0101, 0x1001, 0x0000, 0x0111,
	0x1332, 0x0322, 0x1323, 0x0223, 0x0232, 0x1333, 0x1233, 0x1222, 0x0233, 0x1232, 0x0332, 0x1322, 0x0323, 0x1223, 0x0222, 0x0333
};
const unsigned short SubAndPerTable_De_1[0x100] = {
	0xccc0, 0x88c4, 0x8c80, 0xc880, 0x8884, 0xcc80, 0xc8c0, 0xccc4, 0x8cc4, 0xcc84, 0xc884, 0x8cc0, 0xc8c4, 0x88c0, 0x8880, 0x8c84,
	0x44c8, 0x00cc, 0x0488, 0x4088, 0x008c, 0x4488, 0x40c8, 0x44cc, 0x04cc, 0x448c, 0x408c, 0x04c8, 0x40cc, 0x00c8, 0x0088, 0x048c,
	0x4c40, 0x0844, 0x0c00, 0x4800, 0x0804, 0x4c00, 0x4840, 0x4c44, 0x0c44, 0x4c04, 0x4804, 0x0c40, 0x4844, 0x0840, 0x0800, 0x0c04,
	0xc440, 0x8044, 0x8400, 0xc000, 0x8004, 0xc400, 0xc040, 0xc444, 0x8444, 0xc404, 0xc004, 0x8440, 0xc044, 0x8040, 0x8000, 0x8404,
	0x4448, 0x004c, 0x0408, 0x4008, 0x000c, 0x4408, 0x4048, 0x444c, 0x044c, 0x440c, 0x400c, 0x0448, 0x404c, 0x0048, 0x0008, 0x040c,
	0xcc40, 0x8844, 0x8c00, 0xc800, 0x8804, 0xcc00, 0xc840, 0xcc44, 0x8c44, 0xcc04, 0xc804, 0x8c40, 0xc844, 0x8840, 0x8800, 0x8c04,
	0xc4c0, 0x80c4, 0x8480, 0xc080, 0x8084, 0xc480, 0xc0c0, 0xc4c4, 0x84c4, 0xc484, 0xc084, 0x84c0, 0xc0c4, 0x80c0, 0x8080, 0x8484,
	0xccc8, 0x88cc, 0x8c88, 0xc888, 0x888c, 0xcc88, 0xc8c8, 0xcccc, 0x8ccc, 0xcc8c, 0xc88c, 0x8cc8, 0xc8cc, 0x88c8, 0x8888, 0x8c8c,
	0x4cc8, 0x08cc, 0x0c88, 0x4888, 0x088c, 0x4c88, 0x48c8, 0x4ccc, 0x0ccc, 0x4c8c, 0x488c, 0x0cc8, 0x48cc, 0x08c8, 0x0888, 0x0c8c,
	0xcc48, 0x884c, 0x8c08, 0xc808, 0x880c, 0xcc08, 0xc848, 0xcc4c, 0x8c4c, 0xcc0c, 0xc80c, 0x8c48, 0xc84c, 0x8848, 0x8808, 0x8c0c,
	0xc448, 0x804c, 0x8408, 0xc008, 0x800c, 0xc408, 0xc048, 0xc44c, 0x844c, 0xc40c, 0xc00c, 0x8448, 0xc04c, 0x8048, 0x8008, 0x840c,
	0x4cc0, 0x08c4, 0x0c80, 0x4880, 0x0884, 0x4c80, 0x48c0, 0x4cc4, 0x0cc4, 0x4c84, 0x4884, 0x0cc0, 0x48c4, 0x08c0, 0x0880, 0x0c84,
	0xc4c8, 0x80cc, 0x8488, 0xc088, 0x808c, 0xc488, 0xc0c8, 0xc4cc, 0x84cc, 0xc48c, 0xc08c, 0x84c8, 0xc0cc, 0x80c8, 0x8088, 0x848c,
	0x44c0, 0x00c4, 0x0480, 0x4080, 0x0084, 0x4480, 0x40c0, 0x44c4, 0x04c4, 0x4484, 0x4084, 0x04c0, 0x40c4, 0x00c0, 0x0080, 0x0484,
	0x4440, 0x0044, 0x0400, 0x4000, 0x0004, 0x4400, 0x4040, 0x4444, 0x0444, 0x4404, 0x4004, 0x0440, 0x4044, 0x0040, 0x0000, 0x0404,
	0x4c48, 0x084c, 0x0c08, 0x4808, 0x080c, 0x4c08, 0x4848, 0x4c4c, 0x0c4c, 0x4c0c, 0x480c, 0x0c48, 0x484c, 0x0848, 0x0808, 0x0c0c
};
const unsigned short SubAndPerTable_De_2[0x100] = {
	0x3330, 0x2231, 0x2320, 0x3220, 0x2221, 0x3320, 0x3230, 0x3331, 0x2331, 0x3321, 0x3221, 0x2330, 0x3231, 0x2230, 0x2220, 0x2321,
	0x1132, 0x0033, 0x0122, 0x1022, 0x0023, 0x1122, 0x1032, 0x1133, 0x0133, 0x1123, 0x1023, 0x0132, 0x1033, 0x0032, 0x0022, 0x0123,
	0x1310, 0x0211, 0x0300, 0x1200, 0x0201, 0x1300, 0x1210, 0x1311, 0x0311, 0x1301, 0x1201, 0x0310, 0x1211, 0x0210, 0x0200, 0x0301,
	0x3110, 0x2011, 0x2100, 0x3000, 0x2001, 0x3100, 0x3010, 0x3111, 0x2111, 0x3101, 0x3001, 0x2110, 0x3011, 0x2010, 0x2000, 0x2101,
	0x1112, 0x0013, 0x0102, 0x1002, 0x0003, 0x1102, 0x1012, 0x1113, 0x0113, 0x1103, 0x1003, 0x0112, 0x1013, 0x0012, 0x0002, 0x0103,
	0x3310, 0x2211, 0x2300, 0x3200, 0x2201, 0x3300, 0x3210, 0x3311, 0x2311, 0x3301, 0x3201, 0x2310, 0x3211, 0x2210, 0x2200, 0x2301,
	0x3130, 0x2031, 0x2120, 0x3020, 0x2021, 0x3120, 0x3030, 0x3131, 0x2131, 0x3121, 0x3021, 0x2130, 0x3031, 0x2030, 0x2020, 0x2121,
	0x3332, 0x2233, 0x2322, 0x3222, 0x2223, 0x3322, 0x3232, 0x3333, 0x2333, 0x3323, 0x3223, 0x2332, 0x3233, 0x2232, 0x2222, 0x2323,
	0x1332, 0x0233, 0x0322, 0x1222, 0x0223, 0x1322, 0x1232, 0x1333, 0x0333, 0x1323, 0x1223, 0x0332, 0x1233, 0x0232, 0x0222, 0x0323,
	0x3312, 0x2213, 0x2302, 0x3202, 0x2203, 0x3302, 0x3212, 0x3313, 0x2313, 0x3303, 0x3203, 0x2312, 0x3213, 0x2212, 0x2202, 0x2303,
	0x3112, 0x2013, 0x2102, 0x3002, 0x2003, 0x3102, 0x3012, 0x3113, 0x2113, 0x3103, 0x3003, 0x2112, 0x3013, 0x2012, 0x2002, 0x2103,
	0x1330, 0x0231, 0x0320, 0x1220, 0x0221, 0x1320, 0x1230, 0x1331, 0x0331, 0x1321, 0x1221, 0x0330, 0x1231, 0x0230, 0x0220, 0x0321,
	0x3132, 0x2033, 0x2122, 0x3022, 0x2023, 0x3122, 0x3032, 0x3133, 0x2133, 0x3123, 0x3023, 0x2132, 0x3033, 0x2032, 0x2022, 0x2123,
	0x1130, 0x0031, 0x0120, 0x1020, 0x0021, 0x1120, 0x1030, 0x1131, 0x0131, 0x1121, 0x1021, 0x0130, 0x1031, 0x0030, 0x0020, 0x0121,
	0x1110, 0x0011, 0x0100, 0x1000, 0x0001, 0x1100, 0x1010, 0x1111, 0x0111, 0x1101, 0x1001, 0x0110, 0x1011, 0x0010, 0x0000, 0x0101,
	0x1312, 0x0213, 0x0302, 0x1202, 0x0203, 0x1302, 0x1212, 0x1313, 0x0313, 0x1303, 0x1203, 0x0312, 0x1213, 0x0212, 0x0202, 0x0303
};
const unsigned short PermutationTable_1[0x100] = {
	0x0000, 0x0004, 0x0040, 0x0044, 0x0400, 0x0404, 0x0440, 0x0444, 0x4000, 0x4004, 0x4040, 0x4044, 0x4400, 0x4404, 0x4440, 0x4444,
	0x0008, 0x000c, 0x0048, 0x004c, 0x0408, 0x040c, 0x0448, 0x044c, 0x4008, 0x400c, 0x4048, 0x404c, 0x4408, 0x440c, 0x4448, 0x444c,
	0x0080, 0x0084, 0x00c0, 0x00c4, 0x0480, 0x0484, 0x04c0, 0x04c4, 0x4080, 0x4084, 0x40c0, 0x40c4, 0x4480, 0x4484, 0x44c0, 0x44c4,
	0x0088, 0x008c, 0x00c8, 0x00cc, 0x0488, 0x048c, 0x04c8, 0x04cc, 0x4088, 0x408c, 0x40c8, 0x40cc, 0x4488, 0x448c, 0x44c8, 0x44cc,
	0x0800, 0x0804, 0x0840, 0x0844, 0x0c00, 0x0c04, 0x0c40, 0x0c44, 0x4800, 0x4804, 0x4840, 0x4844, 0x4c00, 0x4c04, 0x4c40, 0x4c44,
	0x0808, 0x080c, 0x0848, 0x084c, 0x0c08, 0x0c0c, 0x0c48, 0x0c4c, 0x4808, 0x480c, 0x4848, 0x484c, 0x4c08, 0x4c0c, 0x4c48, 0x4c4c,
	0x0880, 0x0884, 0x08c0, 0x08c4, 0x0c80, 0x0c84, 0x0cc0, 0x0cc4, 0x4880, 0x4884, 0x48c0, 0x48c4, 0x4c80, 0x4c84, 0x4cc0, 0x4cc4,
	0x0888, 0x088c, 0x08c8, 0x08cc, 0x0c88, 0x0c8c, 0x0cc8, 0x0ccc, 0x4888, 0x488c, 0x48c8, 0x48cc, 0x4c88, 0x4c8c, 0x4cc8, 0x4ccc,
	0x8000, 0x8004, 0x8040, 0x8044, 0x8400, 0x8404, 0x8440, 0x8444, 0xc000, 0xc004, 0xc040, 0xc044, 0xc400, 0xc404, 0xc440, 0xc444,
	0x8008, 0x800c, 0x8048, 0x804c, 0x8408, 0x840c, 0x8448, 0x844c, 0xc008, 0xc00c, 0xc048, 0xc04c, 0xc408, 0xc40c, 0xc448, 0xc44c,
	0x8080, 0x8084, 0x80c0, 0x80c4, 0x8480, 0x8484, 0x84c0, 0x84c4, 0xc080, 0xc084, 0xc0c0, 0xc0c4, 0xc480, 0xc484, 0xc4c0, 0xc4c4,
	0x8088, 0x808c, 0x80c8, 0x80cc, 0x8488, 0x848c, 0x84c8, 0x84cc, 0xc088, 0xc08c, 0xc0c8, 0xc0cc, 0xc488, 0xc48c, 0xc4c8, 0xc4cc,
	0x8800, 0x8804, 0x8840, 0x8844, 0x8c00, 0x8c04, 0x8c40, 0x8c44, 0xc800, 0xc804, 0xc840, 0xc844, 0xcc00, 0xcc04, 0xcc40, 0xcc44,
	0x8808, 0x880c, 0x8848, 0x884c, 0x8c08, 0x8c0c, 0x8c48, 0x8c4c, 0xc808, 0xc80c, 0xc848, 0xc84c, 0xcc08, 0xcc0c, 0xcc48, 0xcc4c,
	0x8880, 0x8884, 0x88c0, 0x88c4, 0x8c80, 0x8c84, 0x8cc0, 0x8cc4, 0xc880, 0xc884, 0xc8c0, 0xc8c4, 0xcc80, 0xcc84, 0xccc0, 0xccc4,
	0x8888, 0x888c, 0x88c8, 0x88cc, 0x8c88, 0x8c8c, 0x8cc8, 0x8ccc, 0xc888, 0xc88c, 0xc8c8, 0xc8cc, 0xcc88, 0xcc8c, 0xccc8, 0xcccc
};
const unsigned short PermutationTable_2[0x100] = {
	0x0000, 0x0001, 0x0010, 0x0011, 0x0100, 0x0101, 0x0110, 0x0111, 0x1000, 0x1001, 0x1010, 0x1011, 0x1100, 0x1101, 0x1110, 0x1111,
	0x0002, 0x0003, 0x0012, 0x0013, 0x0102, 0x0103, 0x0112, 0x0113, 0x1002, 0x1003, 0x1012, 0x1013, 0x1102, 0x1103, 0x1112, 0x1113,
	0x0020, 0x0021, 0x0030, 0x0031, 0x0120, 0x0121, 0x0130, 0x0131, 0x1020, 0x1021, 0x1030, 0x1031, 0x1120, 0x1121, 0x1130, 0x1131,
	0x0022, 0x0023, 0x0032, 0x0033, 0x0122, 0x0123, 0x0132, 0x0133, 0x1022, 0x1023, 0x1032, 0x1033, 0x1122, 0x1123, 0x1132, 0x1133,
	0x0200, 0x0201, 0x0210, 0x0211, 0x0300, 0x0301, 0x0310, 0x0311, 0x1200, 0x1201, 0x1210, 0x1211, 0x1300, 0x1301, 0x1310, 0x1311,
	0x0202, 0x0203, 0x0212, 0x0213, 0x0302, 0x0303, 0x0312, 0x0313, 0x1202, 0x1203, 0x1212, 0x1213, 0x1302, 0x1303, 0x1312, 0x1313,
	0x0220, 0x0221, 0x0230, 0x0231, 0x0320, 0x0321, 0x0330, 0x0331, 0x1220, 0x1221, 0x1230, 0x1231, 0x1320, 0x1321, 0x1330, 0x1331,
	0x0222, 0x0223, 0x0232, 0x0233, 0x0322, 0x0323, 0x0332, 0x0333, 0x1222, 0x1223, 0x1232, 0x1233, 0x1322, 0x1323, 0x1332, 0x1333,
	0x2000, 0x2001, 0x2010, 0x2011, 0x2100, 0x2101, 0x2110, 0x2111, 0x3000, 0x3001, 0x3010, 0x3011, 0x3100, 0x3101, 0x3110, 0x3111,
	0x2002, 0x2003, 0x2012, 0x2013, 0x2102, 0x2103, 0x2112, 0x2113, 0x3002, 0x3003, 0x3012, 0x3013, 0x3102, 0x3103, 0x3112, 0x3113,
	0x2020, 0x2021, 0x2030, 0x2031, 0x2120, 0x2121, 0x2130, 0x2131, 0x3020, 0x3021, 0x3030, 0x3031, 0x3120, 0x3121, 0x3130, 0x3131,
	0x2022, 0x2023, 0x2032, 0x2033, 0x2122, 0x2123, 0x2132, 0x2133, 0x3022, 0x3023, 0x3032, 0x3033, 0x3122, 0x3123, 0x3132, 0x3133,
	0x2200, 0x2201, 0x2210, 0x2211, 0x2300, 0x2301, 0x2310, 0x2311, 0x3200, 0x3201, 0x3210, 0x3211, 0x3300, 0x3301, 0x3310, 0x3311,
	0x2202, 0x2203, 0x2212, 0x2213, 0x2302, 0x2303, 0x2312, 0x2313, 0x3202, 0x3203, 0x3212, 0x3213, 0x3302, 0x3303, 0x3312, 0x3313,
	0x2220, 0x2221, 0x2230, 0x2231, 0x2320, 0x2321, 0x2330, 0x2331, 0x3220, 0x3221, 0x3230, 0x3231, 0x3320, 0x3321, 0x3330, 0x3331,
	0x2222, 0x2223, 0x2232, 0x2233, 0x2322, 0x2323, 0x2332, 0x2333, 0x3222, 0x3223, 0x3232, 0x3233, 0x3322, 0x3323, 0x3332, 0x3333
};

unsigned short SPN_encrypt(int key, unsigned short plainText)
{
	// get round keys
	unsigned short roundKey[Nr + 1] = { 0 };
	for (int i = 0; i < Nr + 1; i++)
	{
		roundKey[i] = (key >> (16 - 4 * i)) & 0xFFFF;
	}

	// encrypt
	for (int i = 0; i < Nr - 1; i++)
	{
		// xor round key
		plainText ^= roundKey[i];

		// substitution and permutation
		plainText = SubAndPerTable_En_1[(plainText >> 8) & 0xFF] | SubAndPerTable_En_2[plainText & 0xFF];
	}

	// xor round key
	plainText ^= roundKey[Nr - 1];

	// substitution
	plainText = (substitution[(plainText >> 12) & 0xF] << 12) |
		(substitution[(plainText >> 8) & 0xF] << 8) |
		(substitution[(plainText >> 4) & 0xF] << 4) |
		(substitution[(plainText) & 0xF]);

	// xor round key
	unsigned short cipherText = plainText ^ roundKey[Nr];

	return cipherText;
}

unsigned short SPN_decrypt(int key, unsigned short cipherText)
{
	// get round keys
	unsigned short roundKey[Nr + 1] = { 0 };
	for (int i = 0; i < Nr + 1; i++)
	{
		roundKey[i] = (key >> (16 - 4 * i)) & 0xFFFF;
	}

	// xor round key
	cipherText ^= roundKey[Nr];

	// decrypt
	for (int i = Nr; i > 1; i--)
	{
		// substitution
		cipherText = SubAndPerTable_De_1[(cipherText >> 8) & 0xFF] | SubAndPerTable_De_2[cipherText & 0xFF];

		// xor round key
		cipherText ^= PermutationTable_1[(roundKey[i - 1] >> 8) & 0xFF] | PermutationTable_2[roundKey[i - 1] & 0xFF];
	}

	// substitution
	cipherText = (substitutionInv[(cipherText >> 12) & 0xF] << 12) |
		(substitutionInv[(cipherText >> 8) & 0xF] << 8) |
		(substitutionInv[(cipherText >> 4) & 0xF] << 4) |
		(substitutionInv[(cipherText) & 0xF]);

	// xor round key
	cipherText ^= roundKey[0];

	unsigned short plainText = cipherText;

	return plainText;
}

inline int quickFreadIntHex()
{
	int s = 0;
	char ch = quickFread();
	while (ch != ' ' && ch != '\n') {
		if (ch <= '9')
		{ // ch >= '0' &&
			s = (s << 4) + ch - '0';
		}
		else
		{
			s = (s << 4) + ch - 'a' + 10;
		}
		ch = quickFread();
	}
	return s;
}

inline void quickFwriteShortHex(unsigned short hx)
{
	char temp[4];
	char index = 0;
	do {
		temp[index++] = hx & 0xF;
		hx >>= 4;
		hx &= 0x0FFF;
	} while (index < 4);
	while (index) {
		if (pOut - outBuf == outBufSize) {
			fwrite(outBuf, 1, outBufSize, stdout);
			pOut = outBuf;
		}
		if (temp[--index] <= 9) {
			*pOut++ = '0' + temp[index];
		}
		else {
			*pOut++ = 'a' + temp[index] - 10;
		}
	}

}

int main()
{
#ifdef _DEBUG
	freopen("..\\Examples\\第一题样例\\1.in", "r", stdin);
#endif // _DEBUG

	int n = 0;
	scanf("%d", &n);
	getchar();

	int key = 0;
	unsigned short plainText = 0;
	for (int i = 0; i < n; i++)
	{
		key = quickFreadIntHex();
		plainText = quickFreadIntHex() & 0xFFFF;

		unsigned short cipherText = SPN_encrypt(key, plainText);
		quickFwriteShortHex(cipherText);
		if (pOut - outBuf == outBufSize) {
			fwrite(outBuf, 1, outBufSize, stdout);
			pOut = outBuf;
		}
		*pOut++ = ' ';
		quickFwriteShortHex(SPN_decrypt(key, cipherText ^ 0x1));
		if (pOut - outBuf == outBufSize) {
			fwrite(outBuf, 1, outBufSize, stdout);
			pOut = outBuf;
		}
		*pOut++ = '\n';

	}
	fwrite(outBuf, 1, pOut - outBuf, stdout);

	return 0;
}
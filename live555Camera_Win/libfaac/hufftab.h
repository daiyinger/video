/*
 * FAAC - Freeware Advanced Audio Coder
 * Copyright (C) 2001 Menno Bakker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: hufftab.h,v 1.3 2001/06/08 18:01:09 menno Exp $
 */

unsigned short huff1[][2] = {
        { 11,  2040},
        { 9,  497},{ 11,  2045},{ 10,  1013},{ 7,  104},{ 10,  1008},
        { 11,  2039},{ 9,  492},{ 11,  2037},{ 10,  1009},{ 7,  114},
        { 10,  1012},{ 7,  116},{ 5,  17},{ 7,  118},{ 9,  491},
        { 7,  108},{ 10,  1014},{ 11,  2044},{ 9,  481},{ 11,  2033},
        { 9,  496},{ 7,  97},{ 9,  502},{ 11,  2034},{ 9,  490},
        { 11,  2043},{ 9,  498},{ 7,  105},{ 9,  493},{ 7,  119},
        { 5,  23},{ 7,  111},{ 9,  486},{ 7,  100},{ 9,  485},
        { 7,  103},{ 5,  21},{ 7,  98},{ 5,  18},{ 1,  0},
        { 5,  20},{ 7,  101},{ 5,  22},{ 7,  109},{ 9,  489},
        { 7,  99},{ 9,  484},{ 7,  107},{ 5,  19},{ 7,  113},
        { 9,  483},{ 7,  112},{ 9,  499},{ 11,  2046},{ 9,  487},
        { 11,  2035},{ 9,  495},{ 7,  96},{ 9,  494},{ 11,  2032},
        { 9,  482},{ 11,  2042},{ 10,  1011},{ 7,  106},{ 9,  488},
        { 7,  117},{ 5,  16},{ 7,  115},{ 9,  500},{ 7,  110},
        { 10,  1015},{ 11,  2038},{ 9,  480},{ 11,  2041},{ 10,  1010},
        { 7,  102},{ 9,  501},{ 11,  2047},{ 9,  503},{ 11,  2036}
    };
unsigned short huff2[][2] = {
    { 9,  499},
        { 7,  111},{ 9,  509},{ 8,  235},{ 6,  35},{ 8,  234},
        { 9,  503},{ 8,  232},{ 9,  506},{ 8,  242},{ 6,  45},
        { 7,  112},{ 6,  32},{ 5,  6},{ 6,  43},{ 7,  110},
        { 6,  40},{ 8,  233},{ 9,  505},{ 7,  102},{ 8,  248},
        { 8,  231},{ 6,  27},{ 8,  241},{ 9,  500},{ 7,  107},
        { 9,  501},{ 8,  236},{ 6,  42},{ 7,  108},{ 6,  44},
        { 5,  10},{ 6,  39},{ 7,  103},{ 6,  26},{ 8,  245},
        { 6,  36},{ 5,  8},{ 6,  31},{ 5,  9},{ 3,  0},
        { 5,  7},{ 6,  29},{ 5,  11},{ 6,  48},{ 8,  239},
        { 6,  28},{ 7,  100},{ 6,  30},{ 5,  12},{ 6,  41},
        { 8,  243},{ 6,  47},{ 8,  240},{ 9,  508},{ 7,  113},
        { 9,  498},{ 8,  244},{ 6,  33},{ 8,  230},{ 8,  247},
        { 7,  104},{ 9,  504},{ 8,  238},{ 6,  34},{ 7,  101},
        { 6,  49},{ 4,  2},{ 6,  38},{ 8,  237},{ 6,  37},
        { 7,  106},{ 9,  507},{ 7,  114},{ 9,  510},{ 7,  105},
        { 6,  46},{ 8,  246},{ 9,  511},{ 7,  109},{ 9,  502}
    };
unsigned short huff3[][2] = {
        { 1,  0},
        { 4,  9},{ 8,  239},{ 4,  11},{ 5,  25},{ 8,  240},
        { 9,  491},{ 9,  486},{ 10,  1010},{ 4,  10},{ 6,  53},
        { 9,  495},{ 6,  52},{ 6,  55},{ 9,  489},{ 9,  493},
        { 9,  487},{ 10,  1011},{ 9,  494},{ 10,  1005},{ 13,  8186},
        { 9,  492},{ 9,  498},{ 11,  2041},{ 11,  2040},{ 10,  1016},
        { 12,  4088},{ 4,  8},{ 6,  56},{ 10,  1014},{ 6,  54},
        { 7,  117},{ 10,  1009},{ 10,  1003},{ 10,  1004},{ 12,  4084},
        { 5,  24},{ 7,  118},{ 11,  2036},{ 6,  57},{ 7,  116},
        { 10,  1007},{ 9,  499},{ 9,  500},{ 11,  2038},{ 9,  488},
        { 10,  1002},{ 13,  8188},{ 8,  242},{ 9,  497},{ 12,  4091},
        { 10,  1013},{ 11,  2035},{ 12,  4092},{ 8,  238},{ 10,  1015},
        { 15,  32766},{ 9,  496},{ 11,  2037},{ 15,  32765},{ 13,  8187},
        { 14,  16378},{ 16,  65535},{ 8,  241},{ 10,  1008},{ 14,  16380},
        { 9,  490},{ 10,  1006},{ 14,  16379},{ 12,  4086},{ 12,  4090},
        { 15,  32764},{ 11,  2034},{ 12,  4085},{ 16,  65534},{ 10,  1012},
        { 11,  2039},{ 15,  32763},{ 12,  4087},{ 12,  4089},{ 15,  32762}
    };
unsigned short huff4[][2] = {
        { 4,  7},
        { 5,  22},{ 8,  246},{ 5,  24},{ 4,  8},{ 8,  239},
        { 9,  495},{ 8,  243},{ 11,  2040},{ 5,  25},{ 5,  23},
        { 8,  237},{ 5,  21},{ 4,  1},{ 8,  226},{ 8,  240},
        { 7,  112},{ 10,  1008},{ 9,  494},{ 8,  241},{ 11,  2042},
        { 8,  238},{ 8,  228},{ 10,  1010},{ 11,  2038},{ 10,  1007},
        { 11,  2045},{ 4,  5},{ 5,  20},{ 8,  242},{ 4,  9},
        { 4,  4},{ 8,  229},{ 8,  244},{ 8,  232},{ 10,  1012},
        { 4,  6},{ 4,  2},{ 8,  231},{ 4,  3},{ 4,  0},
        { 7,  107},{ 8,  227},{ 7,  105},{ 9,  499},{ 8,  235},
        { 8,  230},{ 10,  1014},{ 7,  110},{ 7,  106},{ 9,  500},
        { 10,  1004},{ 9,  496},{ 10,  1017},{ 8,  245},{ 8,  236},
        { 11,  2043},{ 8,  234},{ 7,  111},{ 10,  1015},{ 11,  2041},
        { 10,  1011},{ 12,  4095},{ 8,  233},{ 7,  109},{ 10,  1016},
        { 7,  108},{ 7,  104},{ 9,  501},{ 10,  1006},{ 9,  498},
        { 11,  2036},{ 11,  2039},{ 10,  1009},{ 12,  4094},{ 10,  1005},
        { 9,  497},{ 11,  2037},{ 11,  2046},{ 10,  1013},{ 11,  2044}
    };
unsigned short huff5[][2] = {
        { 13,  8191},
        { 12,  4087},{ 11,  2036},{ 11,  2024},{ 10,  1009},{ 11,  2030},
        { 11,  2041},{ 12,  4088},{ 13,  8189},{ 12,  4093},{ 11,  2033},
        { 10,  1000},{ 9,  488},{ 8,  240},{ 9,  492},{ 10,  1006},
        { 11,  2034},{ 12,  4090},{ 12,  4084},{ 10,  1007},{ 9,  498},
        { 8,  232},{ 7,  112},{ 8,  236},{ 9,  496},{ 10,  1002},
        { 11,  2035},{ 11,  2027},{ 9,  491},{ 8,  234},{ 5,  26},
        { 4,  8},{ 5,  25},{ 8,  238},{ 9,  495},{ 11,  2029},
        { 10,  1008},{ 8,  242},{ 7,  115},{ 4,  11},{ 1,  0},
        { 4,  10},{ 7,  113},{ 8,  243},{ 11,  2025},{ 11,  2031},
        { 9,  494},{ 8,  239},{ 5,  24},{ 4,  9},{ 5,  27},
        { 8,  235},{ 9,  489},{ 11,  2028},{ 11,  2038},{ 10,  1003},
        { 9,  499},{ 8,  237},{ 7,  114},{ 8,  233},{ 9,  497},
        { 10,  1005},{ 11,  2039},{ 12,  4086},{ 11,  2032},{ 10,  1001},
        { 9,  493},{ 8,  241},{ 9,  490},{ 10,  1004},{ 11,  2040},
        { 12,  4089},{ 13,  8188},{ 12,  4092},{ 12,  4085},{ 11,  2026},
        { 10,  1011},{ 10,  1010},{ 11,  2037},{ 12,  4091},{ 13,  8190}
    };
unsigned short huff6[][2] = {
        { 11,  2046},
        { 10,  1021},{ 9,  497},{ 9,  491},{ 9,  500},{ 9,  490},
        { 9,  496},{ 10,  1020},{ 11,  2045},{ 10,  1014},{ 9,  485},
        { 8,  234},{ 7,  108},{ 7,  113},{ 7,  104},{ 8,  240},
        { 9,  486},{ 10,  1015},{ 9,  499},{ 8,  239},{ 6,  50},
        { 6,  39},{ 6,  40},{ 6,  38},{ 6,  49},{ 8,  235},
        { 9,  503},{ 9,  488},{ 7,  111},{ 6,  46},{ 4,  8},
        { 4,  4},{ 4,  6},{ 6,  41},{ 7,  107},{ 9,  494},
        { 9,  495},{ 7,  114},{ 6,  45},{ 4,  2},{ 4,  0},
        { 4,  3},{ 6,  47},{ 7,  115},{ 9,  506},{ 9,  487},
        { 7,  110},{ 6,  43},{ 4,  7},{ 4,  1},{ 4,  5},
        { 6,  44},{ 7,  109},{ 9,  492},{ 9,  505},{ 8,  238},
        { 6,  48},{ 6,  36},{ 6,  42},{ 6,  37},{ 6,  51},
        { 8,  236},{ 9,  498},{ 10,  1016},{ 9,  484},{ 8,  237},
        { 7,  106},{ 7,  112},{ 7,  105},{ 7,  116},{ 8,  241},
        { 10,  1018},{ 11,  2047},{ 10,  1017},{ 9,  502},{ 9,  493},
        { 9,  504},{ 9,  489},{ 9,  501},{ 10,  1019},{ 11,  2044}
    };
unsigned short huff7[][2] = {
        { 1,  0},
        { 3,  5},{ 6,  55},{ 7,  116},{ 8,  242},{ 9,  491},
        { 10,  1005},{ 11,  2039},{ 3,  4},{ 4,  12},{ 6,  53},
        { 7,  113},{ 8,  236},{ 8,  238},{ 9,  494},{ 9,  501},
        { 6,  54},{ 6,  52},{ 7,  114},{ 8,  234},{ 8,  241},
        { 9,  489},{ 9,  499},{ 10,  1013},{ 7,  115},{ 7,  112},
        { 8,  235},{ 8,  240},{ 9,  497},{ 9,  496},{ 10,  1004},
        { 10,  1018},{ 8,  243},{ 8,  237},{ 9,  488},{ 9,  495},
        { 10,  1007},{ 10,  1009},{ 10,  1017},{ 11,  2043},{ 9,  493},
        { 8,  239},{ 9,  490},{ 9,  498},{ 10,  1011},{ 10,  1016},
        { 11,  2041},{ 11,  2044},{ 10,  1006},{ 9,  492},{ 9,  500},
        { 10,  1012},{ 10,  1015},{ 11,  2040},{ 12,  4093},{ 12,  4094},
        { 11,  2038},{ 10,  1008},{ 10,  1010},{ 10,  1014},{ 11,  2042},
        { 11,  2045},{ 12,  4092},{ 12,  4095}
    };
unsigned short huff8[][2] = {
        { 5,  14},
        { 4,  5},{ 5,  16},{ 6,  48},{ 7,  111},{ 8,  241},
        { 9,  506},{ 10,  1022},{ 4,  3},{ 3,  0},{ 4,  4},
        { 5,  18},{ 6,  44},{ 7,  106},{ 7,  117},{ 8,  248},
        { 5,  15},{ 4,  2},{ 4,  6},{ 5,  20},{ 6,  46},
        { 7,  105},{ 7,  114},{ 8,  245},{ 6,  47},{ 5,  17},
        { 5,  19},{ 6,  42},{ 6,  50},{ 7,  108},{ 8,  236},
        { 8,  250},{ 7,  113},{ 6,  43},{ 6,  45},{ 6,  49},
        { 7,  109},{ 7,  112},{ 8,  242},{ 9,  505},{ 8,  239},
        { 7,  104},{ 6,  51},{ 7,  107},{ 7,  110},{ 8,  238},
        { 8,  249},{ 10,  1020},{ 9,  504},{ 7,  116},{ 7,  115},
        { 8,  237},{ 8,  240},{ 8,  246},{ 9,  502},{ 9,  509},
        { 10,  1021},{ 8,  243},{ 8,  244},{ 8,  247},{ 9,  503},
        { 9,  507},{ 9,  508},{ 10,  1023}
    };
unsigned short huff9[][2] = {
        { 1,  0},
        { 3,  5},{ 6,  55},{ 8,  231},{ 9,  478},{ 10,  974},
        { 10,  985},{ 11,  1992},{ 11,  1997},{ 12,  4040},{ 12,  4061},
        { 13,  8164},{ 13,  8172},{ 3,  4},{ 4,  12},{ 6,  53},
        { 7,  114},{ 8,  234},{ 8,  237},{ 9,  482},{ 10,  977},
        { 10,  979},{ 10,  992},{ 11,  2008},{ 12,  4047},{ 12,  4053},
        { 6,  54},{ 6,  52},{ 7,  113},{ 8,  232},{ 8,  236},
        { 9,  481},{ 10,  975},{ 10,  989},{ 10,  987},{ 11,  2000},
        { 12,  4039},{ 12,  4052},{ 12,  4068},{ 8,  230},{ 7,  112},
        { 8,  233},{ 9,  477},{ 9,  483},{ 10,  978},{ 10,  988},
        { 11,  1996},{ 11,  1994},{ 11,  2014},{ 12,  4056},{ 12,  4074},
        { 13,  8155},{ 9,  479},{ 8,  235},{ 9,  476},{ 9,  486},
        { 10,  981},{ 10,  990},{ 11,  1995},{ 11,  2013},{ 11,  2012},
        { 12,  4045},{ 12,  4066},{ 12,  4071},{ 13,  8161},{ 10,  976},
        { 9,  480},{ 9,  484},{ 10,  982},{ 11,  1989},{ 11,  2001},
        { 11,  2011},{ 12,  4050},{ 11,  2016},{ 12,  4057},{ 12,  4075},
        { 13,  8163},{ 13,  8169},{ 11,  1988},{ 9,  485},{ 10,  983},
        { 11,  1990},{ 11,  1999},{ 11,  2010},{ 12,  4043},{ 12,  4058},
        { 12,  4067},{ 12,  4073},{ 13,  8166},{ 13,  8179},{ 13,  8183},
        { 11,  2003},{ 10,  984},{ 10,  993},{ 11,  2004},{ 11,  2009},
        { 12,  4051},{ 12,  4062},{ 13,  8157},{ 13,  8153},{ 13,  8162},
        { 13,  8170},{ 13,  8177},{ 13,  8182},{ 11,  2002},{ 10,  980},
        { 10,  986},{ 11,  1991},{ 11,  2007},{ 11,  2018},{ 12,  4046},
        { 12,  4059},{ 13,  8152},{ 13,  8174},{ 14,  16368},{ 13,  8180},
        { 14,  16370},{ 11,  2017},{ 10,  991},{ 11,  1993},{ 11,  2006},
        { 12,  4042},{ 12,  4048},{ 12,  4069},{ 12,  4070},{ 13,  8171},
        { 13,  8175},{ 14,  16371},{ 14,  16372},{ 14,  16373},{ 12,  4064},
        { 11,  1998},{ 11,  2005},{ 12,  4038},{ 12,  4049},{ 12,  4065},
        { 13,  8160},{ 13,  8168},{ 13,  8176},{ 14,  16369},{ 14,  16376},
        { 14,  16374},{ 15,  32764},{ 12,  4072},{ 11,  2015},{ 12,  4041},
        { 12,  4055},{ 12,  4060},{ 13,  8156},{ 13,  8159},{ 13,  8173},
        { 13,  8181},{ 14,  16377},{ 14,  16379},{ 15,  32765},{ 15,  32766},
        { 13,  8167},{ 12,  4044},{ 12,  4054},{ 12,  4063},{ 13,  8158},
        { 13,  8154},{ 13,  8165},{ 13,  8178},{ 14,  16378},{ 14,  16375},
        { 14,  16380},{ 14,  16381},{ 15,  32767}
    };
unsigned short huff10[][2] = {
        { 6,  34},
        { 5,  8},{ 6,  29},{ 6,  38},{ 7,  95},{ 8,  211},
        { 9,  463},{ 10,  976},{ 10,  983},{ 10,  1005},{ 11,  2032},
        { 11,  2038},{ 12,  4093},{ 5,  7},{ 4,  0},{ 4,  1},
        { 5,  9},{ 6,  32},{ 7,  84},{ 7,  96},{ 8,  213},
        { 8,  220},{ 9,  468},{ 10,  973},{ 10,  990},{ 11,  2023},
        { 6,  28},{ 4,  2},{ 5,  6},{ 5,  12},{ 6,  30},
        { 6,  40},{ 7,  91},{ 8,  205},{ 8,  217},{ 9,  462},
        { 9,  476},{ 10,  985},{ 10,  1009},{ 6,  37},{ 5,  11},
        { 5,  10},{ 5,  13},{ 6,  36},{ 7,  87},{ 7,  97},
        { 8,  204},{ 8,  221},{ 9,  460},{ 9,  478},{ 10,  979},
        { 10,  999},{ 7,  93},{ 6,  33},{ 6,  31},{ 6,  35},
        { 6,  39},{ 7,  89},{ 7,  100},{ 8,  216},{ 8,  223},
        { 9,  466},{ 9,  482},{ 10,  989},{ 10,  1006},{ 8,  209},
        { 7,  85},{ 6,  41},{ 7,  86},{ 7,  88},{ 7,  98},
        { 8,  206},{ 8,  224},{ 8,  226},{ 9,  474},{ 10,  980},
        { 10,  995},{ 11,  2027},{ 9,  457},{ 7,  94},{ 7,  90},
        { 7,  92},{ 7,  99},{ 8,  202},{ 8,  218},{ 9,  455},
        { 9,  458},{ 9,  480},{ 10,  987},{ 10,  1000},{ 11,  2028},
        { 9,  483},{ 8,  210},{ 8,  203},{ 8,  208},{ 8,  215},
        { 8,  219},{ 9,  454},{ 9,  469},{ 9,  472},{ 10,  970},
        { 10,  986},{ 11,  2026},{ 11,  2033},{ 9,  481},{ 8,  212},
        { 8,  207},{ 8,  214},{ 8,  222},{ 8,  225},{ 9,  464},
        { 9,  470},{ 10,  977},{ 10,  981},{ 10,  1010},{ 11,  2030},
        { 11,  2043},{ 10,  1001},{ 9,  461},{ 9,  456},{ 9,  459},
        { 9,  465},{ 9,  471},{ 9,  479},{ 10,  975},{ 10,  992},
        { 10,  1007},{ 11,  2022},{ 11,  2040},{ 12,  4090},{ 10,  1003},
        { 9,  477},{ 9,  467},{ 9,  473},{ 9,  475},{ 10,  978},
        { 10,  972},{ 10,  988},{ 10,  1002},{ 11,  2029},{ 11,  2035},
        { 11,  2041},{ 12,  4089},{ 11,  2034},{ 10,  974},{ 9,  484},
        { 10,  971},{ 10,  984},{ 10,  982},{ 10,  994},{ 10,  997},
        { 11,  2024},{ 11,  2036},{ 11,  2037},{ 11,  2039},{ 12,  4091},
        { 11,  2042},{ 10,  1004},{ 10,  991},{ 10,  993},{ 10,  996},
        { 10,  998},{ 10,  1008},{ 11,  2025},{ 11,  2031},{ 12,  4088},
        { 12,  4094},{ 12,  4092},{ 12,  4095}
    };
unsigned short huff11[][2] = {
        { 4,  0},
        { 5,  6},{ 6,  25},{ 7,  61},{ 8,  156},{ 8,  198},
        { 9,  423},{ 10,  912},{ 10,  962},{ 10,  991},{ 11,  2022},
        { 11,  2035},{ 12,  4091},{ 11,  2028},{ 12,  4090},{ 12,  4094},
        { 10,  910},{ 5,  5},{ 4,  1},{ 5,  8},{ 6,  20},
        { 7,  55},{ 7,  66},{ 8,  146},{ 8,  175},{ 9,  401},
        { 9,  421},{ 9,  437},{ 10,  926},{ 10,  960},{ 10,  930},
        { 10,  973},{ 11,  2006},{ 8,  174},{ 6,  23},{ 5,  7},
        { 5,  9},{ 6,  24},{ 7,  57},{ 7,  64},{ 8,  142},
        { 8,  163},{ 8,  184},{ 9,  409},{ 9,  428},{ 9,  449},
        { 10,  945},{ 10,  918},{ 10,  958},{ 10,  970},{ 8,  157},
        { 7,  60},{ 6,  21},{ 6,  22},{ 6,  26},{ 7,  59},
        { 7,  68},{ 8,  145},{ 8,  165},{ 8,  190},{ 9,  406},
        { 9,  430},{ 9,  441},{ 10,  929},{ 10,  913},{ 10,  933},
        { 10,  981},{ 8,  148},{ 8,  154},{ 7,  54},{ 7,  56},
        { 7,  58},{ 7,  65},{ 8,  140},{ 8,  155},{ 8,  176},
        { 8,  195},{ 9,  414},{ 9,  427},{ 9,  444},{ 10,  927},
        { 10,  911},{ 10,  937},{ 10,  975},{ 8,  147},{ 8,  191},
        { 7,  62},{ 7,  63},{ 7,  67},{ 7,  69},{ 8,  158},
        { 8,  167},{ 8,  185},{ 9,  404},{ 9,  418},{ 9,  442},
        { 9,  451},{ 10,  934},{ 10,  935},{ 10,  955},{ 10,  980},
        { 8,  159},{ 9,  416},{ 8,  143},{ 8,  141},{ 8,  144},
        { 8,  152},{ 8,  166},{ 8,  182},{ 8,  196},{ 9,  415},
        { 9,  431},{ 9,  447},{ 10,  921},{ 10,  959},{ 10,  948},
        { 10,  969},{ 10,  999},{ 8,  168},{ 9,  438},{ 8,  171},
        { 8,  164},{ 8,  170},{ 8,  178},{ 8,  194},{ 8,  197},
        { 9,  408},{ 9,  420},{ 9,  440},{ 10,  908},{ 10,  932},
        { 10,  964},{ 10,  966},{ 10,  989},{ 10,  1000},{ 8,  173},
        { 10,  943},{ 9,  402},{ 8,  189},{ 8,  188},{ 9,  398},
        { 9,  407},{ 9,  410},{ 9,  419},{ 9,  433},{ 10,  909},
        { 10,  920},{ 10,  951},{ 10,  979},{ 10,  977},{ 10,  987},
        { 11,  2013},{ 8,  180},{ 10,  990},{ 9,  425},{ 9,  411},
        { 9,  412},{ 9,  417},{ 9,  426},{ 9,  429},{ 9,  435},
        { 10,  907},{ 10,  946},{ 10,  952},{ 10,  974},{ 10,  993},
        { 10,  992},{ 11,  2002},{ 11,  2021},{ 8,  183},{ 11,  2019},
        { 9,  443},{ 9,  424},{ 9,  422},{ 9,  432},{ 9,  434},
        { 9,  439},{ 10,  923},{ 10,  922},{ 10,  954},{ 10,  949},
        { 10,  982},{ 11,  2007},{ 10,  996},{ 11,  2008},{ 11,  2026},
        { 8,  186},{ 11,  2024},{ 10,  928},{ 9,  445},{ 9,  436},
        { 10,  906},{ 9,  452},{ 10,  914},{ 10,  938},{ 10,  944},
        { 10,  956},{ 10,  983},{ 11,  2004},{ 11,  2012},{ 11,  2011},
        { 11,  2005},{ 11,  2032},{ 8,  193},{ 11,  2043},{ 10,  968},
        { 10,  931},{ 10,  917},{ 10,  925},{ 10,  940},{ 10,  942},
        { 10,  965},{ 10,  984},{ 10,  994},{ 10,  998},{ 11,  2020},
        { 11,  2023},{ 11,  2016},{ 11,  2025},{ 11,  2039},{ 9,  400},
        { 11,  2034},{ 10,  915},{ 9,  446},{ 9,  448},{ 10,  916},
        { 10,  919},{ 10,  941},{ 10,  963},{ 10,  961},{ 10,  978},
        { 11,  2010},{ 11,  2009},{ 11,  2015},{ 11,  2027},{ 11,  2036},
        { 11,  2042},{ 9,  405},{ 11,  2040},{ 10,  957},{ 10,  924},
        { 10,  939},{ 10,  936},{ 10,  947},{ 10,  953},{ 10,  976},
        { 10,  995},{ 10,  997},{ 11,  2018},{ 11,  2014},{ 11,  2029},
        { 11,  2033},{ 11,  2041},{ 11,  2044},{ 9,  403},{ 12,  4093},
        { 10,  988},{ 10,  950},{ 10,  967},{ 10,  972},{ 10,  971},
        { 10,  985},{ 10,  986},{ 11,  2003},{ 11,  2017},{ 11,  2030},
        { 11,  2031},{ 11,  2037},{ 11,  2038},{ 12,  4092},{ 12,  4095},
        { 9,  413},{ 9,  450},{ 8,  181},{ 8,  161},{ 8,  150},
        { 8,  151},{ 8,  149},{ 8,  153},{ 8,  160},{ 8,  162},
        { 8,  172},{ 8,  169},{ 8,  177},{ 8,  179},{ 8,  187},
        { 8,  192},{ 9,  399},{ 5,  4}
    };
unsigned int huff12[][2] = {
        { 18,  262120},
        { 18,  262118},{ 18,  262119},{ 18,  262117},{ 19,  524277},{ 19,  524273},
        { 19,  524269},{ 19,  524278},{ 19,  524270},{ 19,  524271},{ 19,  524272},
        { 19,  524284},{ 19,  524285},{ 19,  524287},{ 19,  524286},{ 19,  524279},
        { 19,  524280},{ 19,  524283},{ 19,  524281},{ 18,  262116},{ 19,  524282},
        { 18,  262115},{ 17,  131055},{ 17,  131056},{ 16,  65525},{ 17,  131054},
        { 16,  65522},{ 16,  65523},{ 16,  65524},{ 16,  65521},{ 15,  32758},
        { 15,  32759},{ 14,  16377},{ 14,  16373},{ 14,  16375},{ 14,  16371},
        { 14,  16374},{ 14,  16370},{ 13,  8183},{ 13,  8181},{ 12,  4089},
        { 12,  4087},{ 12,  4086},{ 11,  2041},{ 12,  4084},{ 11,  2040},
        { 10,  1017},{ 10,  1015},{ 10,  1013},{ 9,  504},{ 9,  503},
        { 8,  250},{ 8,  248},{ 8,  246},{ 7,  121},{ 6,  58},
        { 6,  56},{ 5,  26},{ 4,  11},{ 3,  4},{ 1,  0},
        { 4,  10},{ 4,  12},{ 5,  27},{ 6,  57},{ 6,  59},
        { 7,  120},{ 7,  122},{ 8,  247},{ 8,  249},{ 9,  502},
        { 9,  505},{ 10,  1012},{ 10,  1014},{ 10,  1016},{ 11,  2037},
        { 11,  2036},{ 11,  2038},{ 11,  2039},{ 12,  4085},{ 12,  4088},
        { 13,  8180},{ 13,  8182},{ 13,  8184},{ 14,  16376},{ 14,  16372},
        { 16,  65520},{ 15,  32756},{ 16,  65526},{ 15,  32757},{ 18,  262114},
        { 19,  524249},{ 19,  524250},{ 19,  524251},{ 19,  524252},{ 19,  524253},
        { 19,  524254},{ 19,  524248},{ 19,  524242},{ 19,  524243},{ 19,  524244},
        { 19,  524245},{ 19,  524246},{ 19,  524274},{ 19,  524255},{ 19,  524263},
        { 19,  524264},{ 19,  524265},{ 19,  524266},{ 19,  524267},{ 19,  524262},
        { 19,  524256},{ 19,  524257},{ 19,  524258},{ 19,  524259},{ 19,  524260},
        { 19,  524261},{ 19,  524247},{ 19,  524268},{ 19,  524276},{ 19,  524275}
    };



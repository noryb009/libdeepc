#pragma once

#define TMAGIC "ustar"
#define TMAGLEN 6
#define TVERSION "00"
#define TVERSLEN 2

#define REGTYPE '0'
#define AREGTYPE '\0'
#define LNKTYPE '1'
#define SYMTYPE '2'
#define CHRTYPE '3'
#define BLKTYPE '4'
#define DIRTYPE '5'
#define FIFOTYPE '6'
#define CONTTYPE '7'

#define TOEXEC  00001
#define TOWRITE 00002
#define TOREAD  00004
#define TGEXEC  00010
#define TGWRITE 00020
#define TGREAD  00040
#define TUEXEC  00100
#define TUWRITE 00200
#define TUREAD  00400
#define TSVTX   01000
#define TSGID   02000
#define TSUID   04000

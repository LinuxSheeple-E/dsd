#ifndef DMR_H
#define DMR_H
/*
 * Copyright (C) 2014 Eric A. Cottrell <eric.c.boston@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
// DMR Cach Processing
void processCach (dsd_opts * opts, dsd_state * state, char cachdata[13]);
// DMR SLCO
void processSlco(short opcode, long data);
char *getSlcoString();
// DMR EMB Processing
void processEmb (dsd_opts * opts, dsd_state * state, char syncdata[25]);
// DMR FLCO
void processFlco( char pf, char flco[7], char fid[9], char payload[97]);
char *getFlcoString();
// DMR CSBK
void processCsbk( char lb, char pf, char csbk[7], char fid[9], char payload[97]);
char *getCsbkString();
// DMR Info field
int processBPTC(dsd_opts * opts, dsd_state * state, char infodata[197], char payload[97]);
// DMR Error Correction
long doGolay208(char bitarray[20]);
int doHamming743(char bitarray[7]);
int doHamming1393(char bitarray[13]);
int doHamming15113(char bitarray[15]);
int doHamming16114(char bitarray[16]);
long doHamming17123(char bitarray[17]);
int doQR1676(char bitarray[16]);
void initCRC8();
unsigned char getCRC8();
void doCRC8(char bit);
void initCRC16();
unsigned short getCRC16();
void doCRC16(char bit);
#endif // DMR_H
 

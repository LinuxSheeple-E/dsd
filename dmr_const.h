/*
 * Copyright (C) 2010 DSD Author
 * GPG Key ID: 0x3F1D7FD0 (74EF 430D F7F2 0A48 FCE6  F630 FAA2 635D 3F1D 7FD0)
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

#ifndef _MAIN
extern const int rdW[196];
extern const int rdX[196];

extern const int rW[36];
extern const int rX[36];
extern const int rY[36];
extern const int rZ[36];

#else
/*
 * DMR Data Interleave  15 x 13
 */

const int rdW[196] = {                                    0, // Put R3 in R2 spot
  0,  1,  2,  3,  4,  5,  6,  6,  7,  8,  9, 10, 11, 12, 12,
  0,  1,  2,  3,  4,  5,  5,  6,  7,  8,  9, 10, 11, 12, 12,
  0,  1,  2,  3,  4,  5,  5,  6,  7,  8,  9, 10, 11, 11, 12,
  0,  1,  2,  3,  4,  4,  5,  6,  7,  8,  9, 10, 11, 11, 12,
  0,  1,  2,  3,  4,  4,  5,  6,  7,  8,  9, 10, 10, 11, 12,
  0,  1,  2,  3,  3,  4,  5,  6,  7,  8,  9, 10, 10, 11, 12,
  0,  1,  2,  3,  3,  4,  5,  6,  7,  8,  9,  9, 10, 11, 12,
  0,  1,  2,  2,  3,  4,  5,  6,  7,  8,  9,  9, 10, 11, 12,
  0,  1,  2,  2,  3,  4,  5,  6,  7,  8,  8,  9, 10, 11, 12,
  0,  1,  1,  2,  3,  4,  5,  6,  7,  8,  8,  9, 10, 11, 12,
  0,  1,  1,  2,  3,  4,  5,  6,  7,  7,  8,  9, 10, 11, 12,
  0,  0,  1,  2,  3,  4,  5,  6,  7,  7,  8,  9, 10, 11, 12,
  0,  0,  1,  2,  3,  4,  5,  6,  6,  7,  8,  9, 10, 11, 12
};

const int rdX[196] = {                                    0, // Put R3 in R2 Spot
 12, 10,  8,  6,  4,  2,  0, 13, 11,  9,  7,  5,  3,  1, 14, // 1
 11,  9,  7,  5,  3,  1, 14, 12, 10,  8,  6,  4,  2,  0, 13, // 16
 10,  8,  6,  4,  2,  0, 13, 11,  9,  7,  5,  3,  1, 14, 12, // 31
  9,  7,  5,  3,  1, 14, 12, 10,  8,  6,  4,  2,  0, 13, 11, // 46
  8,  6,  4,  2,  0, 13, 11,  9,  7,  5,  3,  1, 14, 12, 10, // 61
  7,  5,  3,  1, 14, 12, 10,  8,  6,  4,  2,  0, 13, 11,  9, // 76
  6,  4,  2,  0, 13, 11,  9,  7,  5,  3,  1, 14, 12, 10,  8, // 91
  5,  3,  1, 14, 12, 10,  8,  6,  4,  2,  0, 13, 11,  9,  7, // 106
  4,  2,  0, 13, 11,  9,  7,  5,  3,  1, 14, 12, 10,  8,  6, // 121
  3,  1, 14, 12, 10,  8,  6,  4,  2,  0, 13, 11,  9,  7,  5, // 136
  2,  0, 13, 11,  9,  7,  5,  3,  1, 14, 12, 10,  8,  6,  4, // 151
  1, 14, 12, 10,  8,  6,  4,  2,  0, 13, 11,  9,  7,  5,  3, // 166
  0, 13, 11,  9,  7,  5,  3,  1, 14, 12, 10,  8,  6,  4,  2  // 181
};
/*
 * DMR AMBE interleave schedule
 */
const int rW[36] = {
  0, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 0, 2,
  0, 2, 0, 2, 0, 2,
  0, 2, 0, 2, 0, 2
};

const int rX[36] = {
  23, 10, 22, 9, 21, 8,
  20, 7, 19, 6, 18, 5,
  17, 4, 16, 3, 15, 2,
  14, 1, 13, 0, 12, 10,
  11, 9, 10, 8, 9, 7,
  8, 6, 7, 5, 6, 4
};

const int rY[36] = {
  0, 2, 0, 2, 0, 2,
  0, 2, 0, 3, 0, 3,
  1, 3, 1, 3, 1, 3,
  1, 3, 1, 3, 1, 3,
  1, 3, 1, 3, 1, 3,
  1, 3, 1, 3, 1, 3
};

const int rZ[36] = {
  5, 3, 4, 2, 3, 1,
  2, 0, 1, 13, 0, 12,
  22, 11, 21, 10, 20, 9,
  19, 8, 18, 7, 17, 6,
  16, 5, 15, 4, 14, 3,
  13, 2, 12, 1, 11, 0
};

#endif

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

#include "dsd.h"
#include "dmr_const.h"
#include "dmr.h"

static char cach_fr[4][17];
static int  cach_fr_index = 0;
static int  cach_fr_count = 4;
static int  cach_fr_valid = 0;

void processCach (dsd_opts * opts, dsd_state * state, char cachdata[13])
{
  int i, j, k, dibit, lcss;
  long l;
  char cach[25];
  char tact[7];
  char cach_sig[17];
  char cach_deinv_fr[4][17];
  j = 0;
  for (i = 0; i < 12; i++)
    {
      dibit = cachdata[i];
      cach[j] = (1 & (dibit >> 1));    // bit 1
      j++;
      cach[j] = (1 & dibit);   // bit 0
      j++;
    }
  cach[24] = 0;
  j = 0;
  k = 0;
  for(i = 0; i < 24; i++)
    {
      switch(i)
	{
	case 0:
        case 4:
        case 8:
	case 12:
	case 14:
	case 18:
	case 22:
	  tact[j++] = cach[i];
	  break;
	default:
	  cach_sig[k++] = cach[i];
	  break;
	}
    }
  if(doHamming743(tact))
    { // If error just assume next slot and indicate it
      state->currentslot = ++state->currentslot%2;
      if (state->currentslot == 0)
	{
	  state->slot0light[0] = '{';
          state->slot0light[6] = '}';
          state->slot1light[0] = ' ';
          state->slot1light[6] = ' ';
        }
      else
        {
          state->slot1light[0] = '{';
          state->slot1light[6] = '}';
          state->slot0light[0] = ' ';
          state->slot0light[6] = ' ';
        }
      return;
    }
  state->currentslot = tact[1];
  lcss = (tact[2] << 1) | tact[3];
  if (state->currentslot == 0)
    {
      state->slot0light[0] = '[';
      state->slot0light[6] = ']';
      state->slot1light[0] = ' ';
      state->slot1light[6] = ' ';
    }
  else
    {
      state->slot1light[0] = '[';
      state->slot1light[6] = ']';
      state->slot0light[0] = ' ';
      state->slot0light[6] = ' ';
    }
  switch(lcss)
    {
    case 0: //Single fragment LC (not defined for cach) or first fragment CSBK signalling
      cach_fr_valid = 0; // Reset Valid
      cach_fr_index = 0; 
      return;
      break;
    case 1: // First Fragment of LC signaling
      cach_fr_valid = 0; // Reset Valid
      cach_fr_index = 0; 
      break;
    case 2: // Last Fragment of LC or CSBK signaling
      if(++cach_fr_index != (cach_fr_count-1))
	return;
      break;
    case 3:
      if(++cach_fr_index >= cach_fr_count)
	return;
      break;
    }
  for(i = 0; i < 17; i++)
    {
      cach_fr[cach_fr_index][i] = cach_sig[i];		
    }
  cach_fr_valid |= (1 << cach_fr_index);
  if(cach_fr_valid == 15)
    {
      // Deinterleave
      for(i = 0, j = 0; i < 17; i++, j += 4)
	{
	  cach_deinv_fr[0][i] = cach_fr[j/17][j%17];
        }
      for(i = 0, j = 1; i < 17; i++, j += 4)
        {
	  cach_deinv_fr[1][i] = cach_fr[j/17][j%17];
        }
      for(i = 0, j = 2; i < 17; i++, j += 4)
        {
	  cach_deinv_fr[2][i] = cach_fr[j/17][j%17];
        }
      for(i = 0, j = 3; i < 17; i++, j += 4)
        {
	  cach_deinv_fr[3][i] = cach_fr[j/17][j%17];
        }
      for(i = 0; i < 3; i++) // just do simple check now
	{
	  if((k = doHamming17123(cach_deinv_fr[i])))
	    {
	      return;
	    }
	}
      initCRC8();
      for(i = 0; i < 3; i++)
	{
	  for(j = 0; j < 12; j++)
	    {
	      doCRC8(cach_deinv_fr[i][j]);
	    }
	}
      if(getCRC8())
	{
	  return;
	}
      j = 0;
      for(i = 0; i < 4; i++)
	{
	  j <<= 1;
          j |= cach_deinv_fr[0][i];
	}
      l = 0;
      for(i = 4; i < 12; i++)
	{
	  l <<= 1;
          l |= cach_deinv_fr[0][i];
	}
      for(i = 0; i < 12; i++)
	{
	  l <<= 1;
          l |= cach_deinv_fr[1][i];
	}
      for(i = 0; i < 4; i++)
	{
	  l <<= 1;
          l |= cach_deinv_fr[2][i];
	}
      processSlco((short)j, l);
      cach_fr_valid = 0;
    }
}


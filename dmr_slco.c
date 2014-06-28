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

static char slco_string[133];
static char slco_null_string[] = "";
static int  slco_str_valid = 0;

void processSlco(short opcode, long data)
{
  slco_str_valid = 0;
  switch(opcode)
    {
    case 0:  // Null SLCO
      sprintf(slco_string,"Null");
      break;
    case 1:
      sprintf(slco_string,"Activity Update - Slot 0: ");
      switch((data >> 20) & 0x0f)
	{
	case 0:
	  strcat(slco_string,"Idle            ");
	  break;
	case 2:
	  strcat(slco_string,"Group CSBK      ");
	  break;
	case 3:
	  strcat(slco_string,"Individual CSBK ");
	  break;
	case 8:
	  strcat(slco_string,"Group Voice     ");
	  break;
	case 9:
	  strcat(slco_string,"Individual Voice");
	  break;
	case 10:
	  strcat(slco_string,"Individual Data ");
	  break;
	case 11:
	  strcat(slco_string,"Group Data      ");
	  break;
	case 12:
	  strcat(slco_string,"GROUP EMERGENCY ");
	  break;
	case 13:
	  strcat(slco_string,"INDIVL EMERGENCY");
	    break;
	default:
	  strcat(slco_string,"Reserved        ");
	}
      strcat(slco_string,"  Slot 1: ");
      switch((data >> 16) & 0x0f)
	{
	case 0:
	  strcat(slco_string,"Idle            ");
	  break;
	case 2:
	  strcat(slco_string,"Group CSBK      ");
	  break;
	case 3:
	  strcat(slco_string,"Individual CSBK ");
	  break;
	case 8:
	  strcat(slco_string,"Group Voice     ");
	  break;
	case 9:
	  strcat(slco_string,"Individual Voice");
	  break;
	case 10:
	  strcat(slco_string,"Individual Data ");
	  break;
	case 11:
	  strcat(slco_string,"Group Data      ");
	  break;
	case 12:
	  strcat(slco_string,"GROUP EMERGENCY ");
	  break;
	case 13:
	  strcat(slco_string,"INDIVL EMERGENCY");
	    break;
	default:
	  strcat(slco_string,"Reserved        ");
	}
      break;
    case 2: // Trunking SLCO
      //sprintf(slco_string,"Tier 3 (Trunked) Sys_Params %06lX", data);
      switch((data >> 22) & 0x3)
        {
        case 0: // Tiny Model
            sprintf(slco_string,"Tier 3 (Trunked) Sys_Params TN:%ld TS:%ld %s SlotCount: %03ld", (data >> 13) & 0x1ff ,(data >> 10) & 0x7 ,(data & 0x200)?"Reg":"   ", data & 0x1ff);
          break;
        case 1: // Small Model
            sprintf(slco_string,"Tier 3 (Trunked) Sys_Params SN:%ld SS:%ld %s SlotCount: %03ld", (data >> 15) & 0x7f ,(data >> 10) & 0x1f ,(data & 0x200)?"Reg":"   ", data & 0x1ff);
          break;
        case 2: // Large Model
            sprintf(slco_string,"Tier 3 (Trunked) Sys_Params LN:%ld LS:%ld %s SlotCount: %03ld", (data >> 18) & 0xf ,(data >> 10) & 0xff ,(data & 0x200)?"Reg":"   ", data & 0x1ff);
          break;
        default: // Huge Model
            sprintf(slco_string,"Tier 3 (Trunked) Sys_Params HN:%ld HS:%ld %s SlotCount: %03ld", (data >> 20) & 0x3 ,(data >> 10) & 0x3ff ,(data & 0x200)?"Reg":"   ", data & 0x1ff);
          break;
        }
      break;
    case 9: // MotoTrbo Trunking SLCO
      sprintf(slco_string,"Motorola Connect+ Net:%ld Site:%ld Traffic Ch", (data >> 12) & 0xfff, (data >> 4) & 0xff);
      break;
    case 10: // MotoTrbo Trunking SLCO
      sprintf(slco_string,"Motorola Connect+ Net:%ld Site:%ld Control Ch", (data >> 12) & 0xfff, (data >> 4) & 0xff);
      break;
    case 15:
      if(((data >> 16) & 0xffff) == 0x10) // Motorola
	sprintf(slco_string,"Motorola Capacity+ Op:%ld RestCh:%ld Rsrvd:%ld", (data >> 14) & 0x03, (data >> 8) & 0x1f, data & 0xff);
      else
	sprintf(slco_string,"%d %06lX", opcode, data);
	break;
    default: 
      sprintf(slco_string,"%d %06lX", opcode, data);
      break;
    }
  slco_str_valid = 1;
}
char *getSlcoString()
{
  if(slco_str_valid)
    {
      slco_str_valid = 0;
      return slco_string;
    }
  else
    {
      return slco_null_string;
    }
}

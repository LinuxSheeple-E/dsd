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

static char slco_string[81];
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
      sprintf(slco_string,"Slot 0:");
      switch((data >> 20) & 0x0f)
	{
	case 0:
	  strcat(slco_string,"Idle");
	  break;
	case 2:
	  strcat(slco_string,"G CSBK");
	  break;
	case 3:
	  strcat(slco_string,"I CSBK");
	  break;
	case 8:
	  strcat(slco_string,"G Voice");
	  break;
	case 9:
	  strcat(slco_string,"I Voice");
	  break;
	case 10:
	  strcat(slco_string,"I Data");
	  break;
	case 11:
	  strcat(slco_string,"G Data");
	  break;
	case 12:
	  strcat(slco_string,"G Emerg");
	  break;
	case 13:
	  strcat(slco_string,"I Emerg");
	    break;
	default:
	  strcat(slco_string,"Reservd");
	}
      strcat(slco_string,"  1:");
      switch((data >> 16) & 0x0f)
	{
	case 0:
	  strcat(slco_string,"Idle");
	  break;
	case 2:
	  strcat(slco_string,"G CSBK");
	  break;
	case 3:
	  strcat(slco_string,"I CSBK");
	  break;
	case 8:
	  strcat(slco_string,"G Voice");
	  break;
	case 9:
	  strcat(slco_string,"I Voice");
	  break;
	case 10:
	  strcat(slco_string,"I Data");
	  break;
	case 11:
	  strcat(slco_string,"G Data");
	  break;
	case 12:
	  strcat(slco_string,"G Emerg");
	  break;
	case 13:
	  strcat(slco_string,"I Emerg");
	  break;
	default:
	  strcat(slco_string,"Reservd");
	}
      break;
    case 2: // Trunking SLCO
      sprintf(slco_string,"Sys_Params %06lX", data);
      break;
    case 9: // MotoTrbo Trunking SLCO
      sprintf(slco_string,"MtCon+ Tfc N:%ld S:%ld", (data >> 12) & 0xfff, (data >> 4) & 0xff);
      break;
    case 10: // MotoTrbo Trunking SLCO
      sprintf(slco_string,"MtCon+ Ctl N:%ld S:%ld", (data >> 12) & 0xfff, (data >> 4) & 0xff);
      break;
    case 15:
      if(((data >> 16) & 0xffff) == 0x10) // Motorola
	sprintf(slco_string,"MtCap+ Op:%ld RCh:%ld Rsrvd:%ld", (data >> 14) & 0x03, (data >> 8) & 0x1f, data & 0xff);
      else
	sprintf(slco_string,"SLCO:%d %06lX", opcode, data);
	break;
    default: 
      sprintf(slco_string,"SLCO:%d %06lX", opcode, data);
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

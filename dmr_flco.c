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

static char flco_string[133];
static char flco_null_string[] = "";
static int  flco_str_valid = 0;

void processFlco( char pf, char flco[7], char fid[9], char payload[97] )
{
  int i, j, k;
  long l;
  long long ll;
  char tmpStr[81];
  flco_str_valid = 0;
  if ((strcmp (flco, "000000") == 0) && (strcmp (fid, "00000000") == 0))
    {
		sprintf(flco_string, "pf:%c Group Voice Ch Usr ", pf);
		if(payload[0] == '1')
			strcat(flco_string, "EMERGENCY ");
		if(payload[1] == '1')
			strcat(flco_string, "Privacy ");
		// skip two reserved 
		if(payload[4] == '1')
			strcat(flco_string, "Broadcast ");
		if(payload[5] == '1')
			strcat(flco_string, "OVCM ");
        if(payload[6] == '1' && payload[7] == '1')
            strcat(flco_string, "Priority:3 ");
        else if(payload[6] == '1')
            strcat(flco_string, "Priority:2 ");
        else if(payload[7] == '1')
            strcat(flco_string, "Priority:1 ");
        else
            strcat(flco_string, "Priority:0 ");
		l = 0;
		for(i = 8; i < 32; i++)
	        {
		    l <<= 1;
		    l |= (payload[i] == '1')?1:0;
		}
		sprintf(tmpStr, "Group:%ld ", l);
		strcat(flco_string, tmpStr);
		l = 0;
		for(i = 32; i < 56; i++)
	        {
		    l <<= 1;
		    l |= (payload[i] == '1')?1:0;
		}
		sprintf(tmpStr, "RadioId:%ld ", l);
		strcat(flco_string, tmpStr);
    }
  else if ((strcmp (flco, "000011") == 0) && (strcmp (fid, "00000000") == 0))
	  {
		sprintf(flco_string, "pf:%c Unit-Unit Voice Ch Usr ", pf);
		if(payload[0] == '1')
			strcat(flco_string, "EMERGENCY ");
		if(payload[1] == '1')
			strcat(flco_string, "Privacy ");
		// skip two reserved 
		if(payload[4] == '1')
			strcat(flco_string, "Broadcast ");
		if(payload[5] == '1')
			strcat(flco_string, "OVCM ");
        if(payload[6] == '1' && payload[7] == '1')
            strcat(flco_string, "Priority:3 ");
        else if(payload[6] == '1')
            strcat(flco_string, "Priority:2 ");
        else if(payload[7] == '1')
            strcat(flco_string, "Priority:1 ");
        else
            strcat(flco_string, "Priority:0 ");
		l = 0;
		for(i = 8; i < 32; i++)
	        {
		    l <<= 1;
		    l |= (payload[i] == '1')?1:0;
		}
		sprintf(tmpStr, "DestId:%ld ", l);
		strcat(flco_string, tmpStr);
		l = 0;
		for(i = 32; i < 56; i++)
	        {
		    l <<= 1;
		    l |= (payload[i] == '1')?1:0;
		}
		sprintf(tmpStr, "SourceId:%ld ", l);
		strcat(flco_string, tmpStr);
	  }
    else if ((strcmp (flco, "000100") == 0) && (strcmp (fid, "00010000") == 0))
	  {
		sprintf(flco_string, "Mototurbo Capacity+ ");

		  l = 0;
		  for(i = 0; i < 24; i++)
		  {
		      l <<= 1;
		      l |= (payload[i] == '1')?1:0;
		  }
		  sprintf(tmpStr, "?:0x%06lX ", l);
  		  strcat(flco_string, tmpStr);
		  l = 0;
		  for(i = 24; i < 32; i++)
		  {
		      l <<= 1;
		      l |= (payload[i] == '1')?1:0;
		  }
		  sprintf(tmpStr, "Group:%ld ", l);
		  strcat(flco_string, tmpStr);
		  l = 0;
		  for(i = 32; i < 40; i++)
		  {
		      l <<= 1;
		      l |= (payload[i] == '1')?1:0;
		  }
		  sprintf(tmpStr, "RestCh:%ld ", l);
		  strcat(flco_string, tmpStr);
		  l = 0;
		  for(i = 40; i < 56; i++)
		  {
		      l <<= 1;
		      l |= (payload[i] == '1')?1:0;
		  }
		  sprintf(tmpStr, "RadioId:%ld ", l);
		  strcat(flco_string, tmpStr);
          }
	  else
	  {
		ll = 0;
		for(i = 0; i < 56; i++)
	        {
		    ll <<= 1;
		    ll |= (payload[i] == '1')?1:0;
		}
		sprintf(flco_string, "pf:%c flco:%s fid:%s payload:0x%014llX", pf, flco, fid, ll);
	  }
	flco_str_valid = 1;
}
char *getFlcoString()
{
  if(flco_str_valid)
    {
      flco_str_valid = 0;
      return flco_string;
    }
  else
    {
      return flco_null_string;
    }
}

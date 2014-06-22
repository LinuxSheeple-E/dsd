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
  if (strcmp (fid, "00000000") == 0) // Standard feature
    {
      sprintf(flco_string, "pf:%c Standard FID - ", pf);
      if (strcmp (flco, "000000") == 0)
        {
          strcat(flco_string, "Group Voice Ch Usr ");
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
      else if (strcmp (flco, "000011") == 0)
        {
          strcat(flco_string, "Unit-Unit Voice Ch Usr ");
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
      else if (strcmp (flco, "110000") == 0)
        {
          strcat(flco_string, "Terminator Data LC ");
          l = 0;
          for(i = 0; i < 24; i++)
            {
              l <<= 1;
              l |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "%s:%06lX ",(payload[48] == '1')?"Grp":"DestID", l);
          strcat(flco_string, tmpStr);
          l = 0;
          for(i = 24; i < 48; i++)
            {
              l <<= 1;
              l |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "SrcId:%06lX %c %s %c ", l, (payload[49] == '1')?'A':' ', (payload[50] == '1')?"FMF":"   ", (payload[52] == '1')?'S':' ');
          strcat(flco_string, tmpStr);
          l = 0;
          for(i = 53; i < 56; i++)
            {
              l <<= 1;
              l |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "N(S):%ld ", l);
          strcat(flco_string, tmpStr);
        }
      else
      {
        ll = 0LL;
        j = 0;
        for(i = 0; i < 64; i++)
          {
            ll <<= 1;
            ll |= (payload[i] == '1')?1:0;
          }
        for(i = 64; i < 80; i++)
          {
            j <<= 1;
            j |= (payload[i] == '1')?1:0;
          }
        sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
        strcat(flco_string, tmpStr);
      }
    }
  else if (strcmp (fid, "00000100") == 0) 
    {
      sprintf(flco_string, "pf:%c Flyde Micro - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00000101") == 0)
    {
      sprintf(flco_string, "pf:%c PROD-EL SPA - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00000110") == 0) // Trident Microsystems (mainly Motorola Connect Plus)
    {
      sprintf(flco_string, "pf:%c Trident MS (Motorola) - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00000111") == 0)
    {
      sprintf(flco_string, "pf:%c RADIODATA GmbH - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00001000") == 0)
    {
      sprintf(flco_string, "pf:%c Hyteria (4) - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00010000") == 0) // Mototrbo
    {
      sprintf(flco_string, "pf:%c Motorola - ", pf);
     if (strcmp (flco, "000000") == 0)
        {
          strcat(flco_string, "Group Voice Ch Usr ");
          if(payload[0] == '1')
            strcat(flco_string, "EMERGENCY ");
          if(payload[1] == '1')
            strcat(flco_string, "Privacy ");
          if(payload[2] == '1')
            strcat(flco_string, "Unknown 1 ");
          if(payload[3] == '1')
            strcat(flco_string, "Unknown 2 ");
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
      else if (strcmp (flco, "000011") == 0)
        {
          strcat(flco_string, "Unit-Unit Voice Ch Usr ");
          if(payload[0] == '1')
            strcat(flco_string, "EMERGENCY ");
          if(payload[1] == '1')
            strcat(flco_string, "Privacy ");
          if(payload[2] == '1')
            strcat(flco_string, "Unknown 1 ");
          if(payload[3] == '1')
            strcat(flco_string, "Unknown 2 ");
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
      else if (strcmp (flco, "000100") == 0)
        {
          strcat(flco_string, "Capacity+ Group Voice ");
          if(payload[0] == '1')
            strcat(flco_string, "EMERGENCY ");
          if(payload[1] == '1')
            strcat(flco_string, "Privacy ");
          if(payload[2] == '1')
            strcat(flco_string, "Unknown 1 ");
          if(payload[3] == '1')
            strcat(flco_string, "Unknown 2 ");
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
          l = 0;
          for(i = 8; i < 32; i++)
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
          ll = 0LL;
          j = 0;
          for(i = 0; i < 64; i++)
            {
              ll <<= 1;
              ll |= (payload[i] == '1')?1:0;
            }
          for(i = 64; i < 80; i++)
            {
              j <<= 1;
              j |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
          strcat(flco_string, tmpStr);
        }
    }
  else if (strcmp (fid, "00010011") == 0) 
    {
      sprintf(flco_string, "pf:%c EMC S.p.A (19) - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00011100") == 0) 
    {
      sprintf(flco_string, "pf:%c EMC S.p.A (28) - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00110011") == 0) 
    {
      sprintf(flco_string, "pf:%c Radio Activity Srl (51) - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "00111100") == 0) 
    {
      sprintf(flco_string, "pf:%c Radio Activity Srl (60) - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "01011000") == 0) 
    {
      sprintf(flco_string, "pf:%c Tait Electronics - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "01101000") == 0) 
    {
      sprintf(flco_string, "pf:%c Hyteria (104) - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else if (strcmp (fid, "01110111") == 0) 
    {
      sprintf(flco_string, "pf:%c Vertex Standard - ", pf);
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "flco:%s fid:%s payload:0x%016llX%04X", flco, fid, ll, j);
      strcat(flco_string, tmpStr);
    }
  else
    {
      ll = 0LL;
      j = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      for(i = 64; i < 80; i++)
        {
          j <<= 1;
          j |= (payload[i] == '1')?1:0;
        }
      sprintf(flco_string, "pf:%c flco:%s fid:%s payload:0x%016llX%04X", pf, flco, fid, ll, j);
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

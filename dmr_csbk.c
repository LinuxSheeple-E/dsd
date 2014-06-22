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

static char csbk_string[133];
static char csbk_null_string[] = "";
static int  csbk_str_valid = 0;

void processCsbk( char lb, char pf, char csbk[7], char fid[9], char payload[97] )
{
  int i, j, k;
  long l;
  long long ll;
  char tmpStr[81];
  csbk_str_valid = 0;
  if (strcmp (fid, "00000000") == 0) // Standard Feature Set
    {
      sprintf(csbk_string, "lb:%c pf:%c Standard FID - ", lb, pf);
      if (strcmp (csbk, "000100") == 0)
        {
          strcat(csbk_string, "Unit-Unit Voice Request ");
        }
      else if (strcmp (csbk, "000101") == 0)
        {
          strcat(csbk_string, "Unit-Unit Answer Response ");
        }
      else if (strcmp (csbk, "000111") == 0)
        {
          strcat(csbk_string, "Channel Timing ");
        }
      else if (strcmp (csbk, "011001") == 0)
        {
          strcat(csbk_string, "Aloha ");
        }
      else if (strcmp (csbk, "011010") == 0)
        {
          strcat(csbk_string, "UDT Download Header ");
        }
      else if (strcmp (csbk, "011011") == 0)
        {
          strcat(csbk_string, "UDT Upload Header ");
        }
      else if (strcmp (csbk, "011100") == 0)
        {
          strcat(csbk_string, "Ahoy ");
        }
      else if (strcmp (csbk, "011110") == 0)
        {
          strcat(csbk_string, "Ackvitation ");
        }
      else if (strcmp (csbk, "011111") == 0)
        {
          strcat(csbk_string, "Random Access Service Request ");
        }
      else if (strcmp (csbk, "100000") == 0)
        {
          strcat(csbk_string, "Ack Outbound TSCC ");
        }
      else if (strcmp (csbk, "100001") == 0)
        {
          strcat(csbk_string, "Ack Inbound TSCC ");
        }
      else if (strcmp (csbk, "100010") == 0)
        {
          strcat(csbk_string, "Ack Outbound Payload ");
        }
      else if (strcmp (csbk, "100011") == 0)
        {
        strcat(csbk_string, "Ack Inbound Payload  ");
        }
      else if (strcmp (csbk, "100110") == 0)
        {
        strcat(csbk_string, "Nack Response ");
        }
      else if (strcmp (csbk, "101000") == 0)
        {
        strcat(csbk_string, "C Bcast ");
        }
      else if (strcmp (csbk, "101010") == 0)
        {
        strcat(csbk_string, "Maintenance ");
        }
      else if (strcmp (csbk, "101110") == 0)
        {
          strcat(csbk_string, "Clear ");
        }
      else if (strcmp (csbk, "101111") == 0)
        {
          strcat(csbk_string, "Protect ");
        }
      else if (strcmp (csbk, "110000") == 0)
        {
          strcat(csbk_string, "Private Voice Grant ");
        }
      else if (strcmp (csbk, "110001") == 0)
        {
          strcat(csbk_string, "Talkgroup Voice Grant ");
        }
      else if (strcmp (csbk, "110010") == 0)
        {
          strcat(csbk_string, "Private Broadcast Voice Grant ");
        }
      else if (strcmp (csbk, "110011") == 0)
        {
          strcat(csbk_string, "Private Data Grant ");
        }
      else if (strcmp (csbk, "110100") == 0)
        {
          strcat(csbk_string, "Talkgroup Data Grant ");
        }
      else if (strcmp (csbk, "111000") == 0)
        {
          strcat(csbk_string, "BS Downlink Activate / Move ");
          l = 0;
          for(i = 0; i < 24; i++)
            {
              l <<= 1;
              l |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "BId:0x%06lX ", l);
          strcat(csbk_string, tmpStr);
          l = 0;
          for(i = 24; i < 48; i++)
            {
              l <<= 1;
              l |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "RId:0x%06lX ", l);
          strcat(csbk_string, tmpStr);
        }
      else if (strcmp (csbk, "111101") == 0)
        {
          strcat(csbk_string, "Preamble ");
          l = 0;
          for(i = 8; i < 16; i++)
          {
            l <<= 1;
            l |= (payload[i] == '1')?1:0;
          }
          sprintf(tmpStr, "%ld %s blks ", l, (payload[0] == '1')?"Data":"CSBK");
          strcat(csbk_string, tmpStr);
          l = 0;
          for(i = 16; i < 40; i++)
          {
            l <<= 1;
            l |= (payload[i] == '1')?1:0;
          }
          if(payload[1] == '1')
            sprintf(tmpStr, "TGrp:0x%06lX ", l);
          else
            sprintf(tmpStr, "TRId:0x%06lX ", l);
          strcat(csbk_string, tmpStr);
          l = 0;
          for(i = 40; i < 64; i++)
          {
            l <<= 1;
            l |= (payload[i] == '1')?1:0;
          }
          sprintf(tmpStr,"RId:0x%06lX ", l);
          strcat(csbk_string, tmpStr);
        }
      else
        {
          ll = 0;
          for(i = 0; i < 64; i++)
            {
              ll <<= 1;
              ll |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
          strcat(csbk_string, tmpStr);
        }
    }
  else if(strcmp (fid, "00000100") == 0) // Flyde Micro
    {
      sprintf(csbk_string, "lb:%c pf:%c Flyde Micro - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "00000101") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c PROD-EL SPA - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "00000110") == 0) // Trident Microsystems (mainly Motorola Connect Plus)
    {
      sprintf(csbk_string, "lb:%c pf:%c Trident MS (Motorola) - ", lb, pf);
      if (strcmp (csbk, "000001") == 0)
        {
          strcat(csbk_string, "Connect+ Neighbors: ");
          if(payload[0] != '\0')
            {
              for(j = 0; j < 40; j += 8)
                {
                  l = 0;
                  for(i = 0; i < 8; i++)
                    {
                      l <<= 1;
                      l |= (payload[j+i] == '1')?1:0;
                    }
                  if (l == 0)
                    break;
                  sprintf(tmpStr, "%2ld ", l);
                  strcat(csbk_string, tmpStr);
                }
              l = 0;
              for(i = 40; i < 64; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr," ?:0x%06lX ", l);
              strcat(csbk_string, tmpStr);
              
            }
        }
      else if (strcmp (csbk, "000011") == 0)
        {
          strcat(csbk_string, "Connect+ Voice Goto ");
          if(payload[0] != '\0')
            {
              l = 0;
              for(i = 0; i < 24; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr, "SourceRId:%ld ", l);
              strcat(csbk_string, tmpStr);
              l = 0;
              for(i = 24; i < 48; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              if(payload[63] == '1')
                sprintf(tmpStr, "DestRId:%ld ", l);
              else
                sprintf(tmpStr, "GroupId:%ld ", l);
              strcat(csbk_string, tmpStr);
              l = 0;
              ll = 0;
              for(i = 48; i < 52; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              for(i = 53; i < 64; i++)
                {
                  ll <<= 1;
                  ll |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr, "LCN:%ld Slot:%c ?:0x%03llX ", l,(payload[52] == '1')?'1':'0', ll);
              strcat(csbk_string, tmpStr);
            }
        }
      else if (strcmp (csbk, "000110") == 0)
        {
          strcat(csbk_string, "Connect+ Data Goto ");
          if(payload[0] != '\0')
            {
              l = 0;
              for(i = 0; i < 24; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr, "RadioId:%ld ", l);
              strcat(csbk_string, tmpStr);
              l = 0;
              ll = 0;
              for(i = 24; i < 28; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              for(i = 29; i < 64; i++)
                {
                  ll <<= 1;
                  ll |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr, "LCN:%ld Slot:%c ?:0x%09llX ", l,(payload[28] == '1')?'1':'0', ll);
              strcat(csbk_string, tmpStr);
            }
        }
      else if (strcmp (csbk, "011000") == 0)
        {
          strcat(csbk_string, "Connect+ Affiliate ");
          if(payload[0] != '\0')
            {
              l = 0;
              for(i = 0; i < 24; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr, "RadioId:%ld ", l);
              strcat(csbk_string, tmpStr);
              l = 0;
              for(i = 24; i < 48; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr, "GroupId:%ld ", l);
              strcat(csbk_string, tmpStr);
              l = 0;
              for(i = 48; i < 64; i++)
                {
                  l <<= 1;
                  l |= (payload[i] == '1')?1:0;
                }
              sprintf(tmpStr, "?:0x%04lX ", l);
              strcat(csbk_string, tmpStr);
            }
        }
      else
        {
          ll = 0;
          for(i = 0; i < 64; i++)
            {
              ll <<= 1;
              ll |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
          strcat(csbk_string, tmpStr);
        }
    }
  else if(strcmp (fid, "00000111") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c RADIODATA GmbH - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "00000100") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c Hyteria (4) - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "00010000") == 0) // Mototrbo
    {
      sprintf(csbk_string, "lb:%c pf:%c Motorola - ", lb, pf);
      if (strcmp (csbk, "111110") == 0)
      {
        strcat(csbk_string, "Capacity+ Ch Status ");
        l = 0;
        for(i = 0; i < 2; i++)
        {
          l <<= 1;
          l |= (payload[i] == '1')?1:0;
        }
        sprintf(tmpStr, "FL:%1ld Slot %c ", l,(payload[2] == '1')?'1':'0');
        strcat(csbk_string, tmpStr);
        l = 0;
        for(i = 3; i < 8; i++)
        {
          l <<= 1;
          l |= (payload[i] == '1')?1:0;
        }
        sprintf(tmpStr, "RestCh:%2ld ", l);
        strcat(csbk_string, tmpStr);
        l = 0;
        sprintf(tmpStr, "ActiveCh:%c%c%c%c%c%c%c%c ",   (payload[8] == '1')?'1':' ',
                                   (payload[9] == '1')?'2':' ',
                                   (payload[10] == '1')?'3':' ',
                                   (payload[11] == '1')?'4':' ',
                                   (payload[12] == '1')?'5':' ',
                                   (payload[13] == '1')?'6':' ',
                                   (payload[14] == '1')?'7':' ',
                                   (payload[15] == '1')?'8':' ');
        strcat(csbk_string, tmpStr);
        l = 0;
        for(i = 16; i < 24; i++)
        {
          l <<= 1;
          l |= (payload[i] == '1')?1:0;
        }
        if(l == 0)
        {
          sprintf(tmpStr, "Idle  ");
          strcat(csbk_string, tmpStr);
        }
        else
        {
          sprintf(tmpStr, "TGs:%3ld ", l);
          strcat(csbk_string, tmpStr);
          for(j = 24; j < 64; j += 8)
          {
            l = 0;
            for(i = 0; i < 8; i++)
            {
              l <<= 1;
              l |= (payload[j+i] == '1')?1:0;
            }
            if(l == 0)
              break;
            sprintf(tmpStr, "%3ld ", l);
            strcat(csbk_string, tmpStr);
           }
        }
      }
      else
        {
          ll = 0;
          for(i = 0; i < 64; i++)
            {
              ll <<= 1;
              ll |= (payload[i] == '1')?1:0;
            }
          sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
          strcat(csbk_string, tmpStr);
        }
    }
  else if(strcmp (fid, "00010011") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c EMC S.p.A (19) - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "00011100") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c EMC S.p.A (28) - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "00110011") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c Radio Activity Srl (51) - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "00111100") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c Radio Activity Srl (60) - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "01011000") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c Tait Electronics - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "01101000") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c Hyteria (104) - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else if(strcmp (fid, "01110111") == 0)
    {
      sprintf(csbk_string, "lb:%c pf:%c Vertex Standard - ", lb, pf);
      ll = 0;
      for(i = 0; i < 64; i++)
        {
          ll <<= 1;
          ll |= (payload[i] == '1')?1:0;
        }
      sprintf(tmpStr, "csbk:%s fid:%s payload:0x%016llX",csbk, fid, ll);
      strcat(csbk_string, tmpStr);
    }
  else
	{
      ll = 0;
      for(i = 0; i < 64; i++)
	    {
		  ll <<= 1;
		  ll |= (payload[i] == '1')?1:0;
		}
      sprintf(csbk_string, "lb:%c pf:%c csbk:%s fid:%s payload:0x%016llX", lb, pf, csbk, fid, ll);
	}
  csbk_str_valid = 1;
}
char *getCsbkString()
{
  if(csbk_str_valid)
    {
      csbk_str_valid = 0;
      return csbk_string;
    }
  else
    {
      return csbk_null_string;
    }
}

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

#include "dsd.h"
#include "dmr.h"

void
processDMRdata (dsd_opts * opts, dsd_state * state)
{

  int i, j, dibit;
  int *dibit_p;
  long l;
  char *cptr;
  char sync[25];
  char syncdata[25];
  char cachdata[13];
  char slottype[20];
  char infodata[197];
  char payload[97];
  char lb;
  char pf;
  char flco[7];
  char csbk[7];
  char fid[9];
  int  errorflag;
  char tmpstr[133];
  char datastr[133];
  char cc;
  char bursttype[5];

#ifdef DMR_DUMP
  int k;
  char syncbits[49];
  char cachbits[25];
#endif

  cc = 0;
  bursttype[4] = 0;
  datastr[0] = 0;
  
  dibit_p = state->dibit_buf_p - 90;

  // CACH
  for (i = 0; i < 12; i++)
    {
      dibit = *dibit_p;
      dibit_p++;
      if (opts->inverted_dmr == 1)
        {
          dibit = (dibit ^ 2);
        }
      cachdata[i] = dibit;
    }
  cachdata[12] = 0;

#ifdef DMR_DUMP
  k = 0;
  for (i = 0; i < 12; i++)
    {
      dibit = cachdata[i];
      cachbits[k] = (1 & (dibit >> 1)) + 48;    // bit 1
      k++;
      cachbits[k] = (1 & dibit) + 48;   // bit 0
      k++;
    }
  cachbits[24] = 0;
  printf ("%s ", cachbits);
#endif
  
  processCach (opts, state, cachdata);

  // current slot
  for (i = 0; i < 98; i += 2)
    {
      dibit = *dibit_p;
      dibit_p++;
      if (opts->inverted_dmr == 1)
        {
          dibit = (dibit ^ 2);
        }
      infodata[i] = (1 & (dibit >> 1)); // bit 1
      infodata[i+1] = (1 & dibit);        // bit 0
    }

  // slot type
  for (i = 0; i < 10; i += 2)
    {
      dibit = *dibit_p;
      dibit_p++;
      if (opts->inverted_dmr == 1)
        {
          dibit = (dibit ^ 2);
        }
      slottype[i] = (1 & (dibit >> 1)) + 48; // bit 1
      slottype[i+1] = (1 & dibit) + 48;        // bit 0
    }
  // signaling data or sync
  for (i = 0; i < 24; i++)
    {
      dibit = *dibit_p;
      dibit_p++;
      if (opts->inverted_dmr == 1)
        {
          dibit = (dibit ^ 2);
        }
      syncdata[i] = dibit;
      sync[i] = (dibit | 1) + 48;
    }
  sync[24] = 0;
  syncdata[24] = 0;

#ifdef DMR_DUMP
  k = 0;
  for (i = 0; i < 24; i++)
    {
      dibit = syncdata[i];
      syncbits[k] = (1 & (dibit >> 1)) + 48;    // bit 1
      k++;
      syncbits[k] = (1 & dibit) + 48;   // bit 0
      k++;
    }
  syncbits[48] = 0;
  printf ("%s ", syncbits);
#endif

  if ((strcmp (sync, DMR_BS_DATA_SYNC) == 0) || (strcmp (sync, DMR_MS_DATA_SYNC) == 0))
    {
      if (state->currentslot == 0)
        {
          sprintf (state->slot0light, "[slot0]");
        }
      else
        {
          sprintf (state->slot1light, "[slot1]");
        }
    }

  if (opts->errorbars == 1)
    {
      printf ("%s %s ", state->slot0light, state->slot1light);
    }

// current frame - second half
  for (i = 10; i < 20; i += 2)
    {
      dibit = getDibit (opts, state);
      slottype[i] = (1 & (dibit >> 1)) + 48; // bit 1
      slottype[i+1] = (1 & dibit) + 48;        // bit 0
    }
  for (i = 98; i < 196; i += 2)
    {
      dibit = getDibit (opts, state);
      infodata[i] = (1 & (dibit >> 1)); // bit 1
      infodata[i+1] = (1 & dibit);        // bit 0
    }

  sprintf (state->fsubtype, "              ");
  if((errorflag = doGolay208(slottype)) == 0)
    {
      cc = 0;
      for(i = 0; i < 4; i++)
	{
	  cc <<= 1;
	  cc |= (slottype[i] - 48);
	  bursttype[i] = slottype[i+4];
	}
      if (strcmp (bursttype, "0000") == 0)
	{
	  sprintf (state->fsubtype, " PI Header    ");
      if(processBPTC(opts, state, infodata, payload) == 0)
      {
        initCRC16();
        char crc_mask[16] = {0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1};
        for (i = 0; i < 16; i++)
        {
          payload[80+i] ^= crc_mask[i];
        }
        for(i = 0; i < 96; i++)
          doCRC16(payload[i]);
        if(getCRC16() == 0x1d0f)
        {
          for (i = 0, j = 0; i < 80; i++)
          {
            if(i%8 == 0)
            {
              l = 0;
            }
            l <<= 1;
            l |= payload[i];
            if(i%8 == 7)
            {
              sprintf (datastr+j,"%02lX  ", l);
              j += 3;
            }
          }
          datastr[j]= '\0';
        }
      }
	}
      else if (strcmp (bursttype, "0001") == 0)
	{
	  sprintf (state->fsubtype, " VOICE Header ");
      if(processBPTC(opts, state, infodata, payload) == 0)
      {
        initRS1294();
        char rs_mask[24] = {1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0};
        for (i = 0; i < 24; i++)
        {
          payload[72+i] ^= rs_mask[i];
        }
        if(doRS1294(payload) == 0)
        {
          pf = payload[0] + 48;
          for (i = 0; i < 6; i++)
          {
            flco[i] = payload[2+i] + 48;
          }
          flco[6] = '\0';
          for (i = 0; i < 8; i++)
          {
            fid[i] = payload[8+i] + 48;
          }
          fid[8] = '\0';
          for (i = 0; i < 56; i++)
          {
            payload[i] = payload[16+i] + 48;
          }
          payload[56] = '\0';
          processFlco(pf, flco, fid, payload );
        }
      }
      
	}
      else if (strcmp (bursttype, "0010") == 0)
	{
	  sprintf (state->fsubtype, " TLC          ");
      if(processBPTC(opts, state, infodata, payload) == 0)
      {
        initRS1294();
        char rs_mask[24] = {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1};
        for (i = 0; i < 24; i++)
        {
          payload[72+i] ^= rs_mask[i];
        }
        if(doRS1294(payload) == 0)
        {
          pf = payload[0] + 48;
          for (i = 0; i < 6; i++)
          {
            flco[i] = payload[2+i] + 48;
          }
          flco[6] = '\0';
          for (i = 0; i < 8; i++)
          {
            fid[i] = payload[8+i] + 48;
          }
          fid[8] = '\0';
          for (i = 0; i < 56; i++)
          {
            payload[i] = payload[16+i] + 48;
          }
          payload[56] = '\0';
          processFlco(pf, flco, fid, payload );
        }
      }
	}
      else if (strcmp (bursttype, "0011") == 0)
	{
	  sprintf (state->fsubtype, " CSBK         ");
      if(processBPTC(opts, state, infodata, payload) == 0)
      {
        initCRC16();
        char crc_mask[16] = {1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1};
        for (i = 0; i < 16; i++)
        {
          payload[80+i] ^= crc_mask[i];
        }
        for(i = 0; i < 96; i++)
          doCRC16(payload[i]);
        if(getCRC16() == 0x1d0f)
        {
          lb = payload[0] + 48; // This should always be one, otherwise mbc would be used
        
          pf = payload[1] + 48;
          for (i = 0; i < 6; i++)
          {
            csbk[i] = payload[2+i] + 48;
          }
          csbk[6] = '\0';
          for (i = 0; i < 8; i++)
          {
            fid[i] = payload[8+i] + 48;
          }
          fid[8] = '\0';
          for (i = 0; i < 80; i++)
          {
            payload[i] = payload[16+i] + 48;
          }
          payload[80] = '\0';
          processCsbk(lb, pf, csbk, fid, payload );
      }
    }
	}
      else if (strcmp (bursttype, "0100") == 0)
	{
	  sprintf (state->fsubtype, " MBC Header   ");
      if(processBPTC(opts, state, infodata, payload) == 0)
      {
        initCRC16();
        char crc_mask[16] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
        for (i = 0; i < 16; i++)
        {
          payload[80+i] ^= crc_mask[i];
        }
        for(i = 0; i < 96; i++)
          doCRC16(payload[i]);
        if(getCRC16() == 0x1d0f)
        {
          lb = payload[0] + 48; // This should always be zero, otherwise csbk would be used
        
          pf = payload[1] + 48;
          for (i = 0; i < 6; i++)
          {
            csbk[i] = payload[2+i] + 48;
          }
          csbk[6] = '\0';
          for (i = 0; i < 8; i++)
          {
            fid[i] = payload[8+i] + 48;
          }
          fid[8] = '\0';
          for (i = 0; i < 80; i++)
          {
            payload[i] = payload[16+i] + 48;
          }
          payload[80] = '\0';
          processCsbk(lb, pf, csbk, fid, payload );
        }
	  }
    }
    else if (strcmp (bursttype, "0101") == 0)
	{
	  sprintf (state->fsubtype, " MBC          ");
	}
    else if (strcmp (bursttype, "0110") == 0)
	{
	  sprintf (state->fsubtype, " DATA Header  ");
      if(processBPTC(opts, state, infodata, payload) == 0)
      {
        initCRC16();
        char crc_mask[16] = {1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
        for (i = 0; i < 16; i++)
        {
          payload[80+i] ^= crc_mask[i];
        }
        for(i = 0; i < 96; i++)
          doCRC16(payload[i]);
        if(getCRC16() == 0x1d0f)
        {
          // Common elements to all data header types
          l = 0;
          for(i = 16; i < 40; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(datastr, "%s:%ld ",(payload[0])?"Grp":"DestRID", l);
          l = 0;
          for(i = 40; i < 64; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr,"SrcRId:%ld %c ", l, (payload[1])?'A':' ');
          strcat(datastr, tmpstr);
          j = 0;
          for(i = 4; i < 8; i++)
          {
            j <<= 1;
            j |= payload[i];
          }
          switch(j)   // j is used as header type
          {
          case 0:
            strcat(datastr, "UDT  ");
            break;
          case 1:
            strcat(datastr, "Resp ");
            break;
          case 2:
            strcat(datastr, "UDat ");
            break;
          case 3:
            strcat(datastr, "CDat ");
            break;
          case 13:
            strcat(datastr, "DSDa "); // Defined
            break;
          case 14:
            strcat(datastr, "RSDa "); // Raw or Status/Precoded
            break;
          case 15:
            strcat(datastr, "Prop ");
            break;
          default:
            sprintf(tmpstr, "Rsvd (%d) ", j);
            strcat(datastr, tmpstr);
        }
        l = 0;
        strcat(datastr, "SAP:");
        for(i = 8; i < 12; i++)
        {
          l <<= 1;
          l |= payload[i];
        }
        switch(l)
        {
        case 0:
          strcat(datastr, "UDT ");
          break;
        case 2:
          strcat(datastr, "TCP HC ");
          break;
        case 3:
          strcat(datastr, "UDP HC ");
          break;
        case 4:
          strcat(datastr, "IP Pkt ");
          break;
        case 5:
          strcat(datastr, "ARP ");
          break;
        case 9:
          strcat(datastr, "Prop Pkt ");
          break;
        case 10:
          strcat(datastr, "Short Data ");
          break;
        default:
          sprintf(tmpstr, "Rsvd (%ld) ",l);
          strcat(datastr, tmpstr);
        }
        if(j == 2 || j == 3) // Confirmed and unconfirmed data
        {
          strcat(datastr, (payload[2])?"HC ":"   ");
          l = payload[3]; // Take care of MSB
          for(i = 12; i < 16; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "POC:%ld ", l);
          strcat(datastr, tmpstr);
          strcat(datastr, (payload[64])?"FMF ":"    ");
          l = 0;
          for(i = 65; i < 72; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "BF:%ld ", l);
          strcat(datastr, tmpstr);
          if(j == 3) // Confirmed Data
          {
            l = 0;
            for(i = 73; i < 76; i++)
            {
              l <<= 1;
              l |= payload[i];
            }
            sprintf(tmpstr,"%c N(S):%ld ", (payload[72])?'S':' ', l);
            strcat(datastr, tmpstr);
          }
          l = 0;
          for(i = 76; i < 80; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "FSN:%ld ", l);
          strcat(datastr, tmpstr);
        }
        else if(j == 1) // Response
        {
          l = 0;
          for(i = 65; i < 72; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "BF:%ld ", l);
          strcat(datastr, tmpstr);
          l = 0;
          for(i = 72; i < 74; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "Class:%ld ", l);
          strcat(datastr, tmpstr);
          l = 0;
          for(i = 74; i < 77; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "Type:%ld ", l);
          strcat(datastr, tmpstr);
          l = 0;
          for(i = 77; i < 80; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "Status:%ld ", l);
          strcat(datastr, tmpstr);
        }
        else if(j == 13 || j == 14) // Status/precoded or Raw
        {
          int ab; // used to determine raw or status
          ab = 0;
          for(i = 2; i < 4; i++)
          {
            ab <<= 1;
            ab |= payload[i];
          }
          for(i = 12; i < 16; i++)
          {
            ab <<= 1;
            ab |= payload[i];
          }
          sprintf(tmpstr, "AB:%d ", ab);
          strcat(datastr, tmpstr);
          if(j == 13) // Defined short data
          {
            l = 0;
            for(i = 64; i < 70; i++)
            {
              l <<= 1;
              l |= payload[i];
            }
            switch (l)
            {
            case 0:
              strcat(datastr, "Binary ");
              break;
            case 1:
              strcat(datastr, "BCD ");
              break;
            case 2:
              strcat(datastr, "7-bit char ");
              break;
            case 3:
              strcat(datastr, "8-bit ISO 8859-1 ");
              break;
            case 4:
              strcat(datastr, "8-bit ISO 8859-2 ");
              break;
            case 5:
              strcat(datastr, "8-bit ISO 8859-3 ");
              break;
            case 6:
              strcat(datastr, "8-bit ISO 8859-4 ");
              break;
            case 7:
              strcat(datastr, "8-bit ISO 8859-5 ");
              break;
            case 8:
              strcat(datastr, "8-bit ISO 8859-6 ");
              break;
            case 9:
              strcat(datastr, "8-bit ISO 8859-7 ");
              break;
            case 10:
              strcat(datastr, "8-bit ISO 8859-8 ");
              break;
            case 11:
              strcat(datastr, "8-bit ISO 8859-9 ");
              break;
            case 12:
              strcat(datastr, "8-bit ISO 8859-10 ");
              break;
            case 13:
              strcat(datastr, "8-bit ISO 8859-11 ");
              break;
            case 14:
              strcat(datastr, "8-bit ISO 8859-13 ");
              break;
            case 15:
              strcat(datastr, "8-bit ISO 8859-14 ");
              break;
            case 16:
              strcat(datastr, "8-bit ISO 8859-15 ");
              break;
            case 17:
              strcat(datastr, "8-bit ISO 8859-16 ");
              break;
            case 18:
              strcat(datastr, "Unicode UTF-8 ");
              break;
            case 19:
              strcat(datastr, "Unicode UTF-16 ");
              break;
            case 20:
              strcat(datastr, "Unicode UTF-16BE ");
              break;
            case 21:
              strcat(datastr, "Unicode UTF-16LE ");
              break;
            case 22:
              strcat(datastr, "Unicode UTF-32 ");
              break;
            case 23:
              strcat(datastr, "Unicode UTF-32BE ");
              break;
            case 24:
              strcat(datastr, "Unicode UTF-32LE ");
              break;
            default:
              sprintf(tmpstr, "Rsvd Format (%ld) ",l);
              strcat(datastr, tmpstr);
            }
          }
          else
          {
            l = 0;
            for(i = 64; i < 67; i++)
            {
              l <<= 1;
              l |= payload[i];
            }
            sprintf(tmpstr, "SP:%ld ", l);
            strcat(datastr, tmpstr);
            l = 0;
            for(i = 67; i < 70; i++)
            {
              l <<= 1;
              l |= payload[i];
            }
            sprintf(tmpstr, "DP:%ld ", l);
            strcat(datastr, tmpstr);
          }
          if(j == 13 || ab > 0) // Defined or Raw
          {
            sprintf(tmpstr, "%c %s ", (payload[70])?'S':' ', (payload[71])?"FMF":"   ");
            strcat(datastr, tmpstr);
            l = 0;
            for(i = 72; i < 80; i++)
            {
              l <<= 1;
              l |= payload[i];
            }
            sprintf(tmpstr, "BPad:%ld ", l);
            strcat(datastr, tmpstr);
          }
          else // Status
          {
            l = 0;
            for(i = 70; i < 80; i++)
            {
              l <<= 1;
              l |= payload[i];
            }
            sprintf(tmpstr, "S/P:%ld ", l);
            strcat(datastr, tmpstr);
          }
        }
        else if(j == 0) // UDT
        {
          l = 0;
          for(i = 12; i < 16; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          switch (l)
          {
          case 0:
            strcat(datastr, "Binary ");
            break;
          case 1:
            strcat(datastr, "MS Addr ");
            break;
          case 2:
            strcat(datastr, "4-bit BCD ");
            break;
          case 3:
            strcat(datastr, "ISO 7-bit Chars ");
            break;
          case 4:
            strcat(datastr, "ISO 8-bit Chars ");
            break;
          case 5:
            strcat(datastr, "NMEA location ");
            break;
          case 6:
            strcat(datastr, "IP Addr ");
            break;
          case 7:
            strcat(datastr, "16-bit Unicode ");
            break;
          case 8:
            strcat(datastr, "Custom 0 ");
            break;
          case 9:
            strcat(datastr, "Custom 1 ");
            break;
          default:
            sprintf(tmpstr, "Rsvd Format (%ld) ",l);
            strcat(datastr, tmpstr);
          }
          l = 0;
          for(i = 64; i < 69; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "PadN:%ld ", l);
          strcat(datastr, tmpstr);
          l = 0;
          for(i = 70; i < 72; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr, "UAB:%ld %s %s ", l, (payload[72])?"SF":"  ", (payload[73])?"PF":"  ");
          strcat(datastr, tmpstr);
          l = 0;
          for(i = 74; i < 80; i++)
          {
            l <<= 1;
            l |= payload[i];
          }
          sprintf(tmpstr,"UDTO:%ld ", l);
          strcat(datastr, tmpstr);
        }
      }

        }
	}
  else if (strcmp (bursttype, "0111") == 0)
	{
	  sprintf (state->fsubtype, " RATE 1/2 DATA");
      if(processBPTC(opts, state, infodata, payload) == 0)
      {
        
        for (i = 0, j = 0; i < 96; i++)
        {
          if(i%8 == 0)
          {
            l = 0;
          }
          l <<= 1;
          l |= payload[i];
          if(i%8 == 7)
          {
            sprintf (datastr+j,"%02lX  ", l);
            j += 3;
          }
        }
        datastr[j]= '\0';
      }
	}
      else if (strcmp (bursttype, "1000") == 0)
	{
	  sprintf (state->fsubtype, " RATE 3/4 DATA");
	}
      else if (strcmp (bursttype, "1001") == 0)
	{
	  sprintf (state->fsubtype, " Slot idle    ");
	}
      else if (strcmp (bursttype, "1010") == 0)
	{
	  sprintf (state->fsubtype, " Rate 1 DATA  ");
	}
      else
	{
	  sprintf (state->fsubtype, "              ");
	}
    }
      
  // cach, next slot 1st half
  skipDibit (opts, state, 66);

  if (opts->errorbars == 1)
    {
      if (errorflag)
        {
	  printf(" Slot Type Error %d\n", errorflag);
	}
      else if (strcmp (state->fsubtype, "              ") == 0)
        {
          printf (" CC:%2d  Unknown burst type: %s\n", (int)cc, bursttype);
        }
      else 
        {
          printf (" CC:%2d %s %s%s%s\n", (int)cc, state->fsubtype, getCsbkString(), getFlcoString(), datastr);
        }
      cptr = getSlcoString();
      if(strlen(cptr) > 0)
	{
	  printf("  CACH: %s \n", cptr);
	}
    }
}

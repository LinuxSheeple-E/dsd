#include "dsd.h"
#include "dmr_const.h"
#include "dmr.h"

static char emb_fr[4][32];
static int  emb_fr_index = 0;
static int  emb_fr_count = 4;
static int  emb_fr_valid = 0;
static int  emb_str_valid = 0;

void processEmb (dsd_opts * opts, dsd_state * state, char syncdata[25])
{
  int i, j, k, dibit, lcss;
  long l;
  long long ll;
  char emb[49];
  char emb_sig[32];
  char emb_field[16];
  char emb_deinv_fr[8][16];
  char color_code[5];
  char pf;
  char flco[7];
  char fid[9];
  char payload[97];
  for (i = 0; i < 24; i++)
    {
      dibit = syncdata[i];
      emb[j] = (1 & (dibit >> 1));    // bit 1
      j++;
      emb[j] = (1 & dibit);   // bit 0
      j++;
    }
  emb[48] = 0;
  for(i = 0, j = 0; i < 8; i++, j++)
    {
      emb_field[j] = emb[i];
    }
  for(i = 8, j = 0; i < 40; i++, j++)
    {
      emb_sig[j] = emb[i];
    }
  for(i = 40, j = 8; i < 48; i++, j++)
    {
      emb_field[j] = emb[i];
    }
  if(doQR1676(emb_field))
    {
      return;
    }
  for(i = 0; i < 4; i++)
    {
      color_code[i] = emb_field[i] + 48;
    }
  lcss = (emb_field[5] << 1) | emb_field[6];
  switch(lcss)
    {
    case 0: //Single fragment LC or first fragment CSBK signalling (used for Reverse Channel)
      return;
      break;
    case 1: // First Fragment of LC signaling
      emb_fr_index = 0;
      emb_fr_valid = 0;
      break;
    case 2: // Last Fragment of LC or CSBK signaling
      if(++emb_fr_index != (emb_fr_count-1))
	return;
      break;
    case 3:
      if(++emb_fr_index >= emb_fr_count)
	return;
	break;
    }
  for(i = 0; i < 32; i++)
    {
      emb_fr[emb_fr_index][i] = emb_sig[i];		
    }
  emb_fr_valid |= (1 << emb_fr_index);
  if(emb_fr_valid == 15)
    {
      // Deinterleave
      for(i = 0, j = 0; i < 16; i++, j += 8)
        {
	  emb_deinv_fr[0][i] = emb_fr[j/32][j%32];
	}
      for(i = 0, j = 1; i < 16; i++, j += 8)
	{
	  emb_deinv_fr[1][i] = emb_fr[j/32][j%32];
        }
      for(i = 0, j = 2; i < 16; i++, j += 8)
	{
	  emb_deinv_fr[2][i] = emb_fr[j/32][j%32];
	}
      for(i = 0, j = 3; i < 16; i++, j += 8)
        {
	  emb_deinv_fr[3][i] = emb_fr[j/32][j%32];
	}
      for(i = 0, j = 4; i < 16; i++, j += 8)
	{
	  emb_deinv_fr[4][i] = emb_fr[j/32][j%32];
        }
      for(i = 0, j = 5; i < 16; i++, j += 8)
        {
	  emb_deinv_fr[5][i] = emb_fr[j/32][j%32];
        }
      for(i = 0, j = 6; i < 16; i++, j += 8)
        {
	  emb_deinv_fr[6][i] = emb_fr[j/32][j%32];
        }
      for(i = 0, j = 7; i < 16; i++, j += 8)
        {
	  emb_deinv_fr[7][i] = emb_fr[j/32][j%32];
        }
      for(i = 0; i < 8; i++) // just do simple check now
	{
	  if((k = doHamming16114(emb_deinv_fr[i])))
	    {
	      return;
	    }
	}
      pf = emb_deinv_fr[0][0] + 48;
      for (i = 0; i < 6; i++)
	{
	  flco[i] = emb_deinv_fr[0][2+i] + 48;
	}
      flco[6] = '\0';
      for (i = 0; i < 3; i++)
	{
	  fid[i] = emb_deinv_fr[0][i+8] + 48;
	}
      for (i = 0; i < 5; i++)
	{
	  fid[i+3] = emb_deinv_fr[1][i] + 48;
	}
      fid[8] = '\0';
      for (i = 5, k = 0; i < 11; i++, k++)
	{
	  payload[k] = emb_deinv_fr[1][i] + 48;
	}
      for (i = 20; i < 70; i++, k++)
	{
	  payload[k] = emb_deinv_fr[i/10][i%10] + 48;
	}
      payload[k] = '\0';
      processFlco( pf, flco, fid, payload );
    }
}

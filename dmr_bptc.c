#include "dsd.h"
#include "dmr_const.h"
#include "dmr.h"


int
processBPTC(dsd_opts * opts, dsd_state * state, char infodata[197], char payload[97])
{
  int i, j, k;
  const int *w, *x;
  char data_fr[13][15];
  w = rdW;
  x = rdX;
  for(i = 0; i < 196; i++)
  {
          data_fr[*w][*x] = infodata[i]; // bit 1
          w++;
          x++;
  }
  for (i = 0; i < 3; i++)
     data_fr[0][i] = 0; // Zero reserved bits
  for (i = 0; i < 15; i++)
  {
    char data_col[13];
    for(j = 0; j < 13; j++)
    {
      data_col[j] = data_fr[j][i];
    }
    if(doHamming1393(data_col) != 0)
    {
      return -1;
    }
  }
  for (i = 0; i < 9; i++)
  {
    if(doHamming15113(data_fr[i]) != 0)
    {
      return -2;
    }
  }
  for (i = 3, k = 0; i < 11; i++, k++)
  {
     payload[k] = data_fr[0][i];
  }
  for (j = 1; j < 9; j++)
  {
    for (i = 0; i < 11; i++, k++)
    {
      payload[k] = data_fr[j][i];
    }
  }
  payload[k] = '\0';
  return 0;
}
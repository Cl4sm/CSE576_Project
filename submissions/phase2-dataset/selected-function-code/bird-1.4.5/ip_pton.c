int
ip_pton(char *a, ip_addr *o)
{
  u16 words[8];
  int i, j, k, l, hfil;
  char *start;

  if (a[0] == ':')			/* Leading :: */
    {
      if (a[1] != ':')
	return 0;
      a++;
    }
  hfil = -1;
  i = 0;
  while (*a)
    {
      if (*a == ':')			/* :: */
	{
	  if (hfil >= 0)
	    return 0;
	  hfil = i;
	  a++;
	  continue;
	}
      j = 0;
      l = 0;
      start = a;
      for(;;)
	{
	  if (*a >= '0' && *a <= '9')
	    k = *a++ - '0';
	  else if (*a >= 'A' && *a <= 'F')
	    k = *a++ - 'A' + 10;
	  else if (*a >= 'a' && *a <= 'f')
	    k = *a++ - 'a' + 10;
	  else
	    break;
	  j = (j << 4) + k;
	  if (j >= 0x10000 || ++l > 4)
	    return 0;
	}
      if (*a == ':' && a[1])
	a++;
      else if (*a == '.' && (i == 6 || i < 6 && hfil >= 0))
	{				/* Embedded IPv4 address */
	  u32 x;
	  if (!ipv4_pton_u32(start, &x))
	    return 0;
	  words[i++] = x >> 16;
	  words[i++] = x;
	  break;
	}
      else if (*a)
	return 0;
      if (i >= 8)
	return 0;
      words[i++] = j;
    }

  /* Replace :: with an appropriate number of zeros */
  if (hfil >= 0)
    {
      j = 8 - i;
      for(i=7; i-j >= hfil; i--)
	words[i] = words[i-j];
      for(; i>=hfil; i--)
	words[i] = 0;
    }

  /* Convert the address to ip_addr format */
  for(i=0; i<4; i++)
    o->addr[i] = (words[2*i] << 16) | words[2*i+1];
  return 1;
}

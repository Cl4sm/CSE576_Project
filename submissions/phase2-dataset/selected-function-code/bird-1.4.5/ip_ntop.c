ip_ntop(ip_addr a, char *b)
{
  u16 words[8];
  int bestpos, bestlen, curpos, curlen, i;

  /* First of all, preprocess the address and find the longest run of zeros */
  bestlen = bestpos = curpos = curlen = 0;
  for(i=0; i<8; i++)
    {
      u32 x = a.addr[i/2];
      words[i] = ((i%2) ? x : (x >> 16)) & 0xffff;
      if (words[i])
	curlen = 0;
      else
	{
	  if (!curlen)
	    curpos = i;
	  curlen++;
	  if (curlen > bestlen)
	    {
	      bestpos = curpos;
	      bestlen = curlen;
	    }
	}
    }
  if (bestlen < 2)
    bestpos = -1;

  /* Is it an encapsulated IPv4 address? */
  if (!bestpos &&
      (bestlen == 5 && a.addr[2] == 0xffff ||
       bestlen == 6))
    {
      u32 x = a.addr[3];
      b += bsprintf(b, "::%s%d.%d.%d.%d",
		    a.addr[2] ? "ffff:" : "",
		    ((x >> 24) & 0xff),
		    ((x >> 16) & 0xff),
		    ((x >> 8) & 0xff),
		    (x & 0xff));
      return b;
    }

  /* Normal IPv6 formatting, compress the largest sequence of zeros */
  for(i=0; i<8; i++)
    {
      if (i == bestpos)
	{
	  i += bestlen - 1;
	  *b++ = ':';
	  if (i == 7)
	    *b++ = ':';
	}
      else
	{
	  if (i)
	    *b++ = ':';
	  b += bsprintf(b, "%x", words[i]);
	}
    }
  *b = 0;
  return b;
}

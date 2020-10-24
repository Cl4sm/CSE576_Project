{
  register unsigned char *data = buf;
  const unsigned char *ptr = data;
  int c, rl, count = len;

  while (len--)
    {
      if (runlen)
	{
	  *data++ = lastch;
	  --runlen;
	  continue;
	}

      c = nextchar();
      if (c == -1)
	return -1;

      if (c == 0x90)
	{
	  rl = nextchar();
	  if (rl == -1)
	    return -1;

	  if (rl > 0)
	    {
	      runlen = rl - 1;
	      ++len;
	      continue;
	    }
	}

      *data++ = lastch = c;
    }

  crc = crc_binh(ptr, count, crc);

  return count;
}

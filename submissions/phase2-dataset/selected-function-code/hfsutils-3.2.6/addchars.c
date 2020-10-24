static
int addchars(const unsigned char *data, register int len)
{
  register unsigned char c;

  while (len--)
    {
      c = *data++;

      if (lptr == MAXLINELEN &&
	  flushline() == -1)
	return -1;

      switch (state86 & 0xff00)
	{
	case 0x0000:
	  line[lptr++] = enmap[c >> 2];
	  state86 = 0x0100 | (c & 0x03);
	  break;

	case 0x0100:
	  line[lptr++] = enmap[((state86 & 0x03) << 4) | (c >> 4)];
	  state86 = 0x0200 | (c & 0x0f);
	  break;

	case 0x0200:
	  line[lptr++] = enmap[((state86 & 0x0f) << 2) | (c >> 6)];

	  if (lptr == MAXLINELEN &&
	      flushline() == -1)
	    return -1;

	  line[lptr++] = enmap[c & 0x3f];
	  state86 = 0;
	  break;
	}
    }

  return 0;
}

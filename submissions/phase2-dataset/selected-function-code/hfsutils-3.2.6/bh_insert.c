int bh_insert(const void *buf, register int len)
{
  register const unsigned char *data = buf;

  crc = crc_binh(data, len, crc);

  for ( ; len--; ++data)
    {
      if (runlen)
	{
	  if (runlen == 0xff || lastch != *data)
	    {
	      if (rleflush() == -1)
		return -1;
	    }

	  if (lastch == *data)
	    {
	      ++runlen;
	      continue;
	    }
	}

      lastch = *data;
      runlen = 1;
    }

  return 0;
}

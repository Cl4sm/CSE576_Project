int rleflush(void)
{
  unsigned char rle[] = { 0x90, 0x00, 0x90, 0x00 };

  if ((lastch != 0x90 && runlen < 4) ||
      (lastch == 0x90 && runlen < 3))
    {
      /* self representation */

      if (lastch == 0x90)
	{
	  while (runlen--)
	    if (addchars(rle, 2) == -1)
	      return -1;
	}
      else
	{
	  while (runlen--)
	    if (addchars(&lastch, 1) == -1)
	      return -1;
	}
    }
  else
    {
      /* run-length encoded */

      if (lastch == 0x90)
	{
	  rle[3] = runlen;

	  if (addchars(rle, 4) == -1)
	    return -1;
	}
      else
	{
	  rle[1] = lastch;
	  rle[3] = runlen;

	  if (addchars(&rle[1], 3) == -1)
	    return -1;
	}
    }

  runlen = 0;

  return 0;
}

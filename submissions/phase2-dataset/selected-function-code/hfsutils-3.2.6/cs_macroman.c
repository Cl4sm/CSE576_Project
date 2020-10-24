{
  int ilen, olen, i;
  char *macroman, *ptr;
  const char *subst;
  unsigned char ch, msr;

  if (latin1[0x80] != 0xff)
    mktable();

  ilen = lenptr ? *lenptr : strlen(lstr);

  olen = 0;
  for (i = 0; i < ilen; ++i)
    {
      ch = *((unsigned char *) lstr + i);
      msr = latin1[ch];

      if (msr == 0xff)
	olen += strlen(latin1_subst[ch & 0x7f]);
      else
	++olen;
    }

  macroman = malloc(olen + 1);
  if (macroman == 0)
    return 0;

  ptr = macroman;
  for (i = 0; i < ilen; ++i)
    {
      ch = *(unsigned char *) lstr++;
      msr = latin1[ch];

      if (msr == 0xff)
	{
	  /* substitute similar MacOS Standard Roman character(s) */

	  subst = latin1_subst[ch & 0x7f];

	  strcpy(ptr, subst);
	  ptr += strlen(subst);
	}
      else
	*ptr++ = msr;
    }

  *ptr = 0;

  if (lenptr)
    *lenptr = olen;

  return macroman;
}

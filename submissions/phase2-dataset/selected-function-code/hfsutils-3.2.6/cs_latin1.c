{
  int ilen, olen, i;
  char *latin1, *ptr;
  const char *subst;
  unsigned char ch;
  UCS2 unicode;

  ilen = lenptr ? *lenptr : strlen(mstr);

  olen = 0;
  for (i = 0; i < ilen; ++i)
    {
      ch = *((unsigned char *) mstr + i);
      unicode = macroman[ch];

      if (unicode & 0xff00)
	olen += strlen(macroman_subst[ch & 0x7f]);
      else
	++olen;
    }

  latin1 = malloc(olen + 1);
  if (latin1 == 0)
    return 0;

  ptr = latin1;
  for (i = 0; i < ilen; ++i)
    {
      ch = *(unsigned char *) mstr++;
      unicode = macroman[ch];

      if (unicode & 0xff00)
	{
	  /* substitute similar Latin-1 character(s) */

	  subst = macroman_subst[ch & 0x7f];

	  strcpy(ptr, subst);
	  ptr += strlen(subst);
	}
      else
	*ptr++ = unicode;
    }

  *ptr = 0;

  if (lenptr)
    *lenptr = olen;

  return latin1;
}

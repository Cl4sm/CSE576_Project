char *strstr(const char *haystack, const char *needle)
{
  register const char *hptr, *nptr;

  if (*needle == 0)
    return haystack;

  while (*haystack)
    {
      if (*haystack++ != *needle)
	continue;

      hptr = haystack;
      nptr = needle + 1;

      while (1)
	{
	  if (*nptr == 0)
	    return haystack - 1;

	  if (*hptr++ != *nptr++)
	    break;
	}
    }

  return 0;
}

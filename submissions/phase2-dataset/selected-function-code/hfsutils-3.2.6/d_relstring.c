int d_relstring(const char *str1, const char *str2)
{
  register int diff;

  while (*str1 && *str2)
    {
      diff = hfs_charorder[(unsigned char) *str1] -
	     hfs_charorder[(unsigned char) *str2];

      if (diff)
	return diff;

      ++str1, ++str2;
    }

  if (! *str1 && *str2)
    return -1;
  else if (*str1 && ! *str2)
    return 1;

  return 0;
}

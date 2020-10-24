ipv4_pton_u32(char *a, u32 *o)
{
  int i;
  unsigned long int l;
  u32 ia = 0;

  i=4;
  while (i--)
    {
      char *d, *c = strchr(a, '.');
      if (!c != !i)
	return 0;
      l = strtoul(a, &d, 10);
      if (d != c && *d || l > 255)
	return 0;
      ia = (ia << 8) | l;
      if (c)
	c++;
      a = c;
    }
  *o = ia;
  return 1;
}

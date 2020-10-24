int
ec_set_format(struct adata *set, int from, byte *buf, unsigned int size)
{
  u32 *z = int_set_get_data(set);
  byte *end = buf + size - 24;
  int from2 = MAX(from, 0);
  int to = int_set_get_size(set);
  int i;

  for (i = from2; i < to; i += 2)
    {
      if (buf > end)
	{
	  if (from < 0)
	    strcpy(buf, " ...");
	  else
	    *buf = 0;
	  return i;
	}

      if (i > from2)
	*buf++ = ' ';

      buf += ec_format(buf, ec_get(z, i));
    }
  *buf = 0;
  return 0;
}

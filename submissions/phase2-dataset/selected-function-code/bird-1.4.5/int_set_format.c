int
int_set_format(struct adata *set, int way, int from, byte *buf, unsigned int size)
{
  u32 *z = (u32 *) set->data;
  byte *end = buf + size - 24;
  int from2 = MAX(from, 0);
  int to = set->length / 4;
  int i;

  for (i = from2; i < to; i++)
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

      if (way)
	buf += bsprintf(buf, "(%d,%d)", z[i] >> 16, z[i] & 0xffff);
      else
	buf += bsprintf(buf, "%R", z[i]);
    }
  *buf = 0;
  return 0;
}

as_path_format(struct adata *path, byte *buf, unsigned int size)
{
  byte *p = path->data;
  byte *e = p + path->length;
  byte *end = buf + size - 16;
  int sp = 1;
  int l, isset;

  while (p < e)
    {
      if (buf > end)
	{
	  strcpy(buf, " ...");
	  return;
	}
      isset = (*p++ == AS_PATH_SET);
      l = *p++;
      if (isset)
	{
	  if (!sp)
	    *buf++ = ' ';
	  *buf++ = '{';
	  sp = 0;
	}
      while (l-- && buf <= end)
	{
	  if (!sp)
	    *buf++ = ' ';
	  buf += bsprintf(buf, "%u", get_as(p));
	  p += BS;
	  sp = 0;
	}
      if (isset)
	{
	  *buf++ = ' ';
	  *buf++ = '}';
	  sp = 0;
	}
    }
  *buf = 0;
}

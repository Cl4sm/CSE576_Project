static
void show_many(int sz, queueent *ents, char **strs,
	       int flags, int options, int width)
{
  int i, len, misc, maxlen = 0, rows, cols, row;

  misc = misclen(flags);

  for (i = 0; i < sz; ++i)
    {
      len = strlen(strs[i]) + misc;
      if (len > maxlen)
	maxlen = len;
    }

  maxlen += 2;

  cols = width / maxlen;
  if (cols == 0)
    cols = 1;

  rows = sz / cols + (sz % cols != 0);

  for (row = 0; row < rows; ++row)
    {
      i = row;

      while (1)
	{
	  showmisc(&ents[i].dirent, flags);
	  printf("%s", strs[i]);

	  i += rows;
	  if (i >= sz)
	    break;

	  for (len = strlen(strs[i - rows]) + misc;
	       len < maxlen; ++len)
	    putchar(' ');
	}

      putchar('\n');
    }
}

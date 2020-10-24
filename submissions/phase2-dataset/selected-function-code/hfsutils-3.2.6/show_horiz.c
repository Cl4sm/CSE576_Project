void show_horiz(int sz, queueent *ents, char **strs,
		int flags, int options, int width)
{
  int i, len, misc, maxlen = 0, cols;

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

  for (i = 0; i < sz; ++i)
    {
      if (i)
	{
	  if (i % cols == 0)
	    putchar('\n');
	  else
	    {
	      for (len = strlen(strs[i - 1]) + misc;
		   len < maxlen; ++len)
		putchar(' ');
	    }
	}

      showmisc(&ents[i].dirent, flags);
      printf("%s", strs[i]);
    }

  if (i)
    putchar('\n');
}

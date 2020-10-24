static
void show_commas(int sz, queueent *ents, char **strs,
		 int flags, int options, int width)
{
  int i, pos = 0;

  for (i = 0; i < sz; ++i)
    {
      hfsdirent *ent;
      int len;

      ent = &ents[i].dirent;
      len = strlen(strs[i]) + misclen(flags) + ((i < sz - 1) ? 2 : 0);

      if (pos && pos + len >= width)
	{
	  putchar('\n');
	  pos = 0;
	}

      showmisc(ent, flags);
      printf("%s", strs[i]);

      if (i < sz - 1)
	{
	  putchar(',');
	  putchar(' ');
	}

      pos += len;
    }

  if (pos)
    putchar('\n');
}

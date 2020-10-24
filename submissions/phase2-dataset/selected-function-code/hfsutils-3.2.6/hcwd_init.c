int hcwd_init(void)
{
  const char *home, *start;
  char buf[512], *path, *ptr;
  mountent entry;
  int newcur = -1;

  home = getenv("HOME");
  if (home == 0)
    home = "";

  path = malloc(strlen(home) + 1 + sizeof(STATEFNAME));
  if (path == 0)
    return -1;

  strcpy(path, home);
  strcat(path, "/" STATEFNAME);

  statef = fopen(path, "r+");
  if (statef == 0 && errno == ENOENT)
    statef = fopen(path, "w+");

  free(path);

  if (statef == 0)
    return -1;

  if (fgets(buf, sizeof(buf), statef))
    newcur = atoi(buf);

  while (fgets(buf, sizeof(buf), statef))
    {
      start = ptr = buf;

      while (*ptr && *ptr != '\n' && *ptr != '\t')
	++ptr;

      entry.vname[0] = 0;
      if (*ptr == '\t')
	{
	  *ptr++ = 0;
	  strcpy(entry.vname, start);
	  start = ptr;
	}

      while (*ptr && *ptr != '\n' && *ptr != '\t')
	++ptr;

      if (*ptr == '\t')
	{
	  *ptr++ = 0;
	  entry.vcrdate = strtol(start, 0, 0);
	  start = ptr;
	}

      while (*ptr && *ptr != '\n' && *ptr != '\t')
	++ptr;

      if (*ptr == '\t')
	{
	  *ptr++ = 0;
	  entry.path = strdup(start);
	  if (entry.path == 0)
	    return -1;

	  start = ptr;
	}

      while (*ptr && *ptr != '\n' && *ptr != '\t')
	++ptr;

      if (*ptr == '\t')
	{
	  *ptr++ = 0;
	  entry.partno = atoi(start);
	  start = ptr;
	}

      while (*ptr && *ptr != '\n' && *ptr != '\t')
	++ptr;

      if (*ptr == '\n' || *ptr == '\t')
	*ptr = 0;

      if (*start)
	{
	  entry.cwd = strdup(start);
	  if (entry.cwd == 0)
	    return -1;

	  if (addent(&entry) == -1)
	    return -1;
	}
    }

  curvol = (newcur >= 0 && newcur < nmounts) ? newcur : -1;

  return 0;
}

{
  char *cwd, *buf;
  size_t len;

  if (path[0] == '/')
    return strdup(path);

  cwd = getenv("PWD");
  if (cwd && hfsutil_samepath(cwd, "."))
    {
      buf = malloc(strlen(cwd) + 1 + strlen(path) + 1);
      if (buf == 0)
	return 0;

      strcpy(buf, cwd);
    }
  else
    {
      len = 32;
      cwd = malloc(len);
      if (cwd == 0)
	return 0;

      while (getcwd(cwd, len) == 0)
	{
	  if (errno != ERANGE)
	    {
	      free(cwd);
	      return 0;
	    }

	  len <<= 1;
	  buf = realloc(cwd, len);
	  if (buf == 0)
	    {
	      free(cwd);
	      return 0;
	    }

	  cwd = buf;
	}

      buf = realloc(cwd, strlen(cwd) + 1 + strlen(path) + 1);
      if (buf == 0)
	{
	  free(cwd);
	  return 0;
	}
    }

  strcat(buf, "/");
  strcat(buf, path);

  return buf;
}

int opendst(const char *dstname, const char *hint)
{
  int fd;

  if (strcmp(dstname, "-") == 0)
    fd = dup(STDOUT_FILENO);
  else
    {
      struct stat sbuf;
      char *path = 0;

      if (stat(dstname, &sbuf) != -1 &&
	  S_ISDIR(sbuf.st_mode))
	{
	  path = malloc(strlen(dstname) + 1 + strlen(hint) + 1);
	  if (path == 0)
	    {
	      ERROR(ENOMEM, 0);
	      return -1;
	    }

	  strcpy(path, dstname);
	  strcat(path, "/");
	  strcat(path, hint);

	  dstname = path;
	}

      fd = open(dstname, O_WRONLY | O_CREAT | O_TRUNC, 0666);

      if (path)
	free(path);
    }

  if (fd == -1)
    {
      ERROR(errno, "error opening destination file");
      return -1;
    }

  return fd;
}

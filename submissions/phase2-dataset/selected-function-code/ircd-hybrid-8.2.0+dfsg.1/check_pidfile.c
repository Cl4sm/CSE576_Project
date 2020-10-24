check_pidfile(const char *filename)
{
  FILE *fb;
  char buff[IRCD_BUFSIZE];
  pid_t pidfromfile;

  /* Don't do logging here, since we don't have log() initialised */
  if ((fb = fopen(filename, "r")))
  {
    if (fgets(buff, 20, fb) == NULL)
    {
      /* log(L_ERROR, "Error reading from pid file %s (%s)", filename,
       * strerror(errno));
       */
    }
    else
    {
      pidfromfile = atoi(buff);

      if (!kill(pidfromfile, 0))
      {
        /* log(L_ERROR, "Server is already running"); */
        printf("ircd: daemon is already running\n");
        exit(-1);
      }
    }

    fclose(fb);
  }
  else if (errno != ENOENT)
  {
    /* log(L_ERROR, "Error opening pid file %s", filename); */
  }
}

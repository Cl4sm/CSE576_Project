write_pidfile(const char *filename)
{
  FILE *fb;

  if ((fb = fopen(filename, "w")))
  {
    char buff[IRCD_BUFSIZE];
    unsigned int pid = (unsigned int)getpid();

    snprintf(buff, sizeof(buff), "%u\n", pid);

    if ((fputs(buff, fb) == -1))
      ilog(LOG_TYPE_IRCD, "Error writing %u to pid file %s (%s)",
           pid, filename, strerror(errno));

    fclose(fb);
  }
  else
  {
    ilog(LOG_TYPE_IRCD, "Error opening pid file %s", filename);
  }
}

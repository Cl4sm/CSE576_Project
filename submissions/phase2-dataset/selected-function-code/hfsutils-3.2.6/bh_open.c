int bh_open(int fd)
{
  int dupfd, c;
  const char *ptr;

  dupfd = dup(fd);
  if (dupfd == -1)
    {
      ERROR(errno, "error duplicating input stream");
      return -1;
    }

  file = fdopen(dupfd, "rb");
  if (file == 0)
    {
      ERROR(errno, "error creating input buffer");

      close(dupfd);
      return -1;
    }

  state86 = 0;
  runlen  = 0;

  crc = 0x0000;

  /* find hqx header */

  ptr = hqxheader;
  while (ptr == 0 || ptr - hqxheader < HEADERMATCH)
    {
      c = getc(file);
      if (c == EOF)
	{
	  ERROR(EINVAL, "hqx file header not found");

	  fclose(file);
	  return -1;
	}

      if (c == '\n' || c == '\r')
	{
	  ptr = hqxheader;
	  continue;
	}

      if (ptr && c != *ptr++)
	ptr = 0;
    }

  /* skip to CR/LF */

  do
    {
      c = getc(file);
      if (c == EOF)
	{
	  ERROR(EINVAL, "corrupt hqx file");

	  fclose(file);
	  return -1;
	}
    }
  while (c != '\n' && c != '\r');

  /* skip whitespace */

  do
    {
      c = getc(file);
      if (c == EOF)
	{
	  ERROR(EINVAL, "corrupt hqx file");

	  fclose(file);
	  return -1;
	}
    }
  while (ISRETURN(c));

  if (c != ':')
    {
      ERROR(EINVAL, "corrupt hqx file");

      fclose(file);
      return -1;
    }

  return 0;
}

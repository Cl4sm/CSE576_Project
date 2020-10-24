int bh_start(int fd)
{
  int dupfd;

  dupfd = dup(fd);
  if (dupfd == -1)
    {
      ERROR(errno, "error duplicating output stream");
      return -1;
    }

  file = fdopen(dupfd, "ab");
  if (file == 0)
    {
      ERROR(errno, "error creating output buffer");

      close(dupfd);
      return -1;
    }

  line[0] = ':';
  lptr = 1;

  state86 = 0;
  runlen  = 0;

  crc = 0x0000;

  if (fputs(hqxheader, file) == EOF)
    {
      ERROR(EIO, "error writing hqx header");

      fclose(file);
      return -1;
    }

  return 0;
}

int bh_end(void)
{
  int result = 0;

  if (runlen &&
      rleflush() == -1)
    result = -1;

  if (state86 && result == 0 &&
      addchars(zero, 1) == -1)
    result = -1;

  line[lptr++] = ':';

  if (result == 0 &&
      flushline() == -1)
    result = -1;

  if (fclose(file) == EOF && result == 0)
    {
      ERROR(errno, "error flushing output data");
      result = -1;
    }

  return result;
}

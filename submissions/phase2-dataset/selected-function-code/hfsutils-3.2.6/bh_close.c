{
  int c, result = 0;

  /* skip whitespace */

  do
    c = getc(file);
  while (c != EOF && ISRETURN(c));

  /* skip optional exclamation */

  if (c == '!')
    {
      do
	c = getc(file);
      while (c != EOF && ISRETURN(c));
    }

  /* verify trailing colon */

  if (c != ':')
    {
      ERROR(EINVAL, "corrupt end of hqx file");
      result = -1;
    }

  if (fclose(file) == EOF && result == 0)
    {
      ERROR(errno, "error closing input file");
      result = -1;
    }

  return result;
}

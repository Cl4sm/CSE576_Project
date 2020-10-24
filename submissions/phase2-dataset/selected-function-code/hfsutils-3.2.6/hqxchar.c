int hqxchar(void)
{
  int c;

  do
    c = getc(file);
  while (c != EOF && ISRETURN(c));

  if (c == EOF)
    {
      if (feof(file))
	ERROR(EINVAL, "unexpected end of file");
      else
	ERROR(EIO, "error reading input file");

      return -1;
    }

  c = demap[(unsigned char) c];
  if (c == 0)
    {
      ERROR(EINVAL, "illegal character in hqx file");
      return -1;
    }

  return c - 1;
}

write_string(const char *s, struct dbFILE *f)
{
  uint32_t len = 0;

  if (!s)
    return write_uint32(0, f);

  len = strlen(s);

  if (len > 4294967294)
    len = 4294967294;
  if (write_uint32(len + 1, f) < 0)
    return -1;
  if (len > 0 && fwrite(s, 1, len, f->fp) != len)
    return -1;
  if (fputc(0, f->fp) == EOF)
    return -1;

  return 0;
}

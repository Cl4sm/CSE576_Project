read_string(char **ret, struct dbFILE *f)
{
  char *s = NULL;
  uint32_t len = 0;

  if (read_uint32(&len, f) < 0)
    return -1;

  if (len == 0)
  {
    *ret = NULL;
    return 0;
  }

  s = MyCalloc(len);

  if (len != fread(s, 1, len, f->fp))
  {
    MyFree(s);
    return -1;
  }

  *ret = s;
  return 0;
}

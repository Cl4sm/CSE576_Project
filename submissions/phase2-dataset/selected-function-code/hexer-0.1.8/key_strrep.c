  static char *
key_strrep(const int *ks)
{
  int n;
  const int *i;
  char *s, *t;
  int key;

  for (n = 0, i = ks; *i; ++i) {
    if (*i == MAP_ESC)
      key = *++i + HXKEY_BIAS;
    else
      key = *i;
    n += strlen(tio_keyrep(key));
  }
  s = t = (char *)malloc(n + 1);
  for (i = ks; *i; ++i) {
    if (*i == MAP_ESC)
      key = *++i + HXKEY_BIAS;
    else
      key = *i;
    strcpy(t, tio_keyrep(key)); t += strlen(t);
  }
  return s;
}

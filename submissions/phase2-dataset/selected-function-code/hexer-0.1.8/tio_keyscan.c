tio_keyscan(key, s, mode)
  int *key;
  char *s;
  int mode;
  /* Check if `s' is a sting representation of a key.
   * the keycode is written to `*key' and a pointer to the first
   * character after the srep is returned.
   * mode & 1:  scan for escapes starting with a `^'.
   * mode & 2:  scan for escapes starting with a `~'.
   * mode == 0:  scan for all known escapes.
   */
{
  int i;

  if (!mode) mode = ~0;
  if ((mode & 1) && *s == '^' && s[1]) {
    if (s[1] == '?') *key = (int)HXKEY_DELETE;
    if (!(*key = s[1] & 0x1f)) *key = (int)HXKEY_NULL;
    return s + 2;
  }
  if ((mode & 2) && *s == '~')
    for (i = 0; t_keys[i].key; ++i) {
      if (!strncmp(s, t_keys[i].srep, strlen(t_keys[i].srep))) {
        *key = (int)t_keys[i].key;
        return s + strlen(t_keys[i].srep);
      }
    }
  *key = *s;
  return s + !!*s;
}

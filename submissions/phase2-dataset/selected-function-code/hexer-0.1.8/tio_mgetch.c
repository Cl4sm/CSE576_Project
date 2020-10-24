tio_mgetch(map, map_string)
  int map;
  char *map_string;
{
  int key;
  struct map_s *i;
  char *s = map_string;
  int ungets[MAP_MAXLEN], ungets_n = 0;
  int k;
  int prefix;

  if (*tio_interrupt) {
    *tio_interrupt = 0;
    return (int)HXKEY_BREAK;
  }
  while (map_max < map) map_first[++map_max] = 0;
  if (map_string) {
    /* create the `map_string' (a string representation of the unmapped keys
     * typed so far)  */
    if (keys_n)
      for (k = 0; k < keys_n; ++k) {
        strcpy(s, tio_keyrep(keys[k]));
        s += strlen(s);
      }
    else
      *map_string = 0;
  }
  if (tio_getmore()) return tio_getch();
  if (munget_n) return munget[--munget_n];
  key = tio_getch();
  if (key == (int)HXKEY_ERROR) return (int)HXKEY_ERROR;
  if (key == (int)HXKEY_ESCAPE && !keys_n)
    if (!tio_readmore()) return (int)HXKEY_ESCAPE;
  if (key <= 0) return key;
  keys[keys_n++] = key;
  keys[keys_n] = 0;
  for (;;) {
    /* check if `keys' matches */
    for (i = map_first[map]; i; i = i->next)
      if (!key_strcmp(keys, i->from)) { /* match */
        if (ungets_n) {
          ungets[ungets_n] = 0;
          mungets(ungets);
          ungets[ungets_n = 0] = 0;
        }
        keys[keys_n = 0] = 0;
        mungets(i->to);
        if (map_string) *map_string = 0;
        return tio_mgetch(map, map_string);
      }
    /* check if `keys' is a prefix */
    for (i = map_first[map], prefix = 0; i; i = i->next)
      if (!key_strncmp(keys, i->from, keys_n)) { prefix = 1; break; }
    if (!prefix) {
      ungets[ungets_n++] = keys[0];
      if (!--keys_n) break;
      for (k = 0; k < keys_n; ++k) keys[k] = keys[k + 1];
      continue;
    } else
      break;
  }
  if (ungets_n) {
    ungets[ungets_n] = 0;
    mungets(ungets);
    ungets[ungets_n = 0] = 0;
  }
  if (s) {
    strcpy(s, tio_keyrep(key));
    s += strlen(s);
  }
  if (munget_n) return munget[--munget_n];
  return (int)HXKEY_NONE;
}

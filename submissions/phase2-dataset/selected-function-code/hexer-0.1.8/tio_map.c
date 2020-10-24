tio_map(map, from, to, special_f)
  int map;
  char *from, *to;
  int special_f;
  /* define a key mapping from `from' to `to' in keymap `map'.  if `special_f'
   * is set, special characters may also be written in ascii (i.e. "~UP" for
   * cursor up).  `special_f' should be set when reading commands from a file.
   */
{
  struct map_s *i, *j;
  int *kfrom, *kto;

  kfrom = (int *)alloca((strlen(from) + 1) * sizeof(int));
  if (special_f)
    scan_keys(kfrom, from, 0);
  else
    string_to_keys(kfrom, from);
  kto = (int *)alloca((strlen(to) + 1) * sizeof(int));
  if (special_f)
    scan_keys(kto, to, 0);
  else
    string_to_keys(kto, to);
  while (map_max < map) map_first[++map_max] = 0;
  for (i = map_first[map], j = 0; i; j = i, i = i->next)
    if (!key_strcmp(kfrom, i->from)) {
      key_strcpy(i->to, kto);
      return 0;
    }
  i = (struct map_s *)malloc(sizeof(struct map_s));
  i->next = 0;
  key_strcpy(i->from, kfrom);
  key_strcpy(i->to, kto);
  if (j) j->next = i; else map_first[map] = i;
  return 0;
}

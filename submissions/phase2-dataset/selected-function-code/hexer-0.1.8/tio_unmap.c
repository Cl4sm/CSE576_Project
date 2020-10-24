tio_unmap(map, from, special_f)
  int map;
  char *from;
  int special_f;
  /* remove the mapping `from' from the keymap `map'.  the flag `special_f'
   * works the same as in `tio_map()'.
   */
{
  struct map_s *i, *j;
  int *kfrom;

  kfrom = (int *)alloca((strlen(from) + 1) * sizeof(int));
  if (special_f)
    scan_keys(kfrom, from, 0);
  else
    string_to_keys(kfrom, from);
  if (map > map_max) return -1;
  for (i = map_first[map], j = 0; i; j = i, i = i->next)
    if (!key_strcmp(i->from, kfrom)) {
      if (j) j->next = i->next; else map_first[map] = i->next;
      free((char *)i);
      return 0;
    }
  return -1;
}

tio_maplist(map)
  int map;
{
  int n, k;
  struct map_s *i;
  char **from, **to, **list;
  int from_maxwidth;

  for (i = map_first[map], n = 0; i; i = i->next, ++n);
  if (!n) {
    *(list = (char **)malloc(sizeof(char *))) = 0;
    return list;
  }
  from = (char **)alloca(n * sizeof(char *));
  to = (char **)alloca(n * sizeof(char *));
  list = (char **)malloc((n + 1) * sizeof(char *));
  for (i = map_first[map], k = 0; i; i = i->next, ++k) {
    from[k] = key_strrep(i->from);
    to[k] = key_strrep(i->to);
  }
  from_maxwidth = strlen(from[0]);
  for (k = 1; k < n; ++k)
    if (strlen(from[k]) > from_maxwidth) from_maxwidth = strlen(from[k]);
  from_maxwidth += 2;
  for (k = 0; k < n; ++k) {
    list[k] = (char *)malloc(from_maxwidth + strlen(to[k]) + 2);
    strcpy(list[k], from[k]);
    memset(list[k] + strlen(list[k]), ' ', from_maxwidth - strlen(list[k]));
    strcpy(list[k] + from_maxwidth, to[k]);
    list[k][strlen(list[k]) + 1] = 0;
    list[k][strlen(list[k])] = '\n';
    free((char *)from[k]);
    free((char *)to[k]);
  }
  list[n] = 0;
  return list;
}

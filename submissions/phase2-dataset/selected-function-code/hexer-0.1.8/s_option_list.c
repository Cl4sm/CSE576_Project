  char **
s_option_list(prefix, bool_only)
  const char *prefix;
  int bool_only;
{
  struct option_s *i;
  char **list;
  int n;

  for (i = option_first, n = 0; i; i = i->next)
    if (!strncmp(i->option, prefix, strlen(prefix))) ++n;
  list = (char **)malloc((n + 1) * sizeof(char *));
  for (i = option_first, n = 0; i; i = i->next)
    if (!strncmp(i->option, prefix, strlen(prefix)))
      if (!bool_only || i->type == S_BOOL) {
        list[n] = (char *)malloc(strlen(i->option) + 1);
        strcpy(list[n], i->option);
        ++n;
      }
  list[n] = 0;
  return list;
}

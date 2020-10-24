s_option_value_list()
{
  struct option_s *i;
  char **list;
  int n, option_maxlen = 0;

  for (i = option_first, n = 0; i; i = i->next, ++n)
    if (strlen(i->option) > option_maxlen) option_maxlen = strlen(i->option);
  list = (char **)malloc((n + 1) * sizeof(char *));
  for (i = option_first, n = 0; i; i = i->next, ++n) {
    list[n] = (char *)malloc(option_maxlen + 4
                             + strlen(s_get_option(i->option)));
    memset(list[n], ' ', option_maxlen + 2);
    strcpy(list[n], i->option);
    list[n][strlen(list[n])] = ' ';
    strcpy(list[n] + option_maxlen + 2, s_get_option(i->option));
    strcpy(list[n] + strlen(list[n]), "\n");
  }
  list[n] = 0;
  return list;
}

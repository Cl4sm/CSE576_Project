rl_complete(int context, int again)
{
  char **list;
  char prefix[1024];
  char command[1024];
  char line[1024];
  char *p;
  int i, j;
  int prefix_len;
  int stop_f = 0;

  if (!completer) return 0;
  strcpy(line, rl.line);
  line[rl_position] = 0;
  for (i = 0, p = rl.line + rl_position;
       p > rl.line ? (p[-1] != ' ' && p[-1] != '\t') : 0;
       --p, ++i);
  if (i) { strncpy(prefix, p, i); prefix[i] = 0; } else *prefix = 0;
  prefix_len = strlen(prefix);
  while (p > rl.line) {
    if (p[-1] != ' ' && p[-1] != '\t') break;
    --p;
  }
  if (p == rl.line)
    /* the word typed is the first word in the line, so we'll complete
     * a command. */
    list = completer(prefix, "", line, context);
  else {
    for (i = 0; p > rl.line ? isalpha(p[-1]) : 0; --p, ++i);
    if (i) { strncpy(command, p, i); command[i] = 0; } else *command = 0;
    list = completer(prefix, command, line, context);
  }
  if (*list) { /* found at least one completion */
    if (list[1]) { /* more than one completion */
      char c;
      int len;
      /* find the longest unique prefix of all completions */
      for (i = 0, len = strlen(prefix); i < len; ++i)
        for (j = 0; list[j + 1]; ++j) assert(list[j][i] == list[j + 1][i]);
      for (stop_f = 1;; ++i, stop_f = 0) {
	if (!(c = list[0][i])) break;
        for (j = 1; list[j] ? list[j][i] == c : 0; ++j);
        if (list[j]) break;
        prefix[i] = c;
      }
      prefix[i] = 0;
      if (stop_f) {
	if (again) {
	  /* the completer-key has been hit twice on the same position.
	   * print out a completion list. */
	  int k, l, m, n;
	  char **list2;
	  rl_redisplay = 1;
	  for (n = 0; list[n]; ++n);
	  if (n > 99) {
	    char query[256];
	    int yn;
	    sprintf(query, "\nthere are %i possibilities - list them all", n);
	    yn = rl_query_yn(query, 0);
	    if (yn <= 0) goto dont_list;
	  }
	  /* find the maximum length of a completion */
	  for (i = l = 0; list[i]; ++i) if ((j = strlen(list[i])) > l) l = j;
	  m = hx_columns / ++l; /* number of hx_columns */
	  m += !m;
          util_strsort(list);
	  k = (n - 1) / m + 1; /* number of lines */
	  /* rearrange the list sorted in hx_columns */
	  list2 = (char **)malloc(m * k * sizeof(char *));
	  memset(list2, 0, m * k * sizeof(char *));
	  for (i = 0, j = 0; list[i]; ++i) {
	    list2[m * (i % k) + j] = list[i];
	    if (!((i + 1) % k)) ++j;
	  }
	  /* display the list */
	  for (i = 0; i < m * k; ++i) {
	    if (!(i % m)) tio_putchar('\n');
	    if (list2[i]) {
	      tio_printf("%s", list2[i]);
	      j = strlen(list2[i]);
	    } else
	      j = 0;
	    for (; j < l; ++j) tio_putchar(' ');
	  }
	  free((char *)list2);
dont_list:
	  tio_putchar('\n');
	  tio_return();
	  tio_puts(rl_prompt);
	  rl_display_line(0);
	} else
	  tio_bell();
      } else {
	/* comlpete as far as unique */
	int k = strlen(prefix);
	assert(k > prefix_len);
	for (i = strlen(rl.line) + (j = k - prefix_len);
             i >= rl_position;
             --i)
	  rl.line[i + j] = rl.line[i];
	for (i = 0; i < j; ++i)
	  rl.line[rl_position + i] = prefix[prefix_len + i];
	rl_position += j;
	while (rl_position - rl_offset + 1 >= hx_columns - rl_prompt_len)
	  ++rl_offset;
	tio_return();
	tio_puts(rl_prompt);
	rl_display_line(0);
	tio_bell();
      }
    } else { /* exactly one completion */
      int dir_f;
      j = strlen(*list) - prefix_len + 1;
      dir_f = (*list)[strlen(*list) - 1] == '/';
      if (dir_f) --j;
      for (i = strlen(rl.line) + j; i >= rl_position; --i)
	rl.line[i + j] = rl.line[i];
      for (i = 0; i < j - !dir_f; ++i)
	rl.line[rl_position + i] = list[0][prefix_len + i];
      if (!dir_f) rl.line[rl_position + i] = ' ';
      rl_position += j;
      while (rl_position - rl_offset + 1 >= hx_columns - rl_prompt_len)
	++rl_offset;
      tio_return();
      tio_puts(rl_prompt);
      rl_display_line(0);
    }
  } else /* no match */
    tio_bell();
  for (i = 0; list[i]; ++i) free((char *)list[i]);
  free((char *)list);
  return stop_f;
}

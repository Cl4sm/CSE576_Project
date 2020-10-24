rl_history_init(int max)
  /* initialize readline-history.  set the maximum number of lines to
   * `max' (`rl_history_max').
   */
{
  int i, j;

  for (i = 0; i < RL_MAX_CONTEXTS; ++i) {
    rl_history[i] =
      (struct rl_line_s *)malloc(max * sizeof(struct rl_line_s));
    for (j = 0; j < max; ++j) {
      rl_history[i][j].line = (char *)malloc(1);
      *rl_history[i][j].line = 0;
      rl_history[i][j].vline = (char *)malloc(1);
      *rl_history[i][j].vline = 0;
    }
    rl_history_c[i] = 1;
  }
  rl_history_max = max;
  return 0;
}

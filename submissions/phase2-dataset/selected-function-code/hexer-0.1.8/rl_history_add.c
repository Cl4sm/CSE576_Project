rl_history_add(struct rl_line_s line)
  /* add the line `line' to the history of the current context.
   */
{
  int i;

  if (rl_history_c[rl_current_context] + 1 >= rl_history_max) {
    free((char *)rl_history[rl_current_context][0].line);
    for (i = 0; i < rl_history_max - 1; ++i)
      rl_history[rl_current_context][i] =
        rl_history[rl_current_context][i + 1];
    rl_history[rl_current_context][i].line =
      (char *)malloc(strlen(line.line) + 1);
  } else {
    i = rl_history_c[rl_current_context] - 1;
    ++rl_history_c[rl_current_context];
    rl_history[rl_current_context][i].line =
      (char *)realloc(rl_history[rl_current_context][i].line,
                      strlen(line.line) + 1);
  }
  strcpy(rl_history[rl_current_context][i].line, line.line);
}

  static int
rl_display_line(int clear_to_eol)
{
  char line[1024];

  if (clear_to_eol) {
    tio_return();
    if (rl_prompt) tio_printf("%s", rl_prompt);
  } else
    tio_goto_column(rl_prompt_len);
  if (clear_to_eol) tio_clear_to_eol();
  rl_make_vline();
  strncpy(line, rl.vline + rl_offset, hx_columns - rl_prompt_len);
  line[hx_columns - rl_prompt_len - 1] = 0;
  if (rl_offset) tio_printf(RL_LEFT_MORE);
  if (strlen(rl.vline) - rl_offset >= hx_columns - rl_prompt_len)
    line[hx_columns - rl_prompt_len - 2] = 0;
  tio_raw_printf("%s", line + !!rl_offset);
  if (rl_get_vlength(&rl) - rl_offset >= hx_columns - rl_prompt_len)
    tio_printf(RL_RIGHT_MORE);
  tio_goto_column(rl_prompt_len + rl_get_vposition() - rl_offset);
  return 0;
}

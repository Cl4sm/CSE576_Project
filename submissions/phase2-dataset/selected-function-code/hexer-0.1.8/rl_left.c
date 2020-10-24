rl_left(void)
{
  int last_col;
  int position;
  int vposition;
  int skip;
  int key;
  int redisplay_f = 0;

  if (!rl_position) return 0;
  --rl_position;
  position = rl_get_position();
  if (rl.line[position] == RL_ESC) {
    key = rl.line[position + 1] + HXKEY_BIAS;
    if (key == 0xff) key = (int)HXKEY_ESCAPE;
      /* translate `RL_ESC' to `HXKEY_ESCAPE' */
    skip = strlen(tio_keyrep(key));
  } else
    skip = strlen(tio_keyrep(rl.line[position]));
  while (skip--) {
    vposition = rl_get_vposition();
    if (vposition < rl_offset + 1) {
      --rl_offset;
      if (rl_offset < 0) {
	rl_offset = 0;
	tio_left(1);
      } else {
	if (!rl_offset) {
	  tio_goto_column(rl_prompt_len);
	  tio_putchar(rl.vline[0]);
	} else
	  tio_goto_column(rl_prompt_len + 1);
	if (tio_insert_character(rl.vline[rl_offset + 1])) redisplay_f = 1;
      }
      last_col = rl_prompt_len + strlen(rl.vline) - rl_offset;
      if (last_col > hx_columns - 1) {
	last_col = hx_columns - 1;
	tio_goto_column(last_col - 2);
	tio_clear_to_eol();
	tio_putchar(rl.vline[rl_offset + hx_columns + skip - rl_prompt_len - 3]);
	tio_printf(RL_RIGHT_MORE);
	tio_goto_column(rl_prompt_len + vposition + skip - rl_offset);
      } else if (last_col == hx_columns - 1) {
	tio_goto_column(last_col - 2);
	tio_clear_to_eol();
	tio_putchar(rl.vline[rl_offset + hx_columns + skip - rl_prompt_len - 3]);
	tio_putchar(rl.vline[rl_offset + hx_columns + skip - rl_prompt_len - 2]);
	tio_goto_column(rl_prompt_len + vposition + skip - rl_offset);
      }
    } else
      tio_left(1);
  };
  if (redisplay_f) rl_display_line(1);
  return 0;
}
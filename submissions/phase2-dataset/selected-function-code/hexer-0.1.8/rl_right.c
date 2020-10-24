  static int
rl_right(void)
{
  int last_col;
  int append = 0;
  int redisplay_f = 0;
  int skip;
  int position;
  int vposition;
  int key;

  if (rl_position == rl_get_length(&rl)) return 0;
  position = rl_get_position();
  if (rl.line[position] == RL_ESC) {
    key = rl.line[position + 1] + HXKEY_BIAS;
    if (key == 0xff) key = (int)HXKEY_ESCAPE;
    skip = strlen(tio_keyrep(key));
  } else
    skip = strlen(tio_keyrep(rl.line[position]));
  ++rl_position;
  position = rl_get_position();
  vposition = rl_get_vposition();
  while (skip--) {
    append = (position == strlen(rl.line));
    if (vposition - rl_offset + 1 >= hx_columns - rl_prompt_len + append) {
      ++rl_offset;
      tio_goto_column(rl_prompt_len);
      if (tio_delete_character()) redisplay_f = 1;
      else {
	tio_printf(RL_LEFT_MORE);
	last_col = rl_prompt_len + strlen(rl.vline) - rl_offset;
	if (last_col > hx_columns - 1) {
	  last_col = hx_columns - 1;
	  tio_goto_column(last_col - 2);
	  tio_clear_to_eol();
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3]);
	  tio_printf(RL_RIGHT_MORE);
	  tio_goto_column(rl_prompt_len + vposition - rl_offset);
	} else if (last_col == hx_columns - 1) {
	  tio_goto_column(last_col - 2);
	  tio_clear_to_eol();
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3]);
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 2]);
	  tio_left(1);
	} else {
	  tio_goto_column(last_col - 1);
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3]);
	}
      }
    } else
      tio_right(1);
  };
  if (redisplay_f) rl_display_line(1);
  return 0;
}

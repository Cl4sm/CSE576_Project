  static int
rl_insert(int x)
{
  int i;
  int last_col;
  int position = rl_get_position();
  int vposition;
  int append = (position == strlen(rl.line));
  char *s = strdup(tio_keyrep(x));
  int sl = strlen(s);
  int special_f = 0;
  int redisplay_f = 0;

  if (!tio_isprint(x) || x == RL_ESC || !x) special_f = 1;
  for (i = strlen(rl.line) - special_f; i >= position; --i)
    rl.line[i + 1 + special_f] = rl.line[i];
  if (special_f && (x == RL_ESC || x > 0xff || !x)) {
    rl.line[position] = RL_ESC;
    rl.line[position + 1] =
      (char)(x ? x == RL_ESC ? 1 : x - HXKEY_BIAS : HXKEY_NULL - HXKEY_BIAS);
  } else
    rl.line[position] = (char)x;
  rl_make_vline();
  vposition = rl_get_vposition();
  for (i = 0; i < sl; ++i) {
    if (vposition - rl_offset + 2 >= hx_columns - rl_prompt_len + append) {
      ++rl_offset;
      ++vposition;
      tio_goto_column(rl_prompt_len);
      if (tio_delete_character())
        redisplay_f = 1;
      else {
	tio_printf(RL_LEFT_MORE);
	last_col = rl_prompt_len + strlen(rl.vline) - rl_offset;
	if (last_col > hx_columns - 1) {
	  last_col = hx_columns - 1;
          if (!append) {
            tio_goto_column(last_col - 2);
            tio_clear_to_eol();
            tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3]);
            tio_printf(RL_RIGHT_MORE);
          } else {
            tio_goto_column(last_col - 1);
            tio_clear_to_eol();
            tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 2]);
          }
          tio_goto_column(rl_prompt_len + vposition - rl_offset);
	} else {
	  tio_goto_column(last_col - 1);
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 2]);
	}
      }
    } else {
      ++vposition;
      if (tio_insert_character(s[i]))
	redisplay_f = 1;
      else {
	last_col = rl_prompt_len + strlen(rl.vline) - rl_offset;
	if (last_col > hx_columns - 1) {
	  last_col = hx_columns - 1;
          if (!append) {
            tio_goto_column(last_col - 1);
            tio_clear_to_eol();
            tio_printf(RL_RIGHT_MORE);
          } else {
            tio_goto_column(last_col);
            tio_clear_to_eol();
          }
	  tio_goto_column(rl_prompt_len + vposition - rl_offset);
	}
      }
    }
  }
  ++rl_position;
  if (redisplay_f) rl_display_line(1);
  free(s);
  return 0;
}

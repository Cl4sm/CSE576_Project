  static void
rl_verbatim(void)
{
  int key;
  int position = rl_get_position();
  int append = (position == strlen(rl.line));
  int vposition = rl_get_vposition();
  int last_col;

  if (vposition - rl_offset + 2 >= hx_columns - rl_prompt_len + append) {
    ++rl_offset;
    tio_goto_column(rl_prompt_len);
    if (tio_delete_character())
      rl_display_line(1);
    else {
      tio_printf(RL_LEFT_MORE);
      last_col = rl_prompt_len + strlen(rl.vline) - rl_offset;
      if (last_col > hx_columns - 1) {
        last_col = hx_columns - 1;
        tio_goto_column(last_col - 1);
        tio_clear_to_eol();
        tio_printf(RL_RIGHT_MORE);
        tio_goto_column(rl_prompt_len + vposition - rl_offset);
      } else {
        tio_goto_column(last_col);
        tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 2]);
      }
    }
  }
  tio_printf("^@m-000-001");
restart:
  if ((key = tio_mgetch(MAP_EXH, 0)) == (int)HXKEY_ERROR) {
    if (window_changed) {
      tio_ungetch('v' & 0x1f);
      tio_ungetch(HXKEY_ERROR);
      return;
    } else
      goto restart;
  }
  rl_make_vline();
  vposition = rl_get_vposition();
  tio_printf("%c@m-000-001", rl.vline[vposition] ? rl.vline[vposition] : ' ');
  rl_insert(key);
}

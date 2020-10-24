rl_delete(int under_cursor)
{
  int i;
  int last_col;
  int special_f = 0;
  int replen;
  int position = rl_get_position();
  int vposition;
  int key;

  if (!under_cursor && !position) return 0;
  if ((under_cursor && position < strlen(rl.line)) || !under_cursor) {
    if (!under_cursor) --rl_position;
    position = rl_get_position();
    if (rl.line[position] == RL_ESC) {
      key = rl.line[position + 1] + HXKEY_BIAS;
      if (key == 0xff) key = (int)HXKEY_ESCAPE;
      replen = strlen(tio_keyrep(key));
      special_f = 1;
    } else
      replen = strlen(tio_keyrep(rl.line[position]));
    for (i = position + 1 + special_f; i <= strlen(rl.line); ++i) 
      rl.line[i - 1 - special_f] = rl.line[i];
  } else
    return 0;

  rl_make_vline();
  vposition = rl_get_vposition();
  if (!under_cursor) {
    vposition += replen;
    while (replen--) {
      --vposition;
      if (vposition < rl_offset + 1 + !!rl_backspace_jump) {
	if (rl_backspace_jump && rl_offset) {
	  rl_offset -= rl_backspace_jump;
	  if (rl_backspace_jump == 1) {
	    if (rl_offset < 0) {
	      rl_offset = 0;
	      goto l1;
	    }
	    tio_goto_column(rl_prompt_len + 1);
	    tio_putchar(rl.vline[rl_offset + 1]);
	  } else {
	    if (rl_offset < 0) rl_offset = 0;
	    rl_display_line(0);
	  }
	} else {
	  --rl_offset;
	  if (rl_offset < 0) {
	    rl_offset = 0;
	    goto l1;
	  }
	}
      } else {
  l1:   tio_goto_column(rl_prompt_len + vposition - rl_offset);
        if (tio_delete_character())
          rl_display_line(1);
	last_col = rl_prompt_len + strlen(rl.vline) + replen - rl_offset;
	if (last_col > hx_columns - 1) {
	  last_col = hx_columns - 1;
	  tio_goto_column(last_col - 2);
	  tio_clear_to_eol();
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3
                               - replen]);
	  tio_printf(RL_RIGHT_MORE);
	} else if (last_col == hx_columns - 1) {
	  tio_goto_column(last_col - 2);
	  tio_clear_to_eol();
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3
                               - replen]);
	  tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 2
                               - replen]);
	} else if (rl_offset) {
	  --rl_offset;
	  if (!rl_offset) {
	    tio_goto_column(rl_prompt_len);
	    tio_putchar(rl.vline[0]);
	  } else
	    tio_goto_column(rl_prompt_len + 1);
	  if (tio_insert_character(rl.vline[rl_offset + 1]))
            rl_display_line(1);
	}
	tio_goto_column(rl_prompt_len + vposition - rl_offset);
      }
    }
  } else {
    while (replen--) {
      tio_goto_column(rl_prompt_len + vposition - rl_offset);
      if (tio_delete_character())
        rl_display_line(1);
      last_col = rl_prompt_len + strlen(rl.vline) - rl_offset;
      if (last_col > hx_columns - 1) {
	last_col = hx_columns - 1;
	tio_goto_column(last_col - 2);
	tio_clear_to_eol();
	tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3
                             - replen]);
	tio_printf(RL_RIGHT_MORE);
      } else if (last_col == hx_columns - 1) {
	tio_goto_column(last_col - 2);
	tio_clear_to_eol();
	tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 3
                             - replen]);
	tio_putchar(rl.vline[rl_offset + hx_columns - rl_prompt_len - 2
                             - replen]);
      } else if (rl_offset) {
	--rl_offset;
	if (!rl_offset) {
	  tio_goto_column(rl_prompt_len);
	  tio_putchar(rl.vline[0]);
	} else
	  tio_goto_column(rl_prompt_len + 1);
	if (tio_insert_character(rl.vline[rl_offset + 1]))
          rl_display_line(1);
      }
      tio_goto_column(rl_prompt_len + vposition - rl_offset);
    }
  }
  rl_make_vline();
  return 0;
}

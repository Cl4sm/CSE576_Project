  char *
readline(prompt, default_val, context)
  char *prompt;
  char *default_val;
  int context;
{
  int key;
  int escape_f = 0;
  int echo_state;
  struct rl_line_s *hist;
  static int history_initialized = 0;
  int stop_f = 0;

  rl_redisplay = 0;
  if (!history_initialized) rl_history_init(16), history_initialized = 1;
  rl_history_reset(context);

  tio_keypad(1);
  echo_state = tio_echo(0);
  tio_return();
  tio_clear_to_eol();
  tio_normal();
  tio_set_cursor(1);
  if (prompt) tio_printf("%s", prompt);
  memset(rl.line, 0, LINE_MAXLEN);
  rl_prompt = prompt;
  rl_prompt_len = strlen(prompt);
  rl_position = 0;
  rl_redisplay = 0;
  if (default_val ? *default_val : 0) {
    rl_default = default_val;
    strncpy(rl.line, default_val, LINE_MAXLEN - 1);
    rl_make_vline();
    if (rl_prompt_len + rl_position > hx_columns - 2)
      rl_offset = hx_columns - 1 - rl_prompt_len - strlen(rl.vline);
    tio_raw_printf("%s", rl.line + rl_offset);
    tio_goto_column(rl_prompt_len + strlen(rl.vline) - rl_offset);
  }
  for (;;) {
    key = tio_mgetch(MAP_EXH, 0);
    switch (key) {
    case 'a' & 0x1f: /* C-a */
      rl_begin();
      break;
    case 'e' & 0x1f: /* C-e */
      rl_end();
      break;
    case 'k' & 0x1f: /* C-k */
      rl_ck();
      break;
    case 'u' & 0x1f: /* C-u */
      rl_cu();
      break;
    case HXKEY_LEFT:
      rl_left();
      break;
    case HXKEY_RIGHT:
      rl_right();
      break;
    case HXKEY_UP:
      rl_history_set(rl);
      if (!(hist = rl_history_up())) break;
      strcpy(rl.line, hist->line);
      rl_offset = 0;
      if (rl_prompt_len + rl_position > hx_columns - 2)
        rl_offset = rl_prompt_len + strlen(rl.vline) - hx_columns + 1;
      rl_display_line(1);
      break;
    case HXKEY_DOWN:
      rl_history_set(rl);
      if (!(hist = rl_history_down())) break;
      strcpy(rl.line, hist->line);
      rl_offset = 0;
      if (rl_prompt_len + rl_position > hx_columns - 2)
        rl_offset = hx_columns - 2 - rl_prompt_len - strlen(rl.vline);
      rl_display_line(1);
      break;
    case HXKEY_ESCAPE:
      escape_f = 1;
      /* fall through */
    case HXKEY_RETURN:
      tio_return();
      tio_flush();
      goto exit_readline;
    case HXKEY_BACKSPACE:
    case HXKEY_DELETE:
      if (!*rl.line && rl_cancel_on_bs) {
        escape_f = 1;
        goto exit_readline;
      }
      rl_delete(0);
      break;
    case 'd' & 0x1f: /* C-d */
      rl_delete(1);
      break;
    case 'l' & 0x1f: /* C-l */
      rl_display_line(1);
      break;
    case 'v' & 0x1f: /* C-v */
      rl_verbatim();
      break;
    case HXKEY_TAB:
      stop_f = rl_complete(context, stop_f);
      if (stop_f) continue;
      break;
    case HXKEY_ERROR:
      if (window_changed) {
        rl_winch();
        tio_goto_line(hx_lines - 1);
        tio_return();
        tio_clear_to_eol();
        rl_display_line(1);
        window_changed = 0;
      }
      break;
    default:
      if (tio_isprint(key)) rl_insert((char)key);
      break;
    }
    stop_f = 0;
  }
exit_readline:
  if (!escape_f && *rl.line) rl_history_add(rl);
  tio_echo(echo_state);
  return escape_f ? 0 : rl.line;
}

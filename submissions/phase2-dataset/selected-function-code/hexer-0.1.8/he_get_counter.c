he_get_counter(hedit)
  struct he_s *hedit;
  /* Read a counter from the keyboard.  If this is terminated by pressing
   * <esc>, the return value is -1.  If another non-digit key is read,
   * it is put back via `tio_ungetch()'.  Reading the counter can be
   * terminated by pressing <return>.
   */
{
  long count = -1;
  int key, digit;
  enum mode_e { OCT = 8, DEC = 10, HEX = 16 } mode = DEC;
  char *fmt = 0, *prefix = 0;

  tio_goto_line(hx_lines - 1);
  tio_return();
  tio_clear_to_eol();
  for (;;) {
    key = tio_mgetch(0, 0);
    switch (key) {
    case HXKEY_ERROR:
      if (window_changed) he_refresh_screen(hedit);
      break;
    HE_CASE_COMMAND_INSERT:
      /* The commands that are availabe in insert mode do not affect and
       * are not affected by the counter.
       */
      he_command(hedit, key, 0);
      break;
    case HXKEY_BACKSPACE:
    case HXKEY_DELETE:
      if (count > 0)
        count /= (long)mode;
      else {
        count = -1;
        switch (mode) {
        case DEC: goto exit_get_counter;
        case OCT: mode = DEC; break;
        case HEX: mode = OCT;
        }
      }
      break;
    case HXKEY_RETURN:
      goto exit_get_counter;
    default:
      if (key == '0' && count < 0) {
        mode = OCT;
        break;
      }
      if (key == 'x' && count < 0 && mode == OCT) {
        mode = HEX;
        break;
      }
      digit = he_digit(key);
      if (digit >= 0 && digit < (int)mode) {
        if (count < 0) count = 0;
        count *= (long)mode;
        count += digit;
      } else {
        tio_ungetch(key);
        if (mode == OCT && count < 0) count = 0;
        goto exit_get_counter;
      }
    }
    switch (mode) {
    case DEC: fmt = "arg:%li"; prefix = "arg:"; break;
    case OCT: fmt = "arg:0%lo"; prefix = "arg:0"; break;
    case HEX: fmt = "arg:0x%lx"; prefix = "arg:0x";
    }
    if (count > 0) {
      char arg[256];
      int indent = 60;
      sprintf(arg, fmt, count);
      tio_goto_line(hx_lines - 1);
      if (indent + strlen(arg) > hx_columns - 1)
        indent = hx_columns - 1 - strlen(arg);
      if (indent < 0) indent = 0;
      tio_return();
      tio_right(indent);
      tio_printf(arg);
      tio_clear_to_eol();
    } else {
      int indent = 60;
      tio_goto_line(hx_lines - 1);
      if (indent + strlen(prefix) > hx_columns - 1)
        indent = hx_columns - 1 - strlen(prefix);
      if (indent < 0) indent = 0;
      tio_return();
      tio_right(indent);
      tio_printf(prefix);
      tio_clear_to_eol();
    }
    tio_flush();
  }

exit_get_counter:
  tio_goto_line(hx_lines - 1);
  tio_return();
  tio_clear_to_eol();
  he_set_cursor(hedit);
  return count;
}

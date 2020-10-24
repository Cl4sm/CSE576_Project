  int
he_query_yn(int dfl, char *fmt, ...)
#else
  int
he_query_yn(dfl, fmt, va_alist)
  int dfl;
  char *fmt;
  va_dcl
#endif
{
  va_list ap;
  int key;
  int choice;

#if USE_STDARG
  va_start(ap, fmt);
#else
  va_start(ap);
#endif
  tio_keypad(0);
restart:
  tio_goto_line(hx_lines - 1);
  tio_return();
  tio_clear_to_eol();
  if (dfl < 0) { /* no default answer */
    tio_vprintf(fmt, ap);
    tio_printf("? ");
  } else {
    dfl = !!dfl;
    tio_vprintf(fmt, ap);
    tio_printf("? [%c] ", dfl ? 'y' : 'n');
  }
  for (;;) {
    key = tio_getch();
    switch (key) {
    case HXKEY_NONE:
      if (window_changed) he_refresh_screen(current_buffer->hedit);
      goto restart;
    case 'q': case 'Q':
      tio_printf("quit");
      choice = -1;
      goto exit_he_query_yn;
    case HXKEY_ESCAPE:
      tio_printf("escape");
      choice = -1;
      goto exit_he_query_yn;
    case HXKEY_RETURN:
      if (dfl < 0) break;
      choice = dfl;
      switch (choice) {
      case -1: tio_printf("quit"); break;
      case 0: tio_printf("no"); break;
      case 1: tio_printf("yes"); break;
      case 2: tio_printf("always"); break;
      }
      goto exit_he_query_yn;
    case 'y': case 'Y':
      tio_printf("yes");
      choice = 1;
      goto exit_he_query_yn;
    case 'n': case 'N':
      tio_printf("no");
      choice = 0;
      goto exit_he_query_yn;
    case 'a': case 'A':
      tio_printf("always");
      choice = 2;
      goto exit_he_query_yn;
    default:
      break;
    }
  }

exit_he_query_yn:
  tio_return();
  tio_keypad(1);
  tio_flush();
  va_end(ap);
  return choice;
}

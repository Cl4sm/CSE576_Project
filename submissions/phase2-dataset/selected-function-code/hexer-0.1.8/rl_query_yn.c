rl_query_yn(char *prompt, int dfl)
  /* query the user with a y/n-requester.  `prompt' is the query-prompt.
   * `dfl' is the default answer offered to the user.  the value of `dfl'
   * means:
   *   -1:  no default.
   *    0:  default is no.
   *    1:  default is yes.
   * if `dfl == -1', the return key is ignored.  if case fo a windowchange
   * and if `rl_winch != 0', the function `(*rl_winch)()' is called.
   * return values:
   *   -1:  the ESCAPE-key has been pressed.
   *    0:  no.
   *    1:  yes.
   */
{
  int key;
  int choice;

  tio_keypad(0);
  tio_goto_line(hx_lines - 1);
  tio_return();
  tio_clear_to_eol();
  if (dfl < 0) /* no default answer */
    tio_printf("%s? ", prompt);
  else {
    dfl = !!dfl;
    tio_printf("%s? [%c] ", prompt, dfl ? 'y' : 'n');
  }
  for (;;) {
    key = tio_mgetch(0, 0);
    switch (key) {
      case HXKEY_ERROR:
        if (window_changed && rl_winch) rl_winch();
        window_changed = 0;
        return rl_query_yn(prompt, dfl);
      case HXKEY_ESCAPE:
        tio_printf("escape");
        choice = -1;
	goto exit_rl_query_yn;
      case HXKEY_RETURN:
        if (dfl < 0) break;
	choice = dfl;
	goto exit_rl_query_yn;
      case 'y': case 'Y':
        tio_printf("yes");
        choice = 1;
	goto exit_rl_query_yn;
      case 'n': case 'N':
	tio_printf("no");
	choice = 0;
        goto exit_rl_query_yn;
      default:
        break;
    }
  }

exit_rl_query_yn:
  tio_return();
  tio_keypad(1);
  tio_flush();
  return choice;
}

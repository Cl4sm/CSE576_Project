  static int
he_verbatim(struct he_s *hedit)
{
  int key;

  tio_goto_line(hx_lines - 1);
  tio_return();
  tio_right(60);
  tio_clear_to_eol();
  tio_printf("verbatim");
  he_set_cursor(hedit);
restart:
  if ((key = tio_mgetch(0, 0)) == (int)HXKEY_ERROR) {
    if (window_changed) {
      tio_ungetch('v' & 0x1f);
      tio_ungetch(HXKEY_ERROR);
      tio_goto_line(hx_lines - 1);
      tio_return();
      tio_right(60);
      tio_clear_to_eol();
      return (int)HXKEY_NONE;
    } else
      goto restart;
  }
  tio_goto_line(hx_lines - 1);
  tio_return();
  tio_right(60);
  tio_clear_to_eol();
  return key < 0x100 ? key : (int)HXKEY_NONE;
}

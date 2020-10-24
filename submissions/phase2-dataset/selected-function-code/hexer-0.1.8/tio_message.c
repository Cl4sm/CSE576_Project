tio_message(message, indent)
  char **message;
  int indent;
  /* displays the array of strings `message' via `tio_display()' providing
   * a simple pager.  this function assumes, that every string in
   * `message' displays as a single line.
   */
{
  int i;

  for (i = 1; *message; ++message, ++i) {
    tio_display(*message, indent);
    indent = -1;
    if (i > hx_lines - 2) {
      /* tio_printf(" @Ar -- more -- @~@r"); */
      tio_raw_printf(" [ more ]\r");
      switch (tio_getch()) {
        case HXKEY_DOWN:
        case 'd':
        case 'd' & 0x1f:
          i -= hx_lines / 2;
          break;
        case ' ':
        case 'f' & 0x1f:
          i = 0;
          break;
        case HXKEY_ERROR:
          if (i > hx_lines - 2) i = hx_lines - 2;
          tio_goto_line(i);
          window_changed = 0;
          break;
        case HXKEY_RETURN:
        default:
          --i;
          break;
      }
      tio_clear_to_eol();
    }
  }
}

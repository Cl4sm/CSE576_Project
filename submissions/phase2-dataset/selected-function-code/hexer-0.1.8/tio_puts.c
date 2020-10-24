tio_puts(s)
  char *s;
  /* Like `fputs(s, stdout)'.
   */
{
  char *c1, *c2;
  char *out;
  int mask_f, x;

  if (t_line < 0) tio_goto_line(0);
  if (t_column < 0) tio_goto_column(0);
  c1 = s;
  c2 = out = (char *)malloc(strlen(s) + 1);
  for (; *c1; ++c1) {
    mask_f = 0;
    switch (*c1) {
    case '\n':
      if (++t_line >= hx_lines) t_line = hx_lines - 1;
      t_column = 0;
      break;
    case '\r':
      t_column = 0;
      break;
    case '\b':
      if (!--t_line) t_line = 0;
      break;
    case '\t':
      x = t_column;
      t_column += T_TABSTOP;
      if (x % T_TABSTOP) while (!(t_column % T_TABSTOP)) --t_column;
      if (t_column >= hx_columns) {
        t_column = hx_columns - 1;
        while (x++ < t_column) *c2++ = ' ';
        mask_f = 1;
      }
      break;
    default:
      if (++t_column >= hx_columns) {
        t_column = hx_columns - 1;
        mask_f = 1;
      }
      break;
    }
    if (!mask_f) *c2++ = *c1;
  } /* for */
  *c2 = 0;
  fputs(out, stdout);
  free((char *)out);
  return 0;
}

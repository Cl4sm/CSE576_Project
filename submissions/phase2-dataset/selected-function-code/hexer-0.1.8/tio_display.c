  void
tio_display(text, indent_arg)
  /* Send the string `text' to the terminal.  The string may contain the
   * following `@*'-commands:
   *
   *  @@                   @
   *  @c                   clear screen.
   *  @C                   clear to end of line.
   *  @M[+-]rrr[+-]ccc     absolute move. 
   *                       Move the cursor to position row rrr, column ccc.
   *                       If a value is prefixed by a `-', it is interpreted
   *                       relative to the bottom-right corner of the screen.
   *  @m[+-]rrr[+-]ccc     relative move.
   *  @Iiii                set indent.
   *  @b                   bell.
   *  @v                   flash, visible bell.
   *  @U                   underline on.
   *  @u                   underline off.
   *  @Am                  set attribute m, where m is one of the
   *                       following:
   *                        b  bold.
   *                        l  blink.
   *                        h  half bright.
   *                        r  reverse.
   *                        u  underline (equivalent to `@U').
   *                        s  standout.
   *                        ~  normal.
   *  @r                   return.
   *  @n                   newline.
   *
   * `indent_arg' sets the indent.  If `indent_arg < 0', the indent stays
   * unchanged.
   */
  char *text;
  int indent_arg;
{
  int i, j;
  int lin, col;
  int eol_f, back_f, absolute_f;
  char *s = (char *)malloc(strlen(text) + 1);
  static int indent = 0;
  int maxcol = hx_columns - 1;

  if (indent_arg >= 0) indent = indent_arg;
  for (i = 0; i < indent && t_column < maxcol; ++i) tio_putchar(' ');
    /* Note that `t_column' is incremented by calling `tio_putchar()'. */
  strcpy(s, text);
  for (eol_f = 0, i = 0;;) {
    for (j = 0; s[i] != '@' && s[i]; ++i, ++j);
    if (!s[i]) eol_f = 1;
    s[i] = 0;
    if (s[i - j]) tio_puts(s + (i - j));
    if (eol_f) break;
    absolute_f = 0;
    switch (s[i + 1]) {
      case '@': tio_putchar('@'); break;
      case 'a': tio_bell(); break;
      case 'b': tio_left(1); break;
      case 'c': tio_clear(); break;
      case 'C': tio_clear_to_eol(); break;
      case 'v': tio_visible_bell(); break;
      case 'U': tio_underscore(); break;
      case 'u': tio_underscore_off(); break;
      case 'r': tio_return(); break;
      case 'n':
        tio_putchar('\n');
        for (j = 0; j < indent && t_column < maxcol; ++j) tio_putchar(' ');
          /* Note that `t_column' is incremented by calling `tio_putchar()'. */
        break;
      case 'A':
        if (s[i + 1]) {
          switch (s[i + 2]) {
          case 'b': tio_bold(); break;
          case 'l': tio_blink(); break;
          case 'h': tio_half_bright(); break;
          case 'r': tio_reverse(); break;
          case 'u': tio_underscore(); break;
          case '~': tio_normal(); break;
          default: break;
          }
        }
        ++i;
        break;
      case '~': tio_normal(); break;
      case 'I':
        i += 2;
        j = indent;
        if (!sscanf(s + i, "%03d", &indent)) indent = j;
        if (indent != j) tio_rel_move(0, indent - j);
        ++i;
        break;
      case 'M':
        absolute_f = 1;
      case 'm':
        i += 2;
        back_f = 0;
        if (s[i] == '+')
          ++i;
        else if (s[i] == '-') {
          ++i;
          back_f = 1;
        }
        if (!sscanf(s + i, "%03d", &lin)) lin = 0;
        i += 3;
        if (back_f) lin = -lin;
        back_f = 0;
        if (s[i] == '+')
          ++i;
        else if (s[i] == '-') {
          ++i;
          back_f = 1;
        }
        if (!sscanf(s + i, "%03d", &col)) col = 0;
        i += 3;
        if (back_f) col = -col;
        if (absolute_f) tio_move(lin, col); else tio_rel_move(lin, col);
        i -= 2;
        break;
      default:
        break;
    }
    i += 2;
  }
  tio_normal();
  free((char *)s);
}

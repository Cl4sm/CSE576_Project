tio_scroll_up(count, first, last)
  int count;
  int first, last;
  /* Scroll up (forward) `count' lines.  returns -1 if the terminal
   * can't scroll.
   */
{
  int c;

  assert(count);
  if (last < 0 || last == hx_lines - 1) {
    last = hx_lines - 1;
    goto scroll_all;
  }
  if (tio_set_scrolling_region(first, last)) {
    if ((!t_delete_lines && !t_delete_line) ||
        (!t_insert_lines && !t_insert_line))
      return -1;
    tio_goto_line(first);
    if (!(count == 1 && t_delete_line) && t_delete_lines)
      tio_command(t_delete_lines, hx_lines, count);
    else {
      for (c = count; c--;) tio_command(t_delete_line, hx_lines);
    }
    tio_goto_line(last - count + 1);
    if (!(count == 1 && t_insert_line) && t_insert_lines)
      tio_command(t_insert_lines, hx_lines, count);
    else {
      for (c = count; c--;) tio_command(t_insert_line, hx_lines);
    }
  } else {
    if (t_scroll_fwd || t_scroll_fwd_n) {
        tio_goto_line(last);
      if (!(count == 1 && t_scroll_fwd) && t_scroll_fwd_n)
        tio_command(t_scroll_fwd_n, count, count);
      else
        for (c = count; c--;) tio_command(t_scroll_fwd, 1);
    } else {
scroll_all:
      if (t_delete_line || t_delete_lines) {
        tio_goto_line(first);
        if (!(count == 1 && t_delete_line) && t_delete_lines)
          tio_command(t_delete_lines, hx_lines, count);
        else
          for (c = count; c--;) tio_command(t_delete_line, hx_lines);
      } else {
        tio_goto_line(last);
        tio_putchar('\n');
      }
    }
    tio_reset_scrolling_region();
  }
  return 0;
}

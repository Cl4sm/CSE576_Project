tio_scroll_down(count, first, last)
  int count;
  int first, last;
  /* Scroll down (reverse) `count' lines.  returns -1 if the terminal can't
   * scroll backwards.
   */
{
  int c;

  assert(count);
  if (last < 0) last = hx_lines - 1;
  if (!t_insert_line && !t_insert_lines
      && !t_scroll_backwd &&!t_scroll_backwd_n) return -1;
  if (tio_set_scrolling_region(first, last)) {
    if ((!t_delete_lines && !t_delete_line) ||
        (!t_insert_lines && !t_insert_line))
      return -1;
    tio_goto_line(last);
    if (!(count == 1 && t_delete_line) && t_delete_lines)
      tio_command(t_delete_lines, hx_lines, count);
    else {
      for (c = count; c--;) tio_command(t_delete_line, hx_lines);
    }
    tio_goto_line(first);
    if (!(count == 1 && t_insert_line) && t_insert_lines)
      tio_command(t_insert_lines, hx_lines, count);
    else
      for (c = count; c--;) tio_command(t_insert_line, hx_lines);
  } else {
    if (!(count == 1 && t_scroll_backwd) && t_scroll_backwd_n)
      tio_command(t_scroll_backwd_n, count, count);
    else if (t_scroll_backwd)
      for (c = count; c--;) tio_command(t_scroll_backwd, 1);
    else {
      tio_goto_line(first);
      if (!(count == 1 && t_insert_line) && t_insert_lines)
        tio_command(t_insert_lines, hx_lines, count);
      else
        for (c = count; c--;) tio_command(t_insert_line, hx_lines);
    }
    tio_reset_scrolling_region();
  }
  return 0;
}

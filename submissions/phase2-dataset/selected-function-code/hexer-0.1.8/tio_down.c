  void
tio_down(count)
  /* Move cursor down `count' lines.
   */
  int count;
{
  if (!count) return;
  if (t_line < 0) {
    tio_goto_line(0);
    tio_down(count);
  } else {
    if (t_line + count > hx_lines - 1) count = hx_lines - t_line;
    if (!count) return;
    t_line += count;
    if (t_DOwn)
      tio_command(t_DOwn, count, count);
    else if (t_down)
      while (count--) { tio_command(t_down, 1); }
    else
      while (count--) { tio_command("\n", 1); }
  }
}

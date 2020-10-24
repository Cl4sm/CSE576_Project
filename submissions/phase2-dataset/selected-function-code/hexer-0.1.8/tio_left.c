  void
tio_left(count)
  /* Move cursor left `count' lines.
   */
  int count;
{
  if (!count) return;
  if (t_line < 0)
    tio_goto_column(0);
  else {
    if (count > t_column) count = t_column;
    if (!count) return;
    t_column -= count;
    if (t_LEft)
      tio_command(t_LEft, count, count);
    else if (t_left)
      while (count--) { tio_command(t_left, 1); }
    else
      while (count--) putchar('\b');
  }
}

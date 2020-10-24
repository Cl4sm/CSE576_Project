tio_right(count)
  /* Move cursor right `count' lines.
   */
  int count;
{
  if (!count) return;
  if (t_column < 0) {
    tio_goto_column(0);
    tio_right(count);
  } else {
    if (t_column + count > hx_columns - 1) count = hx_columns - t_column;
    if (!count) return;
    t_column += count;
    if (t_RIght)
      tio_command(t_RIght, count, count);
    else if (t_right)
      while (count--) { tio_command(t_right, 1); }
    else
      while (count--) putchar(' ');
  }
}

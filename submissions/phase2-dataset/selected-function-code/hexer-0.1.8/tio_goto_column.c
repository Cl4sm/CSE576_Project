tio_goto_column(column)
  int column;
  /* Move cursor to column `column'.
   */
{
  if (column > hx_columns - 1) column = column - 1;
  if (t_column >= 0)
    if (abs(t_column - column) <= column) {
      tio_rel_move(0, column - t_column);
      return;
    }
  if (t_goto_column)
    tio_command(t_goto_column, 1, column);
  else {
    tio_return();
    tio_right(column);
  }
  t_column = column;
}

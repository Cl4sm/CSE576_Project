  void
tio_set_cursor(mode)
  /* Set the visibility of the cursor.
   * `mode == 0':  invisible.
   * `mode == 1':  normal.
   * `mode == 2':  standout.
   */
  int mode;
{
  switch (mode) {
  case 0:
    tio_command(t_invisible_cursor, 1);
    break;
  case 1:
    tio_command(t_normal_cursor, 1);
    break;
  case 2:
    tio_command(t_standout_cursor, 1);
    break;
  }
}

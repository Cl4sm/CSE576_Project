tio_insert_character(x)
  char x;
  /* Insert the character `x' at the current position.
   */
{
  int insert_mode = 0;

  if (t_insert_char) {
    tio_command(t_insert_char, 1);
  } else if (t_insert_chars) {
    tio_command(t_insert_chars, 1, 1);
  } else if (t_insert_mode) {
    tio_command(t_insert_mode, 1);
    insert_mode = 1;
  } else
    return -1;

  tio_putchar(x);
  if (insert_mode) tio_command(t_end_insert_mode, 1);
  return 0;
}

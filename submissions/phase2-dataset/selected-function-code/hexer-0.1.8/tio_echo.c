  int
tio_echo(on)
  int on;
  /* on=1:  characters are echoed as they are typed.
   * on=0:  characters are not echoed.
   * Thie echo-option has no effect on other `tio_*'-functions,
   * i.e. `tio_getch()' won't ever echo the character it reads.
   * RETURN VALUE:  `tio_echo()' returns the previous echo-state; that is
   *   1 if echo was set and 0 if echo was clear.
   * NOTE:  You may want to turn of the echo if your program is makeing
   *   several successive calls to `tio_getch()', so the characters typed
   *   between two calls to `tio_getch()' won't be echoed on the screen.
   */
{
  struct termios ts;
  int prev_echo = t_echo;

  tcgetattr(0, &ts);
  if (on)
    ts.c_lflag |= ECHO;
  else
    ts.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &ts);
  t_echo = !!on;
  return prev_echo;
}

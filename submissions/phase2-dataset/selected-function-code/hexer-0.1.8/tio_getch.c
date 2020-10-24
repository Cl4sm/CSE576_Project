tio_getch()
  /* Read a character or keypad key from the keyboard.  `tio_getch()' waits
   * for input and returns the key pressed or `HXKEY_ERROR' (-1) on error.
   * This function calls `tio_getch_()' to read the character.
   */
{
  struct termios ts, ts_rec;
  int key;

  tcgetattr(0, &ts_rec);
  ts = ts_start;
  ts.c_lflag &= ~(ECHO | ICANON | IEXTEN);
  ts.c_cc[VMIN] = 1;
  ts.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &ts);
  for (;;) {
    switch (tio_getwait(tio_readwait_timeout)) {
      case -1:
#ifdef TCFLSH
        ioctl(0, TCFLSH, TCIFLUSH);
#else
        tcflush(0, TCIFLUSH);
#endif
        return -1;
      case 0:
        if (window_changed) return -1; else continue;
      default:
        break;
    }
    break;
  }
  key = tio_getch_();
  tcsetattr(0, TCSANOW, &ts_rec);
  return key;
}

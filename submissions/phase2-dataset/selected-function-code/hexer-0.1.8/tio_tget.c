  int
tio_tget(int tmout)
  /* Like `tio_get()', but waits `timeout' tenths of a second for input.
   * `tio_tget()' returns `HXKEY_NONE' (0) if nothing has been read.
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
  switch (tio_getwait(tmout * 100)) {
    case -1:
#ifdef TCFLSH
      ioctl(0, TCFLSH, TCIFLUSH);
#else
      tcflush(0, TCIFLUSH);
#endif
      return -1;
    case 0:
      if (window_changed) return (int)HXKEY_ERROR; else return (int)HXKEY_NONE;
    default:
      break;
  }
  key = tio_getch_();
  tcsetattr(0, TCSANOW, &ts_rec);
  return key;
}

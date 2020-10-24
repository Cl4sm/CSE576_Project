  static int
tio_getch_(void)
  /* Read a character or keypad key from the keyboard.  Keypad keys
   * (like arrow keys, function keys, ...) are recognized only if the
   * terminal is in keypad transmit mode.  (Use `tio_keypad()' to set or
   * reset the keypad transmit mode.)  This function is called by
   * `tio_get()' and `tio_getch()'.
   */
{
  struct termios ts;
  char cbuf[256];
  int i, j;

  tio_flush();

  /* Check if C-c has been hit.
   */
  if (*tio_interrupt) {
    *tio_interrupt = 0;
    return (int)HXKEY_BREAK;
  }

  /* See if there is something in the unget buffer.
   */
  if (tio_unget_count--) return tio_unget_buffer[tio_unget_count];
  tio_unget_count = 0;

  /* Try to read one character from the input stream.  If the terminal is not
   * in keypad transmit mode, the caracter is returned.
   */
  switch (tio_read(cbuf, 1)) {
  case -1:
    return (int)HXKEY_ERROR;
      /* The occurence of an error in `tio_read()' is likely to be caused by
       * the signal `SIGWINCH', indicating that the size of the window has
       * changed.
       */
  case 0:
    return (int)HXKEY_NONE;
  default:
    if (!t_keypad_transmit) {
      if (*cbuf == '\n' || *cbuf == '\r')
        return (int)HXKEY_RETURN;
      else
        return *cbuf ? (int)(unsigned char)*cbuf : (int)HXKEY_NULL;
    }
  }

  /* Check if the character read is the first character of a known key
   * string.  It is *not* assumed that all key strings start with `\e'
   * (escape).
   */
  for (i = 0; t_keys[i].key; ++i)
    if (*t_keys[i].string ? *cbuf == **t_keys[i].string : 0) {
      if (strlen(*t_keys[i].string) == 1)
        return (int)t_keys[i].key;
          /* The read character is a known key string. */
      else
        break;
    }
  if (!t_keys[i].key) {
    if (*cbuf == '\n' || *cbuf == '\r')
      return (int)HXKEY_RETURN;
    else
      return *cbuf ? (int)(unsigned char)*cbuf : (int)HXKEY_NULL;
  }

  /* If the read character is a prefix of a known key string, characters
   * are read from the input stream until a key string is recoginzed,
   * the read string is no longer a prefix of a known key string or no
   * characters arrived for `t_timeout' tenths of a second.
   */
  ts = ts_start;
  ts.c_lflag &= ~(ECHO | ICANON | IEXTEN);
  ts.c_cc[VMIN] = 0;
  ts.c_cc[VTIME] = t_timeout;
  tcsetattr(0, TCSANOW, &ts);
  for (j = 0;;) {
    j += i = tio_read(cbuf + j + 1, 1);
    if (i < 0) return (int)HXKEY_ERROR;
    cbuf[j + 1] = 0;
    if (!i) {
      /* On timeout return the first character of the read string and put
       * the rest of the read string back to the input stream.
       */
      if (j) tio_unread(cbuf + 1, j);
      return *cbuf ? (int)(unsigned char)*cbuf : (int)HXKEY_NULL;
    } else {
      int prefix_f = 0;
      for (i = 0; t_keys[i].key; ++i)
        if (*t_keys[i].string ? !strcmp(cbuf, *t_keys[i].string) : 0)
          return (int)t_keys[i].key;
      for (i = 0; t_keys[i].key; ++i)
        if (*t_keys[i].string ?
            !strncmp(cbuf, *t_keys[i].string, j + 1) : 0) {
          prefix_f = 1;
          break;
        }
      if (!prefix_f) {
        tio_unread(cbuf + 1, j);
        return *cbuf ? (int)(unsigned char)*cbuf : (int)HXKEY_NULL;
      }
    }
  } /* for */
}

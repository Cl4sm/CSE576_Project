  static int
rl_get_vposition(void)
  /* return the position of the cursor in `rl.vline'.
   */
{
  int i;
  int position;
  int vposition;
  int key;

  for (i = position = vposition = 0;
       rl.line[i] && position < rl_position;
       ++i, ++position) {
    if (rl.line[i] != RL_ESC)
      if (tio_isprint(rl.line[i]))
        ++vposition;
      else
        vposition += strlen(tio_keyrep(rl.line[i]));
    else {
      ++i;
      if (rl.line[i] == 1)
        /* ESCAPE-character */
        vposition += strlen(tio_keyrep(HXKEY_ESCAPE));
      else if (rl.line[i] == (int)HXKEY_NULL - HXKEY_BIAS)
        /* null-character */
        vposition += strlen(tio_keyrep(HXKEY_NULL));
      else {
        key = rl.line[i] + HXKEY_BIAS;
        vposition += strlen(tio_keyrep(key));
      }
    }
  }
  return vposition;
}

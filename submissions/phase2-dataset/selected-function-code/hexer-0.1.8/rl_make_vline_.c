rl_make_vline_(struct rl_line_s *rrl)
  /* update the visible line `rrl->vline' from `rrl->line'.
   * the return value is `rrl->vline'.
   */
{
  int i;
  int vposition;
  int key;
  char *rep;

  for (i = vposition = 0; rrl->line[i]; ++i) {
    if (rrl->line[i] != RL_ESC)
      if (tio_isprint(rrl->line[i]))
        rrl->vline[vposition++] = rrl->line[i];
      else {
        strcpy(rrl->vline + vposition, rep = tio_keyrep(rrl->line[i]));
        vposition += strlen(rep);
      }
    else {
      ++i;
      if (rrl->line[i] == 1) {
        /* ESCAPE-character */
        strcpy(rrl->vline + vposition, rep = tio_keyrep(HXKEY_ESCAPE));
        vposition += strlen(rep);
      } else if (rrl->line[i] == (int)HXKEY_NULL - HXKEY_BIAS) {
        /* null-character */
        strcpy(rrl->vline + vposition, rep = tio_keyrep(HXKEY_NULL));
        vposition += strlen(rep);
      } else {
        key = rrl->line[i] + HXKEY_BIAS;
        strcpy(rrl->vline + vposition, rep = tio_keyrep(key));
        vposition += strlen(rep);
      }
    }
  }
  rrl->vline[vposition] = 0;
  return rrl->vline;
}

  void
he_search_command(hedit, exp, dir)
  struct he_s *hedit;
  char *exp;
  int dir;
{
  long position;
  char *rs;
  long rl, ml;
  static char last_exp[4096] = "";

  if (!*exp) {
    if (!*last_exp) {
      he_message(0, "@Abno previous expression@~");
      goto exit;
    } else
      exp = last_exp;
  }
  switch ((position = he_search(hedit, exp, "", dir, 1, 1, -1,
                                &rs, &rl, &ml))) {
  case -2:  /* invalid expression */
    he_message(0, "@Abinvalid expression:@~ %s", rx_error_msg[rx_error]);
    goto exit;
  case -1:  /* no match */
    if (!rx_error)
      he_message(0, "no match");
    else
      he_message(0, "@Abregexp error:@~ %s", rx_error_msg[rx_error]);
    break;
  default:  /* match */
    hedit->position = position;
    free((char *)rs);
    break;
  }
  if (exp != last_exp) strcpy(last_exp, exp);

exit:
  return;
}

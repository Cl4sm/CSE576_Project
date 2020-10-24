void
send_umode(struct Client *client_p, struct Client *source_p,
           unsigned int old, char *umode_buf)
{
  char *m = umode_buf;
  int what = 0;

  /*
   * Build a string in umode_buf to represent the change in the user's
   * mode between the new (source_p->umodes) and 'old'.
   */
  for (unsigned int i = 0; i < 128; ++i)
  {
    unsigned int flag = user_modes[i];

    if (!flag)
      continue;

    if ((flag & old) && !HasUMode(source_p, flag))
    {
      if (what == MODE_DEL)
        *m++ = (char)i;
      else
      {
        what = MODE_DEL;
        *m++ = '-';
        *m++ = (char)i;
      }
    }
    else if (!(flag & old) && HasUMode(source_p, flag))
    {
      if (what == MODE_ADD)
        *m++ = (char)i;
      else
      {
        what = MODE_ADD;
        *m++ = '+';
        *m++ = (char)i;
      }
    }
  }

  *m = '\0';

  if (*umode_buf && client_p)
    sendto_one(client_p, ":%s!%s@%s MODE %s :%s",
               source_p->name, source_p->username,
               source_p->host, source_p->name, umode_buf);
}

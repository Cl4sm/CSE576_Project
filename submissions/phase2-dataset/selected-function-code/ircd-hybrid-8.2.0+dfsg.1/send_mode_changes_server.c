static void
send_mode_changes_server(struct Client *source_p, struct Channel *chptr)
{
  int mbl = 0, pbl = 0, arglen = 0, nc = 0, mc = 0;
  int len = 0;
  int dir = MODE_QUERY;
  const char *arg = NULL;
  char *parptr = NULL;

  parabuf[0] = '\0';
  parptr = parabuf;

  mbl = snprintf(modebuf, sizeof(modebuf), ":%s TMODE %lu %s ", source_p->id,
                 (unsigned long)chptr->channelts, chptr->chname);

  /* Loop the list of modes we have */
  for (unsigned i = 0; i < mode_count; ++i)
  {
    if (mode_changes[i].letter == 0)
      continue;

    if (mode_changes[i].id)
      arg = mode_changes[i].id;
    else
      arg = mode_changes[i].arg;

    if (arg)
      arglen = strlen(arg);
    else
      arglen = 0;

    /*
     * If we're creeping past the buf size, we need to send it and make
     * another line for the other modes
     */
    if ((mc == MAXMODEPARAMS) ||
        ((arglen + mbl + pbl + 2) > IRCD_BUFSIZE) ||
        (pbl + arglen + BAN_FUDGE) >= MODEBUFLEN)
    {
      if (nc)
        sendto_server(source_p, NOCAPS, NOCAPS, "%s %s", modebuf, parabuf);

      nc = 0;
      mc = 0;

      mbl = snprintf(modebuf, sizeof(modebuf), ":%s TMODE %lu %s ", source_p->id,
                     (unsigned long)chptr->channelts, chptr->chname);

      pbl = 0;
      parabuf[0] = '\0';
      parptr = parabuf;
      dir = MODE_QUERY;
    }

    if (dir != mode_changes[i].dir)
    {
      modebuf[mbl++] = (mode_changes[i].dir == MODE_ADD) ? '+' : '-';
      dir = mode_changes[i].dir;
    }

    modebuf[mbl++] = mode_changes[i].letter;
    modebuf[mbl] = '\0';
    nc++;

    if (arg)
    {
      len = sprintf(parptr, "%s ", arg);
      pbl += len;
      parptr += len;
      mc++;
    }
  }

  if (pbl && parabuf[pbl - 1] == ' ')
    parabuf[pbl - 1] = '\0';

  if (nc)
    sendto_server(source_p, NOCAPS, NOCAPS, "%s %s", modebuf, parabuf);
}

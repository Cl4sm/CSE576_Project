static void
send_mode_changes(struct Client *source_p, struct Channel *chptr)
{
  int mbl = 0, pbl = 0, arglen = 0, nc = 0, mc = 0;
  int len = 0;
  int dir = MODE_QUERY;
  const char *arg = NULL;
  char *parptr = NULL;

  /* Bail out if we have nothing to do... */
  if (!mode_count)
    return;

  if (IsServer(source_p))
    mbl = snprintf(modebuf, sizeof(modebuf), ":%s MODE %s ", (IsHidden(source_p) ||
                   ConfigServerHide.hide_servers) ?
                   me.name : source_p->name, chptr->chname);
  else
    mbl = snprintf(modebuf, sizeof(modebuf), ":%s!%s@%s MODE %s ", source_p->name,
                   source_p->username, source_p->host, chptr->chname);

  parabuf[0] = '\0';
  parptr = parabuf;

  for (unsigned int i = 0; i < mode_count; ++i)
  {
    if (mode_changes[i].letter == 0 ||
        mode_changes[i].mems == ONLY_SERVERS)
      continue;

    arg = mode_changes[i].arg;
    if (arg)
      arglen = strlen(arg);
    else
      arglen = 0;

    if ((mc == MAXMODEPARAMS)  ||
        ((arglen + mbl + pbl + 2) > IRCD_BUFSIZE) ||
        ((arglen + pbl + BAN_FUDGE) >= MODEBUFLEN))
    {
      if (mbl && modebuf[mbl - 1] == '-')
        modebuf[mbl - 1] = '\0';

      if (nc)
        sendto_channel_local(ALL_MEMBERS, 0, chptr, "%s %s", modebuf, parabuf);

      nc = 0;
      mc = 0;

      if (IsServer(source_p))
        mbl = snprintf(modebuf, sizeof(modebuf), ":%s MODE %s ", (IsHidden(source_p) ||
                       ConfigServerHide.hide_servers) ?
                       me.name : source_p->name, chptr->chname);
      else
        mbl = snprintf(modebuf, sizeof(modebuf), ":%s!%s@%s MODE %s ", source_p->name,
                       source_p->username, source_p->host, chptr->chname);

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
    sendto_channel_local(ALL_MEMBERS, 0, chptr, "%s %s", modebuf, parabuf);

  send_mode_changes_server(source_p, chptr);
}

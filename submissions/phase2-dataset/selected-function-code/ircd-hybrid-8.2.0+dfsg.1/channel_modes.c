void
channel_modes(struct Channel *chptr, struct Client *client_p, char *mbuf, char *pbuf)
{
  *mbuf++ = '+';
  *pbuf = '\0';

  for (const struct mode_letter *tab = chan_modes; tab->mode; ++tab)
    if (chptr->mode.mode & tab->mode)
      *mbuf++ = tab->letter;

  if (chptr->mode.limit)
  {
    *mbuf++ = 'l';

    if (IsServer(client_p) || HasFlag(client_p, FLAGS_SERVICE) || IsMember(client_p, chptr))
      pbuf += sprintf(pbuf, "%d ", chptr->mode.limit);
  }

  if (chptr->mode.key[0])
  {
    *mbuf++ = 'k';

    if (IsServer(client_p) || HasFlag(client_p, FLAGS_SERVICE) || IsMember(client_p, chptr))
      sprintf(pbuf, "%s ", chptr->mode.key);
  }

  *mbuf = '\0';
}

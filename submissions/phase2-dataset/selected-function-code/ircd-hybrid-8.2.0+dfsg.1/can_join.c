int
can_join(struct Client *source_p, struct Channel *chptr, const char *key)
{
  if ((chptr->mode.mode & MODE_SSLONLY) && !HasUMode(source_p, UMODE_SSL))
    return ERR_SSLONLYCHAN;

  if ((chptr->mode.mode & MODE_REGONLY) && !HasUMode(source_p, UMODE_REGISTERED))
    return ERR_NEEDREGGEDNICK;

  if ((chptr->mode.mode & MODE_OPERONLY) && !HasUMode(source_p, UMODE_OPER))
    return ERR_OPERONLYCHAN;

  if (chptr->mode.mode & MODE_INVITEONLY)
    if (!dlinkFind(&source_p->localClient->invited, chptr))
      if (!find_bmask(source_p, &chptr->invexlist))
        return ERR_INVITEONLYCHAN;

  if (chptr->mode.key[0] && (!key || strcmp(chptr->mode.key, key)))
    return ERR_BADCHANNELKEY;

  if (chptr->mode.limit && dlink_list_length(&chptr->members) >=
      chptr->mode.limit)
    return ERR_CHANNELISFULL;

  if (is_banned(chptr, source_p))
    return ERR_BANNEDFROMCHAN;

  return 0;
}

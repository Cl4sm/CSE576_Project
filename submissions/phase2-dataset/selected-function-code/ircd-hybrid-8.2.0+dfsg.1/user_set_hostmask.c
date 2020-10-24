user_set_hostmask(struct Client *target_p, const char *hostname, const int what)
{
  dlink_node *ptr = NULL;

  if (!strcmp(target_p->host, hostname))
    return;

  switch (what)
  {
    case MODE_ADD:
      AddUMode(target_p, UMODE_HIDDENHOST);
      AddFlag(target_p, FLAGS_IP_SPOOFING);
      break;
    case MODE_DEL:
      DelUMode(target_p, UMODE_HIDDENHOST);

      if (!HasFlag(target_p, FLAGS_AUTH_SPOOF))
        DelFlag(target_p, FLAGS_IP_SPOOFING);
      break;
    default: return;
  }

  if (ConfigGeneral.cycle_on_host_change)
    sendto_common_channels_local(target_p, 0, 0, ":%s!%s@%s QUIT :Changing hostname",
                                 target_p->name, target_p->username, target_p->host);

  if (IsUserHostIp(target_p))
    delete_user_host(target_p->username, target_p->host, !MyConnect(target_p));

  strlcpy(target_p->host, hostname, sizeof(target_p->host));

  add_user_host(target_p->username, target_p->host, !MyConnect(target_p));
  SetUserHost(target_p);

  if (MyClient(target_p))
  {
    sendto_one_numeric(target_p, &me, RPL_VISIBLEHOST, target_p->host);
    clear_ban_cache_client(target_p);
  }

  if (!ConfigGeneral.cycle_on_host_change)
    return;

  DLINK_FOREACH(ptr, target_p->channel.head)
  {
    char modebuf[4], nickbuf[NICKLEN * 3 + 3] = "";
    char *p = modebuf;
    int len = 0;
    const struct Membership *ms = ptr->data;

    if (has_member_flags(ms, CHFL_CHANOP))
    {
      *p++ = 'o';
      len += snprintf(nickbuf + len, sizeof(nickbuf) - len, len ? " %s" : "%s", target_p->name);
    }

    if (has_member_flags(ms, CHFL_HALFOP))
    {
      *p++ = 'h';
      len += snprintf(nickbuf + len, sizeof(nickbuf) - len, len ? " %s" : "%s", target_p->name);
    }

    if (has_member_flags(ms, CHFL_VOICE))
    {
      *p++ = 'v';
      len += snprintf(nickbuf + len, sizeof(nickbuf) - len, len ? " %s" : "%s", target_p->name);
    }

    *p = '\0';

    sendto_channel_local_butone(target_p, 0, 0, ms->chptr, ":%s!%s@%s JOIN :%s",
                                target_p->name, target_p->username, target_p->host,
                                ms->chptr->chname);
    if (nickbuf[0])
      sendto_channel_local_butone(target_p, 0, 0, ms->chptr, ":%s MODE %s +%s %s",
                                  target_p->servptr->name, ms->chptr->chname,
                                  modebuf, nickbuf);

  }

  if (target_p->away[0])
    sendto_common_channels_local(target_p, 0, CAP_AWAY_NOTIFY,
                                 ":%s!%s@%s AWAY :%s",
                                 target_p->name, target_p->username,
                                 target_p->host, target_p->away);
}

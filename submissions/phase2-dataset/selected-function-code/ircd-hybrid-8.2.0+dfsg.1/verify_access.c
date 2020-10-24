verify_access(struct Client *client_p)
{
  struct MaskItem *conf = NULL;
  char non_ident[USERLEN + 1] = "~";

  if (IsGotId(client_p))
  {
    conf = find_address_conf(client_p->host, client_p->username,
                             &client_p->localClient->ip,
                             client_p->localClient->aftype,
                             client_p->localClient->password);
  }
  else
  {
    strlcpy(non_ident + 1, client_p->username, sizeof(non_ident) - 1);
    conf = find_address_conf(client_p->host,non_ident,
                             &client_p->localClient->ip,
                             client_p->localClient->aftype,
                             client_p->localClient->password);
  }

  if (conf)
  {
    if (IsConfClient(conf))
    {
      if (IsConfRedir(conf))
      {
        sendto_one_numeric(client_p, &me, RPL_REDIR,
                           conf->name ? conf->name : "",
                           conf->port);
        return NOT_AUTHORIZED;
      }

      if (IsConfDoIdentd(conf))
        SetNeedId(client_p);

      /* Thanks for spoof idea amm */
      if (IsConfDoSpoofIp(conf))
      {
        if (!ConfigGeneral.hide_spoof_ips && IsConfSpoofNotice(conf))
          sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                               "%s spoofing: %s as %s",
                               client_p->name, client_p->host, conf->name);
        strlcpy(client_p->host, conf->name, sizeof(client_p->host));
        AddFlag(client_p, FLAGS_IP_SPOOFING | FLAGS_AUTH_SPOOF);
      }

      return attach_iline(client_p, conf);
    }
    else if (IsConfKill(conf) || (ConfigGeneral.glines && IsConfGline(conf)))
    {
      if (IsConfGline(conf))
        sendto_one_notice(client_p, &me, ":*** G-lined");
      sendto_one_notice(client_p, &me, ":*** Banned: %s", conf->reason);
      return BANNED_CLIENT;
    }
  }

  return NOT_AUTHORIZED;
}

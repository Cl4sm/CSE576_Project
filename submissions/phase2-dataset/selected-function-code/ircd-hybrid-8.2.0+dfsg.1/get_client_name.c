const char *
get_client_name(const struct Client *client_p, enum addr_mask_type type)
{
  static char buf[HOSTLEN * 2 + USERLEN + 5];

  assert(client_p);

  if (!MyConnect(client_p))
    return client_p->name;

  if (IsServer(client_p) || IsConnecting(client_p) || IsHandshake(client_p))
  {
    if (!irccmp(client_p->name, client_p->host))
      return client_p->name;
    else if (ConfigServerHide.hide_server_ips)
      type = MASK_IP;
  }

  if (ConfigGeneral.hide_spoof_ips)
    if (IsIPSpoof(client_p) && type == SHOW_IP)
      type = MASK_IP;

  /* And finally, let's get the host information, ip or name */
  switch (type)
  {
    case SHOW_IP:
      snprintf(buf, sizeof(buf), "%s[%s@%s]",
               client_p->name,
               client_p->username, client_p->sockhost);
      break;
    case MASK_IP:
      if (client_p->localClient->aftype == AF_INET)
        snprintf(buf, sizeof(buf), "%s[%s@255.255.255.255]",
                 client_p->name, client_p->username);
      else
        snprintf(buf, sizeof(buf), "%s[%s@ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]",
                 client_p->name, client_p->username);
      break;
    default:
      snprintf(buf, sizeof(buf), "%s[%s@%s]",
               client_p->name,
               client_p->username, client_p->host);
  }

  return buf;
}

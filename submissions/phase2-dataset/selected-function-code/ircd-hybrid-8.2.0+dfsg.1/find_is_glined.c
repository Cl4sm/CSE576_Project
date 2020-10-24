struct MaskItem *
find_is_glined(const char *host, const char *user)
{
  struct irc_ssaddr iphost, *piphost = NULL;
  int t = 0;
  int aftype = 0;

  if ((t = parse_netmask(host, &iphost, NULL)) != HM_HOST)
  {
    if (t == HM_IPV6)
      aftype = AF_INET6;
    else
      aftype = AF_INET;
    piphost = &iphost;
  }
  else
    piphost = NULL;

  return find_conf_by_address(host, piphost, CONF_GLINE, aftype, user, NULL, 0);
}

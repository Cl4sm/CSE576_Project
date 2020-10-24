int
conf_connect_allowed(struct irc_ssaddr *addr, int aftype)
{
  struct ip_entry *ip_found = NULL;
  struct MaskItem *conf = find_dline_conf(addr, aftype);

  /* DLINE exempt also gets you out of static limits/pacing... */
  if (conf && (conf->type == CONF_EXEMPT))
    return 0;

  if (conf)
    return BANNED_CLIENT;

  ip_found = ipcache_find_or_add_address(addr);

  if ((CurrentTime - ip_found->last_attempt) < ConfigGeneral.throttle_time)
  {
    if (ip_found->connection_count >= ConfigGeneral.throttle_count)
      return TOO_FAST;

    ++ip_found->connection_count;
  }
  else
    ip_found->connection_count = 1;

  ip_found->last_attempt = CurrentTime;
  return 0;
}

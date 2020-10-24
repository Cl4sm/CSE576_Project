static int
check_xline(struct Client *source_p)
{
  struct MaskItem *conf = NULL;
  const char *reason = NULL;

  if ((conf = find_matching_name_conf(CONF_XLINE, source_p->info, NULL, NULL, 0)))
  {
    ++conf->count;

    if (conf->reason)
      reason = conf->reason;
    else
      reason = CONF_NOREASON;

    sendto_realops_flags(UMODE_REJ, L_ALL, SEND_NOTICE,
                         "X-line Rejecting [%s] [%s], user %s [%s]",
                         source_p->info, reason,
                         get_client_name(source_p, HIDE_IP),
                         source_p->sockhost);

    ++ServerStats.is_ref;
    exit_client(source_p, "Bad user info");
    return 1;
  }

  return 0;
}

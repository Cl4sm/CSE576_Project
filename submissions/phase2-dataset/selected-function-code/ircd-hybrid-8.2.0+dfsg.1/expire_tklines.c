static void
expire_tklines(dlink_list *tklist)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;
  struct MaskItem *conf = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, tklist->head)
  {
    conf = ptr->data;

    if (!conf->until || conf->until > CurrentTime)
      continue;

    if (conf->type == CONF_XLINE)
    {
      if (ConfigGeneral.tkline_expire_notices)
        sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                               "Temporary X-line for [%s] expired", conf->name);
      conf_free(conf);
    }
    else if (conf->type == CONF_NRESV || conf->type == CONF_CRESV)
    {
      if (ConfigGeneral.tkline_expire_notices)
        sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                               "Temporary RESV for [%s] expired", conf->name);
      conf_free(conf);
    }
  }
}

try_connections(void *unused)
{
  dlink_node *ptr = NULL;
  int confrq = 0;

  /* TODO: change this to set active flag to 0 when added to event! --Habeeb */
  if (GlobalSetOptions.autoconn == 0)
    return;

  DLINK_FOREACH(ptr, server_items.head)
  {
    struct MaskItem *conf = ptr->data;

    assert(conf->type == CONF_SERVER);

    /* Also when already connecting! (update holdtimes) --SRB
     */
    if (!conf->port || !IsConfAllowAutoConn(conf))
      continue;


    /* Skip this entry if the use of it is still on hold until
     * future. Otherwise handle this entry (and set it on hold
     * until next time). Will reset only hold times, if already
     * made one successfull connection... [this algorithm is
     * a bit fuzzy... -- msa >;) ]
     */
    if (conf->until > CurrentTime)
      continue;

    assert(conf->class);

    confrq = conf->class->con_freq;
    if (confrq < MIN_CONN_FREQ)
      confrq = MIN_CONN_FREQ;

    conf->until = CurrentTime + confrq;

    /*
     * Found a CONNECT config with port specified, scan clients
     * and see if this server is already connected?
     */
    if (hash_find_server(conf->name))
      continue;

    if (conf->class->ref_count < conf->class->max_total)
    {
      /* Go to the end of the list, if not already last */
      if (ptr->next)
      {
        dlinkDelete(ptr, &server_items);
        dlinkAddTail(conf, &conf->node, &server_items);
      }

      if (find_servconn_in_progress(conf->name))
        return;

      /*
       * We used to only print this if serv_connect() actually
       * succeeded, but since comm_tcp_connect() can call the callback
       * immediately if there is an error, we were getting error messages
       * in the wrong order. SO, we just print out the activated line,
       * and let serv_connect() / serv_connect_callback() print an
       * error afterwards if it fails.
       *   -- adrian
       */
      if (ConfigServerHide.hide_server_ips)
        sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                             "Connection to %s activated.",
                             conf->name);
      else
        sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                             "Connection to %s[%s] activated.",
                             conf->name, conf->host);

      serv_connect(conf, NULL);
      /* We connect only one at time... */
      return;
    }
  }
}

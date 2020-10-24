attach_iline(struct Client *client_p, struct MaskItem *conf)
{
  const struct ClassItem *class = conf->class;
  struct ip_entry *ip_found;
  int a_limit_reached = 0;
  unsigned int local = 0, global = 0, ident = 0;

  ip_found = ipcache_find_or_add_address(&client_p->localClient->ip);
  ip_found->count++;
  AddFlag(client_p, FLAGS_IPHASH);

  count_user_host(client_p->username, client_p->host,
                  &global, &local, &ident);

  /* XXX blah. go down checking the various silly limits
   * setting a_limit_reached if any limit is reached.
   * - Dianora
   */
  if (class->max_total && class->ref_count >= class->max_total)
    a_limit_reached = 1;
  else if (class->max_perip && ip_found->count > class->max_perip)
    a_limit_reached = 1;
  else if (class->max_local && local >= class->max_local)
    a_limit_reached = 1;
  else if (class->max_global && global >= class->max_global)
    a_limit_reached = 1;
  else if (class->max_ident && ident >= class->max_ident &&
           client_p->username[0] != '~')
    a_limit_reached = 1;

  if (a_limit_reached)
  {
    if (!IsConfExemptLimits(conf))
      return TOO_MANY;   /* Already at maximum allowed */

    sendto_one_notice(client_p, &me, ":*** Your connection class is full, "
                      "but you have exceed_limit = yes;");
  }

  return attach_conf(client_p, conf);
}

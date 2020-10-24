struct MaskItem *
create_resv(const char *name, const char *reason, const dlink_list *list)
{
  dlink_node *ptr = NULL;
  struct MaskItem *conf = NULL;
  enum maskitem_type type;

  if (name == NULL || reason == NULL)
    return NULL;

  if (IsChanPrefix(*name))
    type = CONF_CRESV;
  else
    type = CONF_NRESV;

  if (find_exact_name_conf(type, NULL, name, NULL, NULL))
    return NULL;

  conf = conf_make(type);
  conf->name = xstrdup(name);
  conf->reason = xstrndup(reason, IRCD_MIN(strlen(reason), REASONLEN));

  if (list)
  {
    DLINK_FOREACH(ptr, list->head)
    {
      char nick[NICKLEN + 1];
      char user[USERLEN + 1];
      char host[HOSTLEN + 1];
      struct split_nuh_item nuh;
      struct exempt *exptr = NULL;
      char *s = ptr->data;

      if (strlen(s) == 2 && IsAlpha(*(s + 1) && IsAlpha(*(s + 2))))
      {
#ifdef HAVE_LIBGEOIP
        exptr = MyCalloc(sizeof(*exptr));
        exptr->name = xstrdup(s);
        exptr->coid = GeoIP_id_by_code(s);
        dlinkAdd(exptr, &exptr->node, &conf->exempt_list);
#endif
      }
      else
      {
        nuh.nuhmask  = s;
        nuh.nickptr  = nick;
        nuh.userptr  = user;
        nuh.hostptr  = host;

        nuh.nicksize = sizeof(nick);
        nuh.usersize = sizeof(user);
        nuh.hostsize = sizeof(host);

        split_nuh(&nuh);

        exptr = MyCalloc(sizeof(*exptr));
        exptr->name = xstrdup(nick);
        exptr->user = xstrdup(user);
        exptr->host = xstrdup(host);
        exptr->type = parse_netmask(host, &exptr->addr, &exptr->bits);
        dlinkAdd(exptr, &exptr->node, &conf->exempt_list);
      }
    }
  }

  return conf;
}

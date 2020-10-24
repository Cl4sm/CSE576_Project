find_matching_name_conf(enum maskitem_type type, const char *name, const char *user,
                        const char *host, unsigned int flags)
{
  dlink_node *ptr=NULL;
  struct MaskItem *conf=NULL;
  dlink_list *list_p = map_to_list(type);

  switch (type)
  {
  case CONF_SERVICE:
    DLINK_FOREACH(ptr, list_p->head)
    {
      conf = ptr->data;

      if (EmptyString(conf->name))
        continue;
      if ((name != NULL) && !irccmp(name, conf->name))
        return conf;
    }
    break;

  case CONF_XLINE:
  case CONF_ULINE:
  case CONF_NRESV:
  case CONF_CRESV:
    DLINK_FOREACH(ptr, list_p->head)
    {
      conf = ptr->data;

      if (EmptyString(conf->name))
        continue;
      if ((name != NULL) && !match(conf->name, name))
      {
        if ((user == NULL && (host == NULL)))
          return conf;
        if ((conf->flags & flags) != flags)
          continue;
        if (EmptyString(conf->user) || EmptyString(conf->host))
          return conf;
        if (!match(conf->user, user) && !match(conf->host, host))
          return conf;
      }
    }
      break;

  case CONF_SERVER:
    DLINK_FOREACH(ptr, list_p->head)
    {
      conf = ptr->data;

      if ((name != NULL) && !match(name, conf->name))
        return conf;
      else if ((host != NULL) && !match(host, conf->host))
        return conf;
    }
    break;

  default:
    break;
  }
  return NULL;
}

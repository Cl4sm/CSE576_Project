struct MaskItem *
find_exact_name_conf(enum maskitem_type type, const struct Client *who, const char *name,
                     const char *user, const char *host)
{
  dlink_node *ptr = NULL;
  struct MaskItem *conf;
  dlink_list *list_p = map_to_list(type);

  switch(type)
  {
  case CONF_XLINE:
  case CONF_ULINE:
  case CONF_NRESV:
  case CONF_CRESV:

    DLINK_FOREACH(ptr, list_p->head)
    {
      conf = ptr->data;

      if (EmptyString(conf->name))
        continue;

      if (irccmp(conf->name, name) == 0)
      {
        if ((user == NULL && (host == NULL)))
          return conf;
        if (EmptyString(conf->user) || EmptyString(conf->host))
          return conf;
        if (!match(conf->user, user) && !match(conf->host, host))
          return conf;
      }
    }
    break;

  case CONF_OPER:
    DLINK_FOREACH(ptr, list_p->head)
    {
      conf = ptr->data;

      if (EmptyString(conf->name))
        continue;

      if (!irccmp(conf->name, name))
      {
        if (!who)
          return conf;
        if (EmptyString(conf->user) || EmptyString(conf->host))
          return NULL;
        if (!match(conf->user, who->username))
        {
          switch (conf->htype)
          {
            case HM_HOST:
              if (!match(conf->host, who->host) || !match(conf->host, who->sockhost))
                if (!conf->class->max_total || conf->class->ref_count < conf->class->max_total)
                  return conf;
              break;
            case HM_IPV4:
              if (who->localClient->aftype == AF_INET)
                if (match_ipv4(&who->localClient->ip, &conf->addr, conf->bits))
                  if (!conf->class->max_total || conf->class->ref_count < conf->class->max_total)
                    return conf;
              break;
            case HM_IPV6:
              if (who->localClient->aftype == AF_INET6)
                if (match_ipv6(&who->localClient->ip, &conf->addr, conf->bits))
                  if (!conf->class->max_total || conf->class->ref_count < conf->class->max_total)
                    return conf;
              break;
            default:
              assert(0);
          }
        }
      }
    }

    break;

  case CONF_SERVER:
    DLINK_FOREACH(ptr, list_p->head)
    {
      conf = ptr->data;

      if (EmptyString(conf->name))
        continue;

      if (name == NULL)
      {
        if (EmptyString(conf->host))
          continue;
        if (irccmp(conf->host, host) == 0)
          return conf;
      }
      else if (irccmp(conf->name, name) == 0)
        return conf;
    }

    break;

  default:
    break;
  }

  return NULL;
}

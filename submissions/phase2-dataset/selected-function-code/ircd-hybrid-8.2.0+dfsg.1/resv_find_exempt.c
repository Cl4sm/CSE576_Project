int
resv_find_exempt(const struct Client *who, const struct MaskItem *conf)
{
  const dlink_node *ptr = NULL;

  DLINK_FOREACH(ptr, conf->exempt_list.head)
  {
    const struct exempt *exptr = ptr->data;

    if (exptr->coid)
    {
      if (exptr->coid == who->localClient->country_id)
        return 1;
    }
    else if (!match(exptr->name, who->name) && !match(exptr->user, who->username))
    {
      switch (exptr->type)
      {
        case HM_HOST:
          if (!match(exptr->host, who->host) || !match(exptr->host, who->sockhost))
            return 1;
          break;
        case HM_IPV4:
          if (who->localClient->aftype == AF_INET)
            if (match_ipv4(&who->localClient->ip, &exptr->addr, exptr->bits))
              return 1;
          break;
        case HM_IPV6:
          if (who->localClient->aftype == AF_INET6)
            if (match_ipv6(&who->localClient->ip, &exptr->addr, exptr->bits))
              return 1;
          break;
        default:
          assert(0);
      }
    }
  }

  return 0;
}

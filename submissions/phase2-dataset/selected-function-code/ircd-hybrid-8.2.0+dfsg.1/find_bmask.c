find_bmask(const struct Client *who, const dlink_list *const list)
{
  const dlink_node *ptr = NULL;

  DLINK_FOREACH(ptr, list->head)
  {
    const struct Ban *bp = ptr->data;

    if (!match(bp->name, who->name) && !match(bp->user, who->username))
    {
      switch (bp->type)
      {
        case HM_HOST:
          if (!match(bp->host, who->host) || !match(bp->host, who->sockhost))
            return 1;
          break;
        case HM_IPV4:
          if (who->localClient->aftype == AF_INET)
            if (match_ipv4(&who->localClient->ip, &bp->addr, bp->bits))
              return 1;
          break;
        case HM_IPV6:
          if (who->localClient->aftype == AF_INET6)
            if (match_ipv6(&who->localClient->ip, &bp->addr, bp->bits))
              return 1;
          break;
        default:
          assert(0);
      }
    }
  }

  return 0;
}

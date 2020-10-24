static struct Motd *
motd_lookup(const struct Client *client_p)
{
  dlink_node *ptr = NULL;
  const struct ClassItem *class = NULL;

  assert(client_p);

  if (!MyConnect(client_p))  /* Not my user, always return remote motd */
    return MotdList.remote;

  class = get_class_ptr(&client_p->localClient->confs);
  assert(class);

  /* Check the motd blocks first */
  DLINK_FOREACH(ptr, MotdList.other.head)
  {
    struct Motd *motd = ptr->data;

    switch (motd->type)
    {
      case MOTD_CLASS:
        if (!match(motd->mask, class->name))
          return motd;
        break;
      case MOTD_HOSTMASK:
        if (!match(motd->mask, client_p->host))
          return motd;
        break;
      case MOTD_IPMASKV4:
          if (client_p->localClient->aftype == AF_INET)
            if (match_ipv4(&client_p->localClient->ip, &motd->address, motd->addrbits))
              return motd;
        break;
      case MOTD_IPMASKV6:
          if (client_p->localClient->aftype == AF_INET6)
            if (match_ipv6(&client_p->localClient->ip, &motd->address, motd->addrbits))
              return motd;
        break;
      default: break;
    }
  }

  return MotdList.local;  /* Ok, return the default motd */
}

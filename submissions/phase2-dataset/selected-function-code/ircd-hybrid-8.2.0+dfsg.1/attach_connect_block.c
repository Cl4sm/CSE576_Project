attach_connect_block(struct Client *client_p, const char *name,
                     const char *host)
{
  dlink_node *ptr;
  struct MaskItem *conf = NULL;

  assert(client_p != NULL);
  assert(host != NULL);

  if (client_p == NULL || host == NULL)
    return 0;

  DLINK_FOREACH(ptr, server_items.head)
  {
    conf = ptr->data;

    if (match(conf->name, name) || match(conf->host, host))
      continue;

    attach_conf(client_p, conf);
    return -1;
  }

  return 0;
}

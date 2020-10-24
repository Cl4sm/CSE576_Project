detach_conf(struct Client *client_p, enum maskitem_type type)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, client_p->localClient->confs.head)
  {
    struct MaskItem *conf = ptr->data;

    assert(conf->type & (CONF_CLIENT | CONF_OPER | CONF_SERVER));
    assert(conf->ref_count > 0);
    assert(conf->class->ref_count > 0);

    if (!(conf->type & type))
      continue;

    dlinkDelete(ptr, &client_p->localClient->confs);
    free_dlink_node(ptr);

    if (conf->type == CONF_CLIENT)
      remove_from_cidr_check(&client_p->localClient->ip, conf->class);

    if (--conf->class->ref_count == 0 && conf->class->active == 0)
    {
      class_free(conf->class);
      conf->class = NULL;
    }

    if (--conf->ref_count == 0 && conf->active == 0)
      conf_free(conf);
  }
}

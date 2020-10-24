find_conf_name(dlink_list *list, const char *name, enum maskitem_type type)
{
  dlink_node *ptr;
  struct MaskItem* conf;

  DLINK_FOREACH(ptr, list->head)
  {
    conf = ptr->data;

    if (conf->type == type)
    {
      if (conf->name && (!irccmp(conf->name, name) ||
                         !match(conf->name, name)))
      return conf;
    }
  }

  return NULL;
}

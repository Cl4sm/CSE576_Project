conf_add_class_to_conf(struct MaskItem *conf, const char *class_name)
{
  if (class_name == NULL)
  {
    conf->class = class_default;

    if (conf->type == CONF_CLIENT)
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Warning *** Defaulting to default class for %s@%s",
                           conf->user, conf->host);
    else
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Warning *** Defaulting to default class for %s",
                           conf->name);
  }
  else
    conf->class = class_find(class_name, 1);

  if (conf->class == NULL)
  {
    if (conf->type == CONF_CLIENT)
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Warning *** Defaulting to default class for %s@%s",
                           conf->user, conf->host);
    else
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Warning *** Defaulting to default class for %s",
                           conf->name);
    conf->class = class_default;
  }
}

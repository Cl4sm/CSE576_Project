static dlink_list *
map_to_list(enum maskitem_type type)
{
  switch(type)
  {
  case CONF_XLINE:
    return(&xconf_items);
    break;
  case CONF_ULINE:
    return(&uconf_items);
    break;
  case CONF_NRESV:
    return(&nresv_items);
    break;
  case CONF_CRESV:
    return(&cresv_items);
  case CONF_OPER:
    return(&oconf_items);
    break;
  case CONF_SERVER:
    return(&server_items);
    break;
  case CONF_SERVICE:
    return(&service_items);
    break;
  case CONF_CLUSTER:
    return(&cluster_items);
    break;
  default:
    return NULL;
  }
}

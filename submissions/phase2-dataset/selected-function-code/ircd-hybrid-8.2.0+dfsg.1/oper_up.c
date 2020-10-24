oper_up(struct Client *source_p)
{
  const unsigned int old = source_p->umodes;
  const struct MaskItem *conf = source_p->localClient->confs.head->data;

  assert(source_p->localClient->confs.head);

  ++Count.oper;
  SetOper(source_p);

  if (conf->modes)
    AddUMode(source_p, conf->modes);
  else if (ConfigGeneral.oper_umodes)
    AddUMode(source_p, ConfigGeneral.oper_umodes);

  if (!(old & UMODE_INVISIBLE) && HasUMode(source_p, UMODE_INVISIBLE))
    ++Count.invisi;
  if ((old & UMODE_INVISIBLE) && !HasUMode(source_p, UMODE_INVISIBLE))
    --Count.invisi;

  assert(dlinkFind(&oper_list, source_p) == NULL);
  dlinkAdd(source_p, make_dlink_node(), &oper_list);

  AddOFlag(source_p, conf->port);

  if (HasOFlag(source_p, OPER_FLAG_ADMIN))
    AddUMode(source_p, UMODE_ADMIN);

  sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE, "%s is now an operator",
                       get_oper_name(source_p));
  sendto_server(NULL, NOCAPS, NOCAPS, ":%s GLOBOPS :%s is now an operator",
                me.id, get_oper_name(source_p));
  send_umode_out(source_p, source_p, old);
  sendto_one_numeric(source_p, &me, RPL_YOUREOPER);
}

ospf_iface_add(struct object_lock *lock)
{
  struct ospf_iface *ifa = lock->data;
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;

  /* Open socket if interface is not stub */
  if (! ifa->stub && ! ospf_sk_open(ifa))
  {
    log(L_ERR "%s: Cannot open socket for %s, declaring as stub", p->name, ifa->ifname);
    ifa->ioprob = OSPF_I_SK;
    ifa->stub = 1;
  }

  if (! ifa->stub)
  {
    ifa->hello_timer = tm_new_set(ifa->pool, hello_timer_hook, ifa, 0, ifa->helloint);

    if (ifa->type == OSPF_IT_NBMA)
      ifa->poll_timer = tm_new_set(ifa->pool, poll_timer_hook, ifa, 0, ifa->pollint);

    if ((ifa->type == OSPF_IT_BCAST) || (ifa->type == OSPF_IT_NBMA))
      ifa->wait_timer = tm_new_set(ifa->pool, wait_timer_hook, ifa, 0, 0);
  }

  /* Do iface UP, unless there is no link and we use link detection */
  ospf_iface_sm(ifa, (ifa->check_link && !(ifa->iface->flags & IF_LINK_UP)) ? ISM_LOOP : ISM_UP);
}

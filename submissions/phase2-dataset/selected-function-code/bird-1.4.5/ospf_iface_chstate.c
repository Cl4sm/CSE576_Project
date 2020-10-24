void
ospf_iface_chstate(struct ospf_iface *ifa, u8 state)
{
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;
  u8 oldstate = ifa->state;

  if (oldstate == state)
    return;

  ifa->state = state;

  if (ifa->type == OSPF_IT_VLINK)
    OSPF_TRACE(D_EVENTS, "Changing state of virtual link %R from %s to %s",
	       ifa->vid, ospf_is[oldstate], ospf_is[state]);
  else
    OSPF_TRACE(D_EVENTS, "Changing state of iface %s from %s to %s",
	       ifa->ifname, ospf_is[oldstate], ospf_is[state]);

  if ((ifa->type == OSPF_IT_BCAST) && !ifa->cf->real_bcast && ifa->sk)
  {
    if ((state == OSPF_IS_BACKUP) || (state == OSPF_IS_DR))
      ospf_sk_join_dr(ifa);
    else
      ospf_sk_leave_dr(ifa);
  }

  if ((oldstate == OSPF_IS_DR) && (ifa->net_lsa != NULL))
  {
    ifa->net_lsa->lsa.age = LSA_MAXAGE;
    if (state >= OSPF_IS_WAITING)
      ospf_lsupd_flush_nlsa(po, ifa->net_lsa);

    if (can_flush_lsa(po))
      flush_lsa(ifa->net_lsa, po);
    ifa->net_lsa = NULL;
  }

  if ((oldstate > OSPF_IS_LOOP) && (state <= OSPF_IS_LOOP))
    ospf_iface_down(ifa);

  schedule_rt_lsa(ifa->oa);
  // FIXME flushling of link LSA
}

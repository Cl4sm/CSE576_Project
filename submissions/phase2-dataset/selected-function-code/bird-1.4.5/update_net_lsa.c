update_net_lsa(struct ospf_iface *ifa)
{
  struct proto_ospf *po = ifa->oa->po;
 
  if (ifa->net_lsa && ((ifa->net_lsa->inst_t + MINLSINTERVAL) > now))
    return;
  /*
   * It's too early to originate new network LSA. We will
   * try to do it next tick
   */

  if ((ifa->state != OSPF_IS_DR) || (ifa->fadj == 0))
    {
      flush_net_lsa(ifa);
#ifdef OSPFv3
      flush_prefix_net_lsa(ifa);
#endif
    }
  else
    {
      originate_net_lsa(ifa);
#ifdef OSPFv3
      originate_prefix_net_lsa(ifa);
#endif
    }

  schedule_rtcalc(po);
  ifa->orignet = 0;
}

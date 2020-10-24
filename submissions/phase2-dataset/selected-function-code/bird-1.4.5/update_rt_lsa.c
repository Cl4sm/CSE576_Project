update_rt_lsa(struct ospf_area *oa)
{
  struct proto_ospf *po = oa->po;

  if ((oa->rt) && ((oa->rt->inst_t + MINLSINTERVAL)) > now)
    return;
  /*
   * Tick is probably set to very low value. We cannot
   * originate new LSA before MINLSINTERVAL. We will
   * try to do it next tick.
   */

  originate_rt_lsa(oa);
#ifdef OSPFv3
  originate_prefix_rt_lsa(oa);
#endif

  schedule_rtcalc(po);
  oa->origrt = 0;
}

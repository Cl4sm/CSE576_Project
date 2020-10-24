static void
ospf_area_reconfigure(struct ospf_area *oa, struct ospf_area_config *nac)
{
  oa->ac = nac;

  // FIXME better area type reconfiguration
#ifdef OSPFv2
  oa->options = nac->type;
#else /* OSPFv3 */
  oa->options = nac->type | OPT_V6 | (oa->po->stub_router ? 0 : OPT_R);
#endif
  if (oa_is_nssa(oa) && (oa->po->areano > 1))
    oa->po->ebit = 1;

  ospf_ifaces_reconfigure(oa, nac);

  /* Handle net_list */
  fib_free(&oa->net_fib);
  fib_free(&oa->enet_fib);
  add_area_nets(oa, nac);

  /* No need to handle stubnet_list */

  oa->marked = 0;
  schedule_rt_lsa(oa);
}

static void
ospf_area_add(struct proto_ospf *po, struct ospf_area_config *ac, int reconf)
{
  struct proto *p = &po->proto;
  struct ospf_area *oa;

  OSPF_TRACE(D_EVENTS, "Adding area %R", ac->areaid);

  oa = mb_allocz(p->pool, sizeof(struct ospf_area));
  add_tail(&po->area_list, NODE oa);
  po->areano++;

  oa->ac = ac;
  oa->areaid = ac->areaid;
  oa->rt = NULL;
  oa->po = po;
  fib_init(&oa->rtr, p->pool, sizeof(ort), 0, ospf_rt_initort);
  add_area_nets(oa, ac);

  if (oa->areaid == 0)
    po->backbone = oa;

#ifdef OSPFv2
  oa->options = ac->type;
#else /* OSPFv3 */
  oa->options = ac->type | OPT_V6 | (po->stub_router ? 0 : OPT_R);
#endif

  /*
   * Set E-bit for NSSA ABR routers. No need to explicitly call
   * schedule_rt_lsa() for other areas, will be done anyway.
   * We use cf->abr because po->areano is not yet complete.
   */
  if (oa_is_nssa(oa) && ((struct ospf_config *) (p->cf))->abr)
    po->ebit = 1;

  if (reconf)
    ospf_ifaces_reconfigure(oa, ac);
}

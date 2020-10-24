void
ospf_rt_spf(struct proto_ospf *po)
{
  struct proto *p = &po->proto;
  struct ospf_area *oa;

  if (po->areano == 0)
    return;

  OSPF_TRACE(D_EVENTS, "Starting routing table calculation");

  /* 16. (1) */
  ospf_rt_reset(po);

  /* 16. (2) */
  WALK_LIST(oa, po->area_list)
    ospf_rt_spfa(oa);

  /* 16. (3) */
  ospf_rt_sum(ospf_main_area(po));

  /* 16. (4) */
  WALK_LIST(oa, po->area_list)
    if (oa->trcap && (oa->areaid != 0))
      ospf_rt_sum_tr(oa);

  if (po->areano > 1)
    ospf_rt_abr1(po);

  /* 16. (5) */
  ospf_ext_spf(po);

  if (po->areano > 1)
    ospf_rt_abr2(po);

  rt_sync(po);
  lp_flush(po->nhpool);
  
  po->calcrt = 0;
}

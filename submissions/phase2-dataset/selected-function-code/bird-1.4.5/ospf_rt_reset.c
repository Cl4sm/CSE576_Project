void
ospf_rt_reset(struct proto_ospf *po)
{
  struct ospf_area *oa;
  struct top_hash_entry *en;
  struct area_net *anet;
  ort *ri;

  /* Reset old routing table */
  FIB_WALK(&po->rtf, nftmp)
  {
    ri = (ort *) nftmp;
    ri->fn.x0 = 0;
    reset_ri(ri);
  }
  FIB_WALK_END;

  /* Reset SPF data in LSA db */
  WALK_SLIST(en, po->lsal)
  {
    en->color = OUTSPF;
    en->dist = LSINFINITY;
    en->nhs = NULL;
    en->lb = IPA_NONE;
  }

  WALK_LIST(oa, po->area_list)
  {
    /* Reset ASBR routing tables */
    FIB_WALK(&oa->rtr, nftmp)
    {
      ri = (ort *) nftmp;
      reset_ri(ri);
    }
    FIB_WALK_END;

    /* Reset condensed area networks */
    if (po->areano > 1)
    {
      FIB_WALK(&oa->net_fib, nftmp)
      {
	anet = (struct area_net *) nftmp;
	anet->active = 0;
	anet->metric = 0;
      }
      FIB_WALK_END;

      FIB_WALK(&oa->enet_fib, nftmp)
      {
	anet = (struct area_net *) nftmp;
	anet->active = 0;
	anet->metric = 0;
      }
      FIB_WALK_END;
    }
  }
}
